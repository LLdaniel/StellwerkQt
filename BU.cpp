/*
 * railroad crossing [- BU.CXX -]
 **************************************************************************
 */
#include "BU.h"
#include <QDebug>
#include <QBrush>
#ifdef __cplusplus
extern "C"{
#endif
#include <wiringPi.h>
#ifdef __cplusplus
}
#endif
//
BU::BU( int name, Configuration *config ){
  setName( name );
  configuration = config;
  t->start(3000);
}

void BU::setName( int name ){
  if( name > 0 && name < 99){
    if( name < 10 ){
      buname = "BU0" + QString::number( name );
    }
    else buname = "BU" + QString::number( name );//eles no 0 is needed before
  }
  else{
    qDebug()<<"BU::Falsche Benennung: Eine zweistellige Zahl ist nötig.";
  }
}

void BU::setBU_status( bool status ){
  //if( t->remainingTime() <= 0 ){                            //software realized smoothing function
    qDebug()<<t->remainingTime();
    bu_status = status;                                     //change state
    changeColor();
    if(status){
      open();
    }
    else close();
    t->start(3000);                                         // restart timer for next iteration
    //}
}
void BU::changeColor(){
    if(bu_status){
        burect->setBrush(QColor(Qt::gray));
    }
    if(!bu_status){
      burect->setBrush(QColor(Qt::white));
    }
}

void BU::close(){
  if( configuration->getWithHardware() ){
    if(pin > 0){
      digitalWrite(pin, HIGH);
      qDebug()<<"close BU";
    }
  }
}

void BU::open(){
  if( configuration->getWithHardware() ){
    if(pin > 0){
      digitalWrite(pin, LOW);
      qDebug()<<"open BU";
    }
  }
}

BU::~BU(){
  delete burect;
}

