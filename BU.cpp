//*************************************************************************
//Bahnübergang des Stellwerks  [- BU.CXX -]
//*************************************************************************
#include "BU.h"
#include <iostream>
#include <string>
#include <QBrush>
//
BU::BU( int name ){
  setName( name );
}

void BU::setName( int name ){
  if( name > 0 && name < 100){//zweistellig?
    if( name < 10 ){//eine Vor-Null einfügen
      buname = "BU0" + std::to_string( name );
    }
    else buname = "BU" + std::to_string( name );//ansonsten ist keine Vor-Null nötig
  }
  else{
    std::cout<<"BU::Falsche Benennung: Eine zweistellige Zahl ist nötig."<<std::endl;
  }
}

void BU::setBU_status( bool status ){
  //if( freigabe ){//falls nicht verriegelt-->freigabe --- wird nicht mehr verwendet
  bu_status = status;//Jetzt erst wird der Status gesetzt
  changeColor();
    //}
  //if( !freigabe ){//falls verriegelt: keine Möglichkeit zum öffnen --- wird nicht mehr verwendet
  //  std::cout<<"Der Bahnübergang ist verriegelt!"<<std::endl;
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

//void BU::setFreigabe( bool free ){ --- wird nicht mehr verwendet
  // freigabe = free;
  //}

//void BU::evaluateFreigabe(){ --- wird nicht mehr verwendet
//  if( counter == 1 || counter == 0){//zum debuggen
//   std::cout<<"BU::Der Counter =1|0"<<std::endl;
// }
//if(counter == 2 && !bu_status){//Hier wird dann die Freigabe auf true gesetzt
//  std::cout<<"BU::Der Counter = 2"<<std::endl;
//  freigabe = true;//hier freigabe = true -->richtige Zyklenfolge
// }
//}

