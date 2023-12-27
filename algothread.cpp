#include "algothread.h"
#include "myhelper.h"

#include <string>
#include <QPainter>
#include <QMutex>
#include "opencv2/opencv.hpp"

#define HELMET_JSON "/root/configs/helmet.json"
#define PEDESTRIAN_JSON "/root/configs/pedestrian.json"
#define DEEPFACE_JSON "/root/configs/deepface.json"

#define MASKS_JSON "/root/configs/masks.json"
#define SMOKE_JSON "/root/configs/smoke.json"
#define PLATE_JSON "/root/configs/deepplate.json"

buffNode imageBuff[16];
bool algoEnable=false;
extern vector<hostNode> hostList;
extern int giveWidth[];
extern int giveHeight[];

extern QString enterDay;
extern int channelTimeStamp[];



//helmet
static std::map<std::string, cv::Scalar> color_map = {
    {"person", cv::Scalar(255, 0, 0)},
    {"head", cv::Scalar(0, 255, 0)},
    {"helmet", cv::Scalar(0, 0, 255)}
};

//mask
static std::map<std::string, cv::Scalar> color_map2 = {
    {"without_mask", cv::Scalar(255, 0, 0)},
    {"with_mask", cv::Scalar(0, 255, 0)},
    {"mask_weared_incorrect", cv::Scalar(0, 0, 255)}
};

//smoke
static std::map<std::string, cv::Scalar> color_map3 = {
    {"call", cv::Scalar(255, 0, 0)},
    {"smoke", cv::Scalar(0, 255, 0)},
};


//plate
static std::map<std::string, cv::Scalar> color_map4 = {
    {"plate", cv::Scalar(255, 0, 0)},
    {"None", cv::Scalar(0, 255, 0)},
};

//Pedestrian
static std::map<std::string, cv::Scalar> color_map5 = {
    {"person", cv::Scalar(255, 0, 0)},
    {"car", cv::Scalar(0, 255, 0)}
};


void AlgoThread::iniCheckPlate(){
    //没有初始化，就进行初始化
    if(plate_handle==nullptr){
        int flag = InitDeepPlate(plate_handle, string(PLATE_JSON));
        if (flag != 0) {
                printf("channel%d checkPlate fail\n",channel);
                UnInitDeepPlate(plate_handle);
                return;
        }else{
             printf("channel%d checkPlate OK\n",channel);
        }
    }
}

int AlgoThread::checkPlate(int channel,QImage src,bool mustDeal){
    //没有初始化，就进行初始化
    if(plate_handle==nullptr){
        printf("channel%d checkPlate is not init\n",channel);
        return 0;
    }

    cv::Mat image1(src.height(),src.width(),CV_8UC4,(void *)src.constBits());
    cv::Mat image;
    cv::cvtColor(image1, image, cv::COLOR_RGBA2BGR);
    cv::Mat img_show;
    image.copyTo(img_show);

    if(!mustDeal){
            for (auto obj : beforeObjList) {
                        int x1 = int(obj.cx - obj.width / 2);
                        int y1 = int(obj.cy - obj.height / 2);
                        cv::rectangle(img_show, cv::Point2i(x1, y1), cv::Point2i(x1 + obj.width, y1 + obj.height), color_map4[obj.label]);
                        cv::rectangle(img_show, cv::Point2i(x1, y1 - 20), cv::Point2i(x1 + 100, y1), color_map4[obj.label], -1);
            }

            int uiLoc=channel-1;
            int width = giveWidth[uiLoc];
            int height = giveHeight[uiLoc];
            QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
            QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            drawRate(image2);
            QPixmap pix = QPixmap::fromImage(image2);
            emit sendtoShow(uiLoc,pix);
            return 0;
    }


    std::vector<DeepPlateResult> plate_reults;

    const auto start = std::chrono::high_resolution_clock::now();

    DetPlate(plate_handle, image.data, image.cols, image.rows, image.channels(), plate_reults);

    const auto end = std::chrono::high_resolution_clock::now();
    const float duration = std::chrono::duration<float, std::milli>(end - start).count();
    std::cout << "#########inferenceTime: " << duration << " ms" << std::endl;

    if(plate_reults.size()>0){
        beforeObjList.clear();
        for (auto plate : plate_reults) {
            int x1 = int(plate.obj .cx - plate.obj.width / 2);
            int y1 = int(plate.obj.cy - plate.obj.height / 2);
            cv::rectangle(img_show, cv::Point2i(x1, y1), cv::Point2i(x1 + plate.obj.width, y1 + plate.obj.height), color_map4[plate.obj.label], 2);
            cv::rectangle(img_show, cv::Point2i(x1, y1 - 20), cv::Point2i(x1 + 100, y1), color_map4[plate.obj.label], -1);

            char score_text[80];
            sprintf(score_text, "%.2f", round(plate.obj.score * 1e3) / 1e3);
            std::string text = plate.obj.label + " " + score_text;
            cv::putText(img_show, text, cv::Point2i(x1, y1 - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));

            beforeObjList.push_back(plate.obj);
            std::cout << plate.obj.label << " " << x1 << " " << y1 << " " << plate.obj.width
                      << " " << plate.obj.height << " " << plate.obj.score << std::endl;
       }

       const auto start2 = std::chrono::high_resolution_clock::now();
       cvtColor(img_show,img_show,cv::COLOR_BGR2RGB);
       QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
       const auto end2 = std::chrono::high_resolution_clock::now();
       const float duration2 = std::chrono::duration<float, std::milli>(end2 - start2).count();
       std::cout << "#########MatToQImageTime: " << duration2 << " ms" << std::endl;

        int objNum=plate_reults.size();
        QString re=QString("通道%1 发现%2个告警 Time:%4").arg(channel+1).arg(objNum).arg(QDateTime::currentDateTime().toString("MM_dd_hh_mm"));
        cout<<re.toStdString()<<endl;
        drawRate(dst);

        int uiLoc=channel-1;
        int width = giveWidth[uiLoc];
        int height = giveHeight[uiLoc];
        QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        drawRate(image2);
        QPixmap pix = QPixmap::fromImage(image2);

        QString picName=QString("/data/result_img/"+enterDay+"/channel%1_p%2.jpg").arg(uiLoc).arg(pushNum);
        dst.save(picName,"JPG",100);
        pushNum++;
        emit sendtoDb(uiLoc,pix,re,picName);
        printf("find Smoke\n");
    }else{
        int uiLoc=channel-1;
        int width = giveWidth[uiLoc];
        int height = giveHeight[uiLoc];
        QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
        QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        drawRate(image2);
        QPixmap pix = QPixmap::fromImage(image2);
        emit sendtoShow(uiLoc,pix);
    }
}




void AlgoThread::iniCheckSmoke(){
    //没有初始化，就进行初始化
    if(smoke_handle==nullptr){
        int flag = InitSmoke(smoke_handle, SMOKE_JSON,nullptr, 0);
        if (flag != 0) {
                printf("channel%d checkSmoke fail\n",channel);
                UnInitSmoke(smoke_handle);
                return;
        }else{
             printf("channel%d checkSmoke OK\n",channel);
        }
    }
}

int AlgoThread::checkSmoke(int channel,QImage src,bool mustDeal){
    //没有初始化，就进行初始化
    if(smoke_handle==nullptr){
        printf("channel%d checkSmoke is not init\n",channel);
        return 0;
    }

    cv::Mat image1(src.height(),src.width(),CV_8UC4,(void *)src.constBits());
    cv::Mat image;
    cv::cvtColor(image1, image, cv::COLOR_RGBA2BGR);
    cv::Mat img_show;
    image.copyTo(img_show);

    if(!mustDeal){
            for (auto obj : beforeObjList) {
                        int x1 = int(obj.cx - obj.width / 2);
                        int y1 = int(obj.cy - obj.height / 2);
                        cv::rectangle(img_show, cv::Point2i(x1, y1), cv::Point2i(x1 + obj.width, y1 + obj.height), color_map3[obj.label]);
                        cv::rectangle(img_show, cv::Point2i(x1, y1 - 20), cv::Point2i(x1 + 100, y1), color_map3[obj.label], -1);
            }

            int uiLoc=channel-1;
            int width = giveWidth[uiLoc];
            int height = giveHeight[uiLoc];
            QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
            QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            drawRate(image2);
            QPixmap pix = QPixmap::fromImage(image2);
            emit sendtoShow(uiLoc,pix);
            return 0;
    }


    std::vector<ObjectResult> obj_list;

    const auto start = std::chrono::high_resolution_clock::now();

    DetSmoke(smoke_handle, image.data, image.cols, image.rows, image.channels(), obj_list);

    const auto end = std::chrono::high_resolution_clock::now();
    const float duration = std::chrono::duration<float, std::milli>(end - start).count();
    std::cout << "#########inferenceTime: " << duration << " ms" << std::endl;

    if(obj_list.size()>0){
        beforeObjList.clear();
        for (auto obj : obj_list) {
                    int x1 = int(obj.cx - obj.width / 2);
                    int y1 = int(obj.cy - obj.height / 2);
                    cv::rectangle(img_show, cv::Point2i(x1, y1), cv::Point2i(x1 + obj.width, y1 + obj.height), color_map3[obj.label]);
                    cv::rectangle(img_show, cv::Point2i(x1, y1 - 20), cv::Point2i(x1 + 100, y1), color_map3[obj.label], -1);

                    char score_text[80];
                    sprintf(score_text, "%.2f", round(obj.score * 1e3) / 1e3);
                    std::string text = obj.label + " " + score_text;
                    cv::putText(img_show, text, cv::Point2i(x1, y1 - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
                    beforeObjList.push_back(obj);
                    std::cout << obj.label << " " << x1 << " " << y1 << " " << obj.width << " " << obj.height << " " << obj.score << std::endl;
       }

       const auto start2 = std::chrono::high_resolution_clock::now();
       cvtColor(img_show,img_show,cv::COLOR_BGR2RGB);
       QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
       const auto end2 = std::chrono::high_resolution_clock::now();
       const float duration2 = std::chrono::duration<float, std::milli>(end2 - start2).count();
       std::cout << "#########MatToQImageTime: " << duration2 << " ms" << std::endl;

        int objNum=obj_list.size();
        QString re=QString("通道%1 发现%2个告警 Time:%4").arg(channel+1).arg(objNum).arg(QDateTime::currentDateTime().toString("MM_dd_hh_mm"));
        cout<<re.toStdString()<<endl;
        drawRate(dst);

        int uiLoc=channel-1;
        int width = giveWidth[uiLoc];
        int height = giveHeight[uiLoc];
        QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        drawRate(image2);
        QPixmap pix = QPixmap::fromImage(image2);

        QString picName=QString("/data/result_img/"+enterDay+"/channel%1_p%2.jpg").arg(uiLoc).arg(pushNum);
        dst.save(picName,"JPG",100);
        pushNum++;
        emit sendtoDb(uiLoc,pix,re,picName);
        printf("find Smoke\n");
    }else{
        int uiLoc=channel-1;
        int width = giveWidth[uiLoc];
        int height = giveHeight[uiLoc];
        QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
        QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        drawRate(image2);
        QPixmap pix = QPixmap::fromImage(image2);
        emit sendtoShow(uiLoc,pix);
    }
}

void AlgoThread::iniCheckMasks(){
    //没有初始化，就进行初始化
    if(masks_handle==nullptr){
        int flag = InitMasks(masks_handle, MASKS_JSON,nullptr, 0);
        if (flag != 0) {
                printf("channel%d checkMask fail\n",channel);
                UnInitMasks(masks_handle);
                return;
        }else{
             printf("channel%d checkMask OK\n",channel);
        }
    }
}

int AlgoThread::checkMasks(int channel,QImage src,bool mustDeal){
    //没有初始化，就进行初始化
    if(masks_handle==nullptr){
        printf("channel%d checkPedestrian is not init\n",channel);
        return 0;
    }

    cv::Mat image1(src.height(),src.width(),CV_8UC4,(void *)src.constBits());
    cv::Mat image;
    cv::cvtColor(image1, image, cv::COLOR_RGBA2BGR);
    cv::Mat img_show;
    image.copyTo(img_show);

    if(!mustDeal){
            for (auto obj : beforeObjList) {
                        int x1 = int(obj.cx - obj.width / 2);
                        int y1 = int(obj.cy - obj.height / 2);
                        cv::rectangle(img_show, cv::Point2i(x1, y1), cv::Point2i(x1 + obj.width, y1 + obj.height), color_map2[obj.label]);
                        cv::rectangle(img_show, cv::Point2i(x1, y1 - 20), cv::Point2i(x1 + 100, y1), color_map2[obj.label], -1);
            }

            int uiLoc=channel-1;
            int width = giveWidth[uiLoc];
            int height = giveHeight[uiLoc];
            QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
            QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            drawRate(image2);
            QPixmap pix = QPixmap::fromImage(image2);
            emit sendtoShow(uiLoc,pix);
            return 0;
    }


    std::vector<ObjectResult> obj_list;

    const auto start = std::chrono::high_resolution_clock::now();

    DetMasks(masks_handle, image.data, image.cols, image.rows, image.channels(), obj_list);

    const auto end = std::chrono::high_resolution_clock::now();
    const float duration = std::chrono::duration<float, std::milli>(end - start).count();
    std::cout << "#########inferenceTime: " << duration << " ms" << std::endl;

    if(obj_list.size()>0){
        beforeObjList.clear();
        for (auto obj : obj_list) {
                    int x1 = int(obj.cx - obj.width / 2);
                    int y1 = int(obj.cy - obj.height / 2);
                    cv::rectangle(img_show, cv::Point2i(x1, y1), cv::Point2i(x1 + obj.width, y1 + obj.height), color_map2[obj.label]);
                    cv::rectangle(img_show, cv::Point2i(x1, y1 - 20), cv::Point2i(x1 + 100, y1), color_map2[obj.label], -1);

                    char score_text[80];
                    sprintf(score_text, "%.2f", round(obj.score * 1e3) / 1e3);
                    std::string text = obj.label + " " + score_text;
                    cv::putText(img_show, text, cv::Point2i(x1, y1 - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
                    beforeObjList.push_back(obj);
                    std::cout << obj.label << " " << x1 << " " << y1 << " " << obj.width << " " << obj.height << " " << obj.score << std::endl;
       }

       const auto start2 = std::chrono::high_resolution_clock::now();
       cvtColor(img_show,img_show,cv::COLOR_BGR2RGB);
       QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
       const auto end2 = std::chrono::high_resolution_clock::now();
       const float duration2 = std::chrono::duration<float, std::milli>(end2 - start2).count();
       std::cout << "#########MatToQImageTime: " << duration2 << " ms" << std::endl;

        int objNum=obj_list.size();
        QString re=QString("通道%1 发现%2个告警 Time:%4").arg(channel+1).arg(objNum).arg(QDateTime::currentDateTime().toString("MM_dd_hh_mm"));
        cout<<re.toStdString()<<endl;
        drawRate(dst);

        int uiLoc=channel-1;
        int width = giveWidth[uiLoc];
        int height = giveHeight[uiLoc];
        QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        drawRate(image2);
        QPixmap pix = QPixmap::fromImage(image2);

        QString picName=QString("/data/result_img/"+enterDay+"/channel%1_p%2.jpg").arg(uiLoc).arg(pushNum);
        dst.save(picName,"JPG",100);
        pushNum++;
        emit sendtoDb(uiLoc,pix,re,picName);
        printf("find Mask\n");
    }else{
        int uiLoc=channel-1;
        int width = giveWidth[uiLoc];
        int height = giveHeight[uiLoc];
        QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
        QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        drawRate(image2);
        QPixmap pix = QPixmap::fromImage(image2);
        emit sendtoShow(uiLoc,pix);
    }
}

AlgoThread::AlgoThread(QObject *parent) :
    QThread(parent)
{

}

int AlgoThread::findNodeLoc(int channel){
    for(int i=0;i<hostList.size();i++){
        hostNode hn=hostList[i];
        int hc=hn.channel;
        if(channel==hc){
            return i;
        }
    }
    return -1;
}

QImage MatToQImage(const cv::Mat& cvImage) {
    vector<uchar> imgBuf;
    imencode(".bmp", cvImage, imgBuf);

    QByteArray baImg((char*)imgBuf.data(), static_cast<int>(imgBuf.size()));
    QImage image;
    image.loadFromData(baImg, "BMP");
    return image;
}

void AlgoThread::iniCheckHelmet(){
    //没有初始化，就进行初始化
    if(helmet_handle==nullptr){
        int flag = InitHelmet(helmet_handle, HELMET_JSON,nullptr, 0);
        if (flag != 0) {
                printf("channel%d check Helmet is fail\n",channel);
                UnInitHelmet(helmet_handle);
                return;
        }else{
             printf("channel%d check helmet is ok\n",channel);
        }
    }
}

int AlgoThread::checkHelmet(int channel,QImage src,bool mustDeal){
    //没有初始化，就进行初始化
    if(helmet_handle==nullptr){
        printf("channel%d checkHelmet is not init\n",channel);
        return 0;
    }

    cv::Mat image1(src.height(),src.width(),CV_8UC4,(void *)src.constBits());
    cv::Mat image;
    cv::cvtColor(image1, image, cv::COLOR_RGBA2BGR);
    cv::Mat img_show;
    image.copyTo(img_show);
    if(!mustDeal){
            for (auto obj : beforeObjList) {
                        int x1 = int(obj.cx - obj.width / 2);
                        int y1 = int(obj.cy - obj.height / 2);
                        cv::rectangle(img_show, cv::Point2i(x1, y1), cv::Point2i(x1 + obj.width, y1 + obj.height), color_map[obj.label]);
                        cv::rectangle(img_show, cv::Point2i(x1, y1 - 20), cv::Point2i(x1 + 100, y1), color_map[obj.label], -1);
            }

            int uiLoc=channel-1;
            int width = giveWidth[uiLoc];
            int height = giveHeight[uiLoc];
            QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
            QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            drawRate(image2);
            QPixmap pix = QPixmap::fromImage(image2);
            emit sendtoShow(uiLoc,pix);
            return 0;
    }

    std::vector<ObjectResult> obj_list;
    DetHelmet(helmet_handle, image.data, image.cols, image.rows, image.channels(), obj_list);

    beforeObjList.clear();
    if(obj_list.size()>0){
        for (auto obj : obj_list) {
                    int x1 = int(obj.cx - obj.width / 2);
                    int y1 = int(obj.cy - obj.height / 2);
                    cv::rectangle(img_show, cv::Point2i(x1, y1), cv::Point2i(x1 + obj.width, y1 + obj.height), color_map[obj.label]);
                    cv::rectangle(img_show, cv::Point2i(x1, y1 - 20), cv::Point2i(x1 + 100, y1), color_map[obj.label], -1);

                    char score_text[80];
                    sprintf(score_text, "%.2f", round(obj.score * 1e3) / 1e3);
                    std::string text = obj.label + " " + score_text;
                    cv::putText(img_show, text, cv::Point2i(x1, y1 - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
                    //cv::imwrite("./result.png", img_show);

                    beforeObjList.push_back(obj);
                    std::cout << obj.label << " " << x1 << " " << y1 << " " << obj.width << " " << obj.height << " " << obj.score << std::endl;
        }

        const auto start2 = std::chrono::high_resolution_clock::now();
        //cvtColor(img_show,img_show,cv::COLOR_BGR2RGB);
        QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
        const auto end2 = std::chrono::high_resolution_clock::now();
        const float duration2 = std::chrono::duration<float, std::milli>(end2 - start2).count();
        std::cout << "#########MatToQImageTime: " << duration2 << " ms" << std::endl;

        int objNum=obj_list.size();
        QString re=QString("通道%1 发现%2个告警 Time:%4").arg(channel+1).arg(objNum).arg(QDateTime::currentDateTime().toString("MM_dd_hh_mm"));
        cout<<re.toStdString()<<endl;

        int i=channel-1;
        int width = giveWidth[i];
        int height = giveHeight[i];
        QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        drawRate(image2);
        QPixmap pix = QPixmap::fromImage(image2);

        QString picName=QString("/data/result_img/"+enterDay+"/channel%1_p%2.jpg").arg(i).arg(pushNum);
        dst.save(picName,"JPG",100);
        pushNum++;
        emit sendtoDb(channel,pix,re,picName);
        printf("find helmet\n");
    }else{ 
        int uiLoc=channel-1;
        int width = giveWidth[uiLoc];
        int height = giveHeight[uiLoc];
        QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
        QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        drawRate(image2);
        QPixmap pix = QPixmap::fromImage(image2);
        emit sendtoShow(uiLoc,pix);
    }
}

void AlgoThread::iniCheckPedestrian(){
    //没有初始化，就进行初始化
    if(pedestrian_handle==nullptr){
        int flag = InitPedestrian(pedestrian_handle, string(PEDESTRIAN_JSON));
        if (flag != 0) {
                printf("channel%d checkPedestrian fail\n",channel);
                UnInitPedestrian(pedestrian_handle);
                return;
        }else{
             printf("channel%d checkPedestrian OK\n",channel);
        }
    }
}

int AlgoThread::checkPedestrian(int channel,QImage src,bool mustDeal){
    //没有初始化，就进行初始化
    if(pedestrian_handle==nullptr){
        printf("channel%d checkPedestrian is not init\n",channel);
        return 0;
    }

    cv::Mat image1(src.height(),src.width(),CV_8UC4,(void *)src.constBits());
    cv::Mat image;
    cv::cvtColor(image1, image, cv::COLOR_RGBA2BGR);
    cv::Mat img_show;
    image.copyTo(img_show);

    if(!mustDeal){
            for (auto obj : beforeObjList) {
                        int x1 = int(obj.cx - obj.width / 2);
                        int y1 = int(obj.cy - obj.height / 2);
                        cv::rectangle(img_show, cv::Point2i(x1, y1), cv::Point2i(x1 + obj.width, y1 + obj.height), color_map5[obj.label]);
                        cv::rectangle(img_show, cv::Point2i(x1, y1 - 20), cv::Point2i(x1 + 100, y1), color_map5[obj.label], -1);
            }

            int uiLoc=channel-1;
            int width = giveWidth[uiLoc];
            int height = giveHeight[uiLoc];
            QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
            QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            drawRate(image2);
            QPixmap pix = QPixmap::fromImage(image2);
            emit sendtoShow(uiLoc,pix);
            return 0;
    }


    std::vector<ObjectResult> obj_list;

    const auto start = std::chrono::high_resolution_clock::now();

    DetPedestrian(pedestrian_handle, image.data, image.cols, image.rows, image.channels(), obj_list);

    const auto end = std::chrono::high_resolution_clock::now();
    const float duration = std::chrono::duration<float, std::milli>(end - start).count();
    std::cout << "#########inferenceTime: " << duration << " ms" << std::endl;

    if(obj_list.size()>0){
        beforeObjList.clear();
        for (auto obj : obj_list) {
                    int x1 = int(obj.cx - obj.width / 2);
                    int y1 = int(obj.cy - obj.height / 2);
                    cv::rectangle(img_show, cv::Point2i(x1, y1), cv::Point2i(x1 + obj.width, y1 + obj.height), color_map5[obj.label]);
                    cv::rectangle(img_show, cv::Point2i(x1, y1 - 20), cv::Point2i(x1 + 100, y1), color_map5[obj.label], -1);

                    char score_text[80];
                    sprintf(score_text, "%.2f", round(obj.score * 1e3) / 1e3);
                    std::string text = obj.label + " " + score_text;
                    cv::putText(img_show, text, cv::Point2i(x1, y1 - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
                    beforeObjList.push_back(obj);
                    std::cout << obj.label << " " << x1 << " " << y1 << " " << obj.width << " " << obj.height << " " << obj.score << std::endl;
       }

       const auto start2 = std::chrono::high_resolution_clock::now();
       cvtColor(img_show,img_show,cv::COLOR_BGR2RGB);
       QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
       const auto end2 = std::chrono::high_resolution_clock::now();
       const float duration2 = std::chrono::duration<float, std::milli>(end2 - start2).count();
       std::cout << "#########MatToQImageTime: " << duration2 << " ms" << std::endl;

        int objNum=obj_list.size();
        QString re=QString("通道%1 发现%2个告警 Time:%4").arg(channel+1).arg(objNum).arg(QDateTime::currentDateTime().toString("MM_dd_hh_mm"));
        cout<<re.toStdString()<<endl;
        drawRate(dst);

        int i=channel-1;
        int width = giveWidth[i];
        int height = giveHeight[i];
        QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        drawRate(image2);
        QPixmap pix = QPixmap::fromImage(image2);

        QString picName=QString("/data/result_img/"+enterDay+"/channel%1_p%2.jpg").arg(i).arg(pushNum);
        dst.save(picName,"JPG",100);
        pushNum++;
        emit sendtoDb(channel,pix,re,picName);
        printf("find Pedestrian\n");
    }else{
        int uiLoc=channel-1;
        int width = giveWidth[uiLoc];
        int height = giveHeight[uiLoc];
        QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
        QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        drawRate(image2);
        QPixmap pix = QPixmap::fromImage(image2);
        emit sendtoShow(uiLoc,pix);
    }
}

void AlgoThread::iniCheckDeepFace(){
    //没有初始化，就进行初始化
    if(deepface_handle==nullptr){
        int flag = InitDeepFace(deepface_handle, string(DEEPFACE_JSON));
        if (flag != 0) {
                printf("channel%d check deep face is fail\n",channel);
                UnInitDeepFace(deepface_handle);
                return;
        }else{
             printf("channel%d deep face is OK\n",channel);
        }
    }
}

int AlgoThread::checkDeepFace(int channel,QImage src,bool mustDeal){
    //没有初始化，就进行初始化
    if(deepface_handle==nullptr){
        printf("channel%d deep face is not init\n",channel);
        return 0;
    }
    //tempNum++;
    //src.save(QString("src%1.png").arg(tempNum),"PNG",100);

    cv::Mat image(src.height(),src.width(),CV_8UC4,(void *)src.constBits());
    cv::Mat img1;
    cvtColor(image,img1,cv::COLOR_RGBA2BGR);
    cv::Mat img_show;
    img1.copyTo(img_show);

    if(!mustDeal){
            for (auto obj : beforeObjList) {
                        int x1 = int(obj.cx - obj.width / 2);
                        int y1 = int(obj.cy - obj.height / 2);
                        cv::rectangle(img_show, cv::Point2i(x1, y1), cv::Point2i(x1 + obj.width, y1 + obj.height), color_map["person"]);
                        //cv::rectangle(img_show, cv::Point2i(x1, y1 - 20), cv::Point2i(x1 + 100, y1), color_map[obj.label], -1);
            }

            int uiLoc=channel-1;
            int width = giveWidth[uiLoc];
            int height = giveHeight[uiLoc];
            QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
            QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            drawRate(image2);
            QPixmap pix = QPixmap::fromImage(image2);
            emit sendtoShow(uiLoc,pix);
            return 0;
    }


    DeepFaceResult deepfaceResult_01;

    const auto start = std::chrono::high_resolution_clock::now();
    //int nstatus_01 = DetFace(deepface_handle, img_gray_01.data,
    //                         img_gray_01.rows, img_gray_01.cols, 1, ImageFormat::PIX_FORMAT_GRAY, deepfaceResult_01);
    int nstatus_01 = DetFace(deepface_handle, img1.data,
                             img1.rows, img1.cols, 3, deepfaceResult_01);

    const auto end = std::chrono::high_resolution_clock::now();
    const float duration = std::chrono::duration<float, std::milli>(end - start).count();
    std::cout <<" x2inferenceTime: " << duration << " ms" << std::endl;

    beforeObjList.clear();
    if (nstatus_01 == 1) {

           cout << "step1" <<endl;
           GetFaceFeature(deepface_handle, deepfaceResult_01);
           int x1 = deepfaceResult_01.face_rt.x;
           int y1 = deepfaceResult_01.face_rt.y;
           int x2 = deepfaceResult_01.face_rt.x + deepfaceResult_01.face_rt.width;
           int y2 = deepfaceResult_01.face_rt.y + deepfaceResult_01.face_rt.height;
           cout << "face box: " << x1 << "," << y1 << "," << x2 << "," << y2 << std::endl;
           ObjectResult temp;
           temp.cx=x1+deepfaceResult_01.face_rt.width/2;
           temp.cy=y1+deepfaceResult_01.face_rt.height/2;
           temp.width=deepfaceResult_01.face_rt.width;
           temp.height=deepfaceResult_01.face_rt.height;

           beforeObjList.push_back(temp);
           rectangle(img1, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(255,255,0), 2);

           const auto start2 = std::chrono::high_resolution_clock::now();
           QImage dst((uchar*)img1.data, img1.cols, img1.rows,img1.cols*3,QImage::Format_RGB888);
           const auto end2 = std::chrono::high_resolution_clock::now();
           const float duration2 = std::chrono::duration<float, std::milli>(end2 - start2).count();
           std::cout << "#########checkface MatToQImageTime: " << duration2 << " ms" << std::endl;
           //dst.save(QString("gray%1.png").arg(tempNum),"PNG",100);

           int objNum=1;
           QString re=QString("通道%1 发现%2个告警 Time:%4").arg(channel).arg(objNum).arg(QDateTime::currentDateTime().toString("MM_dd_hh_mm"));
           cout<<re.toStdString()<<endl;

           int uiLoc=channel-1;
           int with = giveWidth[uiLoc];
           int height = giveHeight[uiLoc];
           QImage image2=dst.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
           drawRate(image2);
           QPixmap pix = QPixmap::fromImage(image2);

           QString picName=QString("/data/result_img/"+enterDay+"/channel%1_p%2.jpg").arg(channel).arg(pushNum);
           dst.save(picName,"JPG",100);
           pushNum++;
           emit sendtoDb(uiLoc,pix,re,picName);
           printf("find deep face\n");
    }else{

        int uiLoc=channel-1;
        int width = giveWidth[uiLoc];
        int height = giveHeight[uiLoc];
        QImage dst((uchar*)img_show.data, img_show.cols, img_show.rows,img_show.cols*3,QImage::Format_RGB888);
        QImage image2=dst.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        drawRate(image2);
        QPixmap pix = QPixmap::fromImage(image2);
        emit sendtoShow(uiLoc,pix);
    }
}


//1 安全帽 2行人车辆 3人脸识别 4口罩 5抽烟
int AlgoThread::runAlgo(int channel,const QImage src,int algo,bool mustDeal){
    //安全帽检测
    if(algo==1){
         checkHelmet(channel,src,mustDeal);
    }else if(algo==2){
         checkPedestrian(channel,src,mustDeal);
    }else if(algo==3){
         checkDeepFace(channel,src,mustDeal);
    }else if(algo==4){
        checkMasks(channel,src,mustDeal);
    }else if(algo==5){
        checkSmoke(channel,src,mustDeal);
    }else if(algo==6){
        checkPlate(channel,src,mustDeal);
    }
}

void AlgoThread::drawRate(QImage &image2){
    QPainter* painter = new QPainter(&image2); // sorry i forgot the "&"
    painter->setPen(Qt::blue);
    painter->setFont(QFont("Arial", 10));

    // you probably want the to draw the text to the rect of the image
    painter->drawText(image2.rect(), Qt::AlignTop, QString::number(rate,'f',1));
    delete painter;
}

void AlgoThread::initAlgo(){
     int i=channel-1;
     int loc=findNodeLoc(i+1);
     hostNode hn=hostList[loc];
     if(hn.algo==1){
          iniCheckHelmet();
     }else if(hn.algo==2){
         iniCheckPedestrian();
     }else if(hn.algo==3){
         iniCheckDeepFace();
     }else if(hn.algo==4){
         iniCheckMasks();
     }else if(hn.algo==5){
        iniCheckSmoke();
     }else if(hn.algo==6){
        iniCheckPlate();
     }
}

void  AlgoThread::run()
{
   beforeObjList.clear();

    auto rateBegin = std::chrono::high_resolution_clock::now();
    while(1){
        if(algoEnable){
            int i=channel-1;
            //时间戳没有变化，说明不需要处理
            if(beforeTimeStamp==channelTimeStamp[channel-1]){
                  msleep(1);
                  continue;
            }
            beforeTimeStamp=channelTimeStamp[channel-1];
            frameNum++;

            if(frameNum%10==0){
                auto rateEnd = std::chrono::high_resolution_clock::now();

                float rateDiff=(std::chrono::duration<float, std::milli>(rateEnd - rateBegin).count())/10;
                if(rateDiff>0){
                    rate=1000/rateDiff;
                }
                rateBegin=rateEnd;
            }

            QImage image(imageBuff[i].image,imageBuff[i].width,imageBuff[i].height,imageBuff[i].format);
            int loc=findNodeLoc(i+1);
            //找到了通道，需要运行算法
            if(loc>=0){
                    hostNode hn=hostList[loc];
                    int waitAlgoNum=hn.waitAlgoNum;
                    if(waitAlgoNum<1){
                        waitAlgoNum=1;
                    }

                    if(hn.algo>=1){
                        const auto start = std::chrono::high_resolution_clock::now();
                        if(frameNum%hn.waitAlgoNum==0){
                             runAlgo(channel,image,hn.algo,true);
                        }else{
                             runAlgo(channel,image,hn.algo,false);
                        }

                        const auto end = std::chrono::high_resolution_clock::now();
                        const float duration = std::chrono::duration<float, std::milli>(end - start).count();
                        //std::cout << "&&&&"<< i<<"runAlgoTime: " << duration << " ms" << std::endl;
                        msleep(1);
                    }else{
                        int width = giveWidth[i];
                        int height = giveHeight[i];

                        QImage image2=image.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                        drawRate(image2);
                        QPixmap pix = QPixmap::fromImage(image2);
                        emit sendtoShow(i,pix);

                        msleep(1);
                    }
             }
        }else{
             frameNum=0;
        }
         msleep(1);
    }
}
