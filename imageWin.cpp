﻿#include "imageWin.h"
#include <QEvent>
#include <QApplication>
#include <QLabel>
#include <QPixmap>

imageWin::imageWin(QListWidgetItem *item, QRect rect, QWidget *parent): QWidget(parent)
{
    qApp->installEventFilter(this);
    this->setGeometry(rect);//设置显示图片窗口的x、y、w、h
    //this->setFixedSize(rect.width(), rect.height());//设置窗口固定大小
    this->setWindowIcon(item->icon());
    this->setWindowTitle("查看图片");
    this->setWindowModality(Qt::ApplicationModal);//阻塞除当前窗体外的其他所有窗体

    //通过QLabel加载item上的图片
    QLabel *lab = new QLabel(this);
    //lab->setPixmap(item->icon().pixmap(QSize(this->width(), this->height())).scaled(lab->width(),lab->height()));

    QImage img(item->toolTip());
    //QImage image2=img.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPixmap pix = QPixmap::fromImage(img);
    lab->setFixedSize(img.width(), img.height());
    lab->setPixmap(pix);
    //lab->setPixmap(item->icon().pixmap(QSize(this->width(), this->height())));
}

imageWin::~imageWin()
{
    delete this;
}

//事件过滤器，实现点击窗口任意位置，窗口就可以关闭
bool imageWin::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        this->close();
    }
    return QWidget::eventFilter(obj, e);
}
