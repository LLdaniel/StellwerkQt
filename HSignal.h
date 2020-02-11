//*************************************************************************
//Hauptsignal des Stellwerks  [- HSIGNAL.H -]
//*************************************************************************
#ifndef HSIGNAL_H
#define HSIGNAL_H
#include <QObject>
#include "Block.h"
#include "Weiche.h"
#include <vector>
#include <string>
#include "VSignal.h"
#include <QLabel>
#include <QGraphicsSvgItem>
#include <QPushButton>
#include "WSignal.h"
class HSignal : public QObject{
    Q_OBJECT
 public:
  HSignal( int name);
  void setS_id( int name );
  std::string getS_id(){ return s_id; }
  bool setFahrt( HSignal *toZiel );  //Sonderstatus Rückgabewert: HS->HS kann gespeichert werden, zum löschen des Spmemory ist es wichtig zu wissen, ob es geklappt hat
  void setFahrt( WSignal *toZiel );
  bool getS_status(){ return s_status; }
  std::string getZiel(){ return ziel; }
  void addWeichenstatus( HSignal *toZiel , std::vector<std::pair<Weiche* , bool>> weichenpair );
  void showWeichenstatusALL();//-->gesamte Liste wird geprintet
  void showWeichenstatus( HSignal *whichZiel );//mit speziellem Ziel zum printen des betreffenden Weichenstatus
  void addWeichenstatusZuRangier( WSignal *toZiel , std::vector<std::pair<Weiche* , bool>> weichenpair );
  void addBlock( HSignal *toZiel , std::vector<Block*> inputBlock );
  void showBlock( HSignal *whichZiel );
  void showBlockALL();
  void addBlockZuRangier( WSignal *toZiel , std::vector<Block*> inputBlock );
  void deleteNachbar( HSignal *todelete );//Nachbar todelete wird in Block und Weichenstatus gelöscht
  void addVSignal( VSignal *vs , std::string param );
  void showVSignalR();
  void showVSignalV();
  void addWSignal( WSignal *wsig, std::string param );
  void showWSignalR();
  void showWSignalV();
  void deleteVS( VSignal *todelete , std::string param );
  void deleteWS( WSignal *todelete , std::string param );
  void setSpeicher( bool sp ){ speicher = sp; emit illuminateSpeicher(sp, this->getSpeicherziel()->getS_id() );}
  HSignal* getSpeicherziel(){ return speicherziel; }
  bool getSpeicher(){ return speicher; }
  std::pair<QGraphicsRectItem*,QGraphicsRectItem*> getspeicheritems() { return speicheritems; }
  void setZiel( std::string zziel );//Setzt das ziel, je nach dem, ob es ein W oder S ist
  void setfromHS( HSignal* from ){ fromHS = from; }
  void deleteFS();//löscht die FS, falls nicht schon was belegt ist
  bool isNachbar(HSignal *toZiel);//testet, ob das Zielsignal ein Nachbarsignal ist -->toZiel sollte natürlich das von SetFahrt sein
  bool isNachbar(WSignal *toZiel);//testet, ob das Zielsignal ein Nachbarsignal ist -->toZiel sollte natürlich das von SetFahrt sein
  ~HSignal();
  //
  //+++GUI+++
  void addHSignalitem(QGraphicsSvgItem *itemfahrt , QGraphicsSvgItem *itemhalt , QGraphicsSvgItem *itemrangier, QLabel *la, QPushButton *but, QGraphicsRectItem *speicherback, QGraphicsRectItem *speicherfront);
  void moveLabel( int x , int y ){ beschriftung->move(x,y); }
  void moveButton( int x, int y ){ push->move(x,y); }

 signals:
  void refreshStellwerkstechnik( std::string sig , bool stat );
  void listened( HSignal *clickedHS);
  void illuminateSpeicher(bool sp, std::string str);
  void callspmemory(HSignal *s1, HSignal *s2); // connect to spmemory object slot						   
public slots:
  void zugpassiert();//Wenn der Zug das HSignal überfahren hat, fällt es auf Halt + entriegeln der VS + entriegeln der WS ------------------------------------muss noch
  void listenToFS(); //es wird nach click events Ausschau gehalten --> Umwandlung als setFahrt Befehl
  void recieveSpeicher(bool sp, std::string str);
  //void processSpeicher();
 protected:
  std::string s_id;//ID des Hauptsignals zB. S002, S511
  bool s_status = false;//Status des Signals: true:=Fahrt false:=Halt
  std::string ziel;//Hier wird das Zielsignal der aktuellen FS gespeichert
  bool hasHSZiel = false;
  bool hasWSZiel = false;
  std::vector<std::pair<std::string , std::vector<std::pair<Weiche* , bool>> > > weichenstatus;//Weichenstatus gespeichert über eine Liste von Signalen mit einer Liste von Weichen-Status-Paaren: <Ziel <Weiche,bool>>
  std::vector<std::pair<std::string, std::vector<Block*>> > block;//Hier werden die in einer FS involvierten Blöcke in der Form Zielsignal Blockliste gespeichert
  std::vector<VSignal*> vorsignalR;//involvierten Vorsignale werden hier gespeichert-->rückwärts gesehen!
  std::vector<VSignal*> vorsignalV;//involvierten Vorsignale werden hier gespeichert-->vorwärts gesehen!
  std::vector<WSignal*> wsignaleV;//involvierten WSignale im Modus als VS [vor]
  std::vector<WSignal*> wsignaleR;//involvierten WSignale im Modus als VS [rück]
  std::vector<std::pair<std::string , std::vector<std::pair<Weiche* , bool>> > > weichenstatusZuRangier;//Weichenstatus gespeichert für Übergang zur Rangierfahrt
  std::vector<std::pair<std::string, std::vector<Block*>> > blockZuRangier;//Blöcke für den Übergang zur Rangierfahrt

  bool speicher = false;//true:= Speicher gesetzt false:= Speicher nicht gesetzt
  HSignal* speicherziel;//Was ist das Ziel des FS Speicher --> das kann nur ein HSignal sein
  HSignal* fromHS;//hier wird vermerkt, was für ein Startsignal auf das Signal hier eine FS gestellt hat --> benötigt für Speicher auflösen
  bool zuRangier = false;//Falls ein Übergang zur Rangierfahrt erfolgt, wird das hier vermerkt true = rangier
  //
  //+++GUI+++
  QGraphicsSvgItem *fahrt;//zeigt signalbild fahrt
  QGraphicsSvgItem *halt;//zeigt signalbild halt
  QGraphicsSvgItem *rangier;//zeigt signalbild verschub
  std::pair<QGraphicsRectItem*,QGraphicsRectItem*> speicheritems;//Die hellen Punkte zur Anzeige, dass Speicher aktiv ist
  QLabel *beschriftung = new QLabel();//Beschriftung
  QPushButton *push = new QPushButton();
  void changeColor();//verändert die Farbe der Hauptsignalitems
  void darkenSpeicher( int position );//löschen des Speichers für das Einwählen einer gespeicherten FS: betrifft hier den Start-Speicher-Punkt
};
#endif
