#include "myapp.h"
#include "myhelper.h"
#include <QSettings>

//程序运行过程中需要的全局变量
QString myApp::AppPath="";
QString myApp::CurrentImage="main.png";//当前图片名称
bool myApp::IsMove=false;//防区按钮是否可以移动

//存储当前登录用户信息,方便用户密码修改和用户退出操作
QString myApp::CurrentUserName="admin";
QString myApp::CurrentUserPwd="admin";
QString myApp::CurrentUserType="管理员";

//存储全局配置信息,程序启动时加载,关闭时保存
QString myApp::SoftTitle="360安防-报警视频联动管理平台";
QString myApp::SoftVersion="V1.0";
QString myApp::CompanyName="厦门三六零安防设备有限公司";
QString myApp::ContactName="丁工";
QString myApp::ContactTel="15000450819";
QString myApp::LastLoginer="admin";

bool myApp::UseServer=true;
int myApp::ServerPort=6000;
bool myApp::UseSound=false;
QString myApp::SoundName="ALARM1.WAV";
bool myApp::AwalysPlay=false;

bool myApp::AutoRun=false;
bool myApp::AutoLogin=false;
bool myApp::AutoBank=false;
QString myApp::TimeBank="year";

bool myApp::UseLinkMap=false;
QString myApp::LinkMapCOM="COM1";
bool myApp::UseLinkMsg=false;
QString myApp::LinkMsgCOM="COM1";
QString myApp::MsgTel="15000450819;18015622233";
QString myApp::MsgTemplet="防区号|发生报警";

bool myApp::UseLinkNETSend=false;
QString myApp::LinkNETSendAddr1="false|192.168.1.3|5000";
QString myApp::LinkNETSendAddr2="false|192.168.1.4|5000";
QString myApp::LinkNETSendAddr3="false|192.168.1.5|5000";
bool myApp::UseLinkNETReceive=false;
int myApp::LinkNETReceivePort=5000;

//读取配置信息
void myApp::ReadConfig()
{
    QString fileName=myApp::AppPath+"config.txt";
    //如果配置文件不存在,则以初始值继续运行
    //没有这个判断的话,配置赋值都为空
    if (!myHelper::FileIsExist(fileName))
    {
        //对应中文转成正确的编码
        myApp::CurrentUserType=myApp::CurrentUserType.toLatin1();
        myApp::SoftTitle=myApp::SoftTitle.toLatin1();
        myApp::CompanyName=myApp::CompanyName.toLatin1();
        myApp::ContactName=myApp::ContactName.toLatin1();
        myApp::MsgTemplet=myApp::MsgTemplet.toLatin1();
        return;
    }

    QSettings *set=new QSettings(fileName,QSettings::IniFormat);

    myApp::SoftTitle=set->value("AppConfig/SoftTitle").toString();
    myApp::SoftVersion=set->value("AppConfig/SoftVersion").toString();
    myApp::CompanyName=set->value("AppConfig/CompanyName").toString();
    myApp::ContactName=set->value("AppConfig/ContactName").toString();
    myApp::ContactTel=set->value("AppConfig/ContactTel").toString();
    myApp::LastLoginer=set->value("AppConfig/LastLoginer").toString();

    myApp::UseServer=set->value("AppConfig/UseServer").toBool();
    myApp::ServerPort=set->value("AppConfig/ServerPort").toInt();
    myApp::UseSound=set->value("AppConfig/UseSound").toBool();
    myApp::SoundName=set->value("AppConfig/SoundName").toString();
    myApp::AwalysPlay=set->value("AppConfig/AwalysPlay").toBool();

    myApp::AutoRun=set->value("AppConfig/AutoRun").toBool();
    myApp::AutoLogin=set->value("AppConfig/AutoLogin").toBool();
    myApp::AutoBank=set->value("AppConfig/AutoBank").toBool();
    myApp::TimeBank=set->value("AppConfig/TimeBank").toString();

    myApp::UseLinkMap=set->value("AppConfig/UseLinkMap").toBool();
    myApp::LinkMapCOM=set->value("AppConfig/LinkMapCOM").toString();
    myApp::UseLinkMsg=set->value("AppConfig/UseLinkMsg").toBool();
    myApp::LinkMsgCOM=set->value("AppConfig/LinkMsgCOM").toString();
    myApp::MsgTel=set->value("AppConfig/MsgTel").toString();
    myApp::MsgTemplet=set->value("AppConfig/MsgTemplet").toString();

    myApp::UseLinkNETSend=set->value("AppConfig/UseLinkNETSend").toBool();
    myApp::LinkNETSendAddr1=set->value("AppConfig/LinkNETSendAddr1").toString();
    myApp::LinkNETSendAddr2=set->value("AppConfig/LinkNETSendAddr2").toString();
    myApp::LinkNETSendAddr3=set->value("AppConfig/LinkNETSendAddr3").toString();
    myApp::UseLinkNETReceive=set->value("AppConfig/UseLinkNETReceive").toBool();
    myApp::LinkNETReceivePort=set->value("AppConfig/LinkNETReceivePort").toInt();
}

//写入配置信息
void myApp::WriteConfig()
{    
    QString fileName=myApp::AppPath+"config.txt";
    QSettings *set=new QSettings(fileName,QSettings::IniFormat);

    set->beginGroup("AppConfig");

    set->setValue("SoftTitle",myApp::SoftTitle);
    set->setValue("SoftVersion",myApp::SoftVersion);
    set->setValue("CompanyName",myApp::CompanyName);
    set->setValue("ContactName",myApp::ContactName);
    set->setValue("ContactTel",myApp::ContactTel);
    set->setValue("LastLoginer",myApp::LastLoginer);

    set->setValue("UseServer",myApp::UseServer);
    set->setValue("ServerPort",myApp::ServerPort);
    set->setValue("UseSound",myApp::UseSound);
    set->setValue("SoundName",myApp::SoundName);
    set->setValue("AwalysPlay",myApp::AwalysPlay);

    set->setValue("AutoRun",myApp::AutoRun);
    set->setValue("AutoLogin",myApp::AutoLogin);
    set->setValue("AutoBank",myApp::AutoBank);
    set->setValue("TimeBank",myApp::TimeBank);

    set->setValue("UseLinkMap",myApp::UseLinkMap);
    set->setValue("LinkMapCOM",myApp::LinkMapCOM);
    set->setValue("UseLinkMsg",myApp::UseLinkMsg);
    set->setValue("LinkMsgCOM",myApp::LinkMsgCOM);
    set->setValue("MsgTel",myApp::MsgTel);
    set->setValue("MsgTemplet",myApp::MsgTemplet);

    set->setValue("UseLinkNETSend",myApp::UseLinkNETSend);
    set->setValue("LinkNETSendAddr1",myApp::LinkNETSendAddr1);
    set->setValue("LinkNETSendAddr2",myApp::LinkNETSendAddr2);
    set->setValue("LinkNETSendAddr3",myApp::LinkNETSendAddr3);
    set->setValue("UseLinkNETReceive",myApp::UseLinkNETReceive);
    set->setValue("LinkNETReceivePort",myApp::LinkNETReceivePort);

    set->endGroup();
}
