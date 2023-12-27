#ifndef FRMPWD_H
#define FRMPWD_H

#include <QDialog>

namespace Ui {
class frmPwd;
}

class frmPwd : public QDialog
{
    Q_OBJECT

public:
    explicit frmPwd(QWidget *parent = 0);
    ~frmPwd();

private:
    Ui::frmPwd *ui;
};

#endif // FRMPWD_H
