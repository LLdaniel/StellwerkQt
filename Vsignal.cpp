//*************************************************************************
//Vorsignal des Stellwerks  [- VSIGNAL.CXX -]
//*************************************************************************
#include "VSignal.h"
#include <QDebug>
#include <QBrush>
//
VSignal::VSignal( int name ){ setV_id( name ); }

void VSignal::setV_id( int name ){
  QString suffix = QString::number( name );//name zu String konvertieren
  if( name >0 && name <1000){//dreistelligkeit wird hier überprüft
    if( name < 10 ){//zwei Vornullen werden erzeugt
      v_id = "V00" + suffix;//std::to_string( name );
    }
    if( name < 100 && name > 9){//eine Vornull wird erzeugt
      v_id = "V0" + suffix;//std::to_string( name );
    }
    if( name < 1000 && name > 99){//keine Vornull mehr nötig
      v_id = "V" + suffix;//std::to_string( name );
    }
  }
  else qDebug()<<"VSignal:Falsche Benennung. Die Zahl muss dreistellig sein.";
}

void VSignal::setinFS( bool in , QString start , QString ziel ){
  //falls das VSignal in einer FS ist, schreibe auch das pair, ansonsten steht jetzt ".","." im pair
  //prüfe allerdings, ob das überhaupt eine zulässige Richtung ist:
  //if( getRichtung( start, ziel) >= 0){//ja, das ist eine zulässige Richtung
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
  //qDebug()<<"inf ist noch auf -1";
  for(  int i = 0 ; i < richtung.size() ; i++ ){
    //qDebug()<<"bin jetzt schon in der for-schleife";
    if( richtung.at(i).first.compare(start) == 0 && richtung.at(i).second.compare(ziel) == 0){//wenn das VSignal in diese Richtung signalisiert, dann return >=0
      inf = i;
      break;
    }
  }
  return inf;
}

bool VSignal::isAktFS( QString fsStart, QString fsZiel){//checkt, ob VS in bestimmter Richtung gerade aktuell ist
  if( aktFS.first.compare(fsStart) == 0 && aktFS.second.compare(fsZiel) == 0 ){//Wenn es aktuell ist, return true, sonst false
    return true;
  }
  else return false;
}

bool VSignal::isAktFS( QString fsziel ){//checkt, ob VS in bestimmter Richtung gerade aktuell ist
  //qDebug()<<"in isAktFS mit einem Arg";
  if( aktFS.second.compare(fsziel) == 0 ){//Wenn es aktuell ist, return true, sonst false
    //qDebug()<<"if isAkt in  isAktFS mit einem Arg";
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

VSignal::~VSignal(){
  delete fahrt;
  delete halt;
}
