#ifndef FFTHREAD_H
#define FFTHREAD_H

#include <QThread>
#include "qffmpeg.h"
using namespace std;

class FfThread : public QThread
{
    Q_OBJECT
public:
    int channel;
    string address;
    int hostType;
    int algo;
    int waitFrame=1;

    QFFmpeg* ffmpeg;

    bool isStop=false;
    explicit FfThread(QObject *parent = 0);

    void run();    


};

#endif // FFTHREAD_H
