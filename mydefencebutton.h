#ifndef MYDEFENCEBUTTON_H
#define MYDEFENCEBUTTON_H

#include <QWidget>
#include <QTimer>

/*防区类型  00-07
 */
enum DefenceType
{
    DefenceType_00=0,
    DefenceType_01=1,
    DefenceType_02=2,
    DefenceType_03=3,
    DefenceType_04=4,
    DefenceType_05=5,
    DefenceType_06=6,
    DefenceType_07=7
};

/*防区状态  布防、撤防、报警、旁路、断开、故障
 */
enum DefenceStatus
{
    DefenceStatus_BuFang=0,
    DefenceStatus_CheFang=1,
    DefenceStatus_BaoJing=2,
    DefenceStatus_PangLu=3,
    DefenceStatus_DuanKai=4,
    DefenceStatus_GuZhang=5
};

class myDefenceButton: public QWidget
{
    Q_OBJECT
public:

    //构造函数,直接传入一个防区所需要的所有属性
    explicit myDefenceButton(QWidget *parent,QString defenceID,QString defenceName,
                             DefenceType defenceType,int subSystem,QString hostID,QString hostName,
                             QString defenceImage,int defenceX,int defenceY,DefenceStatus defenceStatus);

    //对外公开的外部接口,设置防区ID
    void SetDefenceID(QString defenceID);
    //对外公开的外部接口,设置防区状态,例如防区报警、防区旁路等
    void SetDefenceStatus(DefenceStatus defenceStatus);
    //获取防区ID
    QString GetDefenceID()const{return defenceID;}
    //获取防区名称
    QString GetDefenceName()const{return defenceName;}
    //获取防区类型
    DefenceType GetDefenceType()const{return defenceType;}
    //获取防区对应子系统
    int GetSubSystem()const {return subSystem;}
    //获取防区对应主机ID
    QString GetHostID(){return hostID;}
    //获取防区对应主机名称
    QString GetHostName(){return hostName;}
    //获取防区对应地图文件名称
    QString GetDefenceImage()const {return defenceImage;}
    //获取防区X坐标
    int GetDefenceX()const{return defenceX;}
    //获取防区Y坐标
    int GetDefenceY()const{return defenceY;}
    //获取防区状态
    DefenceStatus GetDefenceStatus()const{return defenceStatus;}

signals:

protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *, QEvent *);

private slots:
    void BaoJingChange();

private:
    QString defenceID;//防区号
    QString defenceName;//防区名
    DefenceType defenceType;//防区类型  00--07
    int subSystem;//防区对应子系统
    QString hostID;//防区对应主机编号
    QString hostName;//防区对应主机名称
    QString defenceImage;//防区地图
    int defenceX;//防区X坐标
    int defenceY;//防区Y坐标
    DefenceStatus defenceStatus;//防区状态

    QRadialGradient *currentRG;
    bool IsRed;//当前报警颜色是否为红色
    QTimer *myTimer;//控制报警闪烁时钟

};

#endif //MYDEFENCEBUTTON_H
