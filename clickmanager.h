//*************************************************************************
// clickmanager of control center [- CLICKMANAGER.H -]
// user facing object, which controlls mouse clicks. activates the 
//      different routes over HSignal and WSignal (normally one
//      clickmanager is enough)
//*************************************************************************
#ifndef CLICKMANAGER_H
#define CLICKMANAGER_H
#include <QObject>
#include <QPair>
#include <HSignal.h>
#include <WSignal.h>

class clickmanager : public QObject{
  Q_OBJECT
public:
  clickmanager(){}
  ~clickmanager(){}
public slots:
  void recieveFS(HSignal *toFS);
  void recieveFS(WSignal *toFS);
protected:
  void reset();
  int counter = 2;//counter registers two last HSignal clicks-->counter=2 cf reciveFS
  int counterForW = 2;//counter registers two last WSignal clicks -->counter=2 cf reciveFS
  int counterForHW = 2;//counter needed for HS->WS signalling
  int counterForWH = 2;//counter, needed for WS->HS signalling
  QPair<HSignal*,HSignal*> clickList;//HS->HS
  QPair<WSignal*,WSignal*> clickListW;//WS->WS
  QPair<HSignal*,WSignal*> clickListHW;//HS->WS
  QPair<WSignal*,HSignal*> clickListWH;//WS->HS
  bool toW = false;//is there a transition from HS->WS 
  bool toH = false;//is there a transition from WS->HS 
};

#endif // CLICKMANAGER_H
