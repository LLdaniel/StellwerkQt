//*************************************************************************
//Speicherliste des Stellwerks von Stellwerkstechnik [- STELLWERKSTECHNIK.CXX -]
//*************************************************************************
#include "Stellwerkstechnik.h"
#include <QDebug>
//
void Stellwerkstechnik::add_Signal( QString strid , bool status ){
  //Konvertierung von string zu int gemäß HSignal Benennung:
  int id =  strid.right(3).toInt();
  //Unterscheidung von Signalen, die schon in der Liste sind, und welche, die neu hinzuzufügen sind:
  int pos = -1;//merkt sich die Position
  if(HSorWS(strid).compare("HS") == 0){//je nach dem, ob WS oder HS...
    //qDebug()<<"STELLWERKSTECHNIK:: HS detektiert in add_Signal";
    for(  int i = 0 ; i < hsignale.size() ; i++){//suchen, ob Signal schon da ist?
      if( hsignale[i].first == id ){//es gibt schon einen Eintrag  in QList [] -> non-const, mit at() -> const!
	pos = i;
	hsignale[i].second = status;//Führe Statusänderung durch
      }
    }
    if(pos == -1){//es gibt keinen Eintrag, da nie == id zugetroffen hat und pos=-1 noch immer steht
      QPair<int,bool> newpair(id,status);
      hsignale.append(newpair);
    }
  }
  pos = -1;//reset zur Vorsicht
  if(HSorWS(strid).compare("WS") == 0){
    //qDebug()<<"STELLWERKSTECHNIK:: WS detektiert in add_Signal";
    for(  int i = 0 ; i < wsignale.size() ; i++){//suchen, ob Signal schon da ist?
      if( wsignale[i].first == id ){//es gibt schon einen Eintrag
	pos = i;
	wsignale[i].second = status;//Führe Statusänderung durch
	//qDebug()<<"NIX neues";
      }
    }
    if(pos == -1){//es gibt keinen Eintrag, da nie == id zugetroffen hat und pos=-1 noch immer steht
      QPair<int,bool> newpair(id,status);
      wsignale.append(newpair);
    }
  }
}

void Stellwerkstechnik::delete_HSignal(QString strtodelete ){
  int todelete = strtodelete.right(3).toInt();
  int remember = -1;
  if(HSorWS(strtodelete).compare("HS") == 0){
    for(  int i = 0; i < hsignale.size() ; i++){
      if(hsignale[i].first == todelete ){//falls das zu löschende Signal gefunden ist
	remember = i;
	//qDebug()<<"i_Stellwerkstechnik::deleteHsignal = "<<i;//DEBUG
	break;
      }
    }
  }
  if(HSorWS(strtodelete).compare("WS") == 0){
    for(  int i = 0; i < wsignale.size() ; i++){
      if(wsignale[i].first == todelete ){//falls das zu löschende Signal gefunden ist
	remember = i;
	//qDebug()<<"i_Stellwerkstechnik::deleteWsignal = "<<i;//DEBUG
	break;
      }
    }
  }
  //deleting...
  hsignale.erase( hsignale.begin() + remember );
}

void Stellwerkstechnik::show_Signal(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies sind alle HSignale in Stellwerkstechnik         ***";
  for(  int i = 0 ; i < hsignale.size() ; i++){
    qDebug()<<"***   "<<hsignale[i].first<<"    =    "<<" "<<hsignale[i].second<<"                                       ***";//alle IDs der HSignale
  }
  qDebug()<<"*** Dies sind alle WSignale in Stellwerkstechnik         ***";
  for(  int j = 0 ; j < wsignale.size() ; j++){
    qDebug()<<"***   "<<hsignale.at(j).first<<"    =    "<<" "<<wsignale.at(j).second<<"                                       ***";//alle IDs der WSignale
  }
  qDebug()<<"************************************************************";
}

bool Stellwerkstechnik::getS_pass_status( QString statusvonSignal ){
  //gebe den status des Signals zurück
  //zunächst den String in int gemäß HSignal/WSignal Benennung umwandeln
  int converted = statusvonSignal.right(3).toInt();
  qDebug()<<"CONVERTED: "<<converted;
  bool feedback = false;
  if( HSorWS(statusvonSignal).compare("HS") == 0 ){
    for(  int i = 0 ; i < hsignale.size() ; i++){//Suchen des betreffenden HSignals und Ausgabe des bool
      if( converted == hsignale[i].first ){
	feedback = hsignale[i].second;
	qDebug()<<"TEC_FEEDBACK_HSPfad"<<feedback;
	break;
      }
    }
  }
  if(HSorWS(statusvonSignal).compare("WS") == 0){
    for(  int i = 0 ; i < wsignale.size() ; i++){//Suchen des betreffenden WSignals und Ausgabe des bool
      if( converted == wsignale[i].first ){
	feedback = wsignale[i].second;
	qDebug()<<"TEC_FEEDBACK_WSPfad"<<feedback;
	break;
      }
    }
  }
  qDebug()<<"TEC_FEEDBACK:"<<feedback;
  return feedback;
}

QString Stellwerkstechnik::HSorWS(QString zutesten){
  QString decided;
  if(zutesten.at(0) == 'S'){ decided = "HS"; }
  if(zutesten.at(0) == 'W'){ decided = "WS"; }
  return decided;
}
