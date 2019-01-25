//*************************************************************************
//Bahnübergang des Stellwerks  [- BU.H -]
//*************************************************************************
#ifndef BU_H
#define BU_H
#include <string>
#include <QGraphicsRectItem>
class BU{
 public:
  BU( int name );
  //
  void setName( int name );
  std::string getName(){ return buname; }
  void setBU_status( bool status );
  bool getBU_status(){ return bu_status; }
  //void setFreigabe( bool free );
  //bool getFreigabe(){ return freigabe; } --- wird nicht mehr verwendet
  //int getCounter(){ return counter; } --- wird nicht mehr verwendet
  //+++GUI+++
  void addBUrect(QGraphicsRectItem *item){ burect = item; }
  void changeColor();
 private:
  std::string buname;//Benennung der BÜs mit BU01, BU56
  bool bu_status = true;//true:=offen ; false:=geschlossen
  //+++GUI+++
  QGraphicsRectItem *burect = new QGraphicsRectItem();
  //bool freigabe = true;//true:=entriegelt ; false:=verriegelt --- wird nicht mehr verwendet
  //int counter = 0;//Counter für die Zyklen im Belegtstatus
  //
  //void evaluateFreigabe(); --- wird nicht mehr verwendet
};
#endif
