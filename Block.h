/*
 * segments for railroad [- BLOCK.H -]
 *******************************************************************************
 * Rail segments with feedback of the current state: occupied or free          *
 *    - includes crossings (BUs) and opens/closes them                         *
 *    - unlocking segments is done with evaluateFreigabe with the normal order *
 *      for block states: free-occupied-free --> unlock triggered              *
 *    - name range from "aa" to "zz"                                           *
 *******************************************************************************
 */
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
  void setFreigabe( bool free );                          //function for opening/closing BUs 
  bool getFreigabe(){ return freigabe; }
  int getCounter(){ return counter; }
  void addBus( BU *bu );
  QList<BU*> getBus(){ return bus; }
  void showBus();
  void deleteBus( BU* todelete );
  void addpassiert( QString grenzS, Block* prevBlock );  //needed for zugpassiert: which segments are in front of signals
  void deletepassiert();                                 //marking haspassiert as false und deletes all entries
  bool getHaspassiert(){ return haspassiert; }
  ~Block();
  //
  // +++ GUI +++
  //
  void addBlockitems( QGraphicsRectItem *rect ){ blockitems << rect; }
  //
public slots:
  void setB_status( bool status );
signals:
  void zugpassiert();                                    //signal goes to slot HSignal::zugpassiert
  void zugpassiertW();                                   //signal goes to slot WSignal::zugpassiert 
 private:
  QString blockname;                                     //Block naming convention: aa, ab, ac ... zz
  bool b_status = true;                                  //true:=free ; false:=occupied
  bool freigabe = true;                                  //true:=unlocked ; false:=locked
  int counter = 0;                                       //counter for state cycles (occupied/free)
  QList<BU*> bus;                                        //list of BUs integrated in this Block
  QList< QPair<QString,Block*> > passiert;               //list needed for zugpassiert: which segments are in front of signals (previous Block + this = precedessor Block, in between: signal)
  bool haspassiert = false;                              //flag if this=Block involved in zugpassiert procedure = true, else false
  Stellwerkstechnik *technik = 0;                        //Signaltechnik (list of all signals at neighbouring Blocks (in between) 
  //
  bool evaluateFreigabe();
  //+++GUI+++
  //
  QList<QGraphicsRectItem*> blockitems;                  //QGraphicsRectItems for visualisation of Blocks 
  void changeColor();                                    //change color in respect to the different states
};
#endif
