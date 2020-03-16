/*
 * railroad crossing [- BU.CXX -]
 **************************************************************************
 */
#include "BU.h"
#include <QDebug>
#include <QBrush>
//
BU::BU( int name ){
  setName( name );
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
  bu_status = status;                                     //change state
  changeColor();
}
void BU::changeColor(){
    if(bu_status){
        burect->setBrush(QColor(153,50,204));             //lila
    }
    if(!bu_status){
        burect->setBrush(QColor(255,64,64));              //light red
    }
}

BU::~BU(){
  delete burect;
}

