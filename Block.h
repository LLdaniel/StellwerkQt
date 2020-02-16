//*************************************************************************
//Block des Stellwerks  [- BLOCK.H -]
//*************************************************************************
#ifndef BLOCK_H
#define BLOCK_H
#include <QObject>
#include <QList>
#include <QPair>
#include <QGraphicsRectItem>
#include <QString>
#include "BU.h"
#include "Stellwerkstechnik.h"
class Block : public QObject{
    Q_OBJECT
public:
  Block( QString name , Stellwerkstechnik *signaltechnik);
  //
  void setName( QString name );
  QString getName(){ return blockname; }
  bool getB_status(){ return b_status; }
  void setFreigabe( bool free );//gleichzeitig wird auch der BU geschlossen/geöffnet
  bool getFreigabe(){ return freigabe; }
  int getCounter(){ return counter; }
  void addBus( BU *bu );
  QList<BU*> getBus(){ return bus; }
  void showBus();
  void deleteBus( BU* todelete );
  void addpassiert( QString grenzS, Block* prevBlock );//Übergabe der Grenzen wegen zugpassiert
  //void addcontrolspeicher( bool hs ){ controlspeicher = hs; }
  void deletepassiert();//markiert haspassiert als false und löscht alle passiert Einträge
  bool getHaspassiert(){ return haspassiert; }
  ~Block();
  //+++GUI+++
  //
  void addBlockitems( QGraphicsRectItem *rect ){ blockitems << rect; }
  //
public slots:
  void setB_status( bool status );
signals:
  void zugpassiert();//Signal das auf slot HSignal::zugpassiert geht
  void zugpassiertW();//Signal das auf slot WSignal::zugpassiert geht
  //void releaseSpeicher();//löst aus, da der Block ein Vorblock ist --> für FS Speicher einwählen benötigt
 private:
  QString blockname;//Benennung der Blöcke mit aa, ab, ac ... zz
  bool b_status = true;//true:=frei ; false:=belegt
  bool freigabe = true;//true:=entriegelt ; false:=verriegelt
  int counter = 0;//Counter für die Zyklen im Belegtstatus
  QList<BU*> bus;//Hier weden die BUs vermerkt
  QList< QPair<QString,Block*> > passiert;//Hier wird das auf der Grenze stehende Signal und der vorherige Block vermerkt
  //bool controlspeicher;//Hier wird vermekrt, ob der Block ein Vorblock für ein HS ist --> für Speicher einwählen benötigt
  bool haspassiert = false;//ist der Block an einer Grenze involviert = true, sonst false
  Stellwerkstechnik *technik = 0;//Signaltechnik (Liste aller Grenzssignale ist hier vermerkt)
  //
  bool evaluateFreigabe();
  //+++GUI+++
  //
  QList<QGraphicsRectItem*> blockitems;//Hier weden alle QGraphicsRectItems des Blocks gesammelt
  void changeColor();//verändert die Farbe der Blockitems
};
#endif
