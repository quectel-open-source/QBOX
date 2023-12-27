#include "frmdefence.h"
#include "ui_frmdefence.h"
#include "myhelper.h"

frmDefence::frmDefence(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmDefence)
{
    ui->setupUi(this);
    myHelper::FormOnlyCloseInCenter(this);
}

frmDefence::~frmDefence()
{
    delete ui;
}
