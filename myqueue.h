#ifndef MYQUEUE_H
#define MYQUEUE_H
#include "HSignal.h"
#include <vector>
#include <QThread>
#include <QObject>

class myqueue : public QThread{
public:
    myqueue();
    void add(HSignal *hs1, HSignal *hs2);//adding new Speicher to queue
    void terminate(){ end = false;}
    void run();
private:
    std::vector<std::pair<HSignal*,HSignal*> > speicherqueue; //Hier werden alle gespeicherten FS vermerkt
    bool end;//bool for terminating process
};

#endif // MYQUEUE_H
