//*************************************************************************
//Bahnübergang des Stellwerks  [- BU.CXX -]
//*************************************************************************
#include "BU.h"
#include <QDebug>
#include <QBrush>
//
BU::BU( int name ){
  setName( name );
}

void BU::setName( int name ){
  if( name > 0 && name < 100){//zweistellig?
    if( name < 10 ){//eine Vor-Null einfügen
      buname = "BU0" + QString::number( name );
    }
    else buname = "BU" + QString::number( name );//ansonsten ist keine Vor-Null nötig
  }
  else{
    qDebug()<<"BU::Falsche Benennung: Eine zweistellige Zahl ist nötig.";
  }
}

void BU::setBU_status( bool status ){
  //if( freigabe ){//falls nicht verriegelt-->freigabe --- wird nicht mehr verwendet
  bu_status = status;//Jetzt erst wird der Status gesetzt
  changeColor();
    //}
  //if( !freigabe ){//falls verriegelt: keine Möglichkeit zum öffnen --- wird nicht mehr verwendet
  //  qDebug()<<"Der Bahnübergang ist verriegelt!";
  //}
}
void BU::changeColor(){
    if(bu_status){
        burect->setBrush(QColor(153,50,204));//lila
    }
    if(!bu_status){
        burect->setBrush(QColor(255,64,64));//sehr helles rot
    }
}

BU::~BU(){
  delete burect;
}

//void BU::setFreigabe( bool free ){ --- wird nicht mehr verwendet
  // freigabe = free;
  //}

//void BU::evaluateFreigabe(){ --- wird nicht mehr verwendet
//  if( counter == 1 || counter == 0){//zum debuggen
//   qDebug()<<"BU::Der Counter =1|0";
// }
//if(counter == 2 && !bu_status){//Hier wird dann die Freigabe auf true gesetzt
//  qDebug()<<"BU::Der Counter = 2";
//  freigabe = true;//hier freigabe = true -->richtige Zyklenfolge
// }
//}

