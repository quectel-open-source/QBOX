#include "frmconfig.h"
#include "ui_frmconfig.h"
//#include <QSqlQuery>
#include "myhelper.h"
#include "myapp.h"

frmConfig::frmConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmConfig)
{
    ui->setupUi(this);
    myHelper::FormOnlyCloseInCenter(this);
    this->InitForm();
    this->on_btnjiben_clicked();
}

frmConfig::~frmConfig()
{
    delete ui;
}

void frmConfig::InitForm()
{
    //加载对应报警声音文件
    QDir soundPath(myApp::AppPath+"SOUND");
    QStringList filter;
    filter<<"*.WAV";
    ui->cboxSoundName->addItems(soundPath.entryList(filter));

    //加载配置文件对应界面展示信息
    ui->txtSoftTitle->setText(myApp::SoftTitle);
    ui->txtSoftVersion->setText(myApp::SoftVersion);
    ui->txtCompanyName->setText(myApp::CompanyName);
    ui->txtContactName->setText(myApp::ContactName);
    ui->txtContactTel->setText(myApp::ContactTel);

    ui->btnUseServer->SetCheck(myApp::UseServer);
    ui->txtServerPort->setText(QString::number(myApp::ServerPort));
    ui->btnUseSound->SetCheck(myApp::UseSound);
    ui->cboxSoundName->setCurrentIndex(ui->cboxSoundName->findText(myApp::SoundName));
    ui->btnAwalysPlay->SetCheck(myApp::AwalysPlay);

    ui->btnAutoRun->SetCheck(myApp::AutoRun);
    ui->btnAutoLogin->SetCheck(myApp::AutoLogin);
    ui->btnAutoBank->SetCheck(myApp::AutoBank);
    bool BankYear=myApp::TimeBank=="year"?true:false;
    ui->rbtnYear->setChecked(BankYear);
    ui->rbtnMonth->setChecked(!BankYear);

    for (int i=1;i<20;i++)
    {
        ui->cboxCOMMap->addItem(QString("COM%1").arg(i));
        ui->cboxCOMMsg->addItem(QString("COM%1").arg(i));
    }
    ui->btnLinkMap->SetCheck(myApp::UseLinkMap);
    ui->cboxCOMMap->setCurrentIndex(ui->cboxCOMMap->findText(myApp::LinkMapCOM));
    ui->btnLinkMsg->SetCheck(myApp::UseLinkMap);
    ui->cboxCOMMsg->setCurrentIndex(ui->cboxCOMMsg->findText(myApp::LinkMsgCOM));
    ui->txtMsgTel->setText(myApp::MsgTel);
    ui->txtMsgTemplet->setText(myApp::MsgTemplet);

    ui->btnLinkNETSend->SetCheck(myApp::UseLinkNETSend);
    QStringList addr1=myApp::LinkNETSendAddr1.split("|");
    ui->ckNETAddr1->setChecked(addr1[0]=="true"?true:false);
    ui->txtNETAddr1->setText(addr1[1]);
    ui->txtNETPort1->setText(addr1[2]);
    QStringList addr2=myApp::LinkNETSendAddr2.split("|");
    ui->ckNETAddr2->setChecked(addr2[0]=="true"?true:false);
    ui->txtNETAddr2->setText(addr2[1]);
    ui->txtNETPort2->setText(addr2[2]);
    QStringList addr3=myApp::LinkNETSendAddr3.split("|");
    ui->ckNETAddr3->setChecked(addr3[0]=="true"?true:false);
    ui->txtNETAddr3->setText(addr3[1]);
    ui->txtNETPort3->setText(addr3[2]);
    ui->btnLinkNETRecive->SetCheck(myApp::UseLinkNETReceive);
    ui->txtLinkNETReceivePort->setText(QString::number(myApp::LinkNETReceivePort));

    //加载年月日时分秒下拉框
    for (int i=2012;i<=2030;i++)
    {
        ui->cboxYear->addItem(QString::number(i));
    }

    for (int i=1;i<=12;i++)
    {
        ui->cboxMonth->addItem(QString::number(i));
    }

    for (int i=1;i<=31;i++)
    {
        ui->cboxDay->addItem(QString::number(i));
    }

    for (int i=0;i<24;i++)
    {
        ui->cboxHour->addItem(QString::number(i));
    }

    for (int i=0;i<60;i++)
    {
        ui->cboxMin->addItem(QString::number(i));
        ui->cboxSec->addItem(QString::number(i));
    }

    //加载当前时间
    QStringList Now=QDateTime::currentDateTime().toString("yyyy-M-d-h-m-s").split("-");

    ui->cboxYear->setCurrentIndex(ui->cboxYear->findText(Now[0]));
    ui->cboxMonth->setCurrentIndex(ui->cboxMonth->findText(Now[1]));
    ui->cboxDay->setCurrentIndex(ui->cboxDay->findText(Now[2]));
    ui->cboxHour->setCurrentIndex(ui->cboxHour->findText(Now[3]));
    ui->cboxMin->setCurrentIndex(ui->cboxMin->findText(Now[4]));
    ui->cboxSec->setCurrentIndex(ui->cboxSec->findText(Now[5]));

    //如果当前不是管理员,则限制很多操作
    if (myApp::CurrentUserType!="管理员")
    {
        ui->btnBank->setEnabled(false);
        ui->btnRestore->setEnabled(false);
        ui->btnSetDefault->setEnabled(false);
        ui->btnAdd->setEnabled(false);
        ui->btnUpdate->setEnabled(false);
        ui->btnDelete->setEnabled(false);
        ui->btnOk->setEnabled(false);
    }
}

void frmConfig::on_btnjiben_clicked()
{
    ui->btnjiben->setText("【基本设置】→");
    ui->btnzhuanfa->setText("转发设置");
    ui->btnuser->setText("用户设置");
    ui->btnqita->setText("其他设置");
    ui->gboxjiben->setVisible(true);
    ui->gboxzhuanfa->setVisible(false);
    ui->gboxUser->setVisible(false);
    ui->gboxqita->setVisible(false);
}

void frmConfig::on_btnzhuanfa_clicked()
{
    ui->btnjiben->setText("基本设置");
    ui->btnzhuanfa->setText("【转发设置】→");
    ui->btnuser->setText("用户设置");
    ui->btnqita->setText("其他设置");
    ui->gboxjiben->setVisible(false);
    ui->gboxzhuanfa->setVisible(true);
    ui->gboxUser->setVisible(false);
    ui->gboxqita->setVisible(false);
}

void frmConfig::on_btnuser_clicked()
{
    ui->btnjiben->setText("基本设置");
    ui->btnzhuanfa->setText("转发设置");
    ui->btnuser->setText("【用户设置】→");
    ui->btnqita->setText("其他设置");
    ui->gboxjiben->setVisible(false);
    ui->gboxzhuanfa->setVisible(false);
    ui->gboxUser->setVisible(true);
    ui->gboxqita->setVisible(false);
}

void frmConfig::on_btnqita_clicked()
{
    ui->btnjiben->setText("基本设置");
    ui->btnzhuanfa->setText("转发设置");
    ui->btnuser->setText("用户设置");
    ui->btnqita->setText("【其他设置】→");
    ui->gboxjiben->setVisible(false);
    ui->gboxzhuanfa->setVisible(false);
    ui->gboxUser->setVisible(false);
    ui->gboxqita->setVisible(true);
}

//试听报警声音
void frmConfig::on_btnPlay_clicked()
{
    myHelper::PlaySound(myApp::AppPath+"sound/"+ui->cboxSoundName->currentText());
}

void frmConfig::on_btnOk_clicked()
{
    //保存配置文件信息
    myApp::SoftTitle=ui->txtSoftTitle->text();
    myApp::SoftVersion=ui->txtSoftVersion->text();
    myApp::CompanyName=ui->txtCompanyName->text();
    myApp::ContactName=ui->txtContactName->text();
    myApp::ContactTel=ui->txtContactTel->text();
    myApp::LastLoginer=myApp::CurrentUserName;

    myApp::UseServer=ui->btnUseServer->GetCheck();
    myApp::ServerPort=ui->txtServerPort->text().toInt();
    myApp::UseSound=ui->btnUseSound->GetCheck();
    myApp::SoundName=ui->cboxSoundName->currentText();
    myApp::AwalysPlay=ui->btnAwalysPlay->GetCheck();

    myApp::AutoRun=ui->btnAutoRun->GetCheck();
    myApp::AutoLogin=ui->btnAutoLogin->GetCheck();
    myApp::AutoBank=ui->btnAutoBank->GetCheck();
    myApp::TimeBank=(ui->rbtnYear->isChecked()?"year":"month");

    myApp::UseLinkMap=ui->btnLinkMap->GetCheck();
    myApp::LinkMapCOM=ui->cboxCOMMap->currentText();
    myApp::UseLinkMsg=ui->btnLinkMsg->GetCheck();
    myApp::LinkMsgCOM=ui->cboxCOMMsg->currentText();
    myApp::MsgTel=ui->txtMsgTel->text();
    myApp::MsgTemplet=ui->txtMsgTemplet->text();

    myApp::UseLinkNETSend=ui->btnLinkNETSend->GetCheck();

    QString IP1=ui->txtNETAddr1->text();
    if (!myHelper::IsIP(IP1))
    {
        myHelper::ShowMessageBoxError("IP地址有误,请重新输入！");
        ui->txtNETAddr1->setFocus();
        return;
    }

    QString addr1=QString("%1|%2|%3")
            .arg(ui->ckNETAddr1->isChecked()?"true":"false")
            .arg(IP1)
            .arg(ui->txtNETPort1->text());
    myApp::LinkNETSendAddr1=addr1;

    QString IP2=ui->txtNETAddr2->text();
    if (!myHelper::IsIP(IP2))
    {
        myHelper::ShowMessageBoxError("IP地址有误,请重新输入！");
        ui->txtNETAddr2->setFocus();
        return;
    }

    QString addr2=QString("%1|%2|%3")
            .arg(ui->ckNETAddr2->isChecked()?"true":"false")
            .arg(IP2)
            .arg(ui->txtNETPort2->text());
    myApp::LinkNETSendAddr2=addr2;

    QString IP3=ui->txtNETAddr3->text();
    if (!myHelper::IsIP(IP3))
    {
        myHelper::ShowMessageBoxError("IP地址有误,请重新输入！");
        ui->txtNETAddr3->setFocus();
        return;
    }

    QString addr3=QString("%1|%2|%3")
            .arg(ui->ckNETAddr3->isChecked()?"true":"false")
            .arg(IP3)
            .arg(ui->txtNETPort3->text());
    myApp::LinkNETSendAddr3=addr3;

    myApp::UseLinkNETReceive=ui->btnLinkNETRecive->GetCheck();
    myApp::LinkNETReceivePort=ui->txtLinkNETReceivePort->text().toInt();

    //调用保存配置文件函数
    myApp::WriteConfig();
    done(1);//关闭界面返回
    this->close();
}

void frmConfig::on_btnClose_clicked()
{
    done(0);//关闭界面返回
    this->close();
}

//修改系统时间
void frmConfig::on_btnSetTime_clicked()
{
    if (myHelper::ShowMessageBoxQuesion("确定要修改系统时间吗？")==0)
    {
        int year=ui->cboxYear->currentText().toInt();
        int month=ui->cboxMonth->currentText().toInt();
        int day=ui->cboxDay->currentText().toInt();
        int hour=ui->cboxHour->currentText().toInt();
        int min=ui->cboxMin->currentText().toInt();
        int sec=ui->cboxSec->currentText().toInt();
        myHelper::SetSystemDateTime(year,month,day,hour,min,sec);
    }
}

//手动备份数据库
void frmConfig::on_btnBank_clicked()
{
    QString temp=myApp::AppPath+QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss%1").arg(".db");
    bool ok=myHelper::CopyFile(myApp::AppPath+"360AMS.db",temp);
    if (ok)
    {
        //myAPI::AddEventInfoX("备份数据库成功");
        myHelper::ShowMessageBoxInfo("备份数据库成功,位置："+temp);
    }
    else
    {
        //myAPI::AddEventInfoX("备份数据库失败");
        myHelper::ShowMessageBoxError("备份数据库失败！");
    }
}

//手动还原数据库
void frmConfig::on_btnRestore_clicked()
{
    myHelper::ShowMessageBoxInfo("请将程序运行目录下的日期时间命名的数据库文件修改为360AMS.db即可！");
}

//恢复出厂值、清空防区数据、主机数据、用户数据、压缩数据库
void frmConfig::on_btnSetDefault_clicked()
{
    if (myHelper::ShowMessageBoxQuesion("确定要将数据库恢复出厂值吗？")==0)
    {
        /*QSqlQuery query;

        QString sql="delete from DefenceInfo;";//清空防区表
        query.exec(sql);

        sql="delete from EventInfo;";//清空事件记录表
        query.exec(sql);

        sql="delete from UserInfo where UserName!='admin';";//清空用户表
        query.exec(sql);

        sql="VACUUM;";//压缩数据库
        query.exec(sql);

        //myAPI::AddMessage("恢复出厂值成功");
        //myAPI::AddEventInfoX("恢复出厂值成功");
        myHelper::ShowMessageBoxInfo("恢复出厂值成功！");*/
    }
}
