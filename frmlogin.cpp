#include "frmlogin.h"
#include "ui_frmlogin.h"
#include "myhelper.h"
#include "frmmain.h"

frmLogin::frmLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmLogin)
{
    ui->setupUi(this);
    myHelper::FormOnlyCloseInCenter(this);
}

frmLogin::~frmLogin()
{
    delete ui;
}

void frmLogin::on_btnLogin_clicked()
{
    frmMain *frm=new frmMain;

    frm->show();

    this->hide();
}

void frmLogin::on_btnClose_clicked()
{
    this->close();
}
