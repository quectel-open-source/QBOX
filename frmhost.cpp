#include "frmhost.h"
#include "ui_frmhost.h"
#include "myhelper.h"
#include <iostream>
#include <csignal>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <json/config.h>
#include <json/json.h>
#include <json/reader.h>
#include <QFile>
#include <ffthread.h>
#include "algothread.h"

using namespace std;
extern int waitAlgoNum;
extern AlgoThread algoList[];

extern vector<hostNode> hostList;
vector<FfThread*> ffThreadList;

QString algoStr[]={"无","安全帽识别","行人车辆识别","人脸识别","口罩识别","抽烟识别","车牌识别"};
QString typeStr[]={"IPC","FILE"};

extern QString hostJson;
extern bool algoEnable;

void frmHost::initHost(){
    ui->listHost->clear();
    for(int i=0;i<hostList.size();i++){
        hostNode hn=hostList[i];
        int hc=hn.channel;
        QString istr=QString("通道%1 算法：%2").arg(hc).arg(algoStr[hn.algo]);
        ui->listHost->addItem(istr);
    }
}


void frmHost::chooseHost(QListWidgetItem *item)
{
    int index=ui->listHost->row(item);

    hostNode hn=hostList[index];
    ui->addressLine->setText(QString::fromStdString(hn.address));
    ui->typeBox->setCurrentIndex(hn.hostType);
    ui->channelBox->setCurrentIndex(hn.channel-1);
    ui->algoBox->setCurrentIndex(hn.algo);
    ui->waitAlgoNumLine->setText(QString::number(hn.waitAlgoNum));
    ui->waitFrameLine->setText(QString::number(hn.waitFrame));
}

void frmHost::btnAdd_clicked(){
    cout<<" add btn"<<endl;

    int nowHc=ui->channelBox->currentIndex()+1;
    int nowIndex=0;

    int isFind=0;
    for(int i=0;i<hostList.size();i++){
        hostNode hn=hostList[i];
        int hc=hn.channel;
        if(hc==nowHc){
            nowIndex=i;
            isFind=1;
            break;
        }
    }
    //如果没有就添加
    if(isFind==0){
        hostNode hn;
        hn.channel=nowHc;
        hn.address=ui->addressLine->text().toStdString();
        hn.hostType=ui->typeBox->currentIndex();
        hn.algo=ui->algoBox->currentIndex();
        hn.waitFrame=ui->waitFrameLine->text().toInt();
        hn.waitAlgoNum=ui->waitAlgoNumLine->text().toInt();
        hostList.push_back(hn);


    }else{
        //有就提醒是否覆盖
        switch(QMessageBox::question(this,"通道重复","你是否想覆盖设置，请选择",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Cancel))
        {
            case QMessageBox::Ok:
                cout<<"Questiong选择Ok"<<endl;
                hostList[nowIndex].algo=ui->algoBox->currentIndex();
                hostList[nowIndex].address=ui->addressLine->text().toStdString();
                hostList[nowIndex].hostType=ui->typeBox->currentIndex();
                hostList[nowIndex].waitFrame=ui->waitFrameLine->text().toInt();
                hostList[nowIndex].waitAlgoNum=ui->waitAlgoNumLine->text().toInt();
                break;
            case QMessageBox::Cancel:
                cout<<"Questiong选择Cancel\n"<<endl;
                break;
            default:
                break;
       }
    }
    initHost();
}

void frmHost::btnDel_clicked(){
    cout<<" del btn"<<endl;
    int nowHc=ui->channelBox->currentIndex()+1;
    int nowIndex=0;

    int isFind=0;
    for(int i=0;i<hostList.size();i++){
        hostNode hn=hostList[i];
        int hc=hn.channel;
        if(hc==nowHc){
            hostList.erase(hostList.begin()+i);
            break;
        }
    }
    initHost();
}

void frmHost::btnSave_clicked(){
    cout<<" save btn"<<endl;
    if (hostList.size() == 0){
            QMessageBox mgBox(QMessageBox::Critical, "错误", "主机列表为空!!",
                              QMessageBox::Yes | QMessageBox::No, this);
            mgBox.setFixedSize(400, 300);
            mgBox.exec();
            return;
        }

        Json::Value root;
        Json::Value run;

        for(int i=0;i<hostList.size();i++){
            hostNode hn=hostList[i];
            QString channelName=QString("channel%1").arg(hn.channel);
            Json::Value devalue;
            devalue["address"]=hn.address;
            devalue["hostType"]=hn.hostType;
            devalue["algo"]=hn.algo;
            devalue["waitAlgoNum"]=hn.waitAlgoNum;
            devalue["waitFrame"]=hn.waitFrame;
            cout<<"i"<<i<<" "<<hn.hostType<<" "<<hn.address<<endl;
            root[channelName.toStdString()]=devalue;
        }

        // 将json内容（缩进格式）输出到文件
        Json::StyledWriter writer;
        ofstream os;
        os.open(hostJson.toLatin1());
        os << writer.write(root);
        os.close();

        QMessageBox::warning(this,"QBox","保存成功！");
        printf("end write json_path: success!  %s\n",hostJson.toLatin1().data());
        return;
}

frmHost::frmHost(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmHost)
{
    //将所有线程停止并清空
    for(int i=0;i<ffThreadList.size();i++){
        ffThreadList[i]->isStop=true;
        ffThreadList[i]->quit();
        ffThreadList[i]->wait();
        ffThreadList[i]->ffmpeg->deInit();
        //释放指针
        free(ffThreadList[i]->ffmpeg);
        free(ffThreadList[i]);
    }
    ffThreadList.clear();
    algoEnable=false;

    ui->setupUi(this);
    connect(ui->listHost, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(chooseHost(QListWidgetItem *)));
    connect(ui->btnAdd, &QPushButton::clicked, this, &frmHost::btnAdd_clicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &frmHost::btnDel_clicked);
    connect(ui->btnSave, &QPushButton::clicked, this, &frmHost::btnSave_clicked);
    printf("enter frmhost\n");
    //窗体居中，并且只有关闭按钮，不能调整大小
    myHelper::FormOnlyCloseInCenter(this);

    initHost(); 
}

frmHost::~frmHost()
{
    printf("exit frmhost\n");
    //关闭后，就把线程启动
    for(int i=0;i<hostList.size();i++){
        hostNode hn=hostList[i];
        FfThread* ft=new FfThread();
        ft->ffmpeg=new QFFmpeg();

        ft->channel=hn.channel;
        ft->address=hn.address;
        ft->hostType=hn.hostType;
        ft->algo=hn.algo;
        ft->waitFrame=hn.waitFrame;

        printf("channel%d address%s\n",ft->channel,ft->address.c_str());
        ft->start();
        ffThreadList.push_back(ft);
    }

    for(int i=0;i<16;i++){
        algoList[i].initAlgo();
    }
    algoEnable=true;

    delete ui;
}

