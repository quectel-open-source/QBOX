#include "myswitchbutton.h"

mySwitchButton::mySwitchButton(QWidget *parent): QPushButton(parent)
{    
    setCursor(QCursor(Qt::PointingHandCursor));
    isCheck=false;
    styleOn="background-image: url(:/image/btncheckon.png); border: 0px;";
    styleOff="background-image: url(:/image/btncheckoff.png); border: 0px;";

    setFixedSize(87,28);//不允许变化大小
    setStyleSheet(styleOff);//设置当前样式
    connect(this,SIGNAL(clicked()),this,SLOT(ChangeOnOff()));
}

void mySwitchButton::ChangeOnOff()
{
    if (isCheck)
    {
        setStyleSheet(styleOff);
        isCheck=false;
    }
    else
    {
        setStyleSheet(styleOn);
        isCheck=true;
    }
}

//设置当前选中状态
void mySwitchButton::SetCheck(bool isCheck)
{
    this->isCheck=!isCheck;
    ChangeOnOff();
}
