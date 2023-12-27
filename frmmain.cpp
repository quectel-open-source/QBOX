#include "frmmain.h"
#include "ui_frmmain.h"
#include "myhelper.h"
#include "frmconfig.h"

#include "frmhost.h"
#include "frmdefence.h"
#include "frmdata.h"
#include "frmpwd.h"
#include "frmlogout.h"

#include <fstream>
#include <iostream>
#include <json/config.h>
#include <json/json.h>
#include <json/reader.h>
#include <QFile>
#include "algothread.h"
#include "imageWin.h"

extern bool algoEnable;

typedef struct resultPicItem
{
    QString picName;
    QString title;
}resultPic;
vector<resultPic> resultPicList;

vector<hostNode> hostList;
QString hostJson("host.json");


#define GRIDW 180
#define GRIDH 100

int giveWidth[16]={184,184,184,184,184,184,184,184,184,184,184,184,184,184,184,184};
int giveHeight[16]={100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100};
int channelTimeStamp[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
AlgoThread algoList[16];

QString enterDay;
void frmMain::enlargeImage(QListWidgetItem *item)
{
    QRect rect = this->geometry();//获取当前窗口坐标及大小 x、y、w、h

    //通过自定义的窗口显示图片
    imageWin *showImageWidget = new imageWin(item , QRect(rect.x()+rect.width()*0.1, rect.y()+rect.height()*0.1, rect.width()*0.8, rect.height()*0.8));
    showImageWidget->show();
}

void frmMain::chooseImage(QListWidgetItem *item)
{
    cout<<item->text().toStdString()<<endl;
    ui->textEdit->append(item->text());
}

void frmMain::refreshList(){
    ui->showList->clear();

    int addNum=0;
    //动态添加item到QListWidget里
    for(int i=resultPicList.size()-1;i>=0;i--)
    {
        resultPic rp=resultPicList[i];
        QListWidgetItem *newitem=new QListWidgetItem(QIcon(":/image/move.png"),rp.title);
        newitem->setToolTip(rp.picName);
        newitem->setText(rp.title);
        newitem->setTextAlignment(Qt::AlignLeft);//设置文字对齐方式：水平居中
        ui->showList->addItem(newitem);
        addNum++;
        if(addNum>30){
            break;
        }
    }
}


void frmMain::SetImage(int i,QPixmap pix)
{   
    QObject::sender()->blockSignals(true);
    widgets.at(i)->setPixmap(pix);
    QObject::sender()->blockSignals(false);
}

void frmMain::SetDb(int i, QPixmap pix,QString title,QString picName){
    QObject::sender()->blockSignals(true);
    widgets.at(i)->setPixmap(pix);

    resultPic rp;
    rp.picName=picName;
    rp.title=title;
    resultPicList.push_back(rp);
    while(resultPicList.size()>30){
        resultPicList.erase(resultPicList.begin());
    }
    printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$resultsize:%d\n",resultPicList.size());

    refreshList();
    QObject::sender()->blockSignals(false);
}

//------------以下复制于16路视频代码---------
void frmMain::initMenu(){
    resultPicList.reserve(50);
    videoMenu = new QMenu(this);
    videoMenu->addAction("截图当前视频", this, SLOT(snapshot_video_one()));
    videoMenu->addAction("截图所有视频", this, SLOT(snapshot_video_all()));
    videoMenu->addSeparator();

    QMenu *menu4 = videoMenu->addMenu("切换到4画面");
    menu4->addAction("通道1-通道4", this, SLOT(show_video_4()));
    menu4->addAction("通道5-通道8", this, SLOT(show_video_4()));
    menu4->addAction("通道9-通道12", this, SLOT(show_video_4()));
    menu4->addAction("通道13-通道16", this, SLOT(show_video_4()));

    QMenu *menu6 = videoMenu->addMenu("切换到6画面");
    menu6->addAction("通道1-通道6", this, SLOT(show_video_6()));
    menu6->addAction("通道6-通道11", this, SLOT(show_video_6()));
    menu6->addAction("通道11-通道16", this, SLOT(show_video_6()));

    QMenu *menu8 = videoMenu->addMenu("切换到8画面");
    menu8->addAction("通道1-通道8", this, SLOT(show_video_8()));
    menu8->addAction("通道9-通道16", this, SLOT(show_video_8()));

    QMenu *menu9 = videoMenu->addMenu("切换到9画面");
    menu9->addAction("通道1-通道9", this, SLOT(show_video_9()));
    menu9->addAction("通道8-通道16", this, SLOT(show_video_9()));

    videoMenu->addAction("切换到16画面", this, SLOT(show_video_16()));

}

void frmMain::initForm(){
    //设置样式表
    QStringList qss;
    qss.append("QFrame{border:0px solid #000000;}");
    qss.append("QLabel{font:75 25px;color:#F0F0F0;border:0.2px solid #090909;background:#000000;}");
    qss.append("QLabel:focus{border:2px solid #00BB9E;background:#555555;}");
    ui->frame->setStyleSheet(qss.join(""));

    videoMax = false;
    videoCount = 16;
    videoType = "1_16";

    for (int i = 0; i < videoCount; i++) {
        QLabel *widget = new QLabel;
        widget->setObjectName(QString("video%1").arg(i + 1));
        widget->installEventFilter(this);
        widget->setFocusPolicy(Qt::StrongFocus);
        widget->setAlignment(Qt::AlignCenter);

        //二选一可以选择显示文字,也可以选择显示背景图片
        widget->setText(QString("通道 %1").arg(i + 1));
        //选择背景图片
        //widget->setPixmap(QPixmap(":/bg_novideo.png"));
        widgets.append(widget);
    }

}

void frmMain::snapshot_video_one()
{

}

void frmMain::snapshot_video_all()
{

}

void frmMain::show_video_all()
{
    if (videoType == "1_4") {
        change_video_4(0);
    } else if (videoType == "5_8") {
        change_video_4(4);
    } else if (videoType == "9_12") {
        change_video_4(8);
    } else if (videoType == "13_16") {
        change_video_4(12);
    } else if (videoType == "1_6") {
        change_video_6(0);
    } else if (videoType == "6_11") {
        change_video_6(5);
    } else if (videoType == "11_16") {
        change_video_6(10);
    } else if (videoType == "1_8") {
        change_video_8(0);
    } else if (videoType == "9_16") {
        change_video_8(8);
    } else if (videoType == "1_9") {
        change_video_9(0);
    } else if (videoType == "8_16") {
        change_video_9(7);
    } else if (videoType == "1_16") {
        change_video_16(0);
    }

}

void frmMain::show_video_4()
{
    videoMax = false;
    QString videoType;
    int index = 0;

    QAction *action = (QAction *)sender();
    QString name = action->text();

    if (name == "通道1-通道4") {
        index = 0;
        videoType = "1_4";
    } else if (name == "通道5-通道8") {
        index = 4;
        videoType = "5_8";
    } else if (name == "通道9-通道12") {
        index = 8;
        videoType = "9_12";
    } else if (name == "通道13-通道16") {
        index = 12;
        videoType = "13_16";
    }

    if (this->videoType != videoType) {
        this->videoType = videoType;
        change_video_4(index);
    }
}

void frmMain::show_video_6()
{
    videoMax = false;
    QString videoType;
    int index = 0;

    QAction *action = (QAction *)sender();
    QString name = action->text();

    if (name == "通道1-通道6") {
        index = 0;
        videoType = "1_6";
    } else if (name == "通道6-通道11") {
        index = 5;
        videoType = "6_11";
    } else if (name == "通道11-通道16") {
        index = 10;
        videoType = "11_16";
    }

    if (this->videoType != videoType) {
        this->videoType = videoType;
        change_video_6(index);
    }
}

void frmMain::show_video_8()
{
    videoMax = false;
    QString videoType;
    int index = 0;

    QAction *action = (QAction *)sender();
    QString name = action->text();

    if (name == "通道1-通道8") {
        index = 0;
        videoType = "1_8";
    } else if (name == "通道9-通道16") {
        index = 8;
        videoType = "9_16";
    }

    if (this->videoType != videoType) {
        this->videoType = videoType;
        change_video_8(index);
    }
}

void frmMain::show_video_9()
{
    videoMax = false;
    QString videoType;
    int index = 0;

    QAction *action = (QAction *)sender();
    QString name = action->text();

    if (name == "通道1-通道9") {
        index = 0;
        videoType = "1_9";
    } else if (name == "通道8-通道16") {
        index = 7;
        videoType = "8_16";
    }

    if (this->videoType != videoType) {
        this->videoType = videoType;
        change_video_9(index);
    }
}

void frmMain::show_video_16()
{
    videoMax = false;
    QString videoType;
    int index = 0;
    videoType = "1_16";

    if (this->videoType != videoType) {
        this->videoType = videoType;
        change_video_16(index);
    }
}

void frmMain::hide_video_all()
{
    for (int i = 0; i < videoCount; i++) {
        ui->gridLayout->removeWidget(widgets.at(i));
        widgets.at(i)->setVisible(false);
    }
}

void frmMain::change_video(int index, int flag)
{
    int count = 0;
    int row = 0;
    int column = 0;

    for (int i = 0; i < videoCount; i++) {
        if (i >= index) {
            ui->gridLayout->addWidget(widgets.at(i), row, column);
            widgets.at(i)->setVisible(true);

            count++;
            column++;
            if (column == flag) {
                row++;
                column = 0;
            }
        }

        //printf("w%d,h%d\n",widgets.at(i)->width(),widgets.at(i)->height());
        if (count == (flag * flag)) {
            break;
        }
    }
}

void frmMain::change_video_4(int index)
{
    hide_video_all();
    change_video(index, 2);
    for (int i = 0; i < 16; i++) {
        giveWidth[i]=GRIDW*2;
        giveHeight[i]=GRIDH*2;
    }
}

void frmMain::change_video_6(int index)
{
    hide_video_all();
    if (index == 0) {
        ui->gridLayout->addWidget(widgets.at(0), 0, 0, 2, 2);
        ui->gridLayout->addWidget(widgets.at(1), 0, 2, 1, 1);
        ui->gridLayout->addWidget(widgets.at(2), 1, 2, 1, 1);
        ui->gridLayout->addWidget(widgets.at(3), 2, 2, 1, 1);
        ui->gridLayout->addWidget(widgets.at(4), 2, 1, 1, 1);
        ui->gridLayout->addWidget(widgets.at(5), 2, 0, 1, 1);

        for (int i = 0; i < 6; i++) {
            if(i==0){
                giveWidth[i]=GRIDW*3;
                giveHeight[i]=GRIDH*3;
            }else{
                giveWidth[i]=GRIDW;
                giveHeight[i]=GRIDH;
            }

            widgets.at(i)->setVisible(true);
        }
    } else if (index == 5) {
        ui->gridLayout->addWidget(widgets.at(5), 0, 0, 2, 2);
        ui->gridLayout->addWidget(widgets.at(6), 0, 2, 1, 1);
        ui->gridLayout->addWidget(widgets.at(7), 1, 2, 1, 1);
        ui->gridLayout->addWidget(widgets.at(8), 2, 2, 1, 1);
        ui->gridLayout->addWidget(widgets.at(9), 2, 1, 1, 1);
        ui->gridLayout->addWidget(widgets.at(10), 2, 0, 1, 1);

        for (int i = 5; i < 11; i++) {
            if(i==5){
                giveWidth[i]=GRIDW*3;
                giveHeight[i]=GRIDH*3;
            }else{
                giveWidth[i]=GRIDW;
                giveHeight[i]=GRIDH;
            }
            widgets.at(i)->setVisible(true);
        }
    } else if (index == 10) {
        ui->gridLayout->addWidget(widgets.at(10), 0, 0, 2, 2);
        ui->gridLayout->addWidget(widgets.at(11), 0, 2, 1, 1);
        ui->gridLayout->addWidget(widgets.at(12), 1, 2, 1, 1);
        ui->gridLayout->addWidget(widgets.at(13), 2, 2, 1, 1);
        ui->gridLayout->addWidget(widgets.at(14), 2, 1, 1, 1);
        ui->gridLayout->addWidget(widgets.at(15), 2, 0, 1, 1);

        for (int i = 10; i < 16; i++) {
            if(i==10){
                giveWidth[i]=GRIDW*3;
                giveHeight[i]=GRIDH*3;
            }else{
                giveWidth[i]=GRIDW;
                giveHeight[i]=GRIDH;
            }
            widgets.at(i)->setVisible(true);
        }
    }
}

void frmMain::change_video_8(int index)
{
    hide_video_all();
    if (index == 0) {
        ui->gridLayout->addWidget(widgets.at(0), 0, 0, 3, 3);
        ui->gridLayout->addWidget(widgets.at(1), 0, 3, 1, 1);
        ui->gridLayout->addWidget(widgets.at(2), 1, 3, 1, 1);
        ui->gridLayout->addWidget(widgets.at(3), 2, 3, 1, 1);
        ui->gridLayout->addWidget(widgets.at(4), 3, 3, 1, 1);
        ui->gridLayout->addWidget(widgets.at(5), 3, 2, 1, 1);
        ui->gridLayout->addWidget(widgets.at(6), 3, 1, 1, 1);
        ui->gridLayout->addWidget(widgets.at(7), 3, 0, 1, 1);

        for (int i = 0; i < 8; i++) {
            if(i==0){
                giveWidth[i]=GRIDW*3;
                giveHeight[i]=GRIDH*3;
            }else{
                giveWidth[i]=GRIDW;
                giveHeight[i]=GRIDH;
            }

            widgets.at(i)->setVisible(true);
            printf("i%d w%d,h%d\n",i,widgets.at(i)->width(),widgets.at(i)->height());
        }
    } else if (index == 8) {
        ui->gridLayout->addWidget(widgets.at(8), 0, 0, 3, 3);
        ui->gridLayout->addWidget(widgets.at(9), 0, 3, 1, 1);
        ui->gridLayout->addWidget(widgets.at(10), 1, 3, 1, 1);
        ui->gridLayout->addWidget(widgets.at(11), 2, 3, 1, 1);
        ui->gridLayout->addWidget(widgets.at(12), 3, 3, 1, 1);
        ui->gridLayout->addWidget(widgets.at(13), 3, 2, 1, 1);
        ui->gridLayout->addWidget(widgets.at(14), 3, 1, 1, 1);
        ui->gridLayout->addWidget(widgets.at(15), 3, 0, 1, 1);

        for (int i = 8; i < 16; i++) {
            if(i==8){
                giveWidth[i]=GRIDW*3;
                giveHeight[i]=GRIDH*3;
            }else{
                giveWidth[i]=GRIDW;
                giveHeight[i]=GRIDH;
            }

            widgets.at(i)->setVisible(true);
            printf("i%d w%d,h%d\n",i,widgets.at(i)->width(),widgets.at(i)->height());
        }
    }
}

void frmMain::change_video_9(int index)
{
    hide_video_all();
    change_video(index, 3);
    for (int i = 0; i < 16; i++) {
        giveWidth[i]=GRIDW*1.5;
        giveHeight[i]=GRIDH*1.5;
    }
}

void frmMain::change_video_16(int index)
{
    hide_video_all();
    change_video(index, 4);
    for (int i = 0; i < 16; i++) {
        giveWidth[i]=GRIDW;
        giveHeight[i]=GRIDH;
    }
}

bool frmMain::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        QLabel *widget = (QLabel *) watched;
        if (!videoMax) {
            videoMax = true;
            hide_video_all();
            ui->gridLayout->addWidget(widget, 0, 0);
            for (int i = 0; i < 16; i++) {
                giveWidth[i]=GRIDW*4;
                giveHeight[i]=GRIDH*4;
            }
            widget->setVisible(true);
        } else {
            videoMax = false;
            for (int i = 0; i < 16; i++) {
                giveWidth[i]=GRIDW;
                giveHeight[i]=GRIDH;
            }
            show_video_all();
        }
    } else if (event->type() == QEvent::MouseButtonPress) {



        if (qApp->mouseButtons() == Qt::RightButton) {
            videoMenu->exec(QCursor::pos());
        }
    }

    return QWidget::eventFilter(watched, event);
}

//-------------------以上参考视频分割----------

void frmMain::JsonIORead(QString path)
{
        Json::Reader reader;
        Json::Value root;
        std::ifstream in(path.toLatin1(), std::ios::binary);
        cout<<"enter json"<<endl;

        if (reader.parse(in, root))
        {
            for(int i=1;i<=16;i++){
                QString channelName=QString("channel%1").arg(i);
                Json::Value devalue=root[channelName.toStdString()];

                if (!devalue.empty())
                {
                    hostNode hn;
                    hn.channel=i;
                    hn.address=devalue["address"].asString();
                    hn.hostType=devalue["hostType"].asInt();
                    hn.algo=devalue["algo"].asInt();
                    hn.waitAlgoNum=devalue["waitAlgoNum"].asInt();
                    hn.waitFrame=devalue["waitFrame"].asInt();

                    hostList.push_back(hn);
                    cout<<"i"<<i<<" "<<hn.hostType<<" "<<hn.address<<endl;
                }
            }
        }

}



frmMain::frmMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);
    ui->btnCheFang->setVisible(false);
    ui->btnBuFang->setVisible(false);

    enterDay=QDateTime::currentDateTime().toString("MM_dd_hh_mm");
    QDir saveDir(QString("/data/result_img/")+enterDay);
    if(!saveDir.exists())//如果目标目录不存在，则进行创建
    {
        if(!saveDir.mkdir(saveDir.absolutePath()))
        {
            printf("[%s] 创建文件夹error:[%s]\n", __func__, enterDay.toLatin1().data());
        }
        printf("[%s] 创建文件夹success:[%s]\n", __func__, enterDay.toLatin1().data());
    }


    this->initForm();
    this->initMenu();
    this->show_video_all();

    for(int i=0;i<16;i++){
        //AlgoThread* at=new AlgoThread();
        algoList[i].channel=i+1;
        algoList[i].start();
        connect(&algoList[i], SIGNAL(sendtoShow(int, QPixmap)), this, SLOT(SetImage(int, QPixmap)));
        connect(&algoList[i], SIGNAL(sendtoDb(int, QPixmap,QString,QString)), this, SLOT(SetDb(int,QPixmap,QString,QString)));
    }

    JsonIORead(hostJson);

    QLabel * logoLabel=new QLabel();
    logoLabel->setPixmap(QPixmap(":/image/smalllogo.png"));
    logoLabel->resize(190,30);
    ui->mainToolBar->insertWidget(ui->action_host,logoLabel);

    myHelper::FormInCenter(this);
    //connect(ui->action_config,SIGNAL(triggered()),this,SLOT(ShowFrmConfig()));
    connect(ui->action_host,SIGNAL(triggered()),this,SLOT(ShowFrmHost()));
    //connect(ui->action_defence,SIGNAL(triggered()),this,SLOT(ShowFrmDefence()));
    connect(ui->action_data,SIGNAL(triggered()),this,SLOT(ShowFrmData()));    
    //connect(ui->action_exit,SIGNAL(triggered()),this,SLOT(ShowFrmExit()));
    setWindowIcon(QIcon("qrc:/image/smalllogo.png"));

    connect(ui->showList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(chooseImage(QListWidgetItem *)));
    connect(ui->showList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(enlargeImage(QListWidgetItem *)));
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::ShowFrmConfig()
{
    frmConfig *config=new frmConfig;
    config->exec();
}

void frmMain::ShowFrmHost()
{
    frmHost *host=new frmHost;
    host->mfrm=this;
    host->setAttribute(Qt::WA_DeleteOnClose,true);
    host->exec();
}

void frmMain::ShowFrmDefence()
{
    frmDefence *defence=new frmDefence;
    defence->exec();
}

void frmMain::ShowFrmData()
{
    frmData *data=new frmData;
    data->exec();
}

void frmMain::ShowFrmExit()
{
    frmLogout *logout=new frmLogout;
    logout->exec();
}
