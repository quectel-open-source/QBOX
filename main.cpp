#include <QApplication>
#include <QTranslator>
#include "myhelper.h"
#include "myapp.h"
#include "frmmain.h"
#include "frmlogin.h"
#include "frmconfig.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("QBox");//设置应用程序名称
    a.setApplicationVersion("V1.00");//设置应用程序版本

    myHelper::SetUTF8Code();
    qApp->setStyle("Plastique");
    qApp->setStyleSheet("QLineEdit{border:1px groove gray;border-radius:6px;}");

    //加载中文字符
    QTranslator translator;
    translator.load(":/image/qt_zh_CN.qm");
    a.installTranslator(&translator);

    //加载和应用黑灰色主题样式表
    QFile qssFile(":/style.qss");  //style.qss  dracula.qss
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen()){
        qApp->setStyleSheet(QLatin1String(qssFile.readAll()));
        qssFile.close();
    }

    //赋值当前应用程序路径
    myApp::AppPath=QApplication::applicationDirPath()+"/";
    myApp::ReadConfig();

    frmLogin w;
    w.show();

    return a.exec();
}
