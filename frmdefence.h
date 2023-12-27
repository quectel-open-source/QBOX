#ifndef FRMDEFENCE_H
#define FRMDEFENCE_H

#include <QDialog>

namespace Ui {
class frmDefence;
}

class frmDefence : public QDialog
{
    Q_OBJECT

public:
    explicit frmDefence(QWidget *parent = 0);
    ~frmDefence();

private:
    Ui::frmDefence *ui;
};

#endif // FRMDEFENCE_H
