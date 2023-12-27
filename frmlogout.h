#ifndef FRMLOGOUT_H
#define FRMLOGOUT_H

#include <QDialog>

namespace Ui {
class frmLogout;
}

class frmLogout : public QDialog
{
    Q_OBJECT

public:
    explicit frmLogout(QWidget *parent = 0);
    ~frmLogout();

private:
    Ui::frmLogout *ui;
};

#endif // FRMLOGOUT_H
