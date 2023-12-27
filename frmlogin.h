#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QDialog>

namespace Ui {
class frmLogin;
}

class frmLogin : public QDialog
{
    Q_OBJECT

public:
    explicit frmLogin(QWidget *parent = 0);
    ~frmLogin();

private slots:
    void on_btnLogin_clicked();

    void on_btnClose_clicked();

private:
    Ui::frmLogin *ui;
};

#endif // FRMLOGIN_H
