#ifndef FRMDATA_H
#define FRMDATA_H

#include <QDialog>

namespace Ui {
class frmData;
}

class frmData : public QDialog
{
    Q_OBJECT

public:
    explicit frmData(QWidget *parent = 0);
    ~frmData();

private:
    Ui::frmData *ui;
};

#endif // FRMDATA_H
