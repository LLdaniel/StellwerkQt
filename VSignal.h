//*************************************************************************
//Vorsignal des Stellwerks  [- VSIGNAL.H -]
//*************************************************************************
#ifndef VSIGNAL_H
#define VSIGNAL_H
#include <vector>
#include <string>
#include <QGraphicsRectItem>
#include <QObject>
//
class VSignal {
 public:
  VSignal( int name );
  void setV_id( int name );
  std::string getV_id(){ return v_id; }
  void setV_status( bool status ){ v_status = status; changeColor(); }
  bool getV_status(){ return v_status; }
  void setinFS ( bool in , std::string start = "." , std::string ziel = "." );//gleichzeitig wird auch aktFS gesetzt
  bool isAktFS( std::string fsStart, std::string fsZiel);//checkt, ob VS in bestimmter Richtung gerade aktuell ist-->für die Abfrage vorsignalV auch gedacht
  bool isAktFS( std::string fsziel );//checkt, ob VS in bestimmter Richtung gerade aktuell ist-->für die Abfrage vorsignalR gedacht, da das Startsignal nicht [ein Übergabeargument]
  bool getinFS(){ return inFS; }
  std::pair<std::string,std::string> getAktFS(){ return aktFS; }
  void setRichtung( std::string start, std::string ziel );
  int getRichtung( std::string start , std::string ziel );//Übergabe Start und Ziel liefert Pos wo die Richtungsangabe steht, kleiner null bedeutet: existiert nicht
  void showRichtung();
  void deleteRichtung( std::string todeleteStart, std::string todeleteZiel );
  void zugpassiert();//VS fällt auf Halt erwarten und inFS wird auf false gesetzt-->kann neu verwendet werden
  //
  //+++GUI+++
  void addVSignalitems(QGraphicsRectItem *rect ){ vsignalitems << rect; changeColor(); }//add new rect items
 protected:
  std::string v_id;//Name des VS: V001, V068
  bool v_status = false;//false::= Halt erwarten, true:= Fahrt erwarten
  bool inFS = false;//zeigt an, ob in einer FS involviert
  std::pair<std::string,std::string> aktFS;//std::pair<Startsignal,Zielsignal>, wird mit inFS gesetzt-->somit steht die Richtung fest
  std::vector<std::pair<std::string, std::string> > richtung;//Hier wird Start, Ziel HSignal gespeichert-->wo kann VSignal involviert sein
  //
  //+++GUI+++
  QList<QGraphicsRectItem*> vsignalitems;//Hier weden alle QGraphicsRectItems des Blocks gesammelt
  void changeColor();//verändert die Farbe der VSignalitems
};
#endif

//es gibt keine Verriegelung bei VS: manuell könnte man eingreifen, ansonsten wird VS erst gestellt, wenn auch die FS frei ist, dh, andernfalls wäre zusätzlich immer ein Block verriegelt/belegt und es würde eh keine FS gestellt werden-->VS sicher
