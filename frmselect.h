#ifndef FRMSELECT_H
#define FRMSELECT_H

#include <QDialog>

namespace Ui {
class frmSelect;
}

class frmSelect : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmSelect(QWidget *parent = 0);
    ~frmSelect();

    //获取当前选择的开始时间和结束时间
    QString SelectStartDate() const {return selectStartDate;}
    QString SelectEndDate() const {return selectEndDate;}
    
private slots:
    void on_btnOk_clicked();

    void on_btnClose_clicked();

private:
    Ui::frmSelect *ui;

    QString selectStartDate;
    QString selectEndDate;
};

#endif // FRMSELECT_H
