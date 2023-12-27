#include "frmlogout.h"
#include "ui_frmlogout.h"
#include "myhelper.h"

frmLogout::frmLogout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmLogout)
{
    ui->setupUi(this);
    myHelper::FormOnlyCloseInCenter(this);
}

frmLogout::~frmLogout()
{
    delete ui;
}
