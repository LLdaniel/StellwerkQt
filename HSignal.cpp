//*************************************************************************
//Hauptsignal des Stellwerks  [- HSIGNAL.CXX -]
//*************************************************************************
#include "HSignal.h"
#include <iostream>

HSignal::HSignal(int name ){
  setS_id( name );
}

void HSignal::setS_id( int name ){
  std::string suffix = std::to_string( name );//name zu String konvertieren
  if( name >0 && name <1000){//dreistelligkeit wird hier überprüft
    if( name < 10 ){//zwei Vornullen werden erzeugt
      s_id = "S00" + suffix;//std::to_string( name );
    }
    if( name < 100 && name > 10){//eine Vornull wird erzeugt
      s_id = "S0" + suffix;//std::to_string( name );
    }
    if( name < 1000 && name > 100){//keine Vornull mehr nötig
      s_id = "S" + suffix;//std::to_string( name );
    }
  }
  else std::cout<<"HSignal:Falsche Benennung. Die Zahl muss dreistellig sein."<<std::endl;
}

void HSignal::addWeichenstatus( HSignal *toZiel , std::vector<std::pair<Weiche* , bool>> weichenpair ){
  std::pair<std::string, std::vector<std::pair<Weiche* , bool>> > hilfspair( toZiel->getS_id() , weichenpair );//Das Hilfspair wird erzeugt, string wird über das Signal selbst erzeugt
  weichenstatus.push_back(hilfspair);
}

void HSignal::addWeichenstatusZuRangier( WSignal *toZiel , std::vector<std::pair<Weiche* , bool>> weichenpair ){
  std::pair<std::string, std::vector<std::pair<Weiche* , bool>> > hilfspair( toZiel->getV_id() , weichenpair );//Das Hilfspair wird erzeugt, string wird über das Signal selbst erzeugt
  weichenstatusZuRangier.push_back(hilfspair);
}

void HSignal::showWeichenstatusALL(){
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<"*** Dies ist der Weichenstatus für Hauptsignal "<<getS_id()<<"      ***"<<std::endl;
  for( unsigned int i = 0 ; i < weichenstatus.size() ; i++){
    std::cout<<"***   "<<weichenstatus.at(i).first<<" :                                             ***"<<std::endl;
    for( unsigned int j = 0 ; j < weichenstatus.at(i).second.size() ; j++ ){//hier sind die Weichenstatuspaarungen
      std::cout<<"***               "<<weichenstatus.at(i).second.at(j).first->getW_id()<<"   -->   "<<weichenstatus.at(i).second.at(j).second<<"                          ***"<<std::endl;
    }
  }
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<""<<std::endl;
}

void HSignal::showWeichenstatus( HSignal *whichZiel ){
  for( unsigned int i = 0 ; i < weichenstatus.size() ; i++){
    if( weichenstatus.at(i).first.compare( whichZiel->getS_id() ) == 0 ){//falls das gesuchte Signal zum Signal in der Liste passt, durchlaufe die Weichenstatuspaarungen
      std::cout<<""<<std::endl;
      std::cout<<"************************************************************"<<std::endl;
      std::cout<<"*** Dies ist der Weichenstatus für Hauptsignal "<<getS_id()<<"      ***"<<std::endl;
      std::cout<<"***   "<<weichenstatus.at(i).first<<" :                                             ***"<<std::endl;
      for( unsigned int j = 0 ; j < weichenstatus.at(i).second.size() ; j++ ){//hier sind die Weichenstatuspaarungen
    std::cout<<"***               "<<weichenstatus.at(i).second.at(j).first->getW_id()<<"   -->   "<<weichenstatus.at(i).second.at(j).second<<"                          ***"<<std::endl;
      }
      std::cout<<"************************************************************"<<std::endl;
      std::cout<<""<<std::endl;
    }
  }
}

void HSignal::addBlock( HSignal *toZiel , std::vector<Block*> inputBlock ){
  std::pair<std::string , std::vector<Block*>> hilfspair(toZiel->getS_id(), inputBlock);
  block.push_back(hilfspair);
  //hier gleich die Verknüpfung der Speicher mit erledigen
  QObject::connect(this,SIGNAL(illuminateSpeicher(bool,std::string)),toZiel,SLOT(recieveSpeicher(bool,std::string) ));
}

void HSignal::addBlockZuRangier( WSignal *toZiel , std::vector<Block*> inputBlock ){
  std::pair<std::string , std::vector<Block*>> hilfspair(toZiel->getV_id(), inputBlock);
  blockZuRangier.push_back(hilfspair);
}

void HSignal::showBlock( HSignal *whichZiel ){
  for( unsigned int i = 0 ; i < block.size() ; i++){
    if( block.at(i).first.compare( whichZiel->getS_id() ) == 0){//Wenn das gesuchte Nachbarsignal gefunden wurde-->Schleife über die Blöcke
      std::cout<<""<<std::endl;
      std::cout<<"************************************************************"<<std::endl;
      std::cout<<"*** Dies sind die Blöcke für Hauptsignal "<<getS_id()<<"            ***"<<std::endl;
      std::cout<<"***   "<<block.at(i).first<<" :                                             ***"<<std::endl;
      for( unsigned int j = 0 ; j < block.at(i).second.size() ; j++ ){//hier sind die Blöcke
    std::cout<<"***               "<<block.at(i).second.at(j)->getName()<<"                                     ***"<<std::endl;
      }
      std::cout<<"************************************************************"<<std::endl;
      std::cout<<""<<std::endl;
    }
  }
}

void HSignal::showBlockALL(){
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<"*** Dies sind die Blöcke für Hauptsignal "<<getS_id()<<"            ***"<<std::endl;
  for( unsigned int i = 0 ; i < block.size() ; i++){
    std::cout<<"***   "<<block.at(i).first<<" :                                             ***"<<std::endl;
    for( unsigned int j = 0 ; j < block.at(i).second.size() ; j++ ){//hier sind die Weichenstatuspaarungen
      std::cout<<"***               "<<block.at(i).second.at(j)->getName()<<"                                     ***"<<std::endl;
    }
  }
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<""<<std::endl;
}

void HSignal::deleteNachbar( HSignal *todelete ){
  //delete meint, sowohl in Weichenstatus, als auch im Block
  //Weichenstatus:
  int remember = -1;
  for( unsigned int i = 0 ; i < weichenstatus.size() ; i++ ){
    if( weichenstatus.at(i).first.compare( todelete->getS_id() ) == 0 ){//falls der zu löschende Status gefunden wurde: Löschauftrag
      remember = i;
      //std::cout<<"i = "<<i<<std::endl;//DEBUG
    }
  }
  if( remember >= 0 ){//nur wenn das zu löschende Signal auch existiert, lösche es
    //std::cout<<"Löscht Weichenstatus"<<std::endl;//DEBUG
    weichenstatus.erase( weichenstatus.begin() + remember );//löscht an i-ter stelle
  }
  //std::cout<<"Block::"<<std::endl;//DEBUG
  remember = -1;
  //Block:
  for( unsigned int j = 0 ; j < block.size() ; j++ ){
    if( block.at(j).first.compare( todelete->getS_id() ) == 0 ){//falls der zu löschende Status gefunden wurde: Löschauftrag
      //std::cout<<"Hier im if"<<std::endl;//DEBUG
      remember = j;
      //std::cout<<"j = "<<j<<std::endl;//DEBUG
    }
  }
  if(remember >= 0 ){
    block.erase( block.begin() + remember);//löscht an j-ter stelle
  }
  //std::cout<<"fertig in delete nachbar"<<std::endl;//DEBUG
}

void HSignal::addVSignal( VSignal *vs , std::string param){

  if( param.compare("v") == 0 ){//Parameter v deutet vorwärts an
    vorsignalV.push_back(vs);
    //std::cout<<"I"<<std::endl;//DEBUG
  }
  if( param.compare("r") == 0 ){//parameter R deutet rückwärts an
    vorsignalR.push_back(vs);
    //std::cout<<"II"<<std::endl;//DEBUG
  }
  if( param.compare("v")!=0 && param.compare("r")!=0  ){
  std::cout<<"Die Parameterübergabe ist ungültig: V/v oder R/r erwünscht!"<<std::endl;
  //std::cout<<"IV"<<std::endl;//DEBUG
  }
}

void HSignal::showVSignalR(){
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<"*** Dies sind die Vorsignale für Hauptsignal [rück]"<<getS_id()<<"  ***"<<std::endl;
  for( unsigned int i = 0 ; i < vorsignalR.size() ; i++){
    std::cout<<"***   --> "<<vorsignalR.at(i)->getV_id()<<"                                           ***"<<std::endl;
  }
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<""<<std::endl;
}

void HSignal::showVSignalV(){
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<"*** Dies sind die Vorsignale für Hauptsignal [vor]"<<getS_id()<<"   ***"<<std::endl;
  for( unsigned int i = 0 ; i < vorsignalV.size() ; i++){
    std::cout<<"***   --> "<<vorsignalV.at(i)->getV_id()<<"                                           ***"<<std::endl;
  }
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<""<<std::endl;
}

void HSignal::addWSignal(WSignal *wsig, std::string param){
    if( param.compare("v") == 0 ){//Parameter v deutet vorwärts an
      wsignaleV.push_back(wsig);
      //std::cout<<"I"<<std::endl;//DEBUG
    }
    if( param.compare("r") == 0 ){//parameter R deutet rückwärts an
      wsignaleR.push_back(wsig);
      //std::cout<<"II"<<std::endl;//DEBUG
    }
    if( param.compare("v")!=0 && param.compare("r")!=0  ){
    std::cout<<"Die Parameterübergabe ist ungültig: v oder r erwünscht!"<<std::endl;
    //std::cout<<"IV"<<std::endl;//DEBUG
    }
}
void HSignal::showWSignalR(){
    std::cout<<"************************************************************"<<std::endl;
    std::cout<<"*** Dies sind die Rangiersignale für Hauptsignal[rück]"<<getS_id()<<"***"<<std::endl;
    for( unsigned int i = 0 ; i < wsignaleR.size() ; i++){
      std::cout<<"***   --> "<<wsignaleR.at(i)->getV_id()<<"                                           ***"<<std::endl;
    }
    std::cout<<"************************************************************"<<std::endl;
    std::cout<<""<<std::endl;
}

void HSignal::showWSignalV(){
    std::cout<<"************************************************************"<<std::endl;
    std::cout<<"*** Dies sind die Rangiersignale für Hauptsignal[vor]"<<getS_id()<<"***"<<std::endl;
    for( unsigned int i = 0 ; i < wsignaleV.size() ; i++){
      std::cout<<"***   --> "<<wsignaleV.at(i)->getV_id()<<"                                           ***"<<std::endl;
    }
    std::cout<<"************************************************************"<<std::endl;
    std::cout<<""<<std::endl;
}

void HSignal::deleteVS( VSignal *todelete , std::string param){
  int remember = -1;//speichert die Position
  if(param.compare("v") == 0){//Parameter v meint vorwärts
    //std::cout<<"es ist v"<<std::endl;//DEBUG
    for(unsigned int i = 0 ; i < vorsignalV.size() ; i++){//durchlaufen der VS
      if(vorsignalV.at(i)->getV_id().compare( todelete->getV_id() ) == 0 ){//falls es gefunden wird
    //std::cout<<"es gibt das vs"<<std::endl;//DEBUG
    remember = i;
    //std::cout<<"i="<<i<<std::endl;
      }
    }
    if(remember >= 0 ){
      //std::cout<<"löscht vs v"<<std::endl;DEBUG
      vorsignalV.erase(vorsignalV.begin() + remember);//hier wird es gelöscht
      //std::cout<<"vs v gelöscht"<<std::endl;//DEBUG
    }
  }
  remember = -1;//reset remember for new calculation
  if(param.compare("r") == 0){//Parameter R meint rückwärts
    //std::cout<<"es ist r"<<std::endl;//DEBUG
    for(unsigned int i = 0 ; i < vorsignalR.size() ; i++){//durchlaufen der VS
      if(vorsignalR.at(i)->getV_id().compare( todelete->getV_id() ) == 0 ){//falls es gefunden wird
    //std::cout<<"es gibt vs2"<<std::endl;//DEBUG
    remember = i;
    //std::cout<<"i="<<i<<std::endl;//DEBUG
      }
    }
    if(remember >= 0 ){
      //std::cout<<"löscht vs r"<<std::endl;//DEBUG
      vorsignalR.erase(vorsignalR.begin() + remember);//hier wird es gelöscht
    }
  }
  //std::cout<<"fertig deletevs"<<std::endl;//DEBUG
}

void HSignal::deleteWS( WSignal *todelete , std::string param){
  int remember = -1;//speichert die Position
  if(param.compare("v") == 0){//Parameter v meint vorwärts
    //std::cout<<"es ist v"<<std::endl;//DEBUG
    for(unsigned int i = 0 ; i < wsignaleV.size() ; i++){//durchlaufen der VS
      if(wsignaleV.at(i)->getV_id().compare( todelete->getV_id() ) == 0 ){//falls es gefunden wird
    //std::cout<<"es gibt das vs"<<std::endl;//DEBUG
    remember = i;
    //std::cout<<"i="<<i<<std::endl;
      }
    }
    if(remember >= 0 ){
      //std::cout<<"löscht vs v"<<std::endl;DEBUG
      wsignaleV.erase(wsignaleV.begin() + remember);//hier wird es gelöscht
      //std::cout<<"vs v gelöscht"<<std::endl;//DEBUG
    }
  }
  remember = -1;//reset remember for new calculation
  if(param.compare("r") == 0){//Parameter R meint rückwärts
    //std::cout<<"es ist r"<<std::endl;//DEBUG
    for(unsigned int i = 0 ; i < wsignaleR.size() ; i++){//durchlaufen der VS
      if(wsignaleR.at(i)->getV_id().compare( todelete->getV_id() ) == 0 ){//falls es gefunden wird
    //std::cout<<"es gibt vs2"<<std::endl;//DEBUG
    remember = i;
    //std::cout<<"i="<<i<<std::endl;//DEBUG
      }
    }
    if(remember >= 0 ){
      //std::cout<<"löscht vs r"<<std::endl;//DEBUG
      wsignaleR.erase(wsignaleR.begin() + remember);//hier wird es gelöscht
    }
  }
  //std::cout<<"fertig deletews"<<std::endl;//DEBUG
}

void HSignal::setZiel(std::string zziel){
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
    for( unsigned int i = 0; i < block.size(); i++){
        if(block.at(i).first.compare( toZiel->getS_id() ) == 0){
            giveback = true;
        }
    }
    return giveback;
}

bool HSignal::isNachbar(WSignal *toZiel){
    bool giveback = false;
    for( unsigned int i = 0; i < blockZuRangier.size(); i++){
        if(blockZuRangier.at(i).first.compare( toZiel->getV_id() ) == 0){
            giveback = true;
        }
    }
    return giveback;
}


void HSignal::setFahrt( HSignal *toZiel ){
  //bool zur Kontrolle, ob FS freigegeben werden darf
  bool blockfreigabe = true;//verriegelt wäre false-->suche solange es entriegelte Blöcke gibt
  bool blockkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  bool weichenverr = false;//verriegelt wäre true-->suche solange es entriegelte Weichen gibt
  bool weichenkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  if( getS_id().compare(toZiel->getS_id() ) != 0 && isNachbar(toZiel) ){//prüfe, ob Zielsignal Nachbar ist und ob Zielsignal nicht das selbe Signal ist wie das Startsignal
  //::::Blockgeschichte ==========================
  for( unsigned int i = 0 ; i < block.size() ; i++ ){//durchlaufe den Blockstatus
    if( block.at(i).first.compare( toZiel->getS_id() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
      for( unsigned int j = 0 ; j < block.at(i).second.size() ; j++ ){//durchlaufe alle relevanten blöcke
    if( !block.at(i).second.at(j)->getB_status() ){//betrachte alle Belegtstatus [falls belegt]
      blockkontrolle = false;//flag auf false setzten
      std::cout<<"öööööö::Folgender Block ist noch belegt"<<block.at(i).second.at(j)->getName()<<std::endl;
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
  for( unsigned int k = 0 ; k < weichenstatus.size() ; k++ ){//durchlaufe weichenstatus
    if( weichenstatus.at(k).first.compare( toZiel->getS_id() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
      for( unsigned int l = 0 ; l < weichenstatus.at(k).second.size() ; l++ ){//durchlaufe alle relevanten Weichen
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
  std::cout<<"___::INFO::___ HSignal::Erlaubnis von SetFahrt: blockkontrolle und weichenkontrolle "<<blockgeschichte<<" | "<<weichengeschichte<<" = blockkontr & blockfreig"<<blockkontrolle<<" "<<blockfreigabe<<std::endl;
  if( blockgeschichte && weichengeschichte ){//Erlaubnis ist gegeben-->Stellen
    //BLOCK
    for( unsigned int m = 0 ; m < block.size() ; m++ ){//durchlaufe den Blockstatus
      if( block.at(m).first.compare( toZiel->getS_id() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
    for( unsigned int n = 0 ; n < block.at(m).second.size() ; n++ ){//durchlaufe alle relevanten blöcke
      block.at(m).second.at(n)->setFreigabe( false );//Verriegeln des Blocks
      //jetzt sind alle Blöcke der FS reserviert
    }
      }
      //Die Schleife für die Namensuche läuft noch fertig
    }
    //WEICHEN
    for( unsigned int o = 0 ; o < weichenstatus.size() ; o++ ){//durchlaufe weichenstatus
      if( weichenstatus.at(o).first.compare( toZiel->getS_id() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
    for( unsigned int p = 0 ; p < weichenstatus.at(o).second.size() ; p++ ){//durchlaufe alle relevanten Weichen
      weichenstatus.at(o).second.at(p).first->setW_status(weichenstatus.at(o).second.at(p).second);//setze die Weiche für die betreffende FS auf den Sollwert
      weichenstatus.at(o).second.at(p).first->setVerriegelung( true );//und setzen der Verriegelung, weil nun die FS besteht
    }
    //jetzt sind auch die Weichen gestellt und reserviert
      }
      //Die Schleife für die Namensuche läuft noch fertig
    }
    //WSIGNALE - alle bis zum nächsten Ziel werden auf Fahrt gestellt [vor]
    for( unsigned int z = 0; z < wsignaleV.size(); z++){
        if( wsignaleV.at(z)->getRichtung( getS_id(), toZiel->getS_id() ) >= 0 ){//wenn es genau diese Richtung gibt, involviere es in die FS stelle es auf Fahrt
          wsignaleV.at(z)->setinFS( true , getS_id() , toZiel->getS_id() );
          wsignaleV.at(z)->setV_status( true );
        }
    }
    //VSIGNALE - alle vorwärts VS als inFS markieren und schon mal Ziel Signal vorsignalisieren!
    for( unsigned int i = 0 ; i < vorsignalV.size() ; i++ ){//durchlaufe alle möglich involvierten VS
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
    for( unsigned int i = 0 ; i < vorsignalR.size() ; i++ ){//durchlaufe alle VS rückwirkend und finde genau das VS, bei dem die aktuelle Richtung vorliegt und inFS true ist -->stelle das entsprechend auf Fahrt erwarten
      std::cout<<"in v rück for schleife"<<std::endl;
      if(vorsignalR.at(i)->getinFS() && vorsignalR.at(i)->isAktFS( getS_id() ) ){//finde das richtige VS
    std::cout<<"setzte VS auf true ..."<<std::endl;
    vorsignalR.at(i)->setV_status( true );
      }
    }
    //////
  }
  //else, zwar isNachbar == true, aber keine Erlaubnis -->setSpeicher
  else{
      speicher = true;
      speicherziel = toZiel;
      emit illuminateSpeicher(true,toZiel->getS_id());
      speicheritems.first->setBrush(Qt::yellow);
      std::cout<<"emitting speicher..."<<std::endl;
      }
  }
}


void HSignal::setFahrt( WSignal *toZiel ){
  //bool zur Kontrolle, ob FS freigegeben werden darf
  bool blockfreigabe = true;//verriegelt wäre false-->suche solange es entriegelte Blöcke gibt
  bool blockkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  bool weichenverr = false;//verriegelt wäre true-->suche solange es entriegelte Weichen gibt
  bool weichenkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  if( getS_id().compare(toZiel->getV_id() ) != 0 && isNachbar(toZiel) ){//prüfe, ob Zielsignal Nachbar ist und ob Zielsignal nicht das selbe Signal ist wie das Startsignal
  //::::Blockgeschichte ==========================
  for( unsigned int i = 0 ; i < blockZuRangier.size() ; i++ ){//durchlaufe den Blockstatus
    if( blockZuRangier.at(i).first.compare( toZiel->getV_id() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
      for( unsigned int j = 0 ; j < blockZuRangier.at(i).second.size() ; j++ ){//durchlaufe alle relevanten blöcke
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
  for( unsigned int k = 0 ; k < weichenstatusZuRangier.size() ; k++ ){//durchlaufe weichenstatus
    if( weichenstatusZuRangier.at(k).first.compare( toZiel->getV_id() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
      for( unsigned int l = 0 ; l < weichenstatusZuRangier.at(k).second.size() ; l++ ){//durchlaufe alle relevanten Weichen
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
    for( unsigned int m = 0 ; m < blockZuRangier.size() ; m++ ){//durchlaufe den Blockstatus
      if( blockZuRangier.at(m).first.compare( toZiel->getV_id() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
    for( unsigned int n = 0 ; n < blockZuRangier.at(m).second.size() ; n++ ){//durchlaufe alle relevanten blöcke
      blockZuRangier.at(m).second.at(n)->setFreigabe( false );//Verriegeln des Blocks
      //jetzt sind alle Blöcke der FS reserviert
    }
      }
      //Die Schleife für die Namensuche läuft noch fertig
    }
    //WEICHEN
    for( unsigned int o = 0 ; o < weichenstatusZuRangier.size() ; o++ ){//durchlaufe weichenstatus
      if( weichenstatusZuRangier.at(o).first.compare( toZiel->getV_id() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
    for( unsigned int p = 0 ; p < weichenstatusZuRangier.at(o).second.size() ; p++ ){//durchlaufe alle relevanten Weichen
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
  std::cout<<"                                                                   So, jetzt sind wir in zugpassiert von signal "<<getS_id()<<std::endl;
  s_status = false;//HS fällt beim passieren auf Halt
  hasHSZiel = false;
  hasWSZiel = false;
  changeColor();
  emit refreshStellwerkstechnik( getS_id() , false );//Liste in Stellwerksim aktualisieren
  //VSIGNALE - jetzt geht VS auf Halt erwarten gehen-->rückwirkendntsprechend inFS muss auch gelöscht werden
  for( unsigned int i = 0 ; i < vorsignalR.size() ; i++ ){//durchlaufe alle VS rückwirkend und finde genau das VS, bei dem die aktuelle Richtung vorliegt und inFS true ist -->stelle das entsprechend auf Halt erwarten
    if(vorsignalR.at(i)->getinFS() && vorsignalR.at(i)->isAktFS( getS_id() ) ){//finde das richtige VS
     vorsignalR.at(i)->zugpassiert();//setinFS=false setV_status=false integriert-->s. VSignal
    }
  }
  //WSIGNALE - jetzt geht WS auf Halt -->rückwirkend entsprechend inFS muss auch gelöscht werden
  for( unsigned int i = 0 ; i < wsignaleR.size() ; i++ ){//durchlaufe alle WS rückwirkend und finde genau das WS, bei dem die aktuelle Richtung vorliegt und inFS true ist -->stelle das entsprechend auf Halt
    if(wsignaleR.at(i)->getinFS() && wsignaleR.at(i)->isAktFS( getS_id() ) ){//finde das richtige VS
     wsignaleR.at(i)->zugpassiert();//setinFS=false setV_status=false integriert-->s. VSignal
    }
  }
}

void HSignal::changeColor(){
    for( int i = 0 ; i < hsignalitems.size() ; i++ ){
        if( getS_status() ){//Fahrt
            if(!zuRangier){//normale Haupt FS
                hsignalitems.at(i)->setBrush(Qt::green);
            }
            if(zuRangier){//Übergang zu Rangierfahrt
                hsignalitems.at(i)->setBrush(Qt::white);
            }
        }
        if( !getS_status() ){//Halt
           hsignalitems.at(i)->setBrush(Qt::red);
        }
    }
    //Hier entsteht der Rangiersignalbereich mit Qt::white
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
      std::cout<<" XXX DAVOR"<<std::endl;
      for( unsigned int i = 0 ; i < block.size() ; i++ ){//durchlaufe den Blockstatus
        if( block.at(i).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
          std::cout<<" XXX DANACH"<<std::endl;
           for( unsigned int j = 0 ; j < block.at(i).second.size() ; j++ ){//durchlaufe alle relevanten blöcke
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
      for( unsigned int k = 0 ; k < weichenstatus.size() ; k++ ){//durchlaufe weichenstatus
        if( weichenstatus.at(k).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
          for( unsigned int l = 0 ; l < weichenstatus.at(k).second.size() ; l++ ){//durchlaufe alle relevanten Weichen
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
        for( unsigned int m = 0 ; m < block.size() ; m++ ){//durchlaufe den Blockstatus
          if( block.at(m).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
        for( unsigned int n = 0 ; n < block.at(m).second.size() ; n++ ){//durchlaufe alle relevanten blöcke
          block.at(m).second.at(n)->setFreigabe( true );//Entriegeln des Blocks
          //jetzt sind alle Blöcke der FS wieder freigegeben
        }
          }
          //Die Schleife für die Namensuche läuft noch fertig
        }
        //WEICHEN
        for( unsigned int o = 0 ; o < weichenstatus.size() ; o++ ){//durchlaufe weichenstatus
          if( weichenstatus.at(o).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
        for( unsigned int p = 0 ; p < weichenstatus.at(o).second.size() ; p++ ){//durchlaufe alle relevanten Weichen
          weichenstatus.at(o).second.at(p).first->setVerriegelung( false );//und setzen der Entriegelung, weil nun die FS aufgelöst wird (stellen der Weiche ist egal...)
        }
        //jetzt sind auch die Weichen entriegelt
          }
          //Die Schleife für die Namensuche läuft noch fertig
        }
        //WSIGNALE - rotstellen aller WSignale im Falle eines Löschens der FS
        for( unsigned int z = 0 ; z < wsignaleV.size() ; z++ ){//durchlaufe alle möglich involvierten VS
          if( wsignaleV.at(z)->getRichtung( getS_id(), getZiel() ) >= 0 ){//wenn es genau diese Richtung gibt, setze inFS = false und signalisiere Halt , weil es keine FS mehr gibt
        //std::cout<<"setting infs false WSIGNALE..."<<std::endl;
        wsignaleV.at(z)->setinFS( false );
        //std::cout<<"vorsignalisiere Ziel S...-->halt erwarten, weil die FS gelöscht wird WSIGNALE"<<std::endl;
        wsignaleV.at(z)->setV_status( false );
          }
        }
        //VSIGNALE - alle vorwärts VS als inFS de-markieren und schon mal Ziel Signal vorsignalisieren!
        for( unsigned int i = 0 ; i < vorsignalV.size() ; i++ ){//durchlaufe alle möglich involvierten VS
          if( vorsignalV.at(i)->getRichtung( getS_id(), getZiel() ) >= 0 ){//wenn es genau diese Richtung gibt, setze inFS = false und signalisiere Halt erwarten, weil es keine FS mehr gibt
        //std::cout<<"setting infs false..."<<std::endl;
        vorsignalV.at(i)->setinFS( false );
        //std::cout<<"vorsignalisiere Ziel S...-->halt erwarten, weil die FS gelöscht wird"<<std::endl;
        vorsignalV.at(i)->setV_status( false );
          }
        }
        //HSIGNAL-Status wird jetzt gelöscht
        //Ziel ist ab jetzt nicht mehr aktuell
        s_status = false;
        hasHSZiel = false;
        changeColor();
        emit refreshStellwerkstechnik( getS_id() , false );//Liste in Stellwerksim aktualiesieren
        //VSIGNALE - jetzt kann auch noch evtl VS auf Halt erwarten gehen-->rückwirkend
        for( unsigned int i = 0 ; i < vorsignalR.size() ; i++ ){//durchlaufe alle VS rückwirkend und finde genau das VS, bei dem die aktuelle Richtung vorliegt und inFS true ist -->stelle das entsprechend auf Halt erwarten
          //std::cout<<"in v rück for schleife deleteFS"<<std::endl;
          if(vorsignalR.at(i)->getinFS() && vorsignalR.at(i)->isAktFS( getS_id() ) ){//finde das richtige VS
        //std::cout<<"setzte VS auf false (deleteFS)..."<<std::endl;
        vorsignalR.at(i)->setV_status( false );
          }
        }
        //jetzt soll auch der komplette Speicher gelöscht werden:
        speicher = false;
        speicheritems.first->setBrush(Qt::darkBlue);
        emit illuminateSpeicher(false,getZiel());
        //////
      }
  }
  //##########################################################################
  if(hasWSZiel){
      //::::Blockgeschichte ==========================
      for( unsigned int i = 0 ; i < blockZuRangier.size() ; i++ ){//durchlaufe den Blockstatus
        if( blockZuRangier.at(i).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
           for( unsigned int j = 0 ; j < blockZuRangier.at(i).second.size() ; j++ ){//durchlaufe alle relevanten blöcke
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
      for( unsigned int k = 0 ; k < weichenstatusZuRangier.size() ; k++ ){//durchlaufe weichenstatus
        if( weichenstatusZuRangier.at(k).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
          for( unsigned int l = 0 ; l < weichenstatusZuRangier.at(k).second.size() ; l++ ){//durchlaufe alle relevanten Weichen
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
        for( unsigned int m = 0 ; m < blockZuRangier.size() ; m++ ){//durchlaufe den Blockstatus
          if( blockZuRangier.at(m).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
        for( unsigned int n = 0 ; n < blockZuRangier.at(m).second.size() ; n++ ){//durchlaufe alle relevanten blöcke
          blockZuRangier.at(m).second.at(n)->setFreigabe( true );//Entriegeln des Blocks
          //jetzt sind alle Blöcke der FS wieder freigegeben
        }
          }
          //Die Schleife für die Namensuche läuft noch fertig
        }
        //WEICHEN
        for( unsigned int o = 0 ; o < weichenstatusZuRangier.size() ; o++ ){//durchlaufe weichenstatus
          if( weichenstatusZuRangier.at(o).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
        for( unsigned int p = 0 ; p < weichenstatusZuRangier.at(o).second.size() ; p++ ){//durchlaufe alle relevanten Weichen
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
       std::cout<<"HSignal:::I am clicked ..."<<std::endl;
       //leite weiter an den Clickmanager
       emit listened(this);

}

void HSignal::recieveSpeicher(bool sp, std::string str){
    std::cout<<"recieving Speicher..."<<std::endl;
    if(str.compare(getS_id()) == 0){//emit illuminate erreicht alle Slots, da in addBlock connected, hier wird geprüft, ob das HS gemeint ist
        std::cout<<"bin drin in recieve speicher if"<<std::endl;
        if(sp){//falls es sich um einen neuen Speicher handelt
            speicheritems.second->setBrush(Qt::yellow);
        }
        if(!sp){//falls es sich um eine Auflösung vom Speicher handelt
            std::cout<<"darken speicheritems"<<std::endl;
            speicheritems.second->setBrush(Qt::darkBlue);
        }
    }

}

void HSignal::processSpeicher(){
    //der Vorblock von diesem Signal hier ist auf belegstatus frei gegangen: woher kam die FS (welches Startsignal) und ist das in einem Speicher verwickelt?
    std::cout<<"PROCESS SPEICHER"<<std::endl;
    if( fromHS->getSpeicher() ){//ist das Startsignal mit Speicher versehen?
        //gehe zu Startsignal und lösche Speicher, ändere Farbe, stelle gespeicherte FS
        fromHS->setSpeicher(false);
        std::cout<<"hier in process SPeicher"<<std::endl;
        fromHS->darkenSpeicher(1);
        //außerdem lösche im speicherziel den Speichervermerk-Punkt
        fromHS->getSpeicherziel()->darkenSpeicher(2);
        //die gespeicherte FS einwählen
        std::cout<<"HHHHHHHH---------------HHHHHHHHHHH"<<std::endl;
        std::cout<<"#### Speicherstatus und ziel "<<fromHS->getS_id()<<" | "<<fromHS->getSpeicher()<<" | "<<fromHS->getSpeicherziel()->getS_id()<<std::endl;
        fromHS->showBlockALL();
        fromHS->setFahrt(fromHS->getSpeicherziel());
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

void HSignal::addHSignalitem(QGraphicsRectItem *schirm, QGraphicsRectItem *trag, QLabel *la, QPushButton *but, QGraphicsRectItem *speicherback, QGraphicsRectItem *speicherfront){
    //schirm und Träger zufügen
    hsignalitems << schirm;
    hsignalitems << trag;
    //Label
    beschriftung = la;
    QString qname = QString::fromStdString( s_id );//cast from std::string to QString
    beschriftung->setText(qname);
    QFont f( "Arial", 5, QFont::Bold);
    beschriftung->setFont(f);
    //und PushButton
    push = but;
    but->setFixedHeight(10);
    but->setFixedWidth(10);
    but->setStyleSheet("background-color: blue");
    //initialisierung, dass alle Farben stimmen
    changeColor();
    QObject::connect(push,SIGNAL(clicked()),this,SLOT(listenToFS()) );//Verknüpfung von PushButton und seinem Signal (für clickmanager)
    //Speicher Anzeiger
    speicherback->setBrush(Qt::darkBlue);
    speicherfront->setBrush(Qt::darkBlue);
    speicheritems.first = speicherfront;
    speicheritems.second = speicherback;

}
