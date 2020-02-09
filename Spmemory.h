//*************************************************************************
//Weiche des Stellwerks  [- MEMORY.H -]
//*************************************************************************
#ifndef SPMEMORY_H
#define SPMEMORY_H
#include <QObject>
#include "HSignal.h"
#include <QList>
#include <QMutex>
class Spmemory : public QObject{
  Q_OBJECT
 public:
  memory(){}
  ~memory(){}
  void addFS( HSignal *SPstart, HSignal *SPziel);
  void showSP();
  public slots:
    void processSpeicher();
    void quit();
 signals:
    void finished();
 private:
    QList<std::pair<HSignal*,HSignal*>> speicherlist;
    QList<std::pair<HSignal*,HSignal*>> buffer;
    QList<unsigned int> deleter;
    bool trySP = true; //control endless loop: Soll immer noch versucht werden, ein Speicher einzuwählen?
};
#endif
