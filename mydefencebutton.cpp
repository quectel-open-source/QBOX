#include "mydefencebutton.h"
#include <QPainter>
#include <QMouseEvent>
#include "myapp.h"

myDefenceButton::myDefenceButton(QWidget *parent, QString defenceID, QString defenceName,
                                 DefenceType defenceType, int subSystem,QString hostID,
                                 QString hostName,QString defenceImage, int defenceX, int defenceY,
                                 DefenceStatus defenceStatus):
    QWidget(parent)
{
    this->setCursor(QCursor(Qt::PointingHandCursor));
    this->defenceID=defenceID;
    this->defenceName=defenceName;
    this->defenceType=defenceType;
    this->subSystem=subSystem;
    this->hostID=hostID;
    this->hostName=hostName;
    this->defenceImage=defenceImage;
    this->defenceX=defenceX;
    this->defenceY=defenceY;
    this->defenceStatus=defenceStatus;

    currentRG = new QRadialGradient(0,0,80);
    currentRG->setColorAt(0, QColor(0, 0, 245));
    currentRG->setColorAt(0.6, QColor(0, 0, 210));
    currentRG->setColorAt(1, QColor(0, 0, 166));

    IsRed=false;
    myTimer=new QTimer(this);
    myTimer->setInterval(500);
    connect(myTimer,SIGNAL(timeout()),this,SLOT(BaoJingChange()));

    this->installEventFilter(this);//安装监听器,监听鼠标拖动
}

bool myDefenceButton::eventFilter(QObject *, QEvent *evt)
{
    if (myApp::IsMove)
    {
        static QPoint lastPnt;
        static bool isHover = false;

        if(evt->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* e = static_cast<QMouseEvent*>(evt);
            if(this->rect().contains(e->pos()) && (e->button() == Qt::LeftButton))
            {
                lastPnt = e->pos();
                isHover = true;
            }
        }
        else if(evt->type() == QEvent::MouseMove && isHover)
        {
            QMouseEvent* e = static_cast<QMouseEvent*>(evt);
            int dx = e->pos().x() - lastPnt.x();
            int dy=e->pos().y()-lastPnt.y();
            this->move(this->x()+dx,this->y()+dy);
        }
        else if(evt->type() == QEvent::MouseButtonRelease && isHover)
        {
            isHover = false;
        }
    }

    return false;
}

//触发报警
void myDefenceButton::BaoJingChange()
{
    if (IsRed)
    {
        currentRG->setColorAt(0, QColor(50, 50, 50));
        currentRG->setColorAt(0.6, QColor(30, 30, 30));
        currentRG->setColorAt(1, QColor(10, 10, 10));
        IsRed=false;
    }
    else
    {
        currentRG->setColorAt(0, QColor(245, 0, 0));
        currentRG->setColorAt(0.6, QColor(210, 0, 0));
        currentRG->setColorAt(1, QColor(166, 0, 0));
        IsRed=true;
    }
    update();
}

void myDefenceButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //设置坐标和缩放
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing); 	/* 使用反锯齿(如果可用) */
    painter.translate(width() / 2, height() / 2);	/* 坐标变换为窗体中心 */
    int side = qMin(width(), height());
    painter.scale(side / 200.0, side / 200.0);		/* 比例缩放 */
    painter.setPen(Qt::NoPen);

    //画外边框
    int radius = 100;
    QLinearGradient lg1(0, -radius, 0, radius);
    lg1.setColorAt(0, QColor(255, 255, 255));
    lg1.setColorAt(1, QColor(166, 166, 166));
    painter.setBrush(lg1);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    //画内边框
    radius -= 13;
    QLinearGradient lg2(0, -radius, 0, radius);
    lg2.setColorAt(0, QColor(166, 166, 166));
    lg2.setColorAt(1, QColor(255, 255, 255));
    painter.setBrush(lg2);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    //画内部指示颜色
    radius -= 4;
    painter.setBrush(*currentRG);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);
    painter.restore();

    //画防区号文字
    painter.save();
    QFont font("Arial",10,QFont::Bold,false);
    painter.setFont(font);
    QPen pen(QColor(255,255,255));
    painter.setPen(pen);
    painter.drawText(rect(),Qt::AlignCenter,this->defenceID);
    painter.restore();
}

//设置防区号
void myDefenceButton::SetDefenceID(QString defenceID)
{
    this->defenceID=defenceID;
    update();
}

//设置防区状态,例如报警了
void myDefenceButton::SetDefenceStatus(DefenceStatus defenceStatus)
{
    this->defenceStatus=defenceStatus;

    if (defenceStatus==DefenceStatus_BuFang)
    {
        myTimer->stop();
        currentRG->setColorAt(0, QColor(0, 245, 0));
        currentRG->setColorAt(0.6, QColor(0, 210, 0));
        currentRG->setColorAt(1, QColor(0, 166, 0));
        update();
    }
    else if (defenceStatus==DefenceStatus_CheFang)
    {
        myTimer->stop();
        currentRG->setColorAt(0, QColor(0, 0, 245));
        currentRG->setColorAt(0.6, QColor(0, 0, 210));
        currentRG->setColorAt(1, QColor(0, 0, 166));
        update();
    }
    else if (defenceStatus==DefenceStatus_PangLu)
    {
        myTimer->stop();
        currentRG->setColorAt(0, QColor(180, 180, 180));
        currentRG->setColorAt(0.6, QColor(140, 140, 140));
        currentRG->setColorAt(1, QColor(129, 129, 129));
        update();
    }
    else if (defenceStatus==DefenceStatus_DuanKai)
    {
        myTimer->stop();
        currentRG->setColorAt(0, QColor(145, 185, 235));
        currentRG->setColorAt(0.6, QColor(120, 170, 215));
        currentRG->setColorAt(1, QColor(102, 153, 204));
        update();
    }
    else if (defenceStatus==DefenceStatus_GuZhang)
    {
        myTimer->stop();
        currentRG->setColorAt(0, QColor(50, 50, 50));
        currentRG->setColorAt(0.6, QColor(30, 30, 30));
        currentRG->setColorAt(1, QColor(10, 10, 10));
        update();
    }
    else if (defenceStatus==DefenceStatus_BaoJing)
    {
        myTimer->start();//启动定时器切换报警背景图
    }
}
