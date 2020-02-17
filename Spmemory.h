//*************************************************************************
//Weiche des Stellwerks  [- SPMEMORY.H -]
//*************************************************************************
#ifndef SPMEMORY_H
#define SPMEMORY_H
#include <QObject>
#include "HSignal.h"
#include <QList>
#include <QPair>
#include <QTimer>
class Spmemory : public QObject{
  Q_OBJECT
  
public:
  Spmemory(){ t->callOnTimeout(this, &Spmemory::processSpeicher); }
  ~Spmemory(){}
  void showSP();
public slots:
  void timing(); // intermediate step to solve blocked thread: step towards processSpeicher
  void processSpeicher();
  void quit();
  //signals:
  //void finished();
public slots:
  void addFS( HSignal *SPstart, HSignal *SPziel);
private:
  QList<QPair<HSignal*,HSignal*>> speicherlist;
  QList<QPair<HSignal*,HSignal*>> buffer;
  QList<int> deleter;
  bool trySP = true; //control endless loop: Soll immer noch versucht werden, ein Speicher einzuwählen?
  QTimer *t = new QTimer(this);
};
#endif
