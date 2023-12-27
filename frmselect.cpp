/*检查时间：2013-10-19
 *检查人员：刘典武
*/

#include "frmselect.h"
#include "ui_frmselect.h"
#include "myhelper.h"

frmSelect::frmSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmSelect)
{
    ui->setupUi(this);

    myHelper::FormInCenter(this);
    myHelper::FormNotResize(this);
    myHelper::FormOnlyCloseButton(this);

    //重置日期框为当前日期
    ui->dateStart->setDate(QDate::currentDate());
    ui->dateEnd->setDate(QDate::currentDate());
}

frmSelect::~frmSelect()
{
    delete ui;
}

void frmSelect::on_btnOk_clicked()
{
    if (ui->dateStart->date()>ui->dateEnd->date())
    {
        myHelper::ShowMessageBoxError("开始时间不能大于结束时间！");
        return;
    }

    this->selectStartDate=ui->dateStart->date().toString("yyyy-MM-dd");
    this->selectEndDate=ui->dateEnd->date().toString("yyyy-MM-dd");
    done(1);//关闭界面返回
    this->close();
}

void frmSelect::on_btnClose_clicked()
{
    done(0);
    this->close();
}
