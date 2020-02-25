//*************************************************************************
//Hauptsignal des Stellwerks  [- HSIGNAL.CXX -]
//*************************************************************************
#include "HSignal.h"
#include <QDebug>

HSignal::HSignal(int name){
  setS_id( name );
}

void HSignal::setS_id( int name ){
  QString suffix = QString::number( name );//name zu String konvertieren
  if( name >0 && name <1000){//dreistelligkeit wird hier überprüft
    if( name < 10 ){//zwei Vornullen werden erzeugt
      s_id = "S00" + suffix;//QString::number( name );
    }
    if( name < 100 && name > 10){//eine Vornull wird erzeugt
      s_id = "S0" + suffix;//QString::number( name );
    }
    if( name < 1000 && name > 100){//keine Vornull mehr nötig
      s_id = "S" + suffix;//QString::number( name );
    }
  }
  else qDebug()<<"HSignal:Falsche Benennung. Die Zahl muss dreistellig sein.";
}

void HSignal::addWeichenstatus( HSignal *toZiel , QList<QPair<Weiche* , bool>> weichenpair ){
  QPair<QString, QList<QPair<Weiche* , bool>> > hilfspair( toZiel->getS_id() , weichenpair );//Das Hilfspair wird erzeugt, string wird über das Signal selbst erzeugt
  weichenstatus.push_back(hilfspair);
}

void HSignal::addWeichenstatusZuRangier( WSignal *toZiel , QList<QPair<Weiche* , bool>> weichenpair ){
  QPair<QString, QList<QPair<Weiche* , bool>> > hilfspair( toZiel->getV_id() , weichenpair );//Das Hilfspair wird erzeugt, string wird über das Signal selbst erzeugt
  weichenstatusZuRangier.push_back(hilfspair);
}

void HSignal::showWeichenstatusALL(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies ist der Weichenstatus für Hauptsignal "<<getS_id()<<"      ***";
  for(  int i = 0 ; i < weichenstatus.size() ; i++){
    qDebug()<<"***   "<<weichenstatus.at(i).first<<" :                                             ***";
    for(  int j = 0 ; j < weichenstatus.at(i).second.size() ; j++ ){//hier sind die Weichenstatuspaarungen
      qDebug()<<"***               "<<weichenstatus.at(i).second.at(j).first->getW_id()<<"   -->   "<<weichenstatus.at(i).second.at(j).second<<"                          ***";
    }
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void HSignal::showWeichenstatus( HSignal *whichZiel ){
  for(  int i = 0 ; i < weichenstatus.size() ; i++){
    if( weichenstatus.at(i).first.compare( whichZiel->getS_id() ) == 0 ){//falls das gesuchte Signal zum Signal in der Liste passt, durchlaufe die Weichenstatuspaarungen
      qDebug()<<"";
      qDebug()<<"************************************************************";
      qDebug()<<"*** Dies ist der Weichenstatus für Hauptsignal "<<getS_id()<<"      ***";
      qDebug()<<"***   "<<weichenstatus.at(i).first<<" :                                             ***";
      for(  int j = 0 ; j < weichenstatus.at(i).second.size() ; j++ ){//hier sind die Weichenstatuspaarungen
	qDebug()<<"***               "<<weichenstatus.at(i).second.at(j).first->getW_id()<<"   -->   "<<weichenstatus.at(i).second.at(j).second<<"                          ***";
      }
      qDebug()<<"************************************************************";
      qDebug()<<"";
    }
  }
}

void HSignal::addBlock( HSignal *toZiel , QList<Block*> inputBlock ){
  QPair<QString , QList<Block*>> hilfspair(toZiel->getS_id(), inputBlock);
  block.push_back(hilfspair);
  //hier gleich die Verknüpfung der Speicher mit erledigen
  QObject::connect(this,&HSignal::illuminateSpeicher, toZiel, &HSignal::recieveSpeicher);
}

void HSignal::addBlockZuRangier( WSignal *toZiel , QList<Block*> inputBlock ){
  QPair<QString , QList<Block*>> hilfspair(toZiel->getV_id(), inputBlock);
  blockZuRangier.push_back(hilfspair);
}

void HSignal::showBlock( HSignal *whichZiel ){
  for(  int i = 0 ; i < block.size() ; i++){
    if( block.at(i).first.compare( whichZiel->getS_id() ) == 0){//Wenn das gesuchte Nachbarsignal gefunden wurde-->Schleife über die Blöcke
      qDebug()<<"";
      qDebug()<<"************************************************************";
      qDebug()<<"*** Dies sind die Blöcke für Hauptsignal "<<getS_id()<<"            ***";
      qDebug()<<"***   "<<block.at(i).first<<" :                                             ***";
      for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){//hier sind die Blöcke
	qDebug()<<"***               "<<block.at(i).second.at(j)->getName()<<"                                     ***";
      }
      qDebug()<<"************************************************************";
      qDebug()<<"";
    }
  }
}

void HSignal::showBlockALL(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies sind die Blöcke für Hauptsignal "<<getS_id()<<"            ***";
  for(  int i = 0 ; i < block.size() ; i++){
    qDebug()<<"***   "<<block.at(i).first<<" :                                             ***";
    for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){//hier sind die Weichenstatuspaarungen
      qDebug()<<"***               "<<block.at(i).second.at(j)->getName()<<"                                     ***";
    }
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void HSignal::deleteNachbar( HSignal *todelete ){
  //delete meint, sowohl in Weichenstatus, als auch im Block
  //Weichenstatus:
  int remember = -1;
  for(  int i = 0 ; i < weichenstatus.size() ; i++ ){
    if( weichenstatus.at(i).first.compare( todelete->getS_id() ) == 0 ){//falls der zu löschende Status gefunden wurde: Löschauftrag
      remember = i;
      //qDebug()<<"i = "<<i;//DEBUG
    }
  }
  if( remember >= 0 ){//nur wenn das zu löschende Signal auch existiert, lösche es
    //qDebug()<<"Löscht Weichenstatus";//DEBUG
    weichenstatus.erase( weichenstatus.begin() + remember );//löscht an i-ter stelle
  }
  //qDebug()<<"Block::";//DEBUG
  remember = -1;
  //Block:
  for(  int j = 0 ; j < block.size() ; j++ ){
    if( block.at(j).first.compare( todelete->getS_id() ) == 0 ){//falls der zu löschende Status gefunden wurde: Löschauftrag
      //qDebug()<<"Hier im if";//DEBUG
      remember = j;
      //qDebug()<<"j = "<<j;//DEBUG
    }
  }
  if(remember >= 0 ){
    block.erase( block.begin() + remember);//löscht an j-ter stelle
  }
  //qDebug()<<"fertig in delete nachbar";//DEBUG
}

void HSignal::addVSignal( VSignal *vs , QString param){

  if( param.compare("v") == 0 ){//Parameter v deutet vorwärts an
    vorsignalV.push_back(vs);
    //qDebug()<<"I";//DEBUG
  }
  if( param.compare("r") == 0 ){//parameter R deutet rückwärts an
    vorsignalR.push_back(vs);
    //qDebug()<<"II";//DEBUG
  }
  if( param.compare("v")!=0 && param.compare("r")!=0  ){
    qDebug()<<"Die Parameterübergabe ist ungültig: V/v oder R/r erwünscht!";
    //qDebug()<<"IV";//DEBUG
  }
}

void HSignal::showVSignalR(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies sind die Vorsignale für Hauptsignal [rück]"<<getS_id()<<"  ***";
  for(  int i = 0 ; i < vorsignalR.size() ; i++){
    qDebug()<<"***   --> "<<vorsignalR.at(i)->getV_id()<<"                                           ***";
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void HSignal::showVSignalV(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies sind die Vorsignale für Hauptsignal [vor]"<<getS_id()<<"   ***";
  for(  int i = 0 ; i < vorsignalV.size() ; i++){
    qDebug()<<"***   --> "<<vorsignalV.at(i)->getV_id()<<"                                           ***";
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void HSignal::addWSignal(WSignal *wsig, QString param){
  if( param.compare("v") == 0 ){//Parameter v deutet vorwärts an
    wsignaleV.push_back(wsig);
    //qDebug()<<"I";//DEBUG
  }
  if( param.compare("r") == 0 ){//parameter R deutet rückwärts an
    wsignaleR.push_back(wsig);
    //qDebug()<<"II";//DEBUG
  }
  if( param.compare("v")!=0 && param.compare("r")!=0  ){
    qDebug()<<"Die Parameterübergabe ist ungültig: v oder r erwünscht!";
    //qDebug()<<"IV";//DEBUG
  }
}
void HSignal::showWSignalR(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies sind die Rangiersignale für Hauptsignal[rück]"<<getS_id()<<"***";
  for(  int i = 0 ; i < wsignaleR.size() ; i++){
    qDebug()<<"***   --> "<<wsignaleR.at(i)->getV_id()<<"                                           ***";
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void HSignal::showWSignalV(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies sind die Rangiersignale für Hauptsignal[vor]"<<getS_id()<<"***";
  for(  int i = 0 ; i < wsignaleV.size() ; i++){
    qDebug()<<"***   --> "<<wsignaleV.at(i)->getV_id()<<"                                           ***";
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void HSignal::deleteVS( VSignal *todelete , QString param){
  int remember = -1;//speichert die Position
  if(param.compare("v") == 0){//Parameter v meint vorwärts
    //qDebug()<<"es ist v";//DEBUG
    for( int i = 0 ; i < vorsignalV.size() ; i++){//durchlaufen der VS
      if(vorsignalV.at(i)->getV_id().compare( todelete->getV_id() ) == 0 ){//falls es gefunden wird
	//qDebug()<<"es gibt das vs";//DEBUG
	remember = i;
	//qDebug()<<"i="<<i;
      }
    }
    if(remember >= 0 ){
      //qDebug()<<"löscht vs v";DEBUG
      vorsignalV.erase(vorsignalV.begin() + remember);//hier wird es gelöscht
      //qDebug()<<"vs v gelöscht";//DEBUG
    }
  }
  remember = -1;//reset remember for new calculation
  if(param.compare("r") == 0){//Parameter R meint rückwärts
    //qDebug()<<"es ist r";//DEBUG
    for( int i = 0 ; i < vorsignalR.size() ; i++){//durchlaufen der VS
      if(vorsignalR.at(i)->getV_id().compare( todelete->getV_id() ) == 0 ){//falls es gefunden wird
	//qDebug()<<"es gibt vs2";//DEBUG
	remember = i;
	//qDebug()<<"i="<<i;//DEBUG
      }
    }
    if(remember >= 0 ){
      //qDebug()<<"löscht vs r";//DEBUG
      vorsignalR.erase(vorsignalR.begin() + remember);//hier wird es gelöscht
    }
  }
  //qDebug()<<"fertig deletevs";//DEBUG
}

void HSignal::deleteWS( WSignal *todelete , QString param){
  int remember = -1;//speichert die Position
  if(param.compare("v") == 0){//Parameter v meint vorwärts
    //qDebug()<<"es ist v";//DEBUG
    for( int i = 0 ; i < wsignaleV.size() ; i++){//durchlaufen der VS
      if(wsignaleV.at(i)->getV_id().compare( todelete->getV_id() ) == 0 ){//falls es gefunden wird
	//qDebug()<<"es gibt das vs";//DEBUG
	remember = i;
	//qDebug()<<"i="<<i;
      }
    }
    if(remember >= 0 ){
      //qDebug()<<"löscht vs v";DEBUG
      wsignaleV.erase(wsignaleV.begin() + remember);//hier wird es gelöscht
      //qDebug()<<"vs v gelöscht";//DEBUG
    }
  }
  remember = -1;//reset remember for new calculation
  if(param.compare("r") == 0){//Parameter R meint rückwärts
    //qDebug()<<"es ist r";//DEBUG
    for( int i = 0 ; i < wsignaleR.size() ; i++){//durchlaufen der VS
      if(wsignaleR.at(i)->getV_id().compare( todelete->getV_id() ) == 0 ){//falls es gefunden wird
	//qDebug()<<"es gibt vs2";//DEBUG
	remember = i;
	//qDebug()<<"i="<<i;//DEBUG
      }
    }
    if(remember >= 0 ){
      //qDebug()<<"löscht vs r";//DEBUG
      wsignaleR.erase(wsignaleR.begin() + remember);//hier wird es gelöscht
    }
  }
  //qDebug()<<"fertig deletews";//DEBUG
}

void HSignal::setZiel(QString zziel){
  if(zziel.at(0) == 'S'){//es ist ein HS
    hasHSZiel = true;
    ziel = zziel;
  }
  if(zziel.at(0) == 'W'){
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
  bool erfolg = false; //Rückgabewert vorbereiten
  //bool zur Kontrolle, ob FS freigegeben werden darf
  bool blockfreigabe = true;//verriegelt wäre false-->suche solange es entriegelte Blöcke gibt
  bool blockkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  bool weichenverr = false;//verriegelt wäre true-->suche solange es entriegelte Weichen gibt
  bool weichenkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  if( getS_id().compare(toZiel->getS_id() ) != 0 && isNachbar(toZiel) ){//prüfe, ob Zielsignal Nachbar ist und ob Zielsignal nicht das selbe Signal ist wie das Startsignal
    //::::Blockgeschichte ==========================
    for(  int i = 0 ; i < block.size() ; i++ ){//durchlaufe den Blockstatus
      if( block.at(i).first.compare( toZiel->getS_id() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
	for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){//durchlaufe alle relevanten blöcke
	  if( !block.at(i).second.at(j)->getB_status() ){//betrachte alle Belegtstatus [falls belegt]
	    blockkontrolle = false;//flag auf false setzten
	    //qDebug()<<"öööööö::Folgender Block ist noch belegt"<<block.at(i).second.at(j)->getName();
	    break;
	  }
	  if( !block.at(i).second.at(j)->getFreigabe() ){//betrachte alle Belegtstatus [falls verriegelt]
	    blockfreigabe = false;//flag auf false setzen
	    break;
	  }
	  //wenn die Schleife komplett durchlaufen wird, dann stehen blockfreigabe und blockkontrolle auf = true -->also nichts belegt oder verriegelt
	}
      }
      //Die Schleife für die Namensuche läuft noch fertig
      //else blockkontrolle = false;//Sicherung, dass nicht das Signal sich selbst als Ziel hat
    }
    //::::Weichengeschichte ========================
    for(  int k = 0 ; k < weichenstatus.size() ; k++ ){//durchlaufe weichenstatus
      if( weichenstatus.at(k).first.compare( toZiel->getS_id() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
	for(  int l = 0 ; l < weichenstatus.at(k).second.size() ; l++ ){//durchlaufe alle relevanten Weichen
	  if( !weichenstatus.at(k).second.at(l).first->getBelegung() ){//betrachte alle Belegtstatus [falls belegt]
	    weichenkontrolle = false;//flag auf false setzen
	    break;
	  }
	  if( weichenstatus.at(k).second.at(l).first->getVerriegelung() ){//betrachte Verriegelung [falls verriegelt = true]
	    weichenverr = true;//flag auf true setzen-->hier ist was verriegeltes
	    break;
	  }
	}
	//wenn Loop komplett durchlaufen, dann stehen die Weichnfreigabe und weichenverr so, dass eine FS gestellt werden darf
      }
      //Die Schleife für die Namensuche läuft noch fertig
    }
    //::::Erlaubnisabgleich =======================
    bool blockgeschichte = false;//komplette Freigabe Block möglich?
    bool weichengeschichte = false;//komplett Freigabe Weichen möglich?
    blockgeschichte = blockkontrolle && blockfreigabe;//blockgeschichte = true --> Erlaubnis
    weichengeschichte = weichenkontrolle && !weichenverr;//weichengeschichte = true --> erlaubnis
    //::::Das eigentliche Stellen =================
    //qDebug()<<"___::INFO::___ HSignal::Erlaubnis von SetFahrt: blockkontrolle und weichenkontrolle "<<blockgeschichte<<" | "<<weichengeschichte<<" = blockkontr & blockfreig"<<blockkontrolle<<" "<<blockfreigabe;
    if( blockgeschichte && weichengeschichte ){//Erlaubnis ist gegeben-->Stellen
      //BLOCK
      for(  int m = 0 ; m < block.size() ; m++ ){//durchlaufe den Blockstatus
	if( block.at(m).first.compare( toZiel->getS_id() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
	  for(  int n = 0 ; n < block.at(m).second.size() ; n++ ){//durchlaufe alle relevanten blöcke
	    block.at(m).second.at(n)->setFreigabe( false );//Verriegeln des Blocks
	    //jetzt sind alle Blöcke der FS reserviert
	  }
	}
	//Die Schleife für die Namensuche läuft noch fertig
      }
      //WEICHEN
      for(  int o = 0 ; o < weichenstatus.size() ; o++ ){//durchlaufe weichenstatus
	if( weichenstatus.at(o).first.compare( toZiel->getS_id() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
	  for(  int p = 0 ; p < weichenstatus.at(o).second.size() ; p++ ){//durchlaufe alle relevanten Weichen
	    weichenstatus.at(o).second.at(p).first->setW_status(weichenstatus.at(o).second.at(p).second);//setze die Weiche für die betreffende FS auf den Sollwert
	    weichenstatus.at(o).second.at(p).first->setVerriegelung( true );//und setzen der Verriegelung, weil nun die FS besteht
	  }
	  //jetzt sind auch die Weichen gestellt und reserviert
	}
	//Die Schleife für die Namensuche läuft noch fertig
      }
      //WSIGNALE - alle bis zum nächsten Ziel werden auf Fahrt gestellt [vor]
      for(  int z = 0; z < wsignaleV.size(); z++){
	if( wsignaleV.at(z)->getRichtung( getS_id(), toZiel->getS_id() ) >= 0 ){//wenn es genau diese Richtung gibt, involviere es in die FS stelle es auf Fahrt
	  wsignaleV.at(z)->setinFS( true , getS_id() , toZiel->getS_id() );
	  wsignaleV.at(z)->setV_status( true );
	}
      }
      //VSIGNALE - alle vorwärts VS als inFS markieren und schon mal Ziel Signal vorsignalisieren!
      for(  int i = 0 ; i < vorsignalV.size() ; i++ ){//durchlaufe alle möglich involvierten VS
	if( vorsignalV.at(i)->getRichtung( getS_id(), toZiel->getS_id() ) >= 0 ){//wenn es genau diese Richtung gibt, involviere es in die FS und signalisiere den aktuellen Stand des Zielsignals
	  vorsignalV.at(i)->setinFS( true , getS_id() , toZiel->getS_id() );
	  vorsignalV.at(i)->setV_status( toZiel->getS_status() );
	}
      }
      //HSIGNAL-Status wird jetzt auf Fahrt gesetzt
      setZiel( toZiel->getS_id() );
      toZiel->setfromHS(this);
      s_status = true;
      hasHSZiel = true;
      changeColor();
      emit refreshStellwerkstechnik( getS_id() , true );//Liste in Stellwerksim aktualiesieren
      //VSIGNALE - jetzt kann auch noch evtl VS auf Fahrt erwarten gehen-->rückwirkend
      for(  int i = 0 ; i < vorsignalR.size() ; i++ ){//durchlaufe alle VS rückwirkend und finde genau das VS, bei dem die aktuelle Richtung vorliegt und inFS true ist -->stelle das entsprechend auf Fahrt erwarten
	//qDebug()<<"in v rück for schleife";
	if(vorsignalR.at(i)->getinFS() && vorsignalR.at(i)->isAktFS( getS_id() ) ){//finde das richtige VS
	  //qDebug()<<"setzte VS auf true ...";
	  vorsignalR.at(i)->setV_status( true );
	}
      }
      //////
      erfolg = true;  // Rückgabe Wert auf erfolgreiches stellen!
    }
    //else, zwar isNachbar == true, aber keine Erlaubnis -->setSpeicher
    if( !speicher and !erfolg ){ //Wenn kein Speicher gestellt ist und kein erfolg, dann setze ihn hiermit (kein Speicher vorher, Fahrt versuch --> kommt in den Speicher)
      speicher = true;
      speicherziel = toZiel;
      emit illuminateSpeicher(true,toZiel->getS_id());
      speicheritems.first->setBrush(Qt::yellow);
      //qDebug()<<"emitting speicher...";
      emit callspmemory(this, toZiel);
      erfolg = false; //Rückgabe Wert auf erfolgreiches stellen false
      //qDebug()<<"Rückgabe bei emitting speicher ist bei = "<<erfolg;
    }
    if( speicher and !erfolg){ //kein Stellerfolg, ist aber schon als Speicher vermerkt --> bleibt im Speicher, aber nicht neu einwählen, ist da nämlich schon drin!
      erfolg = false;
    }
  }
  return erfolg;
}


void HSignal::setFahrt( WSignal *toZiel ){
  //bool zur Kontrolle, ob FS freigegeben werden darf
  bool blockfreigabe = true;//verriegelt wäre false-->suche solange es entriegelte Blöcke gibt
  bool blockkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  bool weichenverr = false;//verriegelt wäre true-->suche solange es entriegelte Weichen gibt
  bool weichenkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  if( getS_id().compare(toZiel->getV_id() ) != 0 && isNachbar(toZiel) ){//prüfe, ob Zielsignal Nachbar ist und ob Zielsignal nicht das selbe Signal ist wie das Startsignal
    //::::Blockgeschichte ==========================
    for(  int i = 0 ; i < blockZuRangier.size() ; i++ ){//durchlaufe den Blockstatus
      if( blockZuRangier.at(i).first.compare( toZiel->getV_id() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
	for(  int j = 0 ; j < blockZuRangier.at(i).second.size() ; j++ ){//durchlaufe alle relevanten blöcke
	  if( !blockZuRangier.at(i).second.at(j)->getB_status() ){//betrachte alle Belegtstatus [falls belegt]
	    blockkontrolle = false;//flag auf false setzten
	    break;
	  }
	  if( !blockZuRangier.at(i).second.at(j)->getFreigabe() ){//betrachte alle Belegtstatus [falls verriegelt]
	    blockfreigabe = false;//flag auf false setzen
	    break;
	  }
	  //wenn die Schleife komplett durchlaufen wird, dann stehen blockfreigabe und blockkontrolle auf = true -->also nichts belegt oder verriegelt
	}
      }
      //Die Schleife für die Namensuche läuft noch fertig
      //else blockkontrolle = false;//Sicherung, dass nicht das Signal sich selbst als Ziel hat
    }
    //::::Weichengeschichte ========================
    for(  int k = 0 ; k < weichenstatusZuRangier.size() ; k++ ){//durchlaufe weichenstatus
      if( weichenstatusZuRangier.at(k).first.compare( toZiel->getV_id() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
	for(  int l = 0 ; l < weichenstatusZuRangier.at(k).second.size() ; l++ ){//durchlaufe alle relevanten Weichen
	  if( !weichenstatusZuRangier.at(k).second.at(l).first->getBelegung() ){//betrachte alle Belegtstatus [falls belegt]
	    weichenkontrolle = false;//flag auf false setzen
	    break;
	  }
	  if( weichenstatusZuRangier.at(k).second.at(l).first->getVerriegelung() ){//betrachte Verriegelung [falls verriegelt = true]
	    weichenverr = true;//flag auf true setzen-->hier ist was verriegeltes
	    break;
	  }
	}
	//wenn Loop komplett durchlaufen, dann stehen die Weichnfreigabe und weichenverr so, dass eine FS gestellt werden darf
      }
      //Die Schleife für die Namensuche läuft noch fertig
    }
    //::::Erlaubnisabgleich =======================
    bool blockgeschichte = false;//komplette Freigabe Block möglich?
    bool weichengeschichte = false;//komplett Freigabe Weichen möglich?
    blockgeschichte = blockkontrolle && blockfreigabe;//blockgeschichte = true --> Erlaubnis
    weichengeschichte = weichenkontrolle && !weichenverr;//weichengeschichte = true --> erlaubnis
    //::::Das eigentliche Stellen =================
    if( blockgeschichte && weichengeschichte ){//Erlaubnis ist gegeben-->Stellen
      //BLOCK
      for(  int m = 0 ; m < blockZuRangier.size() ; m++ ){//durchlaufe den Blockstatus
	if( blockZuRangier.at(m).first.compare( toZiel->getV_id() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
	  for(  int n = 0 ; n < blockZuRangier.at(m).second.size() ; n++ ){//durchlaufe alle relevanten blöcke
	    blockZuRangier.at(m).second.at(n)->setFreigabe( false );//Verriegeln des Blocks
	    //jetzt sind alle Blöcke der FS reserviert
	  }
	}
	//Die Schleife für die Namensuche läuft noch fertig
      }
      //WEICHEN
      for(  int o = 0 ; o < weichenstatusZuRangier.size() ; o++ ){//durchlaufe weichenstatus
	if( weichenstatusZuRangier.at(o).first.compare( toZiel->getV_id() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
	  for(  int p = 0 ; p < weichenstatusZuRangier.at(o).second.size() ; p++ ){//durchlaufe alle relevanten Weichen
	    weichenstatusZuRangier.at(o).second.at(p).first->setW_status(weichenstatusZuRangier.at(o).second.at(p).second);//setze die Weiche für die betreffende FS auf den Sollwert
	    weichenstatusZuRangier.at(o).second.at(p).first->setVerriegelung( true );//und setzen der Verriegelung, weil nun die FS besteht
	  }
	  //jetzt sind auch die Weichen gestellt und reserviert
	}
	//Die Schleife für die Namensuche läuft noch fertig
      }
      //HSIGNAL-Status wird jetzt auf Fahrt gesetzt
      zuRangier = true;//kurz anzeigen, dass es sich um den Übergang zu einer Rangier FS handelt
      s_status = true;
      setZiel(toZiel->getV_id());
      hasWSZiel = true;
      changeColor();
      zuRangier = false;//resetten, dass nicht die nächste auch eine Rangierstraße wird
      emit refreshStellwerkstechnik( getS_id() , true );//Liste in Stellwerksim aktualisieren wird hier wie normales grün behandelt...
      //////
    }
  }
}

void HSignal::zugpassiert(){
  qDebug()<<"                                                                   So, jetzt sind wir in zugpassiert von signal "<<getS_id();
  s_status = false;//HS fällt beim passieren auf Halt
  hasHSZiel = false;
  hasWSZiel = false;
  changeColor();
  emit refreshStellwerkstechnik( getS_id() , false );//Liste in Stellwerksim aktualisieren
  //VSIGNALE - jetzt geht VS auf Halt erwarten gehen-->rückwirkendntsprechend inFS muss auch gelöscht werden
  for(  int i = 0 ; i < vorsignalR.size() ; i++ ){//durchlaufe alle VS rückwirkend und finde genau das VS, bei dem die aktuelle Richtung vorliegt und inFS true ist -->stelle das entsprechend auf Halt erwarten
    if(vorsignalR.at(i)->getinFS() && vorsignalR.at(i)->isAktFS( getS_id() ) ){//finde das richtige VS
      vorsignalR.at(i)->zugpassiert();//setinFS=false setV_status=false integriert-->s. VSignal
    }
  }
  //WSIGNALE - jetzt geht WS auf Halt -->rückwirkend entsprechend inFS muss auch gelöscht werden
  for(  int i = 0 ; i < wsignaleR.size() ; i++ ){//durchlaufe alle WS rückwirkend und finde genau das WS, bei dem die aktuelle Richtung vorliegt und inFS true ist -->stelle das entsprechend auf Halt
    if(wsignaleR.at(i)->getinFS() && wsignaleR.at(i)->isAktFS( getS_id() ) ){//finde das richtige VS
      wsignaleR.at(i)->zugpassiert();//setinFS=false setV_status=false integriert-->s. VSignal
    }
  }
}

void HSignal::changeColor(){
  if( getS_status() ){//Fahrt
    if(!zuRangier){//normale Haupt FS
      halt->setVisible(false);
      rangier->setVisible(false);
      fahrt->setVisible(true);
    }
    if(zuRangier){//Übergang zu Rangierfahrt
      halt->setVisible(false);
      rangier->setVisible(true);
      fahrt->setVisible(false);
    }
  }
  if( !getS_status() ){//Halt
    halt->setVisible(true);
    rangier->setVisible(false);
    fahrt->setVisible(false);
  }
}

void HSignal::deleteFS(){
  //bool zur Kontrolle, ob FS gelöscht werden darf (alles muss verriegelt und unbelegt sein)
  bool blockfreigabe = false;//entriegelt wäre true-->suche solange es verriegelte Blöcke gibt
  bool blockkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  bool weichenverr = true;//entriegelt wäre false-->suche solange es verriegelte Weichen gibt
  bool weichenkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  //nun große Unterscheidung, ob deleteFS von einer HS->HS oder einer HS->WS
  //##########################################################################
  if(hasHSZiel){
    //::::Blockgeschichte ==========================
    qDebug()<<" XXX DAVOR";
    for(  int i = 0 ; i < block.size() ; i++ ){//durchlaufe den Blockstatus
      if( block.at(i).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
	qDebug()<<" XXX DANACH";
	for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){//durchlaufe alle relevanten blöcke
	  if( !block.at(i).second.at(j)->getB_status() ){//betrachte alle Belegtstatus [falls belegt]
	    blockkontrolle = false;//flag auf false setzten
	    break;
	  }
	  if( block.at(i).second.at(j)->getFreigabe() ){//betrachte alle Belegtstatus [falls entriegelt]
	    blockfreigabe = true;//flag auf true setzen
	    break;
	  }
	  //wenn die Schleife komplett durchlaufen wird, dann stehen blockfreigabe = false und blockkontrolle auf = true -->also nichts belegt aber verriegelt
	}
      }
      //Die Schleife für die Namensuche läuft noch fertig
    }
    //::::Weichengeschichte ========================
    for(  int k = 0 ; k < weichenstatus.size() ; k++ ){//durchlaufe weichenstatus
      if( weichenstatus.at(k).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
	for(  int l = 0 ; l < weichenstatus.at(k).second.size() ; l++ ){//durchlaufe alle relevanten Weichen
	  if( !weichenstatus.at(k).second.at(l).first->getBelegung() ){//betrachte alle Belegtstatus [falls belegt]
	    weichenkontrolle = false;//flag auf false setzen
	    break;
	  }
	  if( !weichenstatus.at(k).second.at(l).first->getVerriegelung() ){//betrachte Verriegelung [falls entriegelt = false]
	    weichenverr = false;//flag auf false setzen-->hier ist was entriegeltes
	    break;
	  }
	}
	//wenn Loop komplett durchlaufen, dann stehen die Weichnkontrolle und weichenverr = true, FS unbelegt aber verriegelt
      }
      //Die Schleife für die Namensuche läuft noch fertig
    }
    //:::Auswertung des Ergebnisses, ob FS gelöscht werden darf ============
    bool blockgeschichte = false;//komplette Löscherlaubnis Block?
    bool weichengeschichte = false;//komplette Löscherlaubnis Weichen?
    blockgeschichte = blockkontrolle && !blockfreigabe;//blockgeschichte = true --> Erlaubnis
    weichengeschichte = weichenkontrolle && weichenverr;//weichengeschichte = true --> erlaubnis
    //:::Das eigentliche Löschen ===========
    if( blockgeschichte && weichengeschichte ){//Erlaubnis zum Löschen ist gegeben-->Löschen
      //BLOCK
      for(  int m = 0 ; m < block.size() ; m++ ){//durchlaufe den Blockstatus
	if( block.at(m).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
	  for(  int n = 0 ; n < block.at(m).second.size() ; n++ ){//durchlaufe alle relevanten blöcke
	    block.at(m).second.at(n)->setFreigabe( true );//Entriegeln des Blocks
	    //jetzt sind alle Blöcke der FS wieder freigegeben
	  }
	}
	//Die Schleife für die Namensuche läuft noch fertig
      }
      //WEICHEN
      for(  int o = 0 ; o < weichenstatus.size() ; o++ ){//durchlaufe weichenstatus
	if( weichenstatus.at(o).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
	  for(  int p = 0 ; p < weichenstatus.at(o).second.size() ; p++ ){//durchlaufe alle relevanten Weichen
	    weichenstatus.at(o).second.at(p).first->setVerriegelung( false );//und setzen der Entriegelung, weil nun die FS aufgelöst wird (stellen der Weiche ist egal...)
	  }
	  //jetzt sind auch die Weichen entriegelt
	}
	//Die Schleife für die Namensuche läuft noch fertig
      }
      //WSIGNALE - rotstellen aller WSignale im Falle eines Löschens der FS
      for(  int z = 0 ; z < wsignaleV.size() ; z++ ){//durchlaufe alle möglich involvierten VS
	if( wsignaleV.at(z)->getRichtung( getS_id(), getZiel() ) >= 0 ){//wenn es genau diese Richtung gibt, setze inFS = false und signalisiere Halt , weil es keine FS mehr gibt
	  //qDebug()<<"setting infs false WSIGNALE...";
	  wsignaleV.at(z)->setinFS( false );
	  //qDebug()<<"vorsignalisiere Ziel S...-->halt erwarten, weil die FS gelöscht wird WSIGNALE";
	  wsignaleV.at(z)->setV_status( false );
	}
      }
      //VSIGNALE - alle vorwärts VS als inFS de-markieren und schon mal Ziel Signal vorsignalisieren!
      for(  int i = 0 ; i < vorsignalV.size() ; i++ ){//durchlaufe alle möglich involvierten VS
	if( vorsignalV.at(i)->getRichtung( getS_id(), getZiel() ) >= 0 ){//wenn es genau diese Richtung gibt, setze inFS = false und signalisiere Halt erwarten, weil es keine FS mehr gibt
	  //qDebug()<<"setting infs false...";
	  vorsignalV.at(i)->setinFS( false );
	  //qDebug()<<"vorsignalisiere Ziel S...-->halt erwarten, weil die FS gelöscht wird";
	  vorsignalV.at(i)->setV_status( false );
	}
      }
      //HSIGNAL-Status wird jetzt gelöscht
      //Ziel ist ab jetzt nicht mehr aktuell
      s_status = false;
      //hasHSZiel = false;  //nicht: da FS Speicher bereits in Spmemory --> dann passiert seg fault -> s. auch unten
      changeColor();
      emit refreshStellwerkstechnik( getS_id() , false );//Liste in Stellwerksim aktualiesieren
      //VSIGNALE - jetzt kann auch noch evtl VS auf Halt erwarten gehen-->rückwirkend
      for(  int i = 0 ; i < vorsignalR.size() ; i++ ){//durchlaufe alle VS rückwirkend und finde genau das VS, bei dem die aktuelle Richtung vorliegt und inFS true ist -->stelle das entsprechend auf Halt erwarten
	//qDebug()<<"in v rück for schleife deleteFS";
	if(vorsignalR.at(i)->getinFS() && vorsignalR.at(i)->isAktFS( getS_id() ) ){//finde das richtige VS
	  //qDebug()<<"setzte VS auf false (deleteFS)...";
	  vorsignalR.at(i)->setV_status( false );
	}
      }
      //////
    }
  }
  //##########################################################################
  if(hasWSZiel){
    //::::Blockgeschichte ==========================
    for(  int i = 0 ; i < blockZuRangier.size() ; i++ ){//durchlaufe den Blockstatus
      if( blockZuRangier.at(i).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
	for(  int j = 0 ; j < blockZuRangier.at(i).second.size() ; j++ ){//durchlaufe alle relevanten blöcke
	  if( !blockZuRangier.at(i).second.at(j)->getB_status() ){//betrachte alle Belegtstatus [falls belegt]
	    blockkontrolle = false;//flag auf false setzten
	    break;
	  }
	  if( blockZuRangier.at(i).second.at(j)->getFreigabe() ){//betrachte alle Belegtstatus [falls entriegelt]
	    blockfreigabe = true;//flag auf true setzen
	    break;
	  }
	  //wenn die Schleife komplett durchlaufen wird, dann stehen blockfreigabe = false und blockkontrolle auf = true -->also nichts belegt aber verriegelt
	}
      }
      //Die Schleife für die Namensuche läuft noch fertig
    }
    //::::Weichengeschichte ========================
    for(  int k = 0 ; k < weichenstatusZuRangier.size() ; k++ ){//durchlaufe weichenstatus
      if( weichenstatusZuRangier.at(k).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
	for(  int l = 0 ; l < weichenstatusZuRangier.at(k).second.size() ; l++ ){//durchlaufe alle relevanten Weichen
	  if( !weichenstatusZuRangier.at(k).second.at(l).first->getBelegung() ){//betrachte alle Belegtstatus [falls belegt]
	    weichenkontrolle = false;//flag auf false setzen
	    break;
	  }
	  if( !weichenstatusZuRangier.at(k).second.at(l).first->getVerriegelung() ){//betrachte Verriegelung [falls entriegelt = false]
	    weichenverr = false;//flag auf false setzen-->hier ist was entriegeltes
	    break;
	  }
	}
	//wenn Loop komplett durchlaufen, dann stehen die Weichnkontrolle und weichenverr = true, FS unbelegt aber verriegelt
      }
      //Die Schleife für die Namensuche läuft noch fertig
    }
    //:::Auswertung des Ergebnisses, ob FS gelöscht werden darf ============
    bool blockgeschichte = false;//komplette Löscherlaubnis Block?
    bool weichengeschichte = false;//komplette Löscherlaubnis Weichen?
    blockgeschichte = blockkontrolle && !blockfreigabe;//blockgeschichte = true --> Erlaubnis
    weichengeschichte = weichenkontrolle && weichenverr;//weichengeschichte = true --> erlaubnis
    //:::Das eigentliche Löschen ===========
    if( blockgeschichte && weichengeschichte ){//Erlaubnis zum Löschen ist gegeben-->Löschen
      //BLOCK
      for(  int m = 0 ; m < blockZuRangier.size() ; m++ ){//durchlaufe den Blockstatus
	if( blockZuRangier.at(m).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
	  for(  int n = 0 ; n < blockZuRangier.at(m).second.size() ; n++ ){//durchlaufe alle relevanten blöcke
	    blockZuRangier.at(m).second.at(n)->setFreigabe( true );//Entriegeln des Blocks
	    //jetzt sind alle Blöcke der FS wieder freigegeben
	  }
	}
	//Die Schleife für die Namensuche läuft noch fertig
      }
      //WEICHEN
      for(  int o = 0 ; o < weichenstatusZuRangier.size() ; o++ ){//durchlaufe weichenstatus
	if( weichenstatusZuRangier.at(o).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
	  for(  int p = 0 ; p < weichenstatusZuRangier.at(o).second.size() ; p++ ){//durchlaufe alle relevanten Weichen
	    weichenstatusZuRangier.at(o).second.at(p).first->setVerriegelung( false );//und setzen der Entriegelung, weil nun die FS aufgelöst wird (stellen der Weiche ist egal...)
	  }
	  //jetzt sind auch die Weichen entriegelt
	}
	//Die Schleife für die Namensuche läuft noch fertig
      }
      //HSIGNAL-Status wird jetzt gelöscht
      //Ziel ist ab jetzt nicht mehr aktuell
      s_status = false;
      hasWSZiel = false;
      changeColor();
      emit refreshStellwerkstechnik( getS_id() , false );//Liste in Stellwerksim aktualiesieren
      //////
    }
  }
}

void HSignal::listenToFS(){
  qDebug()<<"HSignal:::I am clicked ...";
  //leite weiter an den Clickmanager
  emit listened(this);

}

void HSignal::recieveSpeicher(bool sp, QString str){
  //qDebug()<<"recieving Speicher...";
  if(str.compare(getS_id()) == 0){//emit illuminate erreicht alle Slots, da in addBlock connected, hier wird geprüft, ob das HS gemeint ist
    //qDebug()<<"bin drin in recieve speicher if";
    if(sp){//falls es sich um einen neuen Speicher handelt
      speicheritems.second->setBrush(Qt::yellow);
      //fromHS()->speicheritems.first->setBrush(Qt::yellow); //wird an geeigneter Stelle oben/unten erledigt, suche nach 'illuminate'
    }
    if(!sp){//falls es sich um eine Auflösung vom Speicher handelt
      //qDebug()<<"darken speicheritems";
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
  //schirm und Träger zufügen
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
