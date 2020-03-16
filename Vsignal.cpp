/*
 * distant signal [- VSIGNAL.CXX -]
 **************************************************************************
 */
#include "VSignal.h"
#include <QDebug>
#include <QBrush>
//
VSignal::VSignal( int name ){ setV_id( name ); }

void VSignal::setV_id( int name ){
  QString suffix = QString::number( name );            //convert name to QString 
  if( name >0 && name <1000){
    if( name < 10 ){
      v_id = "V00" + suffix;
    }
    if( name < 100 && name > 9){
      v_id = "V0" + suffix;
    }
    if( name < 1000 && name > 99){
      v_id = "V" + suffix;
    }
  }
  else qDebug()<<"VSignal:Falsche Benennung. Die Zahl muss dreistellig sein.";
}

void VSignal::setinFS( bool in , QString start , QString ziel ){
  //
  //if distant signal is inFS, set also pair with start and end signal, otherwise there will be ".","." in the pair
  inFS = in;
  aktFS.first = start;
  aktFS.second = ziel;
}

void VSignal::setRichtung(QString start, QString ziel ){
  QPair<QString,QString> hilfspair( start , ziel );
  richtung.push_back(hilfspair);
}

int VSignal::getRichtung( QString start , QString ziel){
  int inf = -1;
  for(  int i = 0 ; i < richtung.size() ; i++ ){
    if( richtung.at(i).first.compare(start) == 0 && richtung.at(i).second.compare(ziel) == 0){//if distant signal shows this direction, then return >=0
      inf = i;
      break;
    }
  }
  return inf;
}

bool VSignal::isAktFS( QString fsStart, QString fsZiel){               //current direction active?
  if( aktFS.first.compare(fsStart) == 0 && aktFS.second.compare(fsZiel) == 0 ){//if its active, return true, otherwise false
    return true;
  }
  else return false;
}

bool VSignal::isAktFS( QString fsziel ){                              //current direction active?
  if( aktFS.second.compare(fsziel) == 0 ){                            //if its active, return true, otherwise false
    return true;
  }
  else return false;
}

void VSignal::showRichtung(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies ist sind die Richtungen von Vorsignal "<<getV_id()<<"      ***";
  for(  int i = 0 ; i < richtung.size() ; i++){
    qDebug()<<"***   "<<richtung.at(i).first<<" -->   "<<richtung.at(i).second<<"                                    ***";
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void VSignal::deleteRichtung( QString todeleteStart, QString todeleteZiel ){
  int del = getRichtung(todeleteStart,todeleteZiel);
  if( del >= 0 ){                                                               //check direction
    richtung.erase( richtung.begin() + getRichtung(todeleteStart,todeleteZiel) );
  }
}

void VSignal::zugpassiert(){
  setV_status( false );                            //distant signal shows "expect stop"
  setinFS( false );                                //delete inFS
  changeColor();
}

void VSignal::changeColor(){
  //
  //keep in mind: WSignal inherits from VSignal
  if(v_status){                     
    halt->setVisible(false);
    fahrt->setVisible(true);
  }
  if(!v_status){
    halt->setVisible(true);
    fahrt->setVisible(false);
  }
}

VSignal::~VSignal(){
  delete fahrt;
  delete halt;
}
