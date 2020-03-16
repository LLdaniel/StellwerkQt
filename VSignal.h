/*
 * distant signal [- VSIGNAL.H -]
 *******************************************************************************
 * Distant signals show the expected sign of the main signal                   *
 *******************************************************************************
 */
#ifndef VSIGNAL_H
#define VSIGNAL_H
#include <QList>
#include <QString>
#include <QPair>
#include <QGraphicsRectItem>
#include <QGraphicsSvgItem>
#include <QObject>
//
class VSignal {
 public:
  VSignal( int name );
  void setV_id( int name );
  QString getV_id(){ return v_id; }
  void setV_status( bool status ){ v_status = status; changeColor(); }
  bool getV_status(){ return v_status; }
  void setinFS ( bool in , QString start = "." , QString ziel = "." );       //at the same time aktFS will be set
  bool isAktFS( QString fsStart, QString fsZiel);                            //is in current route? also useful for vorsignalV 
  bool isAktFS( QString fsziel );                                            //is in current route? also useful for vorsignalR (with no second argument)
  bool getinFS(){ return inFS; }
  QPair<QString,QString> getAktFS(){ return aktFS; }
  void setRichtung( QString start, QString ziel );
  int getRichtung( QString start , QString ziel );                           //return direction from start to end <O: not existent
  void showRichtung();
  void deleteRichtung( QString todeleteStart, QString todeleteZiel );
  void zugpassiert();                                                        //distant signal shows "expect stop" + inFS will be set to false
  ~VSignal();
  //
  //+++ GUI +++
  //
  void addVSignalitems(QGraphicsSvgItem *signalbild1, QGraphicsSvgItem *signalbild2){fahrt = signalbild1; halt = signalbild2;}
protected:
  QString v_id;                                                              //name: V001, V068
  bool v_status = false;                                                     //false:= expect stop, true:= expect go
  bool inFS = false;                                                         //currently in a route?
  QPair<QString,QString> aktFS;                                              //will be set with setinFS
  QList<QPair<QString, QString> > richtung;                                  //start and end signal is saved here-> possibilites, where distant signal can be involved
  //
  //+++ GUI +++
  //
  QGraphicsSvgItem *fahrt; 
  QGraphicsSvgItem *halt;
  void changeColor();                                                        //change display on plan
};
#endif
