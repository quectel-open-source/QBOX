#ifndef MYSWITCHBUTTON_H
#define MYSWITCHBUTTON_H

#include <QPushButton>

enum SwitchButtonStyle
{
    SwitchButton_style1=0,
    SwitchButton_style2=1
};

/*开关按钮控件
 *作者：刘典武
 *时间：2013-8-8
 */
class mySwitchButton : public QPushButton
{
    Q_OBJECT
public:
    explicit mySwitchButton(QWidget *parent = 0);

    //获取当前选中状态
    bool GetCheck() const {return isCheck;}

    //设置当前选中状态
    void SetCheck(bool isCheck);

private:    
    bool isCheck;
    QString styleOn;
    QString styleOff;
    
signals:
    
private slots:    
    void ChangeOnOff();
    
};

#endif //MYSWITCHBUTTON_H
