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
  if( getBU_status() != status ){                           //update only
    bu_status = status;                                     //change state
    changeColor();
    if(status){
      open();
    }
    else close();
  }
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
    }
  }
}

void BU::open(){
  if( configuration->getWithHardware() ){
    if(pin > 0){
      digitalWrite(pin, LOW);
    }
  }
}

BU::~BU(){
  delete burect;
}

