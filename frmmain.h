#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include <QLabel>
#include <QtGui>
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtWidgets>
#endif
#include "include/ObjectType.hpp"
#include <vector>

namespace Ui {
class frmMain;
}

class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();
    void initForm();
    void initMenu();

    bool videoMax;
    int videoCount;
    QString videoType;
    QMenu *videoMenu;
    QList<QLabel *> widgets;
    void JsonIORead(QString path);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void ShowFrmConfig();    
    void ShowFrmHost();
    void ShowFrmDefence();
    void ShowFrmData();   
    void ShowFrmExit();

    void snapshot_video_one();
    void snapshot_video_all();

    void show_video_all();
    void show_video_4();
    void show_video_6();
    void show_video_8();
    void show_video_9();
    void show_video_16();

    void hide_video_all();
    void change_video(int index, int flag);
    void change_video_4(int index);
    void change_video_6(int index);
    void change_video_8(int index);
    void change_video_9(int index);
    void change_video_16(int index);

    void refreshList();
    void enlargeImage(QListWidgetItem *item);
    void chooseImage(QListWidgetItem *item);
private:
    Ui::frmMain *ui;

public slots:
    void SetImage(int i,const QPixmap dst);
    void SetDb(int i, QPixmap dst,QString title,QString picName);
};

#endif // FRMMAIN_H
