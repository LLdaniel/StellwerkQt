/*
 * shunt signal [- WSIGNAL.CXX -]
 **************************************************************************
 */
#include "WSignal.h"
#include <QDebug>
#include <QMenu>

void WSignal::setV_id( int name ){
  QString suffix = QString::number( name );                                                 //name convert to QString
  if( name >0 && name <1000){
    if( name < 10 ){
      v_id = "W00" + suffix;
    }
    if( name < 100 && name > 9){
      v_id = "W0" + suffix;
    }
    if( name < 1000 && name > 99){
      v_id = "W" + suffix;
    }
  }
  else qWarning()<<"__WSignal__: Falsche Benennung. Die Zahl muss dreistellig sein.";
}

void WSignal::setFahrt( WSignal *toZiel ){
  //
  //different bools to check, if route can be set
  bool blockfreigabe = true;                                             //locked would be false-->search as long as there are unlocked segments
  bool blockkontrolle = true;                                            //occupied would be false-->search as long as there are unoccupied segments
  bool weichenverr = false;                                              //locked would be true-->search as long as there are unlocked turnouts
  bool weichenkontrolle = true;                                          //occupied would be false-->search as long as there are unoccupied turnouts
  if( getV_id().compare(toZiel->getV_id() ) != 0 && isNachbar(toZiel) ){ //prüfe, ob Zielsignal Nachbar ist und ob Zielsignal nicht das selbe Signal ist wie das Startsignal
    //::::Blockstatus ==========================
    for(  int i = 0 ; i < block.size() ; i++ ){                          //loop over segments
      if( block.at(i).first.compare( toZiel->getV_id() ) == 0 ){         
	for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){
	  if( !block.at(i).second.at(j)->getB_status() ){                //look at occupied segments [if occupied]
	    blockkontrolle = false;                                      //change flag to false 
	    break;
	  }
	  if( !block.at(i).second.at(j)->getFreigabe() ){                //look at locked segments[if locked]
	    blockfreigabe = false;                                       //change flag to false
	    break;
	  }
	  //
	  //loop finished and permission is possible
	}
      }
    }
    //::::Weichenstatus ========================
    for(  int k = 0 ; k < weichenstatus.size() ; k++ ){                   //loop over turnouts
      if( weichenstatus.at(k).first.compare( toZiel->getV_id() ) == 0 ){  
	for(  int l = 0 ; l < weichenstatus.at(k).second.size() ; l++ ){
	  if( !weichenstatus.at(k).second.at(l).first->getBelegung() ){   //look at turnout occupation [if occupied]
	    weichenkontrolle = false;                                     //change flag to false
	    break;
	  }
	  //if( weichenstatus.at(k).second.at(l).first->getVerriegelung() ){//look at turnout lock status[if locked = true]
	  // weichenverr = true;                                           //change flag to true 
	  //break;
	  //}// deprecated: hardware issue
	}
	//
	//after the whole loop is finished, possibile permission exists
      }
    }
    //::::Erlaubnisabgleich =======================
    bool blockgeschichte = false;                                         //complete permission segments?
    bool weichengeschichte = false;                                       //complete permission turnouts?
    blockgeschichte = blockkontrolle && blockfreigabe;
    weichengeschichte = weichenkontrolle && !weichenverr;
    //::::Das eigentliche Stellen =================
    if( blockgeschichte && weichengeschichte ){                           //permission granted
      //BLOCK
      for(  int m = 0 ; m < block.size() ; m++ ){                         //loop over segments
	if( block.at(m).first.compare( toZiel->getV_id() ) == 0 ){
	  for(  int n = 0 ; n < block.at(m).second.size() ; n++ ){
	    block.at(m).second.at(n)->setFreigabe( false );               //lock segments
	    //
	    //no every segment in the route is locked
	  }
	}
      }
      //WEICHEN
      for(  int o = 0 ; o < weichenstatus.size() ; o++ ){                  //loop over turnouts
	if( weichenstatus.at(o).first.compare( toZiel->getV_id() ) == 0 ){
	  for(  int p = 0 ; p < weichenstatus.at(o).second.size() ; p++ ){
	    weichenstatus.at(o).second.at(p).first->setW_status(weichenstatus.at(o).second.at(p).second); //change turnouts to the needed direction
	    //weichenstatus.at(o).second.at(p).first->setVerriegelung( true ); //lock turnouts // deprecated: hardware issue
	  }
	  //
	  //every turnout is locked now
	}
      }
      //WSIGNAL change shunt signal to "go"
      setZiel( toZiel->getV_id() );
      v_status = true;
      hasWSZiel = true;
      changeColor();
      emit refreshStellwerkstechnikW(getV_id(), true);
      //////
    }
  }
}

void WSignal::setFahrt(QString toZiel) // WS to HS
{
  //different bools to check permission for route
  bool blockfreigabe = true;                                                 //locked would be false-->search as long as there are unlocked segments
  bool blockkontrolle = true;                                                //occupied would be false-->search as long as there are unoccupied segments
  bool weichenverr = false;                                                  //locked would be true-->search as long as there are unlocked turnouts
  bool weichenkontrolle = true;                                              //occupied would be false-->search as long as there are unoccupied turnouts
  if( getV_id().compare(toZiel ) != 0 && isNachbar(toZiel) ){                //is signal neighbour
    //::::Blockstatus ==========================
    for(  int i = 0 ; i < blockZuH.size() ; i++ ){                           //loop over segments
      if( blockZuH.at(i).first.compare( toZiel ) == 0 ){
        for(  int j = 0 ; j < blockZuH.at(i).second.size() ; j++ ){
	  if( !blockZuH.at(i).second.at(j)->getB_status() ){                 //look at occupied segments [if occupied]
	    blockkontrolle = false;                                          //change flag to false
	    break;
	  }
	  if( !blockZuH.at(i).second.at(j)->getFreigabe() ){                 //look at the occupation status [if occupied]
	    blockfreigabe = false;                                           //change flag to false
	    break;
	  }
	  //
	  //whole loop is finished, blockfreigabe und blockkontrolle auf = true -->possible permission
        }
      }
    }
    //::::Weichenstatus ========================
    for(  int k = 0 ; k < weichenstatusZuH.size() ; k++ ){                   //loop over turnouts
      if( weichenstatusZuH.at(k).first.compare( toZiel ) == 0 ){
        for(  int l = 0 ; l < weichenstatusZuH.at(k).second.size() ; l++ ){
	  if( !weichenstatusZuH.at(k).second.at(l).first->getBelegung() ){   //look at occupation status turnouts [if occupied]
	    weichenkontrolle = false;                                        //change flag to
	    break;
	  }
	  //if( weichenstatusZuH.at(k).second.at(l).first->getVerriegelung() ){//look at turnout status [if locked =true]
	  // weichenverr = true;                                              //change flag to true 
	  // break;
	  //}//deprecated: hardware issue
        }
        //
	//loop finished now possible route permission
      }
    }
    //::::summarize permission  =======================
    bool blockgeschichte = false;                                            //complete segment permission?
    bool weichengeschichte = false;                                          //complete turnout permission?
    blockgeschichte = blockkontrolle && blockfreigabe;
    weichengeschichte = weichenkontrolle && !weichenverr;
    //::::Das eigentliche Stellen =================
    if( blockgeschichte && weichengeschichte ){                              //permission granted
      //BLOCK
      for(  int m = 0 ; m < blockZuH.size() ; m++ ){                         //loop over segments
        if( blockZuH.at(m).first.compare( toZiel ) == 0 ){
	  for(  int n = 0 ; n < blockZuH.at(m).second.size() ; n++ ){
	    blockZuH.at(m).second.at(n)->setFreigabe( false );               //lock segments
	    //
	    //every segment in the route is now locked
	  }
        }
      }
      //WEICHEN
      for(  int o = 0 ; o < weichenstatusZuH.size() ; o++ ){                  //loop over turnouts
        if( weichenstatusZuH.at(o).first.compare( toZiel ) == 0 ){            
	  for(  int p = 0 ; p < weichenstatusZuH.at(o).second.size() ; p++ ){
	    weichenstatusZuH.at(o).second.at(p).first->setW_status(weichenstatusZuH.at(o).second.at(p).second);//change turnouts according to the route needs
	    //weichenstatusZuH.at(o).second.at(p).first->setVerriegelung( true );//lock turnouts //deprecated: hardware issue
	  }
	  //
	  //turnouts now locked
        }
      }
      //WSIGNAL change shunt signal status to "go"
      v_status = true;
      setZiel(toZiel);
      hasHSZiel = true;
      changeColor();
      emit refreshStellwerkstechnikW(getV_id(),true);
      //////
    }
  }
}

void WSignal::deleteFS(){
  //
  //different bools to check, if route can be deleted
  bool blockfreigabe = false;                                                       //unlocked would be true-->search as long as there are locked segments
  bool blockkontrolle = true;                                                       //occupied would be 1false-->search as long as there are unoccupied segments
  bool weichenverr = true;                                                          //unlocked would be false-->search as long as there are locked turnouts
  bool weichenkontrolle = true;                                                     //occupied would be false-->search as long as there are unoccupied turnouts
  //differentiation between delete FS for routes WS->WS or WS->HS
  //##########################################################################
  if(hasWSZiel){
    //::::Blockgeschichte ==========================
    for(  int i = 0 ; i < block.size() ; i++ ){                                     //loop over segments
      if( block.at(i).first.compare( getZiel() ) == 0 ){
	for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){
	  if( !block.at(i).second.at(j)->getB_status() ){                           // look at segment occupation [if occupied]
	    blockkontrolle = false;                                                 //change flag to false
	    break;
	  }
	  if( block.at(i).second.at(j)->getFreigabe() ){                            //look segment status[if unlocked]
	    blockfreigabe = true;                                                   //change flag to true
	    break;
	  }
	  //
	  //finsihed loop, possible permission
	}
      }
    }
    //::::Weichengeschichte ========================
    for(  int k = 0 ; k < weichenstatus.size() ; k++ ){                             //loop over turnouts
      if( weichenstatus.at(k).first.compare( getZiel() ) == 0 ){
	for(  int l = 0 ; l < weichenstatus.at(k).second.size() ; l++ ){
	  if( !weichenstatus.at(k).second.at(l).first->getBelegung() ){             //look at occupation status [if occupied]
	    weichenkontrolle = false;                                               //change flag to false 
	    break;
	  }
	  //if( !weichenstatus.at(k).second.at(l).first->getVerriegelung() ){         //look betrachte Verriegelung [falls entriegelt = false]
	  // weichenverr = false;                                                    //change flag to false 
	  // break;
	  //}// deprecated: hardware issue
	}
	//
	//complete loop finshed and possible permission
      }
    }
    //:::summarize permissions to delete route  ============
    bool blockgeschichte = false;                                                   //complete segment permission?
    bool weichengeschichte = false;                                                 //complete turnout permission?
    blockgeschichte = blockkontrolle && !blockfreigabe;
    weichengeschichte = weichenkontrolle && weichenverr;
    //:::Das eigentliche Löschen ===========
    if( blockgeschichte && weichengeschichte ){                                     //permission granted
      //BLOCK
      for(  int m = 0 ; m < block.size() ; m++ ){                                   //loop over segments
	if( block.at(m).first.compare( getZiel() ) == 0 ){
	  for(  int n = 0 ; n < block.at(m).second.size() ; n++ ){
	    block.at(m).second.at(n)->setFreigabe( true );                          //unlock segments
	    //
	    //now segments are unlocked
	  }
	}
      }
      //WEICHEN
      //for(  int o = 0 ; o < weichenstatus.size() ; o++ ){                          //loop over turnouts
      //	if( weichenstatus.at(o).first.compare( getZiel() ) == 0 ){
      //  for(  int p = 0 ; p < weichenstatus.at(o).second.size() ; p++ ){
      //    weichenstatus.at(o).second.at(p).first->setVerriegelung( false );      //unlock turnouts
      //  }
	  //
	  //at the end of the loop: turnouts unlocked
      //}
      //}//...deprecated: hardware issue
      //WSIGNAL status changed to "stop"
      //end signal is not up to date now
      v_status = false;
      hasWSZiel = false;
      changeColor();
      emit refreshStellwerkstechnikW(getV_id(), false);
      //////
    }
  }
  if(hasHSZiel){
    //::::Blockstatus ==========================
    for(  int i = 0 ; i < blockZuH.size() ; i++ ){                                  //loop over segments
      if( blockZuH.at(i).first.compare( getZiel() ) == 0 ){
	for(  int j = 0 ; j < blockZuH.at(i).second.size() ; j++ ){
	  if( !blockZuH.at(i).second.at(j)->getB_status() ){                        //look at segment occupation status [if unoccupied]
	    blockkontrolle = false;                                                 //change flag to false
	    break;
	  }
	  if( blockZuH.at(i).second.at(j)->getFreigabe() ){                         //look at segment status [if unlocked]
	    blockfreigabe = true;                                                   //change flag to true
	    break;
	  }
	  //complete loop finished, possible permission 
	}
      }
    }
    //::::Weichenstatus ========================
    for(  int k = 0 ; k < weichenstatusZuH.size() ; k++ ){                          //loop over turnouts
      if( weichenstatusZuH.at(k).first.compare( getZiel() ) == 0 ){
	for(  int l = 0 ; l < weichenstatusZuH.at(k).second.size() ; l++ ){
	  if( !weichenstatusZuH.at(k).second.at(l).first->getBelegung() ){          //look at all turnout occupation [if occupied]
	    weichenkontrolle = false;                                               //change flag to false
	    break;
	  }
	  //if( !weichenstatusZuH.at(k).second.at(l).first->getVerriegelung() ){      //look at lock status [if unlocked = false]
	  // weichenverr = false;                                                    //change flag to false 
	  // break;
	  //}//deprecated: hardware issue
	}
	//all turnouts unlocked and permission is possible
      }
    }
    //:::summarize permission ============
    bool blockgeschichte = false;                                                    //complete permission segments
    bool weichengeschichte = false;                                                  //complete permission turnouts
    blockgeschichte = blockkontrolle && !blockfreigabe;
    weichengeschichte = weichenkontrolle && weichenverr;                        
    //:::deleting now... ===========
    if( blockgeschichte && weichengeschichte ){                                      //permission to delete granted
      //BLOCK
      for(  int m = 0 ; m < blockZuH.size() ; m++ ){                                 //loop over segments
	if( blockZuH.at(m).first.compare( getZiel() ) == 0 ){
	  for(  int n = 0 ; n < blockZuH.at(m).second.size() ; n++ ){
	    blockZuH.at(m).second.at(n)->setFreigabe( true );                        //unlock segments
	    //
	    //all segments are now unlocked
	  }
	}
	//Die Schleife für die Namensuche läuft noch fertig
      }
      //WEICHEN
      //for(  int o = 0 ; o < weichenstatusZuH.size() ; o++ ){                         //loop over turnouts
      //	if( weichenstatusZuH.at(o).first.compare( getZiel() ) == 0 ){
      //  for(  int p = 0 ; p < weichenstatusZuH.at(o).second.size() ; p++ ){        
      //    weichenstatusZuH.at(o).second.at(p).first->setVerriegelung( false );     //unlocking turnouts
      //  }
	  //
	  //all turnouts are now unlocked
      //}
      //}//...deprecated: hardware issue
      //WSIGNAL- change status to "stop"
      //end signal is not up to date now
      v_status = false;
      hasWSZiel = false;
      changeColor();
      emit refreshStellwerkstechnikW(getV_id(), false);
      //////
    }
  }
}

void WSignal::addWeichenstatus( WSignal *toZiel , QList<QPair<Weiche* , bool>> weichenpair ){
  QPair<QString, QList<QPair<Weiche* , bool>> > hilfspair( toZiel->getV_id() , weichenpair );
  weichenstatus.push_back(hilfspair);
}

void WSignal::addWeichenstatusZuH( QString toZiel , QList<QPair<Weiche* , bool>> weichenpair ){
  QPair<QString, QList<QPair<Weiche* , bool>> > hilfspair( toZiel , weichenpair );
  weichenstatusZuH.push_back(hilfspair);
}

void WSignal::showWeichenstatusALL(){
  qInfo()<<"************************************************************";
  qInfo()<<"*** Dies ist der Weichenstatus für Rangiersignal"<<getV_id()<<"     ***";
  for(  int i = 0 ; i < weichenstatus.size() ; i++){
    qInfo()<<"***   "<<weichenstatus.at(i).first<<" :                                             ***";
    for(  int j = 0 ; j < weichenstatus.at(i).second.size() ; j++ ){
      qInfo()<<"***               "<<weichenstatus.at(i).second.at(j).first->getW_id()<<"   -->   "<<weichenstatus.at(i).second.at(j).second<<"                          ***";
    }
  }
  qInfo()<<"************************************************************";
  qInfo()<<"";
}

void WSignal::showWeichenstatus( WSignal *whichZiel ){
  for(  int i = 0 ; i < weichenstatus.size() ; i++){
    if( weichenstatus.at(i).first.compare( whichZiel->getV_id() ) == 0 ){
      qInfo()<<"";
      qInfo()<<"************************************************************";
      qInfo()<<"*** Dies ist der Weichenstatus für Rangiersignal"<<getV_id()<<"      ***";
      qInfo()<<"***   "<<weichenstatus.at(i).first<<" :                                             ***";
      for(  int j = 0 ; j < weichenstatus.at(i).second.size() ; j++ ){
	qInfo()<<"***               "<<weichenstatus.at(i).second.at(j).first->getW_id()<<"   -->   "<<weichenstatus.at(i).second.at(j).second<<"                          ***";
      }
      qInfo()<<"************************************************************";
      qInfo()<<"";
    }
  }
}

void WSignal::addBlock( WSignal *toZiel , QList<Block*> inputBlock ){
  QPair<QString , QList<Block*>> hilfspair(toZiel->getV_id(), inputBlock);
  block.push_back(hilfspair);
}

void WSignal::addBlockZuH(QString toZiel, QList<Block *> inputBlock)
{
  QPair<QString , QList<Block*>> hilfspair(toZiel, inputBlock);
  blockZuH.push_back(hilfspair);
}

void WSignal::showBlock( WSignal *whichZiel ){
  for(  int i = 0 ; i < block.size() ; i++){
    if( block.at(i).first.compare( whichZiel->getV_id() ) == 0){
      qInfo()<<"";
      qInfo()<<"************************************************************";
      qInfo()<<"*** Dies sind die Blöcke für Hauptsignal "<<getV_id()<<"            ***";
      qInfo()<<"***   "<<block.at(i).first<<" :                                             ***";
      for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){//hier sind die Blöcke
	qInfo()<<"***               "<<block.at(i).second.at(j)->getName()<<"                                     ***";
      }
      qInfo()<<"************************************************************";
      qInfo()<<"";
    }
  }
}

void WSignal::showBlockALL(){
  qInfo()<<"************************************************************";
  qInfo()<<"*** Dies sind die Blöcke für Hauptsignal "<<getV_id()<<"            ***";
  for(  int i = 0 ; i < block.size() ; i++){
    qInfo()<<"***   "<<block.at(i).first<<" :                                             ***";
    for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){
      qInfo()<<"***               "<<block.at(i).second.at(j)->getName()<<"                                     ***";
    }
  }
  qInfo()<<"************************************************************";
  qInfo()<<"";
}


bool WSignal::isNachbar(WSignal *toZiel){
  bool giveback = false;
  for(  int i = 0; i < block.size(); i++){
    if(block.at(i).first.compare( toZiel->getV_id() ) == 0){
      giveback = true;
    }
  }
  return giveback;
}

bool WSignal::isNachbar(QString toZiel){
  bool giveback = false;
  if(!blockZuH.isEmpty()){
    for(  int i = 0; i < blockZuH.size(); i++){
      if(blockZuH.at(i).first.compare( toZiel ) == 0){
	giveback = true;
      }
    }
  }
  else giveback = true;     //it is empty, but this can happen (Schattenbahnhof)
  return giveback;
}

void WSignal::deleteNachbar( WSignal *todelete ){
  //
  //delete means: in weichenstatus and blockstatus
  //Weichenstatus:
  int remember = -1;
  for(  int i = 0 ; i < weichenstatus.size() ; i++ ){
    if( weichenstatus.at(i).first.compare( todelete->getV_id() ) == 0 ){//found
      remember = i;
    }
  }
  if( remember >= 0 ){                                       //only when the to delete signal is existent
    weichenstatus.erase( weichenstatus.begin() + remember ); //delete i-th position
  }
  remember = -1;
  //Block:
  for(  int j = 0 ; j < block.size() ; j++ ){
    if( block.at(j).first.compare( todelete->getV_id() ) == 0 ){//found
      remember = j;
    }
  }
  if(remember >= 0 ){
    block.erase( block.begin() + remember);                  //delete i-th position
  }
}

void WSignal::addButtonAndLabel(QLabel *lab, WSignalQPushButton *but){
  //Label
  beschriftung = lab;
  QString qname = v_id;
  beschriftung->setText(qname);
  QFont f( "Arial", 10, QFont::Bold);
  beschriftung->setFont(f);
  //and PushButton
  push = but;
  but->setFixedHeight(20);
  but->setFixedWidth(20);
  but->setStyleSheet("background-color: blue");
  QObject::connect(push,&WSignalQPushButton::clicked,this,&WSignal::listenToFS);
  QObject::connect(push,&WSignalQPushButton::rightClicked,this,&WSignal::showShowContexts);
}

void WSignal::listenToFS(){
  //
  //connected to clickmanager
  emit listened(this);
}

void WSignal::zugpassiertW(){
  v_status = false;
  hasWSZiel = false;
  hasHSZiel = false;
  changeColor();
  emit refreshStellwerkstechnikW(getV_id(),false);
}

void WSignal::setZiel(QString zziel){
  if(zziel.at(0) == 'S'){                                //HS
    hasHSZiel = true;
    ziel = zziel;
  }
  if(zziel.at(0) == 'W'){                                //WS
    hasWSZiel = true;
    ziel = zziel;
  }
}

void WSignal::showShowContexts(){
  this->push->getMenu()->popup(QCursor::pos());
  this->push->raise();
}

WSignal::~WSignal(){
  delete beschriftung;
  delete push;
}
