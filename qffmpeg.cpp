#include "qffmpeg.h"
#include "algothread.h"
#include <QDateTime>
#include <QDebug>

extern buffNode imageBuff[];
extern int channelTimeStamp[];

QFFmpeg::QFFmpeg(QObject *parent) :
    QObject(parent)
{    

}

bool QFFmpeg::fileInit(){
    av_register_all();//注册所有组件
    //参数1:封装格式上下文->AVFormatContext->包含了视频信息(视频格式、大小等等...)双指针定义一颗星*，
    //参数2:要打开流的路径（文件名）
    //AVFormatContext保存视频（视频流）相关信息的结构体
    formatContent = avformat_alloc_context();
    int res = avformat_open_input(&formatContent,url.toStdString().c_str(),nullptr,nullptr);//打开摄像头

    if(res!=0)
    {
        qDebug()<<"打开视频失败";
        return false;
    }

    /*3、打开成功之后相关的结构体信息放在了formatContent里面，接下来获取视频文件信息*/
    //3.1先看有没有视频流信息（avformat_find_stream_info），进行判断的原因是有可能打开普通文件
    res = avformat_find_stream_info(formatContent,nullptr);
    if(res<0)
    {
        qDebug()<<"打开流媒体信息失败";
        return false;
    }

    videoType = -1;
    //3.2一个视频中有多股码流（用循环），存在AVFormatContext的streams数组中
    for(int i=0;i<formatContent->nb_streams;i++)
    {
        if(formatContent->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)//streams->有AVStream结构体,AVStream->codec
        {
            //找到视频流(只有一个)
            videoType = i;//标识视频流这个类型
            break;
        }
    }
    if(videoType == -1)
    {
        qDebug()<<"没有找到视频流相关信息";
        return false;
    }

    //3.3根据视频流查找编码器对应的上下文对象结构体，存储编码器以及宽高格式等
    codec = formatContent->streams[videoType]->codec;
    /*4、有视频流，则查找对应视频流的解码器*/
    decoder = avcodec_find_decoder(codec->codec_id);//需要解码器的id
    if(decoder ==nullptr)
    {
        qDebug()<<"没有找到对应的解码器";
        return false;
    }

    /*5、找到解码器后打开解码器*/
    //参数：1.初始化的上下文对象 2.打开的解码器 3.类似目录的东西（没有）
    res = avcodec_open2(codec,decoder,nullptr);
    if(res!=0)
    {
        qDebug()<<"解码器打开失败";
        return false;
    }


    /*6、获取到的每一帧码流（视频流）数据写到文件中，进行循环解码*/
    /*6.1写到文件用FILE结构体*/
    //码流数据是存到buffer里面，也需要我们动态开空间(AVBufferRef *buf;)
    //开空间不知道一帧的码流数据是多少？其实编解码器告诉了宽高，以此可以计算出给码流数据开多大空间
    int bufSize = codec->width*codec->height;//计算一帧（图）数据的大小
    //av_new_packet(&pktContent,bufSize);
    //printf("width%d,height%d\n", codec->width,codec->height);
    avpicture_alloc(&pictureRGB,AV_PIX_FMT_RGB32,codec->width,codec->height);
    //因为解码之后要伸展，所以先进行转换规则的设置，转换完进入第七步解码
    swsContent = nullptr;
    swsContent = sws_getContext(codec->width,codec->height,codec->pix_fmt,
                                codec->width,codec->height,AV_PIX_FMT_RGB32,
                                SWS_BICUBIC,nullptr,nullptr,nullptr);

    picture = av_frame_alloc();//保存原始RGB数据
    qDebug()<<"初始化视频文件成功";
    return true;
}

int tmp=0;
int QFFmpeg::filePlay()
{
    mutex.lock();
    int result=1;

    int count = 0;//保存帧数的变量
    if(av_read_frame(formatContent,&pktContent) >= 0)//成功读到了数据（一帧一帧读（循环）：av_read_frame）
    {
        if(pktContent.stream_index == videoType)
        {
            int got_picture_ptr = -1;

            avcodec_decode_video2(codec,picture,&got_picture_ptr,&pktContent);
            if(got_picture_ptr != 0)
            {
                //把解码得到的损坏的像素数据剔除(存到pictureRGB中)
                sws_scale(swsContent,picture->data,picture->linesize,0,picture->height,
                          pictureRGB.data,pictureRGB.linesize);

                memcpy(imageBuff[channel-1].image,pictureRGB.data[0],codec->width*codec->height*4);
                imageBuff[channel-1].width=codec->width;
                imageBuff[channel-1].height=codec->height;
                imageBuff[channel-1].format=QImage::Format_RGB32;

//                tmp++;
//                if(tmp%10==0){
//                    for(int i=0;i<16;i++){
//                        printf("%d, ",channelTimeStamp[i]);
//                    }
//                    printf("\n");
//                }
            }

        }
        //每次都存在同一块内存空间里，要清空上一次的操作
        av_packet_unref(&pktContent);//不是free
    }else{
        //printf("video end!\n");
    }
    av_free_packet(&pktContent);//释放资源,否则内存会一直上升

    mutex.unlock();
    return result;
}

QFFmpeg::~QFFmpeg()
{

}

void QFFmpeg::deInit(){
    printf("deInit ffmpeg channel:%d\n",channel);
    if(hostType==0){
        avformat_close_input(&pAVFormatContext);
        avformat_free_context(pAVFormatContext);

        av_packet_unref(&pAVPacket);

        av_frame_free(&pAVFrame);
        sws_freeContext(pSwsContext);
    }else{
        avformat_close_input(&formatContent);
        avformat_free_context(formatContent);

        av_packet_unref(&pktContent);

        av_frame_free(&picture);
        sws_freeContext(swsContent);
    }
}

bool QFFmpeg::rtspInit()
{
    videoStreamIndex=-1;
    av_register_all();//注册库中所有可用的文件格式和解码器

    avformat_network_init();//初始化网络流格式,使用RTSP网络流时必须先执行
    pAVFormatContext = avformat_alloc_context();//申请一个AVFormatContext结构的内存,并进行简单初始化
    pAVFrame=av_frame_alloc();

    AVDictionary* options = NULL;
    av_dict_set(&options, "rtsp_transport", "tcp", 0);
    av_dict_set(&options, "stimeout", "2000000", 0);
    av_dict_set(&options, "max_delay", "500000", 0);
    av_dict_set(&options, "thread_queue_size", "512", 0);

    //打开视频流
    int result=avformat_open_input(&pAVFormatContext, url.toStdString().c_str(),NULL,&options);
    if (result<0){
        qDebug()<<"打开视频流失败";
        return false;
    }
    av_dict_free(&options);

    //获取视频流信息
    result=avformat_find_stream_info(pAVFormatContext,NULL);
    if (result<0){
        qDebug()<<"获取视频流信息失败";
        return false;
    }

    //获取视频流索引
    videoStreamIndex = -1;
    for (uint i = 0; i < pAVFormatContext->nb_streams; i++) {
        if (pAVFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex==-1){
        qDebug()<<"获取视频流索引失败";
        return false;
    }

    //获取视频流的分辨率大小
    pAVCodecContext = pAVFormatContext->streams[videoStreamIndex]->codec;
    videoWidth=pAVCodecContext->width;
    videoHeight=pAVCodecContext->height;

    avpicture_alloc(&pAVPicture,AV_PIX_FMT_RGB24,videoWidth,videoHeight);

    //获取视频流解码器
    AVCodec *pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);

    //AVPixelFormat fmt=pAVCodecContext->pix_fmt;

    pSwsContext = sws_getContext(videoWidth,videoHeight,AV_PIX_FMT_YUV420P,videoWidth,videoHeight,AV_PIX_FMT_RGB24,SWS_BICUBIC,0,0,0);
    //pSwsContext = sws_getContext(videoWidth,videoHeight,AV_PIX_FMT_YUV420P,videoWidth,videoHeight,AV_PIX_FMT_RGB32,SWS_BICUBIC,0,0,0);

    //打开对应解码器
    result=avcodec_open2(pAVCodecContext,pAVCodec,NULL);
    if (result<0){
        qDebug()<<"打开解码器失败";
        return false;
    }

    qDebug()<<"初始化视频流成功";
    return true;
}

int QFFmpeg::rtspPlay()
{
    mutex.lock();
    int result=1;

    int frameFinished=0;
        if (av_read_frame(pAVFormatContext, &pAVPacket) >= 0){
            if(pAVPacket.stream_index==videoStreamIndex){
                //qDebug()<<"开始解码"<<QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
                avcodec_decode_video2(pAVCodecContext, pAVFrame, &frameFinished, &pAVPacket);
                sws_scale(pSwsContext,(const uint8_t* const *)pAVFrame->data,pAVFrame->linesize,0,
                          videoHeight,pAVPicture.data,pAVPicture.linesize);

                memcpy(imageBuff[channel-1].image,pAVPicture.data[0],videoWidth*videoHeight*3);
                imageBuff[channel-1].width=videoWidth;
                imageBuff[channel-1].height=videoHeight;
                imageBuff[channel-1].format=QImage::Format_RGB888;
            }
        }else{
            printf("over\n");
            result=-1;
        }
        av_free_packet(&pAVPacket);//释放资源,否则内存会一直上升
    mutex.unlock();
    return result;
}
