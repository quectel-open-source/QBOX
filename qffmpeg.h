#ifndef QFFMPEG_H
#define QFFMPEG_H

//必须加以下内容,否则编译不能通过,为了兼容C和C99标准
#ifndef INT64_C
#define INT64_C
#define UINT64_C
#endif

//引入ffmpeg头文件
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
}

#include <QObject>
#include <QMutex>
#include <QImage>

class QFFmpeg : public QObject
{
    Q_OBJECT
public:
    explicit QFFmpeg(QObject *parent = 0);
    ~QFFmpeg();

    bool rtspInit();
    int rtspPlay();

    bool fileInit();
    int filePlay();

    void SetUrl(QString url){this->url=url;}
    QString Url()const{return url;}
    int VideoWidth()const{return videoWidth;}
    int VideoHeight()const{return videoHeight;}
    //AlgoBody ab;

    int videoWidth=0;
    int videoHeight=0;
    QString url;
    int channel;
    int hostType;
    void deInit();

private:
    QMutex mutex;
    //Rstp内容
    AVPicture  pAVPicture;
    AVPacket pAVPacket;

    AVFormatContext *pAVFormatContext;
    AVCodecContext *pAVCodecContext;
    AVFrame *pAVFrame;//存储解码后的原始音频或视频数据
    SwsContext * pSwsContext;

    AVCodec *pAVCodec;
    int videoStreamIndex;

    //文件的内容
    AVPacket pktContent;
    AVPicture pictureRGB;

    AVCodecContext* codec;
    AVFormatContext* formatContent;
    AVFrame *picture;
    SwsContext *swsContent;

    AVCodec *decoder;
    int videoType;


public slots:

};

#endif // QFFMPEG_H
