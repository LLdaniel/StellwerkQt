/*
 * route memory [- SPMEMORY.H -]
 *******************************************************************************
 * If current route can not be set e.g. is occupied/reserved, a route memory   *
 * will be automatically set. It tries to set this memorized route as long as  *
 * the route permission is granted.                                            *
 *******************************************************************************
 */
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
  void timing();               // intermediate step to solve blocked thread: step towards processSpeicher
  void processSpeicher();
  void quit();
public slots:
  void addFS( HSignal *SPstart, HSignal *SPziel);
private:
  QList<QPair<HSignal*,HSignal*>> speicherlist;
  QList<QPair<HSignal*,HSignal*>> buffer;
  QList<int> deleter;
  bool trySP = true;          //control endless loop: is it necessary to set the memory route or not?
  QTimer *t = new QTimer(this);
};
#endif
