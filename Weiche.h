//*************************************************************************
//Weiche des Stellwerks  [- WEICHE.H -]
//*************************************************************************
#ifndef WEICHE_H
#define WEICHE_H
#include <string>
#include <QGraphicsRectItem>
#include <QLabel>
class Weiche{
public:
Weiche( int name );//Name wird als int übergeben und später zum string umgebastelt
  //
  void setBelegung( bool newbelegung );
  bool getBelegung(){ return belegung; }
  int getCounter(){ return counter; }
  void setW_id( int name );
  std::string getW_id(){ return w_id; }
  void setW_status( bool status );
  bool getW_status(){ return w_status; }
  void setVerriegelung ( bool lock ){ verriegelung = lock; changeColor(); }
  bool getVerriegelung(){ return verriegelung; }
  ~Weiche();
  //
  //+++GUI+++
  void addWeichenitem(QGraphicsRectItem *ab , QGraphicsRectItem *ge , QLabel *la);
  void moveLabel( int x , int y ){ beschriftung->move(x,y); }
private:
  std::string w_id;//dreistellige Ziffernfolge der Form 068
  bool w_status = true;//true:=gerade ; false:=abknickend
  bool verriegelung = false;//true:=verriegelt ; false:=entriegelt
bool belegung = true;//true:=frei ; false:=belegt
  int counter = 0;//Counter für die Zyklen im Belegtstatus
  //
  void evaluateVerriegelung();
  //
  //+++GUI+++
  QGraphicsRectItem *abknickend = new QGraphicsRectItem();//Flanke abknickend
  QGraphicsRectItem *gerade = new QGraphicsRectItem();//Flanke geradeaus
  QLabel *beschriftung = new QLabel();//Beschriftung
  //
  void changeColor();//ändert je nach status, belegung und verriegelung die farbe
};
#endif
//Weiche muss umgeschrieben werden: Hardwaretechnisch ist keine Belegtanzeige möglich auf der Weiche (Massebrücke)
//Deshalb ist die Alternatividee: bei jeder FS wird vermerkt, was der Block vor der Weiche ist:
//Wenn dieser von belegt auf frei geht, ist der Zug drüber weg und es kann nach 1-2s die Weiche entriegelt werden
//Sonderfall ist, wenn der Zug schon auf dem vorderen Block steht: dann muss
//Schweierigkeiten außerdem bei: Weichen die eine Weiche davor haben
