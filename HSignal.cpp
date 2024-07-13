/*
 * main signal [- HSIGNAL.CXX -]
 **************************************************************************
 */
#include "HSignal.h"
#include <QDebug>

HSignal::HSignal(int name){
  setS_id( name );
}

void HSignal::setS_id( int name ){
  QString suffix = QString::number( name );    //name to QString 
  if( name >0 && name <1000){
    if( name < 10 ){
      s_id = "S00" + suffix;
    }
    if( name < 100 && name > 9){
      s_id = "S0" + suffix;
    }
    if( name < 1000 && name > 99){
      s_id = "S" + suffix;
    }
  }
  else qInfo()<<"__HSignal__: Falsche Benennung. Die Zahl muss dreistellig sein.";
}

void HSignal::addWeichenstatus( HSignal *toZiel , QList<QPair<Weiche* , bool>> weichenpair ){
  QPair<QString, QList<QPair<Weiche* , bool>> > hilfspair( toZiel->getS_id() , weichenpair );
  weichenstatus.push_back(hilfspair);
}

void HSignal::addWeichenstatusZuRangier( WSignal *toZiel , QList<QPair<Weiche* , bool>> weichenpair ){
  QPair<QString, QList<QPair<Weiche* , bool>> > hilfspair( toZiel->getV_id() , weichenpair );
  weichenstatusZuRangier.push_back(hilfspair);
}

void HSignal::showWeichenstatusALL(){
  qInfo()<<"************************************************************";
  qInfo()<<"*** Dies ist der Weichenstatus für Hauptsignal "<<getS_id()<<"      ***";
  for(  int i = 0 ; i < weichenstatus.size() ; i++){
    qInfo()<<"***   "<<weichenstatus.at(i).first<<" :                                             ***";
    for(  int j = 0 ; j < weichenstatus.at(i).second.size() ; j++ ){//hier sind die Weichenstatuspaarungen
      qInfo()<<"***               "<<weichenstatus.at(i).second.at(j).first->getW_id()<<"   -->   "<<weichenstatus.at(i).second.at(j).second<<"                          ***";
    }
  }
  qInfo()<<"************************************************************";
  qInfo()<<"";
}

void HSignal::showWeichenstatus( HSignal *whichZiel ){
  for(  int i = 0 ; i < weichenstatus.size() ; i++){
    if( weichenstatus.at(i).first.compare( whichZiel->getS_id() ) == 0 ){
      qInfo()<<"";
      qInfo()<<"************************************************************";
      qInfo()<<"*** Dies ist der Weichenstatus für Hauptsignal "<<getS_id()<<"      ***";
      qInfo()<<"***   "<<weichenstatus.at(i).first<<" :                                             ***";
      for(  int j = 0 ; j < weichenstatus.at(i).second.size() ; j++ ){
	qInfo()<<"***               "<<weichenstatus.at(i).second.at(j).first->getW_id()<<"   -->   "<<weichenstatus.at(i).second.at(j).second<<"                          ***";
      }
      qInfo()<<"************************************************************";
      qInfo()<<"";
    }
  }
}

void HSignal::addBlock( HSignal *toZiel , QList<Block*> inputBlock ){
  QPair<QString , QList<Block*>> hilfspair(toZiel->getS_id(), inputBlock);
  block.push_back(hilfspair);
  //
  //connect for memory display on plan
  QObject::connect(this,&HSignal::illuminateSpeicher, toZiel, &HSignal::recieveSpeicher);
}

void HSignal::addBlockZuRangier( WSignal *toZiel , QList<Block*> inputBlock ){
  QPair<QString , QList<Block*>> hilfspair(toZiel->getV_id(), inputBlock);
  blockZuRangier.push_back(hilfspair);
}

void HSignal::showBlock( HSignal *whichZiel ){
  for(  int i = 0 ; i < block.size() ; i++){
    if( block.at(i).first.compare( whichZiel->getS_id() ) == 0){
      qInfo()<<"";
      qInfo()<<"************************************************************";
      qInfo()<<"*** Dies sind die Blöcke für Hauptsignal "<<getS_id()<<"            ***";
      qInfo()<<"***   "<<block.at(i).first<<" :                                             ***";
      for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){//hier sind die Blöcke
	qInfo()<<"***               "<<block.at(i).second.at(j)->getName()<<"                                     ***";
      }
      qInfo()<<"************************************************************";
      qInfo()<<"";
    }
  }
}

void HSignal::showBlockALL(){
  qInfo()<<"************************************************************";
  qInfo()<<"*** Dies sind die Blöcke für Hauptsignal "<<getS_id()<<"            ***";
  for(  int i = 0 ; i < block.size() ; i++){
    qInfo()<<"***   "<<block.at(i).first<<" :                                             ***";
    for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){
      qInfo()<<"***               "<<block.at(i).second.at(j)->getName()<<"                                     ***";
    }
  }
  qInfo()<<"************************************************************";
  qInfo()<<"";
}

void HSignal::deleteNachbar( HSignal *todelete ){
  //
  //delete segments and turnouts
  //Weichenstatus:
  int remember = -1;
  for(  int i = 0 ; i < weichenstatus.size() ; i++ ){
    if( weichenstatus.at(i).first.compare( todelete->getS_id() ) == 0 ){//status for to deleting signal found: remember it
      remember = i;
    }
  }
  if( remember >= 0 ){                       //does todelete really exist?
    weichenstatus.erase( weichenstatus.begin() + remember );//delete on i-th location
  }
  remember = -1;
  //
  //Block:
  for(  int j = 0 ; j < block.size() ; j++ ){
    if( block.at(j).first.compare( todelete->getS_id() ) == 0 ){//status for to deleting signal found: remember it
      remember = j;
    }
  }
  if(remember >= 0 ){
    block.erase( block.begin() + remember);//deletes on j-th location
  }
}

void HSignal::addVSignal( VSignal *vs , QString param){

  if( param.compare("v") == 0 ){                      //parameter v looks forwards
    vorsignalV.push_back(vs);
  }
  if( param.compare("r") == 0 ){                      //parameter r looks backwards
    vorsignalR.push_back(vs);
  }
  if( param.compare("v")!=0 && param.compare("r")!=0  ){
    qWarning()<<"__HSignal__: Die Parameterübergabe ist ungültig: V/v oder R/r erwünscht!";
  }
}

void HSignal::showVSignalR(){
  qInfo()<<"************************************************************";
  qInfo()<<"*** Dies sind die Vorsignale für Hauptsignal [rück]"<<getS_id()<<"  ***";
  for(  int i = 0 ; i < vorsignalR.size() ; i++){
    qInfo()<<"***   --> "<<vorsignalR.at(i)->getV_id()<<"                                           ***";
  }
  qInfo()<<"************************************************************";
  qInfo()<<"";
}

void HSignal::showVSignalV(){
  qInfo()<<"************************************************************";
  qInfo()<<"*** Dies sind die Vorsignale für Hauptsignal [vor]"<<getS_id()<<"   ***";
  for(  int i = 0 ; i < vorsignalV.size() ; i++){
    qInfo()<<"***   --> "<<vorsignalV.at(i)->getV_id()<<"                                           ***";
  }
  qInfo()<<"************************************************************";
  qInfo()<<"";
}

void HSignal::addWSignal(WSignal *wsig, QString param){
  if( param.compare("v") == 0 ){                        //parameter v looks forwards
    wsignaleV.push_back(wsig);
  }
  if( param.compare("r") == 0 ){                        //parameter r looks backwards
    wsignaleR.push_back(wsig);
  }
  if( param.compare("v")!=0 && param.compare("r")!=0  ){
    qWarning()<<"__HSignal__: Die Parameterübergabe ist ungültig: v oder r erwünscht!";
  }
}
void HSignal::showWSignalR(){
  qInfo()<<"************************************************************";
  qInfo()<<"*** Dies sind die Rangiersignale für Hauptsignal[rück]"<<getS_id()<<"***";
  for(  int i = 0 ; i < wsignaleR.size() ; i++){
    qInfo()<<"***   --> "<<wsignaleR.at(i)->getV_id()<<"                                           ***";
  }
  qInfo()<<"************************************************************";
  qInfo()<<"";
}

void HSignal::showWSignalV(){
  qInfo()<<"************************************************************";
  qInfo()<<"*** Dies sind die Rangiersignale für Hauptsignal[vor]"<<getS_id()<<"***";
  for(  int i = 0 ; i < wsignaleV.size() ; i++){
    qInfo()<<"***   --> "<<wsignaleV.at(i)->getV_id()<<"                                           ***";
  }
  qInfo()<<"************************************************************";
  qInfo()<<"";
}

void HSignal::deleteVS( VSignal *todelete , QString param){
  int remember = -1;                                       //remembers position
  if(param.compare("v") == 0){                             //forwards = v parameter
    for( int i = 0 ; i < vorsignalV.size() ; i++){         //loop over distant signals
      if(vorsignalV.at(i)->getV_id().compare( todelete->getV_id() ) == 0 ){//found
	remember = i;
      }
    }
    if(remember >= 0 ){                                   //does it exist?
      vorsignalV.erase(vorsignalV.begin() + remember);    //delete it
    }
  }
  remember = -1;                                          //reset remember for new calculation
  if(param.compare("r") == 0){                            //backwards = r parameter
    for( int i = 0 ; i < vorsignalR.size() ; i++){        //loop over distant signals
      if(vorsignalR.at(i)->getV_id().compare( todelete->getV_id() ) == 0 ){//found
	remember = i;
      }
    }
    if(remember >= 0 ){                                   //does it exist?
      vorsignalR.erase(vorsignalR.begin() + remember);    //delete it
    }
  }
}

void HSignal::deleteWS( WSignal *todelete , QString param){
  int remember = -1;                                       //remembers position
  if(param.compare("v") == 0){                             //forwards = v parameter
    for( int i = 0 ; i < wsignaleV.size() ; i++){          //loop over shunt signals
      if(wsignaleV.at(i)->getV_id().compare( todelete->getV_id() ) == 0 ){//found
	remember = i;
      }
    }
    if(remember >= 0 ){                                    //does it exist?
      wsignaleV.erase(wsignaleV.begin() + remember);       //delete it
    }
  }
  remember = -1;                                           //reset remember for new calculation
  if(param.compare("r") == 0){                             //backwards = r parameter
    for( int i = 0 ; i < wsignaleR.size() ; i++){          //loop over shunt signals
      if(wsignaleR.at(i)->getV_id().compare( todelete->getV_id() ) == 0 ){//found
	remember = i;
      }
    }
    if(remember >= 0 ){                                    //does it exist?
      wsignaleR.erase(wsignaleR.begin() + remember);       //delete it
    }
  }
}

void HSignal::setZiel(QString zziel){
  if(zziel.at(0) == 'S'){                                  //its a main signal
    hasHSZiel = true;
    ziel = zziel;
  }
  if(zziel.at(0) == 'W'){                                  //its a shunt signal
    hasWSZiel = true;
    ziel = zziel;
  }
}

bool HSignal::isNachbar(HSignal *toZiel){
  bool giveback = false;
  for(  int i = 0; i < block.size(); i++){
    if(block.at(i).first.compare( toZiel->getS_id() ) == 0){
      giveback = true;
    }
  }
  return giveback;
}

bool HSignal::isNachbar(WSignal *toZiel){
  bool giveback = false;
  for(  int i = 0; i < blockZuRangier.size(); i++){
    if(blockZuRangier.at(i).first.compare( toZiel->getV_id() ) == 0){
      giveback = true;
    }
  }
  return giveback;
}


bool HSignal::setFahrt( HSignal *toZiel ){
  bool erfolg = false;                                     //prepare return value
  //
  //different bools to check, if route can be allowed
  bool blockfreigabe = true;                               //locked would be false -->search as long as there are unlocked segments
  bool blockkontrolle = true;                              //occupied would be false -->search as long as there are unoccupied segments
  bool weichenverr = false;                                //locked would be true -->search as long as there are unlocked turnouts
  bool weichenkontrolle = true;                            //occupied would be false -->search as long as there are unoccupied turnouts
  if( getS_id().compare(toZiel->getS_id() ) != 0 && isNachbar(toZiel) ){//check if signal is a neighbour signal + not the same
    //::::Blockstatus ==========================
    for(  int i = 0 ; i < block.size() ; i++ ){            //loop over segments
      if( block.at(i).first.compare( toZiel->getS_id() ) == 0 ){//found
	for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){//loop over segment status
	  if( !block.at(i).second.at(j)->getB_status() ){   //[if occupied]
	    blockkontrolle = false;                         //change flag to false
	    break;
	  }
	  if( !block.at(i).second.at(j)->getFreigabe() ){   //[if locked]
	    blockfreigabe = false;                          //change flag to false
	    break;
	  }
	  //
	  //succesfully looped through all segments: blockfreigabe and blockkontrolle are =true -->nothing occupied and locked
	}
      }
      //
      //end of name searching loop
    }
    //::::Weichenstatus ========================
    for(  int k = 0 ; k < weichenstatus.size() ; k++ ){      //loop over turnouts
      if( weichenstatus.at(k).first.compare( toZiel->getS_id() ) == 0 ){//found
	for(  int l = 0 ; l < weichenstatus.at(k).second.size() ; l++ ){//loop over occupied status
	  if( !weichenstatus.at(k).second.at(l).first->getBelegung() ){//[if occupied]
	    weichenkontrolle = false;                                  //change flag to false
	    break;
	  }
	  //if( weichenstatus.at(k).second.at(l).first->getVerriegelung() ){//[if locked = true]
	  //  weichenverr = true;                                           //change flag to true setzen
	  //  break;
	  //} //deprecated: hardware issue
	}
	//
        //succesfully looped through all turnouts: weichenkontrolle and weichenstatus allow route
      }
      //
      //end of name searching loop
    }
    //
    //::::summarize permission =======================
    bool blockgeschichte = false;                            //route allowed from segments?
    bool weichengeschichte = false;                          //route allowed from turnouts?
    blockgeschichte = blockkontrolle && blockfreigabe;       //sum
    weichengeschichte = weichenkontrolle && !weichenverr;    //sum
    //::::setting route...=================
    if( blockgeschichte && weichengeschichte ){              //permission granted
      //BLOCK
      for(  int m = 0 ; m < block.size() ; m++ ){            //loop over segments
	if( block.at(m).first.compare( toZiel->getS_id() ) == 0 ){
	  for(  int n = 0 ; n < block.at(m).second.size() ; n++ ){
	    block.at(m).second.at(n)->setFreigabe( false );  //lock segments
	    //
	    //all segments are locked for the route
	  }
	}
	//
	//end of name searching loop
      }
      //WEICHEN
      for(  int o = 0 ; o < weichenstatus.size() ; o++ ){     //loop over turnouts
	if( weichenstatus.at(o).first.compare( toZiel->getS_id() ) == 0 ){
	  for(  int p = 0 ; p < weichenstatus.at(o).second.size() ; p++ ){
	    weichenstatus.at(o).second.at(p).first->setW_status(weichenstatus.at(o).second.at(p).second);//change turnout status as it should be for the current route
	    //weichenstatus.at(o).second.at(p).first->setVerriegelung( true );// and lock them; its a active route now //deprecated: hardware issue
	  }
	  //
	  //everything set for route
	}
      }
      //WSIGNALE - change status to end signal [forwards]
      for(  int z = 0; z < wsignaleV.size(); z++){
	if( wsignaleV.at(z)->getRichtung( getS_id(), toZiel->getS_id() ) >= 0 ){//is there exact this direction? involve it in the current route and set it to "go"
	  wsignaleV.at(z)->setinFS( true , getS_id() , toZiel->getS_id() );
	  wsignaleV.at(z)->setV_status( true );
	}
      }
      //VSIGNALE - change status to end signal [forwards]
      for(  int i = 0 ; i < vorsignalV.size() ; i++ ){
	if( vorsignalV.at(i)->getRichtung( getS_id(), toZiel->getS_id() ) >= 0 ){//is there exact this direction? involve it in the current route and set it to "go"
	  vorsignalV.at(i)->setinFS( true , getS_id() , toZiel->getS_id() );
	  vorsignalV.at(i)->setV_status( toZiel->getS_status() );
	}
      }
      //HSIGNAL - changing main signal status to "go"
      setZiel( toZiel->getS_id() );
      toZiel->setfromHS(this);
      s_status = true;
      hasHSZiel = true;
      changeColor();
      emit refreshStellwerkstechnik( getS_id() , true ); //update list in Stellwerktechnik
      //VSIGNALE - change possible distant signals [backwards]
      for(  int i = 0 ; i < vorsignalR.size() ; i++ ){   //loop over all distant signals backwards and find the one, which has exact the current direction and inFS=true --> change it to "expect go"
	if(vorsignalR.at(i)->getinFS() && vorsignalR.at(i)->isAktFS( getS_id() ) ){
	  vorsignalR.at(i)->setV_status( true );
	}
      }
      //////
      erfolg = true;                                    // return value was successful!
    }
    //
    //case else: altought isNachbar == true, but no permission granted -->set memory
    if( !speicher and !erfolg ){                        //there must be no memory before -->memory set
      speicher = true;
      speicherziel = toZiel;
      emit illuminateSpeicher(true,toZiel->getS_id());
      speicheritems.first->setBrush(Qt::yellow);
      emit callspmemory(this, toZiel);
      erfolg = false;                                  //return value was not successful!
    }
    if( speicher and !erfolg){                         //no permission and at the same time already in memory: --> stay in memory but do not create a new memory, it is already there!
      erfolg = false;
    }
  }
  return erfolg;
}


void HSignal::setFahrt( WSignal *toZiel ){
  //different bools to check permission for route
  bool blockfreigabe = true;                            //locked would be false -->search as long as there are unlocked segments
  bool blockkontrolle = true;                           //occupied would be false -->search as long as there are unoccupied turnouts
  bool weichenverr = false;                             //locked would be true -->search as long as there are unlocked turnouts
  bool weichenkontrolle = true;                         //occupied would be false -->search as long as there are unoccuüied turnoutsuche
  if( getS_id().compare(toZiel->getV_id() ) != 0 && isNachbar(toZiel) ){//is end signal a neighbour signal
    //::::Blockstatus ==========================
    for(  int i = 0 ; i < blockZuRangier.size() ; i++ ){
      if( blockZuRangier.at(i).first.compare( toZiel->getV_id() ) == 0 ){
	for(  int j = 0 ; j < blockZuRangier.at(i).second.size() ; j++ ){
	  if( !blockZuRangier.at(i).second.at(j)->getB_status() ){//look at all segment status [if occupied]
	    blockkontrolle = false;                       //change flag to false
	    break;
	  }
	  if( !blockZuRangier.at(i).second.at(j)->getFreigabe() ){//look at all segment locks [if locked]
	    blockfreigabe = false;                        //change flag to false 
	    break;
	  }
	  //
	  //completely looped through segments, blockfreigabe und blockkontrolle auf = true -->route permission possible
	}
      }
      //end loop of name search
    }
    //::::Weichenstatus ========================
    for(  int k = 0 ; k < weichenstatusZuRangier.size() ; k++ ){//loop over turnout status
      if( weichenstatusZuRangier.at(k).first.compare( toZiel->getV_id() ) == 0 ){
	for(  int l = 0 ; l < weichenstatusZuRangier.at(k).second.size() ; l++ ){
	  if( !weichenstatusZuRangier.at(k).second.at(l).first->getBelegung() ){//look at all occupation status [if occupied]
	    weichenkontrolle = false;                      //change flag to false 
	    break;
	  }
	  //if( weichenstatusZuRangier.at(k).second.at(l).first->getVerriegelung() ){//look at all lock status [if locked = true]
	  //  weichenverr = true;                            //change flag to true 
	  //  break;
	  //} //deprecated: hardware issue
	}
	//
	//completely looped through turnouts and permission possible for route
      }
    }
    //::::summarize permission=======================
    bool blockgeschichte = false;                           //complete segment permission?
    bool weichengeschichte = false;                         //complete turnout permission?
    blockgeschichte = blockkontrolle && blockfreigabe;
    weichengeschichte = weichenkontrolle && !weichenverr;
    //::::Das eigentliche Stellen =================
    if( blockgeschichte && weichengeschichte ){             //permission granted
      //BLOCK
      for(  int m = 0 ; m < blockZuRangier.size() ; m++ ){  //loop over blocks
	if( blockZuRangier.at(m).first.compare( toZiel->getV_id() ) == 0 ){
	  for(  int n = 0 ; n < blockZuRangier.at(m).second.size() ; n++ ){
	    blockZuRangier.at(m).second.at(n)->setFreigabe( false );//lock segments
	  }
	}
      }
      //WEICHEN
      for(  int o = 0 ; o < weichenstatusZuRangier.size() ; o++ ){//loop over turnouts
	if( weichenstatusZuRangier.at(o).first.compare( toZiel->getV_id() ) == 0 ){
	  for(  int p = 0 ; p < weichenstatusZuRangier.at(o).second.size() ; p++ ){
	    weichenstatusZuRangier.at(o).second.at(p).first->setW_status(weichenstatusZuRangier.at(o).second.at(p).second);  //change turnout into direction which is needed for route
	    //weichenstatusZuRangier.at(o).second.at(p).first->setVerriegelung( true );//and lock them down, they are now part of the route // deprecated: hardware issue
	  }
	  //all turnouts set and locked
	}
      }
      //HSIGNAL- change main signal status to "go"
      zuRangier = true;                                        //display that its a transition from HS to WS 
      s_status = true;
      setZiel(toZiel->getV_id());
      hasWSZiel = true;
      changeColor();
      zuRangier = false;                                       //reset, maybe the next route is no shunting route
      emit refreshStellwerkstechnik( getS_id() , true );       //update stellwerkstechnik: shunting is traded as "go" in stellwerkstechnik
      //////
    }
  }
}

void HSignal::zugpassiert(){
  s_status = false;                                            //passing main signal -->go tot "stop"
  hasHSZiel = false;
  hasWSZiel = false;
  changeColor();
  emit refreshStellwerkstechnik( getS_id() , false );          //update list in Stellwerkstechnik
  qDebug()<<"__HSignal__: Emit zugpassiert:"<<getS_id();
  //
  //VSIGNALE - change all distant signals to "expect stop" [backwards], also inFS should be deleted
  for(  int i = 0 ; i < vorsignalR.size() ; i++ ){             //find exact that distant signal, which is inFS and has the current direction 
    if(vorsignalR.at(i)->getinFS() && vorsignalR.at(i)->isAktFS( getS_id() ) ){
      vorsignalR.at(i)->zugpassiert();          //setinFS=false is in setV_status=false integrated-->cf. VSignal
    }
  }
  //WSIGNALE - change all shunt signals to "stop" [backwards] + delete inFS
  for(  int i = 0 ; i < wsignaleR.size() ; i++ ){              //find shunt signal with the current direction and which is inFS
    if(wsignaleR.at(i)->getinFS() && wsignaleR.at(i)->isAktFS( getS_id() ) ){
      wsignaleR.at(i)->zugpassiert();//setinFS=false in setV_status=false integrated-->cf. VSignal
    }
  }
}

void HSignal::changeColor(){
  if( getS_status() ){                                          //"go"
    if(!zuRangier){                                             //no transition to shunt signal
      halt->setVisible(false);
      rangier->setVisible(false);
      fahrt->setVisible(true);
    }
    if(zuRangier){                                              //transition to shunt signal
      halt->setVisible(false);
      rangier->setVisible(true);
      fahrt->setVisible(false);
    }
  }
  if( !getS_status() ){                                         //"stop"
    halt->setVisible(true);
    rangier->setVisible(false);
    fahrt->setVisible(false);
  }
}

void HSignal::deleteFS(){
  //differnt bools to check, if route can be deleted (all must be locked and unoccupied)
  bool blockfreigabe = false;                                   //unlocked would be true, search as long as there are locked segments
  bool blockkontrolle = true;                                   //occupied would be false-->search as long as there are unoccupied segments
  bool weichenverr = true;                                      //unlocked would be false-->search as long as there are locked turnouts
  bool weichenkontrolle = true;                                 //occupied would be false-->search as long as there are unoccupied turnouts
  //
  //great difference for deleteFS between HS->HS or HS->WS
  //##########################################################################
  if(hasHSZiel){
    //::::Blockstatus ==========================
    for(  int i = 0 ; i < block.size() ; i++ ){                 //loop over segments
      if( block.at(i).first.compare( getZiel() ) == 0 ){
	for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){
	  if( !block.at(i).second.at(j)->getB_status() ){       //look at all occupation status [if occupied]
	    blockkontrolle = false;                             //change flag to false
	    break;
	  }
	  if( block.at(i).second.at(j)->getFreigabe() ){        //look at all lock status [if unlocked]
	    blockfreigabe = true;                               //change flag to true
	    break;
	  }
	  //
	  //looped through complete segments, now blockfreigabe = false and blockkontrolle auf = true --> permission to delete
	}
      }
    }
    //::::Weichenstatus ========================
    for(  int k = 0 ; k < weichenstatus.size() ; k++ ){          //loop over turnouts
      if( weichenstatus.at(k).first.compare( getZiel() ) == 0 ){
	for(  int l = 0 ; l < weichenstatus.at(k).second.size() ; l++ ){
	  if( !weichenstatus.at(k).second.at(l).first->getBelegung() ){//look at all occupation status [if occupied]
	    weichenkontrolle = false;                                  //change flag to false 
	    break;
	  }
	  //if( !weichenstatus.at(k).second.at(l).first->getVerriegelung() ){//look at all lock status [unlocked = false]
	  //  weichenverr = false;                                       //change flag to false setzen
	  //  break;
	  //} // deprecated: hardware issue
	}
	//
	//looped through all turnouts, now permission is possible
      }
    }
    //:::summarize the permission to delete ============
    bool blockgeschichte = false;                                  //complete segment permission?
    bool weichengeschichte = false;                                //complete turnout permission?
    blockgeschichte = blockkontrolle && !blockfreigabe;        
    weichengeschichte = weichenkontrolle && weichenverr;
    //:::deleting... ===========
    if( blockgeschichte && weichengeschichte ){                    //permission to delete granted
      //BLOCK
      for(  int m = 0 ; m < block.size() ; m++ ){                  //loop over segments
	if( block.at(m).first.compare( getZiel() ) == 0 ){
	  for(  int n = 0 ; n < block.at(m).second.size() ; n++ ){
	    block.at(m).second.at(n)->setFreigabe( true );         //uunlock segments
	    //
	    //all segments unlocked 
	  }
	}
      }
      //WEICHEN
      //for(  int o = 0 ; o < weichenstatus.size() ; o++ ){          //loop over turnouts
      //if( weichenstatus.at(o).first.compare( getZiel() ) == 0 ){
      //  for(  int p = 0 ; p < weichenstatus.at(o).second.size() ; p++ ){
      //    weichenstatus.at(o).second.at(p).first->setVerriegelung( false );//unlocking turnouts
      //  }
	  //
	  //all turnouts are unlocked
      //}
      //} //.... deprecated: hardware issue
      //WSIGNALE - set shunt signals to "stop"
      for(  int z = 0 ; z < wsignaleV.size() ; z++ ){//loop over involved shunt signals
	if( wsignaleV.at(z)->getRichtung( getS_id(), getZiel() ) >= 0 ){//if there is exact this direction and inFS is true, set shunt signal to stop and delete inFS
	  wsignaleV.at(z)->setinFS( false );
	  wsignaleV.at(z)->setV_status( false );
	}
      }
      //VSIGNALE - signal end singal and delete inFS for distant signals
      for(  int i = 0 ; i < vorsignalV.size() ; i++ ){              //loop over distant signals
	if( vorsignalV.at(i)->getRichtung( getS_id(), getZiel() ) >= 0 ){//if there is exact this direction, delete inFS and show "expect stop"
	  vorsignalV.at(i)->setinFS( false );
	  vorsignalV.at(i)->setV_status( false );
	}
      }
      //HSIGNAL-status can now be set to "stop"
      //end signal is not up to date now
      s_status = false;
      //hasHSZiel = false;  //don`t do this-> segfault if the route is in spmemory
      changeColor();
      emit refreshStellwerkstechnik( getS_id() , false );           //update list in Stellwerkstechnik
      //VSIGNALE - change backwards distant signal status to "expect stop"
      for(  int i = 0 ; i < vorsignalR.size() ; i++ ){//find distant signal with exact this direction and inFS, then change to "expect stop"
	if(vorsignalR.at(i)->getinFS() && vorsignalR.at(i)->isAktFS( getS_id() ) ){
	  vorsignalR.at(i)->setV_status( false );
	}
      }
      //////
    }
  }
  //##########################################################################
  if(hasWSZiel){
    //::::Blockstatus ==========================
    for(  int i = 0 ; i < blockZuRangier.size() ; i++ ){        //loop over segments
      if( blockZuRangier.at(i).first.compare( getZiel() ) == 0 ){
	for(  int j = 0 ; j < blockZuRangier.at(i).second.size() ; j++ ){
	  if( !blockZuRangier.at(i).second.at(j)->getB_status() ){//look at all occupation status [if occupied]
	    blockkontrolle = false;                               //change flag to false
	    break;
	  }
	  if( blockZuRangier.at(i).second.at(j)->getFreigabe() ){  //look at all lock status [if unlocked]
	    blockfreigabe = true;                                  //change flag to true
	    break;
	  }
	  //
	  //looped over all segments and  blockfreigabe = false und blockkontrolle auf = true -->permission possible
	}
      }
    }
    //::::Weichenstatus ========================
    for(  int k = 0 ; k < weichenstatusZuRangier.size() ; k++ ){//loop over all turnouts
      if( weichenstatusZuRangier.at(k).first.compare( getZiel() ) == 0 ){
	for(  int l = 0 ; l < weichenstatusZuRangier.at(k).second.size() ; l++ ){
	  if( !weichenstatusZuRangier.at(k).second.at(l).first->getBelegung() ){//look at all occupation status [if unoccupied]
	    weichenkontrolle = false;                                     //change flag to false
	    break;
	  }
	  //if( !weichenstatusZuRangier.at(k).second.at(l).first->getVerriegelung() ){//look at all lock status [if unlocked  = false]
	  //  weichenverr = false;                                     //change flag to false
	  //  break;
	  //} //deprecated: hardware issue
	}
	//
	//looped over everything and Weichnkontrolle und weichenverr = true, delete permission possible
      }
    }
    //:::summarize permission to delete route ============
    bool blockgeschichte = false;                             //complete segment permission?
    bool weichengeschichte = false;                           //complete turnout permission?
    blockgeschichte = blockkontrolle && !blockfreigabe;
    weichengeschichte = weichenkontrolle && weichenverr;
    //:::Das eigentliche Löschen ===========
    if( blockgeschichte && weichengeschichte ){               //permission granted to delete route
      //BLOCK
      for(  int m = 0 ; m < blockZuRangier.size() ; m++ ){    //loop over segments
	if( blockZuRangier.at(m).first.compare( getZiel() ) == 0 ){
	  for(  int n = 0 ; n < blockZuRangier.at(m).second.size() ; n++ ){
	    blockZuRangier.at(m).second.at(n)->setFreigabe( true );//unlock segments
	    //
	    //all segments are unlocked now
	  }
	}
      }
      //WEICHEN
      //for(  int o = 0 ; o < weichenstatusZuRangier.size() ; o++ ){//loop over turnouts
      //if( weichenstatusZuRangier.at(o).first.compare( getZiel() ) == 0 ){
      //  for(  int p = 0 ; p < weichenstatusZuRangier.at(o).second.size() ; p++ ){
      //    weichenstatusZuRangier.at(o).second.at(p).first->setVerriegelung( false );//and unlock all turnouts
      //  }
	  //
	  //all turnouts are now unlocked
      //}
      //}// ...deprecated: hardware issue
      //HSIGNAL- main signal status will be set to "stop"
      //Ziel ist ab jetzt nicht mehr aktuell
      s_status = false;
      hasWSZiel = false;
      changeColor();
      emit refreshStellwerkstechnik( getS_id() , false );    //update stellwerkstechnik
      //////
    }
  }
}

void HSignal::listenToFS(){
  //send trigger to clickmanager
  emit listened(this);

}

void HSignal::recieveSpeicher(bool sp, QString str){
  if(str.compare(getS_id()) == 0){                             //emit illuminate reaches all slots, because the connection is created in addBlock: here will be checked, which main signal should respond
    if(sp){                                                    //new memory
      speicheritems.second->setBrush(Qt::yellow);
      //
      //fromHS()->speicheritems.first->setBrush(Qt::yellow); is done on other position, simply search for "illuminate"
    }
    if(!sp){                                                    //clear memory
      speicheritems.second->setBrush(Qt::darkBlue);
      fromHS->speicheritems.first->setBrush(Qt::darkBlue);
    }
  }

}

void HSignal::darkenSpeicher( int position ){
  if( position == 1){
    speicheritems.first->setBrush(Qt::darkBlue);
  }
  if( position == 2){
    speicheritems.second->setBrush(Qt::darkBlue);
  }
}

void HSignal::addHSignalitem(QGraphicsSvgItem *itemfahrt, QGraphicsSvgItem *itemhalt, QGraphicsSvgItem *itemrangier, QLabel *la, QPushButton *but, QGraphicsRectItem *speicherback, QGraphicsRectItem *speicherfront){
  fahrt = itemfahrt;
  halt = itemhalt;
  rangier = itemrangier;
  //Label
  beschriftung = la;
  QString qname = s_id;
  beschriftung->setText(qname);
  QFont f( "Arial", 10, QFont::Bold);
  beschriftung->setFont(f);
  //und PushButton
  push = but;
  but->setFixedHeight(20);
  but->setFixedWidth(20);
  but->setStyleSheet("background-color: blue");
  QObject::connect(push, &QPushButton::clicked, this, &HSignal::listenToFS);
  //Speicher Anzeiger
  speicherback->setBrush(Qt::darkBlue);
  speicherfront->setBrush(Qt::darkBlue);
  speicheritems.first = speicherfront;
  speicheritems.second = speicherback;

}

HSignal::~HSignal(){
  delete fahrt;
  delete halt;
  delete rangier;
  delete beschriftung;
  delete push;
}
