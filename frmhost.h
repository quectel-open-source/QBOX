#ifndef FRMHOST_H
#define FRMHOST_H

#include <QDialog>
#include <QWidget>
#include <QListWidget>
#include <QMainWindow>

namespace Ui {
class frmHost;
}

class frmHost : public QDialog
{
    Q_OBJECT

public:
    QMainWindow* mfrm;
    void initHost();

    explicit frmHost(QWidget *parent = 0);
    ~frmHost();

public slots:
    void chooseHost(QListWidgetItem *item);
    void btnAdd_clicked();
    void btnSave_clicked();
    void btnDel_clicked();

private:
    Ui::frmHost *ui;
};

#endif // FRMHOST_H
