//*************************************************************************
//Vorsignal des Stellwerks  [- VSIGNAL.CXX -]
//*************************************************************************
#include "VSignal.h"
#include <iostream>
#include <string>
#include <QBrush>
//
VSignal::VSignal( int name ){ setV_id( name ); }

void VSignal::setV_id( int name ){
  std::string suffix = std::to_string( name );//name zu String konvertieren
  if( name >0 && name <1000){//dreistelligkeit wird hier überprüft
    if( name < 10 ){//zwei Vornullen werden erzeugt
      v_id = "V00" + suffix;//std::to_string( name );
    }
    if( name < 100 && name > 10){//eine Vornull wird erzeugt
      v_id = "V0" + suffix;//std::to_string( name );
    }
    if( name < 1000 && name > 100){//keine Vornull mehr nötig
      v_id = "V" + suffix;//std::to_string( name );
    }
  }
  else std::cout<<"VSignal:Falsche Benennung. Die Zahl muss dreistellig sein."<<std::endl;
}

void VSignal::setinFS( bool in , std::string start , std::string ziel ){
  //falls das VSignal in einer FS ist, schreibe auch das pair, ansonsten steht jetzt ".","." im pair
  //prüfe allerdings, ob das überhaupt eine zulässige Richtung ist:
  //if( getRichtung( start, ziel) >= 0){//ja, das ist eine zulässige Richtung
  inFS = in;
  aktFS.first = start;
  aktFS.second = ziel;
}

void VSignal::setRichtung(std::string start, std::string ziel ){
  std::pair<std::string,std::string> hilfspair( start , ziel );
  richtung.push_back(hilfspair);
}

int VSignal::getRichtung( std::string start , std::string ziel){
  int inf = -1;
  //std::cout<<"inf ist noch auf -1"<<std::endl;
  for( unsigned int i = 0 ; i < richtung.size() ; i++ ){
    //std::cout<<"bin jetzt schon in der for-schleife"<<std::endl;
    if( richtung.at(i).first.compare(start) == 0 && richtung.at(i).second.compare(ziel) == 0){//wenn das VSignal in diese Richtung signalisiert, dann return >=0
      inf = i;
      break;
    }
  }
  return inf;
}

bool VSignal::isAktFS( std::string fsStart, std::string fsZiel){//checkt, ob VS in bestimmter Richtung gerade aktuell ist
  if( aktFS.first.compare(fsStart) == 0 && aktFS.second.compare(fsZiel) == 0 ){//Wenn es aktuell ist, return true, sonst false
    return true;
  }
  else return false;
}

bool VSignal::isAktFS( std::string fsziel ){//checkt, ob VS in bestimmter Richtung gerade aktuell ist
  //std::cout<<"in isAktFS mit einem Arg"<<std::endl;
  if( aktFS.second.compare(fsziel) == 0 ){//Wenn es aktuell ist, return true, sonst false
    //std::cout<<"if isAkt in  isAktFS mit einem Arg"<<std::endl;
    return true;
  }
  else return false;
}

void VSignal::showRichtung(){
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<"*** Dies ist sind die Richtungen von Vorsignal "<<getV_id()<<"      ***"<<std::endl;
  for( unsigned int i = 0 ; i < richtung.size() ; i++){
    std::cout<<"***   "<<richtung.at(i).first<<" -->   "<<richtung.at(i).second<<"                                    ***"<<std::endl;
  }
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<""<<std::endl;
}

void VSignal::deleteRichtung( std::string todeleteStart, std::string todeleteZiel ){
  int del = getRichtung(todeleteStart,todeleteZiel);
  if( del >= 0 ){//Abprüfen, nicht dass getRichtung schon nichts gefunden hat
    richtung.erase( richtung.begin() + getRichtung(todeleteStart,todeleteZiel) );
  }
}

void VSignal::zugpassiert(){
  setV_status( false );//VS geht auf Halt erwarten
  setinFS( false );//VS geht auf nicht-inFS
  changeColor();
}

void VSignal::changeColor(){
    //WSignal erbt von VSignal, will aber unterschiedliche Farben haben
  if(v_status){//wenn VS Fahrt erwarten zeigt
    halt->setVisible(false);
    fahrt->setVisible(true);
  }
  if(!v_status){//wenn VS Halt erwarten zeigt
    halt->setVisible(true);
    fahrt->setVisible(false);
  }
}
