#include "frmdata.h"
#include "ui_frmdata.h"
#include "myhelper.h"

frmData::frmData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmData)
{
    ui->setupUi(this);
    myHelper::FormOnlyCloseInCenter(this);
}

frmData::~frmData()
{
    delete ui;
}
