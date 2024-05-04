/*
 * railroad crossings [- BU.H -]
 *******************************************************************************
 * automatically managed railroad crossing                                     *
 *    - is managed with rail segments                                          *
 *******************************************************************************
 */
#ifndef BU_H
#define BU_H
#include <QString>
#include <QGraphicsRectItem>
#include <QTimer>
#include "Configuration.h"
#include "Block.h"
class BU : public QObject{
  Q_OBJECT
 public:
  BU( int name, Configuration *config );
  //
  void setName( int name );
  QString getName(){ return buname; }
  void setBU_status( bool status );
  bool getBU_status(){ return bu_status; }
  void addSegments( Block *block ){ buSegments.push_back(block); }
  bool evaluateBU();
  //
  //+++ GUI +++
  //
  void addBUrect(QGraphicsRectItem *item){ burect = item; }
  void changeColor();
  //
  //+++ GPIO +++
  //
  int getGpio(){ return pin; }
  void setGpio( int BUpin ){ pin = BUpin; }
  void close();
  void open();
  ~BU();
 private:
  QString buname;                                           //BU naming convention BU01, BU56
  bool bu_status = true;                                    //true:=open ; false:=closed
  Configuration *configuration;
  QList<Block*> buSegments;
  //
  //+++ GUI +++
  //
  QGraphicsRectItem *burect = new QGraphicsRectItem();
  //
  //+++ GPIO +++
  //
  int pin = -1;
};
#endif
