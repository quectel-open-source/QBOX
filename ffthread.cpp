#include "ffthread.h"
#include "qffmpeg.h"
extern int channelTimeStamp[];

FfThread::FfThread(QObject *parent) :
    QThread(parent)
{
}


void  FfThread::run()
{
    ffmpeg->url=QString::fromStdString(address);
    ffmpeg->channel=channel;
    ffmpeg->hostType=hostType;

    if(waitFrame<1){
        waitFrame=1;
    }

    channelTimeStamp[channel-1]=0;
    //RTSP
    if(hostType==0){
        bool re=ffmpeg->rtspInit();
        while(re&&!isStop){
            ffmpeg->rtspPlay();
            msleep(waitFrame);
            channelTimeStamp[channel-1]++;
        }
    }else{//file
        bool re=ffmpeg->fileInit();
        while(re&&!isStop){
            ffmpeg->filePlay();
            msleep(waitFrame);
            channelTimeStamp[channel-1]++;
        }
    }

}
