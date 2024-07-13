/*
 * Stellwerkstechnik  [- STELLWERKSTECHNIK.CXX -]
 **************************************************************************
 */
#include "Stellwerkstechnik.h"
#include <QDebug>
//
void Stellwerkstechnik::add_Signal( QString strid , bool status ){
  //
  //convert string to HSignal naming convention:
  int id =  strid.right(3).toInt();
  //
  //differentiate between already registered and new signals:
  int pos = -1;                                                      //remember Position
  if(HSorWS(strid).compare("HS") == 0){                              //WS or HS?
    for(  int i = 0 ; i < hsignale.size() ; i++){                    //singal already existent?
      if( hsignale[i].first == id ){                                 //entry found  note: in QList [] -> non-const, mit at() -> const!
	pos = i;
	hsignale[i].second = status;                                 //change status
      }
    }
    if(pos == -1){                                                   //no entry existent: pos=-1 
      QPair<int,bool> newpair(id,status);
      hsignale.append(newpair);
    }
  }
  pos = -1;                                                          //safety measure, set to -1
  if(HSorWS(strid).compare("WS") == 0){
    for(  int i = 0 ; i < wsignale.size() ; i++){                    //singal already existent?
      if( wsignale[i].first == id ){                                            
	pos = i;
	wsignale[i].second = status;//Führe Statusänderung durch     //entry found  note: in QList [] -> non-const, mit at() -> const!
      }
    }
    if(pos == -1){                                                   //no entry existent: pos=-1 
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
      if(hsignale[i].first == todelete ){                             //find to delete signal HS
	remember = i;
	break;
      }
    }
  }
  if(HSorWS(strtodelete).compare("WS") == 0){
    for(  int i = 0; i < wsignale.size() ; i++){
      if(wsignale[i].first == todelete ){                             //find to delete signal WS
	remember = i;
	break;
      }
    }
  }
  //
  //deleting...
  hsignale.erase( hsignale.begin() + remember );
}

void Stellwerkstechnik::show_Signal(){
  qInfo()<<"************************************************************";
  qInfo()<<"*** Dies sind alle HSignale in Stellwerkstechnik         ***";
  for(  int i = 0 ; i < hsignale.size() ; i++){
    qInfo()<<"***   "<<hsignale[i].first<<"    =    "<<" "<<hsignale[i].second<<"                                       ***";//alle IDs der HSignale
  }
  qInfo()<<"*** Dies sind alle WSignale in Stellwerkstechnik         ***";
  for(  int j = 0 ; j < wsignale.size() ; j++){
    qInfo()<<"***   "<<hsignale.at(j).first<<"    =    "<<" "<<wsignale.at(j).second<<"                                       ***";//alle IDs der WSignale
  }
  qInfo()<<"************************************************************";
}
    
bool Stellwerkstechnik::getS_pass_status( QString statusvonSignal ){
  //
  //convert string to HSignal naming convention:
  int converted = statusvonSignal.right(3).toInt();
  bool feedback = false;
  if( HSorWS(statusvonSignal).compare("HS") == 0 ){
    for(  int i = 0 ; i < hsignale.size() ; i++){                     //search for signal and return bool HS 
      if( converted == hsignale[i].first ){
	feedback = hsignale[i].second;
	break;
      }
    }
  }
  if(HSorWS(statusvonSignal).compare("WS") == 0){
    for(  int i = 0 ; i < wsignale.size() ; i++){                    //search for signal and return bool WS 
      if( converted == wsignale[i].first ){
	feedback = wsignale[i].second;
	break;
      }
    }
  }
  return feedback;
}

QString Stellwerkstechnik::HSorWS(QString zutesten){
  QString decided;
  if(zutesten.at(0) == 'S'){ decided = "HS"; }
  if(zutesten.at(0) == 'W'){ decided = "WS"; }
  return decided;
}
