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
    qWarning()<<"__BU__: Falsche Benennung: Eine zweistellige Zahl ist nötig.";
  }
}

void BU::setBU_status( bool status ){
  if(bu_status != status){                                  //only update when different state
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

bool BU::evaluateBU(){
  bool calculatedStatus = true;
  if( configuration->getWithHardware() and  buSegments.size() > 0){
    for( int i = 0; i < buSegments.size(); i++ ){
      calculatedStatus = calculatedStatus and buSegments.at(i)->getB_status();
    }
  }
  return calculatedStatus;
}

BU::~BU(){
  delete burect;
}

