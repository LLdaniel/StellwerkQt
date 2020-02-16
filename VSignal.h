//*************************************************************************
//Vorsignal des Stellwerks  [- VSIGNAL.H -]
//*************************************************************************
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
  void setinFS ( bool in , QString start = "." , QString ziel = "." );//gleichzeitig wird auch aktFS gesetzt
  bool isAktFS( QString fsStart, QString fsZiel);//checkt, ob VS in bestimmter Richtung gerade aktuell ist-->für die Abfrage vorsignalV auch gedacht
  bool isAktFS( QString fsziel );//checkt, ob VS in bestimmter Richtung gerade aktuell ist-->für die Abfrage vorsignalR gedacht, da das Startsignal nicht [ein Übergabeargument]
  bool getinFS(){ return inFS; }
  QPair<QString,QString> getAktFS(){ return aktFS; }
  void setRichtung( QString start, QString ziel );
  int getRichtung( QString start , QString ziel );//Übergabe Start und Ziel liefert Pos wo die Richtungsangabe steht, kleiner null bedeutet: existiert nicht
  void showRichtung();
  void deleteRichtung( QString todeleteStart, QString todeleteZiel );
  void zugpassiert();//VS fällt auf Halt erwarten und inFS wird auf false gesetzt-->kann neu verwendet werden
  ~VSignal();
  //
  //+++GUI+++
  void addVSignalitems(QGraphicsSvgItem *signalbild1, QGraphicsSvgItem *signalbild2){fahrt = signalbild1; halt = signalbild2;}
protected:
  QString v_id;//Name des VS: V001, V068
  bool v_status = false;//false::= Halt erwarten, true:= Fahrt erwarten
  bool inFS = false;//zeigt an, ob in einer FS involviert
  QPair<QString,QString> aktFS;//QPair<Startsignal,Zielsignal>, wird mit inFS gesetzt-->somit steht die Richtung fest
  QList<QPair<QString, QString> > richtung;//Hier wird Start, Ziel HSignal gespeichert-->wo kann VSignal involviert sein
  //
  //+++GUI+++
  QGraphicsSvgItem *fahrt;//Signalbild 1 wird hier gespeichert
  QGraphicsSvgItem *halt;//Signalbild 2 wird hier gespeichert
  void changeColor();//verändert die Farbe der VSignalitems
};
#endif

//es gibt keine Verriegelung bei VS: manuell könnte man eingreifen, ansonsten wird VS erst gestellt, wenn auch die FS frei ist, dh, andernfalls wäre zusätzlich immer ein Block verriegelt/belegt und es würde eh keine FS gestellt werden-->VS sicher
