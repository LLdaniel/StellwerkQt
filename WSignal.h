/*
 * shunt signal [- WSIGNAL.H -]
 *******************************************************************************
 * shunt signal for shunting trips, possible routes include:                   *
 *    - shunt signal to main signal                                            *
 *    - shunt signal to shunt signal                                           *
 *******************************************************************************
 */
#ifndef WSIGNAL_H
#define WSIGNAL_H
#include "VSignal.h"
#include <QLabel>
#include "WSignalQPushButton.h"
#include <QObject>
#include <Block.h>
#include <Weiche.h>
#include <QString>
#include <QList>
#include <QPair>
class WSignal : public QObject,public VSignal{                 //WSignal inherits from VSignal
  Q_OBJECT
public:
 WSignal( int name ) :  VSignal( name ){ setV_id( name ); }// with :HSignal(name) call the Super Constructor
  void setV_id( int name );                                   //overload setS_id, because of W as prefix
  void setFahrt(WSignal* toZiel);
  void setFahrt(QString toZiel);
  QString getZiel(){ return ziel; }
  void addWeichenstatus( WSignal *toZiel , QList<QPair<Weiche* , bool>> weichenpair );
  void addWeichenstatusZuH( QString toZiel , QList<QPair<Weiche* , bool>> weichenpair );
  void showWeichenstatusALL();
  void showWeichenstatus( WSignal *whichZiel );
  void addBlock( WSignal *toZiel , QList<Block*> inputBlock );
  void addBlockZuH( QString toZiel , QList<Block*> inputBlock );
  void showBlock( WSignal *whichZiel );
  void showBlockALL();
  void deleteNachbar( WSignal *todelete );
  void setZiel( QString zziel );
  void deleteFS();
  bool isNachbar(WSignal *toZiel);                            //check if end signal is neighbour
  bool isNachbar(QString toZiel);                             //check if end signal is neighbour overload
  ~WSignal();
  //
  //+++ GUI +++
  //
  void addButtonAndLabel(QLabel *lab, WSignalQPushButton *but);
  void moveLabel( int x , int y ){ beschriftung->move(x,y); }
  void moveButton( int x, int y ){ push->move(x,y); }

signals:
  void listened( WSignal *clickedWS);
  void refreshStellwerkstechnikW( QString sig , bool stat ); //update stellwerkstechnik
public slots:
  void listenToFS();                                         //look for clicks, which will be conerted to route commands from clickmanager
  void zugpassiertW();                                       //zugpassiert for routes of character WS->WS FS
  void showShowContexts();
  //void zugpassiert()                                       //zugpassiert from routes of character HS->HS with WS as VS: WS inherits from VS and inherits therefore VS`s zugpassiert
private:
  QString ziel;                                              //save end signal 
  bool hasWSZiel = false;
  bool hasHSZiel = false;
  QList<QPair<QString , QList<QPair<Weiche* , bool>> > > weichenstatus; // involved turnouts in specific routes to WS
  QList<QPair<QString, QList<Block*>> > block;                          // involved segments in specific routes to WS          
  QList<QPair<QString , QList<QPair<Weiche* , bool>> > > weichenstatusZuH;// involved turnouts in specific routes to HS
  QList<QPair<QString, QList<Block*>> > blockZuH;                         // involved segments in specific routes to HS       
  //+++GUI+++
  QLabel *beschriftung = new QLabel();
  WSignalQPushButton *push = new WSignalQPushButton();
};
#endif
