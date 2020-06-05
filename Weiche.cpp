/*
 * turnout [- WEICHE.CXX -]
 **************************************************************************
 */
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
Weiche::Weiche (int name, bool state/*=true*/, bool kreuz/*=false*/){
  setW_id( name );
  w_status = state;
  kreuzung = kreuz;
}

void Weiche::setBelegung( bool newbelegung ){
  if( counter == 2){ counter = 0; }                           //reset counter if  =2
  counter++;                                                  //otherwise: remember state change in cycle
  evaluateVerriegelung();                                     //unlock maybe
  belegung = newbelegung;                                     //set status
  changeColor();
}

void Weiche::setW_id( int name ){
  QString suffix = QString::number( name );                   //convert name to QString
  if( name > 0 && name <10 ){
    w_id = "00" + suffix;
  }
  if( name > 9 && name < 100 ){
    w_id = "0" + suffix;
  }
  if( name > 99 && name <1000 ){
    w_id = suffix;
  }
}

void Weiche::setW_status( bool status ){
  if( verriegelung || !belegung ){                           //turnout locked, no change in status possible 
    qDebug()<<"Weiche::Diese Weiche ist verriegelt!";
  }
  else{                                                      //ok, change status is permitted
    w_status = status;
    //qDebug()<<"change turnout "<<w_id<<"permitted"; 
    switchWeiche(status);
    if(kreuzung){emit kreuzungsweiche(status);}              //if part in double turnout: call to Connector
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
    QFont f( "Arial", 10, QFont::Bold);
    beschriftung->setFont(f);
    changeColor();
}

void Weiche::evaluateVerriegelung(){
  if( counter == 1 || counter == 0){                         //debug
  }
  if( counter == 2 && !belegung){                            //unlock turnout
    verriegelung = false;                                    //unlock after cycle true-false-true
    changeColor();
  }
}

void Weiche::changeColor(){
  if( w_status ){                                            //current turnout state is displayed with yellow: straight
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
  if( !w_status ){                                          //current turnout state is displayed with yellow: deviated
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
  if(pin0 > 0 and pin1 > 0){                                 //connect to hadware command only when there are two directions set
    if(linksrechts){
      //qDebug()<<"Stelle weiche "<<w_id<<" auf true";
      digitalWrite(pin0, HIGH);
      delay(20);//[ms]
      digitalWrite(pin0, LOW);
    }
    if(!linksrechts){
      qDebug()<<"Stelle weiche "<<w_id<<" auf false";
      digitalWrite(pin1, HIGH);
      delay(20);//[ms]
      digitalWrite(pin1, LOW);
    }
  }
  else{
    qDebug()<<"Weiche mit Kennung "<<getW_id()<<" ist nicht als pin initialisiert";
  }
}
