#include "frmpwd.h"
#include "ui_frmpwd.h"
#include "myhelper.h"

frmPwd::frmPwd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmPwd)
{
    ui->setupUi(this);
    myHelper::FormOnlyCloseInCenter(this);
}

frmPwd::~frmPwd()
{
    delete ui;
}
