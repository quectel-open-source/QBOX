#ifndef ALGOTHREAD_H
#define ALGOTHREAD_H

#include "include/Helmet.hpp"
#include "include/Pedestrian.hpp"
#include "include/DeepFace.hpp"

#include "include/Smoke.hpp"
#include "include/Masks.hpp"
#include "include/DeepPlate.hpp"
#include <iostream>

#include <QImage>
#include <QThread>
#include <QPixmap>
using namespace std;

typedef struct buffNodeItem{
    uchar image[3000*3000*4];
    int width=200;
    int height=150;
    enum QImage::Format format=QImage::Format_RGB888;
}buffNode;



class AlgoThread : public QThread
{
    Q_OBJECT
public:
    int runAlgo(int channel,const QImage image,int algo,bool mustDeal);
    int findNodeLoc(int channel);
    explicit AlgoThread(QObject *parent = 0);
    int channel;
    int frameNum=0;
    int pushNum=0;
    float rate=30.0;
    void drawRate(QImage &image2);
    int beforeTimeStamp=0;
    void initAlgo();

    std::vector<ObjectResult> beforeObjList;
    void run();
private:
    YHandle helmet_handle=nullptr;
    YHandle pedestrian_handle=nullptr;
    YHandle deepface_handle = nullptr;

    YHandle masks_handle = nullptr;
    YHandle smoke_handle = nullptr;
    YHandle plate_handle = nullptr;

    void iniCheckHelmet();
    void iniCheckPedestrian();
    void iniCheckDeepFace();
    void iniCheckMasks();
    void iniCheckSmoke();
    void iniCheckPlate();

    int checkHelmet(int channel,QImage src,bool mustDeal);
    int checkPedestrian(int channel,QImage src,bool mustDeal);
    int checkDeepFace(int channel,QImage src,bool mustDeal);
    int checkMasks(int channel,QImage src,bool mustDeal);
    int checkSmoke(int channel,QImage src,bool mustDeal);
    int checkPlate(int channel,QImage src,bool mustDeal);

signals:
    void sendtoShow(int uiLoc,QPixmap image);
    void sendtoDb(int uiLoc,QPixmap image,QString title,QString picName);
};

#endif // FFTHREAD_H
