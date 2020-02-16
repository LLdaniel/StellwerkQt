#ifndef CLICKMANAGER_H
#define CLICKMANAGER_H
#include <QObject>
#include <QPair>
#include <HSignal.h>
#include <WSignal.h>

class clickmanager : public QObject{
  Q_OBJECT
public:
  clickmanager();
  ~clickmanager(){}
public slots:
  void recieveFS(HSignal *toFS);
  void recieveFS(WSignal *toFS);
protected:
  void reset();
  int counter = 2;//counter registriert immer die 2 letzten HSignal clicks -->counter=2 siehe reciveFS
  int counterForW = 2;//counter registriert immer die 2 letzten WSignal clicks -->counter=2 siehe reciveFS
  int counterForHW = 2;//counter, der für HS->WS Signalisierung gebraucht wird
  int counterForWH = 2;//counter, der für WS->HS Signalisierung gebraucht wird
  QPair<HSignal*,HSignal*> clickList;//HS->HS
  QPair<WSignal*,WSignal*> clickListW;//WS->WS
  QPair<HSignal*,WSignal*> clickListHW;//HS->WS
  QPair<WSignal*,HSignal*> clickListWH;//WS->HS
  bool toW = false;//zeigt an, ob es sich um einen Übergang HS->WS handeln könnte
  bool toH = false;//zeigt an, ob es sich um einen Übergang WS->HS handeln könnte
};

#endif // CLICKMANAGER_H
