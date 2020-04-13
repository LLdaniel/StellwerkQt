/*
 * turnout [- WEICHE.H -]
 */
#ifndef WEICHE_H
#define WEICHE_H
#include <QString>
#include <QGraphicsRectItem>
#include <QLabel>
class Weiche{
public:
  Weiche( int name, bool state = true );                                    //name as int, will be converted to QString
  //
  void setBelegung( bool newbelegung );
  bool getBelegung(){ return belegung; }
  int getCounter(){ return counter; }
  void setW_id( int name );
  QString getW_id(){ return w_id; }
  void setW_status( bool status );
  bool getW_status(){ return w_status; }
  void setVerriegelung ( bool lock ){ verriegelung = lock; changeColor(); }
  bool getVerriegelung(){ return verriegelung; }
  ~Weiche();
  //
  //+++ GUI +++
  //
  void addWeichenitem(QGraphicsRectItem *ab , QGraphicsRectItem *ge , QLabel *la);
  void moveLabel( int x , int y ){ beschriftung->move(x,y); }
  //
  //+++ GPIO +++
  //
  int getGpio( bool linksrechts );                     //0 straight || 1 deviated
  void setGpio( int pinGerade, int pinAbknickend );
  void switchWeiche(bool linksrechts);
private:
  QString w_id;                                        //i.e. 068
  bool w_status;                                       //true:=straight ; false:=deviated
  bool verriegelung = false;                           //true:=locked ; false:=unlocked
  bool belegung = true;                                //true:=unoccupied ; false:=occupied
  int counter = 0;                                     //counter for occupation cycles to determine unlock status
  //
  void evaluateVerriegelung();
  //
  //+++ GUI +++
  //
  QGraphicsRectItem *abknickend = new QGraphicsRectItem();
  QGraphicsRectItem *gerade = new QGraphicsRectItem();
  QLabel *beschriftung = new QLabel();
  //
  void changeColor();                                  //change appearance on plan
  //
  //+++ GPIO +++
  //
  int pin0 = -1;                                       //initialized as -1 if there is no hardware connection, so control center works without it 
  int pin1 = -1;
};
#endif
