#ifndef FRMCONFIG_H
#define FRMCONFIG_H

#include <QDialog>

namespace Ui {
class frmConfig;
}

class frmConfig : public QDialog
{
    Q_OBJECT

public:
    explicit frmConfig(QWidget *parent = 0);
    ~frmConfig();

private slots:
    void on_btnjiben_clicked();

    void on_btnzhuanfa_clicked();

    void on_btnuser_clicked();

    void on_btnqita_clicked();

    void on_btnPlay_clicked();

    void on_btnOk_clicked();

    void on_btnClose_clicked();

    void on_btnSetTime_clicked();

    void on_btnBank_clicked();

    void on_btnRestore_clicked();

    void on_btnSetDefault_clicked();

private:
    Ui::frmConfig *ui;

    void InitForm();
};

#endif // FRMCONFIG_H
