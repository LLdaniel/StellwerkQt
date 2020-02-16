//*************************************************************************
//Rangiersignal des Stellwerks  [- WSIGNAL.H -]
//*************************************************************************
#ifndef WSIGNAL_H
#define WSIGNAL_H
#include "VSignal.h"
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <Block.h>
#include <Weiche.h>
#include <QString>
#include <QList>
#include <QPair>
class WSignal : public QObject,public VSignal{//WSignal erbt nun von VSignal
  Q_OBJECT
public:
 WSignal( int name ) :  VSignal( name ){ setV_id( name ); }// with :HSignal(name) call the Super Constructor
  void setV_id( int name );//overload setS_id, wegen W als Präfix
  void setFahrt(WSignal* toZiel);
  void setFahrt(QString toZiel);
  QString getZiel(){ return ziel; }
  void addWeichenstatus( WSignal *toZiel , QList<QPair<Weiche* , bool>> weichenpair );
  void addWeichenstatusZuH( QString toZiel , QList<QPair<Weiche* , bool>> weichenpair );
  void showWeichenstatusALL();//-->gesamte Liste wird geprintet
  void showWeichenstatus( WSignal *whichZiel );//mit speziellem Ziel zum printen des betreffenden Weichenstatus
  void addBlock( WSignal *toZiel , QList<Block*> inputBlock );
  void addBlockZuH( QString toZiel , QList<Block*> inputBlock );
  void showBlock( WSignal *whichZiel );
  void showBlockALL();
  void deleteNachbar( WSignal *todelete );//Nachbar todelete wird in Block und Weichenstatus gelöscht
  void setZiel( QString zziel );
  void deleteFS();//löscht die FS, falls nicht schon was belegt ist
  bool isNachbar(WSignal *toZiel);//testet, ob das Zielsignal ein Nachbarsignal ist -->toZiel sollte natürlich das von SetFahrt sein
  bool isNachbar(QString toZiel);//testet, ob das Zielsignal ein Nachbarsignal ist -->toZiel sollte natürlich das von SetFahrt sein
  ~WSignal();
  //+++GUI+++
  void addButtonAndLabel(QLabel *lab, QPushButton *but);
  void moveLabel( int x , int y ){ beschriftung->move(x,y); }
  void moveButton( int x, int y ){ push->move(x,y); }

signals:
  void listened( WSignal *clickedWS);
  void refreshStellwerkstechnikW( QString sig , bool stat );//informiere Stellwerkstechnik über statusÄnderungen des WS
public slots:
  void listenToFS(); //es wird nach click events Ausschau gehalten --> Umwandlung als setFahrt Befehl
  void zugpassiertW();//Zugpassiert für WS->WS FS
  //void zugpassiert() für das Schalten der WS als Art VS wird ja von VS geerbt
private:
   //mal eine Überlegung wert: WSignal braucht eigentlich auc noch ein zugpassiert... -->zur Not wegen Stellwerkstec bei setFahrt noch Inspirationen holen
  QString ziel;//Hier wird das Zielsignal der aktuellen FS gespeichert
  bool hasWSZiel = false;
  bool hasHSZiel = false;
  QList<QPair<QString , QList<QPair<Weiche* , bool>> > > weichenstatus;//Weichenstatus gespeichert über eine Liste von Signalen mit einer Liste von Weichen-Status-Paaren: <Ziel <Weiche,bool>>
  QList<QPair<QString, QList<Block*>> > block;//Hier werden die in einer FS involvierten Blöcke in der Form Zielsignal Blockliste gespeichert
  QList<QPair<QString , QList<QPair<Weiche* , bool>> > > weichenstatusZuH;//Weichenstatus gespeichert über eine Liste von Signalen mit einer Liste von Weichen-Status-Paaren: <Ziel <Weiche,bool>>
  QList<QPair<QString, QList<Block*>> > blockZuH;//Hier werden die in einer FS involvierten Blöcke in der Form Zielsignal Blockliste gespeichert
  //+++GUI+++
  QLabel *beschriftung = new QLabel();//Beschriftung
  QPushButton *push = new QPushButton();
};
#endif
