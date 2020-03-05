//*************************************************************************
//Rangiersignal des Stellwerks  [- WSIGNAL.CXX -]
//*************************************************************************
#include "WSignal.h"
#include <QDebug>


void WSignal::setV_id( int name ){
  QString suffix = QString::number( name );//name zu QString konvertieren
  if( name >0 && name <1000){//dreistelligkeit wird hier überprüft
    if( name < 10 ){//zwei Vornullen werden erzeugt
      v_id = "W00" + suffix;//std::to_string( name );
    }
    if( name < 100 && name > 9){//eine Vornull wird erzeugt
      v_id = "W0" + suffix;//std::to_string( name );
    }
    if( name < 1000 && name > 99){//keine Vornull mehr nötig
      v_id = "W" + suffix;//std::to_string( name );
    }
  }
  else qDebug()<<"WSignal:Falsche Benennung. Die Zahl muss dreistellig sein.";
}

void WSignal::setFahrt( WSignal *toZiel ){
  //bool zur Kontrolle, ob FS freigegeben werden darf
  bool blockfreigabe = true;//verriegelt wäre false-->suche solange es entriegelte Blöcke gibt
  bool blockkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  bool weichenverr = false;//verriegelt wäre true-->suche solange es entriegelte Weichen gibt
  bool weichenkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  if( getV_id().compare(toZiel->getV_id() ) != 0 && isNachbar(toZiel) ){//prüfe, ob Zielsignal Nachbar ist und ob Zielsignal nicht das selbe Signal ist wie das Startsignal
    //::::Blockgeschichte ==========================
    for(  int i = 0 ; i < block.size() ; i++ ){//durchlaufe den Blockstatus
      if( block.at(i).first.compare( toZiel->getV_id() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
	for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){//durchlaufe alle relevanten blöcke
	  if( !block.at(i).second.at(j)->getB_status() ){//betrachte alle Belegtstatus [falls belegt]
	    blockkontrolle = false;//flag auf false setzten
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
      if( weichenstatus.at(k).first.compare( toZiel->getV_id() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
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
    if( blockgeschichte && weichengeschichte ){//Erlaubnis ist gegeben-->Stellen
      //BLOCK
      for(  int m = 0 ; m < block.size() ; m++ ){//durchlaufe den Blockstatus
	if( block.at(m).first.compare( toZiel->getV_id() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
	  for(  int n = 0 ; n < block.at(m).second.size() ; n++ ){//durchlaufe alle relevanten blöcke
	    block.at(m).second.at(n)->setFreigabe( false );//Verriegeln des Blocks
	    //jetzt sind alle Blöcke der FS reserviert
	  }
	}
	//Die Schleife für die Namensuche läuft noch fertig
      }
      //WEICHEN
      for(  int o = 0 ; o < weichenstatus.size() ; o++ ){//durchlaufe weichenstatus
	if( weichenstatus.at(o).first.compare( toZiel->getV_id() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
	  for(  int p = 0 ; p < weichenstatus.at(o).second.size() ; p++ ){//durchlaufe alle relevanten Weichen
	    weichenstatus.at(o).second.at(p).first->setW_status(weichenstatus.at(o).second.at(p).second);//setze die Weiche für die betreffende FS auf den Sollwert
	    weichenstatus.at(o).second.at(p).first->setVerriegelung( true );//und setzen der Verriegelung, weil nun die FS besteht
	  }
	  //jetzt sind auch die Weichen gestellt und reserviert
	}
	//Die Schleife für die Namensuche läuft noch fertig
      }
      //WSIGNAL-Status wird jetzt auf Fahrt gesetzt
      setZiel( toZiel->getV_id() );
      v_status = true;
      hasWSZiel = true;
      changeColor();
      emit refreshStellwerkstechnikW(getV_id(), true);
      qDebug()<<"Hallo aus WS, emitting refreshStellwerkstechnikW...";
      //////
    }
  }
}

void WSignal::setFahrt(QString toZiel) // WS to HS
{
  //bool zur Kontrolle, ob FS freigegeben werden darf
  bool blockfreigabe = true;//verriegelt wäre false-->suche solange es entriegelte Blöcke gibt
  bool blockkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  bool weichenverr = false;//verriegelt wäre true-->suche solange es entriegelte Weichen gibt
  bool weichenkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  if( getV_id().compare(toZiel ) != 0 && isNachbar(toZiel) ){//prüfe, ob Zielsignal Nachbar ist und ob Zielsignal nicht das selbe Signal ist wie das Startsignal
    //::::Blockgeschichte ==========================
    for(  int i = 0 ; i < blockZuH.size() ; i++ ){//durchlaufe den Blockstatus
      if( blockZuH.at(i).first.compare( toZiel ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
        for(  int j = 0 ; j < blockZuH.at(i).second.size() ; j++ ){//durchlaufe alle relevanten blöcke
	  if( !blockZuH.at(i).second.at(j)->getB_status() ){//betrachte alle Belegtstatus [falls belegt]
	    blockkontrolle = false;//flag auf false setzten
	    break;
	  }
	  if( !blockZuH.at(i).second.at(j)->getFreigabe() ){//betrachte alle Belegtstatus [falls verriegelt]
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
    for(  int k = 0 ; k < weichenstatusZuH.size() ; k++ ){//durchlaufe weichenstatus
      if( weichenstatusZuH.at(k).first.compare( toZiel ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
        for(  int l = 0 ; l < weichenstatusZuH.at(k).second.size() ; l++ ){//durchlaufe alle relevanten Weichen
	  if( !weichenstatusZuH.at(k).second.at(l).first->getBelegung() ){//betrachte alle Belegtstatus [falls belegt]
	    weichenkontrolle = false;//flag auf false setzen
	    break;
	  }
	  if( weichenstatusZuH.at(k).second.at(l).first->getVerriegelung() ){//betrachte Verriegelung [falls verriegelt = true]
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
      for(  int m = 0 ; m < blockZuH.size() ; m++ ){//durchlaufe den Blockstatus
        if( blockZuH.at(m).first.compare( toZiel ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
	  for(  int n = 0 ; n < blockZuH.at(m).second.size() ; n++ ){//durchlaufe alle relevanten blöcke
	    blockZuH.at(m).second.at(n)->setFreigabe( false );//Verriegeln des Blocks
	    //jetzt sind alle Blöcke der FS reserviert
	  }
        }
        //Die Schleife für die Namensuche läuft noch fertig
      }
      //WEICHEN
      for(  int o = 0 ; o < weichenstatusZuH.size() ; o++ ){//durchlaufe weichenstatus
        if( weichenstatusZuH.at(o).first.compare( toZiel ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
	  for(  int p = 0 ; p < weichenstatusZuH.at(o).second.size() ; p++ ){//durchlaufe alle relevanten Weichen
	    weichenstatusZuH.at(o).second.at(p).first->setW_status(weichenstatusZuH.at(o).second.at(p).second);//setze die Weiche für die betreffende FS auf den Sollwert
	    weichenstatusZuH.at(o).second.at(p).first->setVerriegelung( true );//und setzen der Verriegelung, weil nun die FS besteht
	  }
	  //jetzt sind auch die Weichen gestellt und reserviert
        }
        //Die Schleife für die Namensuche läuft noch fertig
      }
      //WSIGNAL-Status wird jetzt auf Fahrt gesetzt
      v_status = true;
      setZiel(toZiel);
      hasHSZiel = true;
      changeColor();
      qDebug()<<"---->emitting refresh stellwerkstec in WSignal";
      emit refreshStellwerkstechnikW(getV_id(),true);
      //////
    }
  }
}

void WSignal::deleteFS(){
  //bool zur Kontrolle, ob FS gelöscht werden darf (alles muss verriegelt und unbelegt sein)
  bool blockfreigabe = false;//entriegelt wäre true-->suche solange es verriegelte Blöcke gibt
  bool blockkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  bool weichenverr = true;//entriegelt wäre false-->suche solange es verriegelte Weichen gibt
  bool weichenkontrolle = true;//belegt wäre false-->suche solange es freie Blöcke gibt
  //nun große Unterscheidung, ob deleteFS von einer WS->WS oder einer WS->HS
  //##########################################################################
  if(hasWSZiel){
    //::::Blockgeschichte ==========================
    for(  int i = 0 ; i < block.size() ; i++ ){//durchlaufe den Blockstatus
      if( block.at(i).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
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
      //WSIGNAL-Status wird jetzt gelöscht
      //Ziel ist ab jetzt nicht mehr aktuell
      v_status = false;
      hasWSZiel = false;
      changeColor();
      emit refreshStellwerkstechnikW(getV_id(), false);
      //////
    }
  }
  if(hasHSZiel){
    //::::Blockgeschichte ==========================
    for(  int i = 0 ; i < blockZuH.size() ; i++ ){//durchlaufe den Blockstatus
      if( blockZuH.at(i).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
	for(  int j = 0 ; j < blockZuH.at(i).second.size() ; j++ ){//durchlaufe alle relevanten blöcke
	  if( !blockZuH.at(i).second.at(j)->getB_status() ){//betrachte alle Belegtstatus [falls belegt]
	    blockkontrolle = false;//flag auf false setzten
	    break;
	  }
	  if( blockZuH.at(i).second.at(j)->getFreigabe() ){//betrachte alle Belegtstatus [falls entriegelt]
	    blockfreigabe = true;//flag auf true setzen
	    break;
	  }
	  //wenn die Schleife komplett durchlaufen wird, dann stehen blockfreigabe = false und blockkontrolle auf = true -->also nichts belegt aber verriegelt
	}
      }
      //Die Schleife für die Namensuche läuft noch fertig
    }
    //::::Weichengeschichte ========================
    for(  int k = 0 ; k < weichenstatusZuH.size() ; k++ ){//durchlaufe weichenstatus
      if( weichenstatusZuH.at(k).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
	for(  int l = 0 ; l < weichenstatusZuH.at(k).second.size() ; l++ ){//durchlaufe alle relevanten Weichen
	  if( !weichenstatusZuH.at(k).second.at(l).first->getBelegung() ){//betrachte alle Belegtstatus [falls belegt]
	    weichenkontrolle = false;//flag auf false setzen
	    break;
	  }
	  if( !weichenstatusZuH.at(k).second.at(l).first->getVerriegelung() ){//betrachte Verriegelung [falls entriegelt = false]
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
      for(  int m = 0 ; m < blockZuH.size() ; m++ ){//durchlaufe den Blockstatus
	if( blockZuH.at(m).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Blockstatus gefunden ist
	  for(  int n = 0 ; n < blockZuH.at(m).second.size() ; n++ ){//durchlaufe alle relevanten blöcke
	    blockZuH.at(m).second.at(n)->setFreigabe( true );//Entriegeln des Blocks
	    //jetzt sind alle Blöcke der FS wieder freigegeben
	  }
	}
	//Die Schleife für die Namensuche läuft noch fertig
      }
      //WEICHEN
      for(  int o = 0 ; o < weichenstatusZuH.size() ; o++ ){//durchlaufe weichenstatus
	if( weichenstatusZuH.at(o).first.compare( getZiel() ) == 0 ){//Wenn das Zielsignal im Weichenstatus gefunden ist
	  for(  int p = 0 ; p < weichenstatusZuH.at(o).second.size() ; p++ ){//durchlaufe alle relevanten Weichen
	    weichenstatusZuH.at(o).second.at(p).first->setVerriegelung( false );//und setzen der Entriegelung, weil nun die FS aufgelöst wird (stellen der Weiche ist egal...)
	  }
	  //jetzt sind auch die Weichen entriegelt
	}
	//Die Schleife für die Namensuche läuft noch fertig
      }
      //WSIGNAL-Status wird jetzt gelöscht
      //Ziel ist ab jetzt nicht mehr aktuell
      v_status = false;
      hasWSZiel = false;
      changeColor();
      emit refreshStellwerkstechnikW(getV_id(), false);
      //////
    }
  }
}

void WSignal::addWeichenstatus( WSignal *toZiel , QList<QPair<Weiche* , bool>> weichenpair ){
  QPair<QString, QList<QPair<Weiche* , bool>> > hilfspair( toZiel->getV_id() , weichenpair );//Das Hilfspair wird erzeugt, string wird über das Signal selbst erzeugt
  weichenstatus.push_back(hilfspair);
}

void WSignal::addWeichenstatusZuH( QString toZiel , QList<QPair<Weiche* , bool>> weichenpair ){
  QPair<QString, QList<QPair<Weiche* , bool>> > hilfspair( toZiel , weichenpair );//Das Hilfspair wird erzeugt, string wird über das Signal selbst erzeugt
  weichenstatusZuH.push_back(hilfspair);
}

void WSignal::showWeichenstatusALL(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies ist der Weichenstatus für Rangiersignal"<<getV_id()<<"     ***";
  for(  int i = 0 ; i < weichenstatus.size() ; i++){
    qDebug()<<"***   "<<weichenstatus.at(i).first<<" :                                             ***";
    for(  int j = 0 ; j < weichenstatus.at(i).second.size() ; j++ ){//hier sind die Weichenstatuspaarungen
      qDebug()<<"***               "<<weichenstatus.at(i).second.at(j).first->getW_id()<<"   -->   "<<weichenstatus.at(i).second.at(j).second<<"                          ***";
    }
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void WSignal::showWeichenstatus( WSignal *whichZiel ){
  for(  int i = 0 ; i < weichenstatus.size() ; i++){
    if( weichenstatus.at(i).first.compare( whichZiel->getV_id() ) == 0 ){//falls das gesuchte Signal zum Signal in der Liste passt, durchlaufe die Weichenstatuspaarungen
      qDebug()<<"";
      qDebug()<<"************************************************************";
      qDebug()<<"*** Dies ist der Weichenstatus für Rangiersignal"<<getV_id()<<"      ***";
      qDebug()<<"***   "<<weichenstatus.at(i).first<<" :                                             ***";
      for(  int j = 0 ; j < weichenstatus.at(i).second.size() ; j++ ){//hier sind die Weichenstatuspaarungen
	qDebug()<<"***               "<<weichenstatus.at(i).second.at(j).first->getW_id()<<"   -->   "<<weichenstatus.at(i).second.at(j).second<<"                          ***";
      }
      qDebug()<<"************************************************************";
      qDebug()<<"";
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
    if( block.at(i).first.compare( whichZiel->getV_id() ) == 0){//Wenn das gesuchte Nachbarsignal gefunden wurde-->Schleife über die Blöcke
      qDebug()<<"";
      qDebug()<<"************************************************************";
      qDebug()<<"*** Dies sind die Blöcke für Hauptsignal "<<getV_id()<<"            ***";
      qDebug()<<"***   "<<block.at(i).first<<" :                                             ***";
      for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){//hier sind die Blöcke
	qDebug()<<"***               "<<block.at(i).second.at(j)->getName()<<"                                     ***";
      }
      qDebug()<<"************************************************************";
      qDebug()<<"";
    }
  }
}

void WSignal::showBlockALL(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies sind die Blöcke für Hauptsignal "<<getV_id()<<"            ***";
  for(  int i = 0 ; i < block.size() ; i++){
    qDebug()<<"***   "<<block.at(i).first<<" :                                             ***";
    for(  int j = 0 ; j < block.at(i).second.size() ; j++ ){//hier sind die Weichenstatuspaarungen
      qDebug()<<"***               "<<block.at(i).second.at(j)->getName()<<"                                     ***";
    }
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
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
  for(  int i = 0; i < blockZuH.size(); i++){
    if(blockZuH.at(i).first.compare( toZiel ) == 0){
      giveback = true;
    }
  }
  return giveback;
}

void WSignal::deleteNachbar( WSignal *todelete ){
  //delete meint, sowohl in Weichenstatus, als auch im Block
  //Weichenstatus:
  int remember = -1;
  for(  int i = 0 ; i < weichenstatus.size() ; i++ ){
    if( weichenstatus.at(i).first.compare( todelete->getV_id() ) == 0 ){//falls der zu löschende Status gefunden wurde: Löschauftrag
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
    if( block.at(j).first.compare( todelete->getV_id() ) == 0 ){//falls der zu löschende Status gefunden wurde: Löschauftrag
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

void WSignal::addButtonAndLabel(QLabel *lab, QPushButton *but){
  //Label
  beschriftung = lab;
  QString qname = v_id;
  beschriftung->setText(qname);
  QFont f( "Arial", 10, QFont::Bold);
  beschriftung->setFont(f);
  //und PushButton
  push = but;
  but->setFixedHeight(20);
  but->setFixedWidth(20);
  but->setStyleSheet("background-color: blue");
  QObject::connect(push,&QPushButton::clicked,this,&WSignal::listenToFS);
  //QObject::connect(push,SIGNAL(clicked()),this,SLOT(listenToFS()) );//Verknüpfung von PushButton und seinem Signal (für clickmanager)
}

void WSignal::listenToFS(){
  qDebug()<<"WSignal:::I amclicked ...";
  //leite weiter an den Clickmanager
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
  if(zziel.at(0) == 'S'){//es ist ein HS
    hasHSZiel = true;
    ziel = zziel;
  }
  if(zziel.at(0) == 'W'){
    hasWSZiel = true;
    ziel = zziel;
  }
}

WSignal::~WSignal(){
  delete beschriftung;
  delete push;
}
