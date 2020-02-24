//*************************************************************************
//Weiche des Stellwerks  [- WEICHE.CXX -]
//*************************************************************************
#include "Weiche.h"
#include <QDebug>
#include <QFont>
#ifdef __cplusplus
extern "C"{
#endif
#include <wiringPi.h>
#include <sr595.h>
#ifdef __cplusplus
}
#endif
//
Weiche::Weiche (int name){
  setW_id( name );
}

void Weiche::setBelegung( bool newbelegung ){
  if( counter == 2){ counter = 0; }//reset counter falls =2
  counter++;//ansonsten vermerk, dass ein Belegungszyklus gemacht wird-->Statusänderung
  evaluateVerriegelung();//evtl entriegeln
  belegung = newbelegung;//jetzt kann der Status gesetzt werden
  changeColor();
}

void Weiche::setW_id( int name ){
  QString suffix = QString::number( name );//name zu String konvertieren
  if( name > 0 && name <10 ){//einstellig
    w_id = "00" + suffix;
  }
  if( name > 10 && name <100 ){//zweistellig
    w_id = "0" + suffix;
  }
  if( name > 100 && name <1000 ){//dreistellig
    w_id = suffix;
  }
}

void Weiche::setW_status( bool status ){
  if( verriegelung || !belegung ){//wenn eine Verriegelung/Belegung besteht; keine Stellerlaubnis
    qDebug()<<"Weiche::Diese Weiche ist verriegelt!";
  }
  else{//ok, dann ist stellen erlaubt
    w_status = status;
    switchWeiche(status);
    changeColor();
  }
}

void Weiche::addWeichenitem(QGraphicsRectItem *ab, QGraphicsRectItem *ge, QLabel *la)
{
    abknickend = ab;
    gerade = ge;
    beschriftung = la;
    QString qname = w_id ;
    beschriftung->setText(qname);
    QFont f( "Arial", 5, QFont::Bold);
    beschriftung->setFont(f);
    changeColor();
}

void Weiche::evaluateVerriegelung(){
  if( counter == 1 || counter == 0){//zum debuggen
    //qDebug()<<"Weiche::counter = 1|0";
  }
  if( counter == 2 && !belegung){//hier wird dann die Weiche freigegeben-->ist der richtige Zyklus hier
    //qDebug()<<"Weiche::Der counter = 2";
    verriegelung = false;//konkrete Entriegelung nach Zyklus true-false-true
    changeColor();
  }
}

void Weiche::changeColor(){
  if( w_status ){//immer die Stellung mit gelb anzeigen --> hier geradeaus
    gerade->setBrush(QColor(238,201,0));
    if(!belegung){
      abknickend->setBrush(Qt::red);
    }
    if(belegung && verriegelung){
      abknickend->setBrush(Qt::white);
    }
    if(belegung && !verriegelung){
      abknickend->setBrush(QColor(79,79,79));
    }
  }
  if( !w_status ){//immer die Stellung mit gelb anzeigen --> hier abknickend
    abknickend->setBrush(QColor(238,201,0));
    if(!belegung){
      gerade->setBrush(Qt::red);
    }
    if(belegung && verriegelung){
      gerade->setBrush(Qt::white);
    }
    if(belegung && !verriegelung){
      gerade->setBrush(QColor(79,79,79));
    }
  }
}

Weiche::~Weiche(){
  delete abknickend;
  delete gerade;
  delete beschriftung;
}


int Weiche::getGpio( bool linksrechts ){
  int aktuellerPin = -1;
  if(linksrechts){
    aktuellerPin = pin0;
  }
  if(!linksrechts){
    aktuellerPin = pin1;
  }
  return aktuellerPin;
}

void Weiche::setGpio(int pinGerade, int pinAbknickend){
  pin0 = pinGerade;
  pin1 = pinAbknickend;
}

void Weiche::switchWeiche(bool linksrechts){
  if(pin0 > 0 and pin1 > 0){//nur wenn beide pins initialisiert sind, soll auch gestellt werden
    if(linksrechts){
      digitalWrite(pin0, HIGH);
      delay(25);//[ms]
      digitalWrite(pin0, LOW);
    }
    if(!linksrechts){
      digitalWrite(pin1, HIGH);
      delay(25);//[ms]
      digitalWrite(pin1, LOW);
    }
  }
  else{
    qDebug()<<"Weiche mit Kennung "<<getW_id()<<" ist nicht als pin initialisiert";
  }
}
