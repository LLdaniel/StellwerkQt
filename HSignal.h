/*
 * main signal [- HSIGNAL.H -]
 *******************************************************************************
 * core object for setting routes                                              *
 *    - all other railroad objects are managed from this class mainly          *
 *******************************************************************************
 */
#ifndef HSIGNAL_H
#define HSIGNAL_H
#include <QObject>
#include "Block.h"
#include "Weiche.h"
#include <QString>
#include <QList>
#include <QPair> 
#include "VSignal.h"
#include <QLabel>
#include <QGraphicsSvgItem>
#include "HSignalQPushButton.h"
#include "WSignal.h"
class HSignal : public QObject{
    Q_OBJECT
 public:
  HSignal( int name);
  void setS_id( int name );
  QString getS_id(){ return s_id; }
  bool setFahrt( HSignal *toZiel );                      //special role return value: HS->HS can be saved in memory, but for spmemory it is relevant, if it was successful or not/keep in memory or not
  void setFahrt( WSignal *toZiel );
  bool getS_status(){ return s_status; }
  QString getZiel(){ return ziel; }
  void addWeichenstatus( HSignal *toZiel , QList<QPair<Weiche* , bool>> weichenpair );
  void showWeichenstatusALL();                           //full list
  void showWeichenstatus( HSignal *whichZiel );          //list with special end signal
  void addWeichenstatusZuRangier( WSignal *toZiel , QList<QPair<Weiche* , bool>> weichenpair );
  void addBlock( HSignal *toZiel , QList<Block*> inputBlock );
  void showBlock( HSignal *whichZiel );
  void showBlockALL();
  void addBlockZuRangier( WSignal *toZiel , QList<Block*> inputBlock );
  void deleteNachbar( HSignal *todelete );               
  void addVSignal( VSignal *vs , QString param );
  void showVSignalR();
  void showVSignalV();
  void addWSignal( WSignal *wsig, QString param );
  void showWSignalR();
  void showWSignalV();
  void deleteVS( VSignal *todelete , QString param );
  void deleteWS( WSignal *todelete , QString param );
  void setSpeicher( bool sp ){ speicher = sp; emit illuminateSpeicher(sp, this->getSpeicherziel()->getS_id() );}
  HSignal* getSpeicherziel(){ return speicherziel; }
  bool getSpeicher(){ return speicher; }
  QPair<QGraphicsRectItem*,QGraphicsRectItem*> getspeicheritems() { return speicheritems; }
  void setZiel( QString zziel );                         //set end WSignal or HSignal
  void setfromHS( HSignal* from ){ fromHS = from; }
  void deleteFS();                                       //delete route, unless train occupies segments
  bool isNachbar(HSignal *toZiel);                       //is end signal a neighbour? HS
  bool isNachbar(WSignal *toZiel);                       //is end signal a neighbour? WS
  ~HSignal();
  //
  //+++ GUI +++
  //
  void addHSignalitem(QGraphicsSvgItem *itemfahrt , QGraphicsSvgItem *itemhalt , QGraphicsSvgItem *itemrangier, QLabel *la, HSignalQPushButton *but, QGraphicsRectItem *speicherback, QGraphicsRectItem *speicherfront);
  void moveLabel( int x , int y ){ beschriftung->move(x,y); }
  void moveButton( int x, int y ){ push->move(x,y); }

 signals:
  void refreshStellwerkstechnik( QString sig , bool stat );
  void listened( HSignal *clickedHS);
  void illuminateSpeicher(bool sp, QString str);
  void callspmemory(HSignal *s1, HSignal *s2);           // connect to spmemory object slot						   
public slots:
  void zugpassiert();                                    //train passed signal and goes to Hp0
  void listenToFS();                                     //waiting for click events, which will be converted in setFahrt commands (cf clickmanager)
  void recieveSpeicher(bool sp, QString str);
  //void processSpeicher();
  void showShowContexts();
 protected:
  QString s_id;                                          //ID of main signal i.e. S002, S511
  bool s_status = false;                                 //status main signals: true:=go false:=stop
  QString ziel;                                          //current end signal
  bool hasHSZiel = false;
  bool hasWSZiel = false;
  QList<QPair<QString , QList<QPair<Weiche* , bool>> > > weichenstatus;//Weichenstatus: which status should the tournouts have, when setting route to end signal
  QList<QPair<QString, QList<Block*>> > block;           //Blockstatus: which segments are involved in the planned route
  QList<VSignal*> vorsignalR;                            //involved distant signal: viewpoint backwards
  QList<VSignal*> vorsignalV;                            //involved distant signal: viewpoint forwards
  QList<WSignal*> wsignaleV;                             //involved shunt signal in distant signal mode forwards
  QList<WSignal*> wsignaleR;                             //involved shunt signal in distant signal mode backwards
  QList<QPair<QString , QList<QPair<Weiche* , bool>> > > weichenstatusZuRangier;//Weichenstatus: which status should the tournouts have, when setting route to end signal for shunting trip
  QList<QPair<QString, QList<Block*>> > blockZuRangier; //Blockstatus: which segments are involved in the planned route for shunting trip
  bool speicher = false;                                //true:= mempory is set false:= no memory
  HSignal* speicherziel;                                //end signal for memory route only HS allowed
  HSignal* fromHS;                                      //start signal of the current route: needed to display memory in the plan
  bool zuRangier = false;                               //if true: shunting trip
  //
  //+++ GUI +++
  //
  QGraphicsSvgItem *fahrt;                              //shows signal "go"
  QGraphicsSvgItem *halt;                               //shows signal "stop"
  QGraphicsSvgItem *rangier;                            //shows signal "shunting"
  QPair<QGraphicsRectItem*,QGraphicsRectItem*> speicheritems;//display if memory is active: small squares
  QLabel *beschriftung = new QLabel();                  //singal label
  HSignalQPushButton *push = new HSignalQPushButton();
  void changeColor();                                   //changes signal on plan
  void darkenSpeicher( int position );                  //delete memory items (squares) if memory goes inactive here it is relevant for the start signal
};
#endif
