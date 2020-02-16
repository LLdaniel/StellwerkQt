//*************************************************************************
//Block des Stellwerks  [- BLOCK.CXX -]
//*************************************************************************
#include "Block.h"
#include <QDebug>
#include <QBrush>
//
Block::Block(QString name , Stellwerkstechnik *signaltechnik){
  setName( name );
  technik = signaltechnik;
}

void Block::setName( QString name ){
  //Bessere überprüfung noch nötig!-->Absicherung gegen zB. a5 31 ...
  blockname = name;
}

void Block::setB_status( bool status ){
  if( counter == 2){//resetten des Counters
    counter = 0;
  }
  counter++;//jede statusänderung wird registriert und entsprechend hochgezählt
  //bool test =
  evaluateFreigabe();//Die Freigeabe (evtl) wird errechnet
  b_status = status;//Jetzt erst wird der Status gesetzt
  //if(test && controlspeicher){qDebug()<<"<--TEST VAR"; emit releaseSpeicher(); }//siehe Anmerkung in Anmerkungsliste HSignal
  changeColor();
}

void Block::setFreigabe( bool free ){
  freigabe = free;
  changeColor();
  for(  int i = 0 ; i < bus.size() ; i++ ){//Jetzt wird der BU geschlossen/geöffnet
    bus.at(i)->setBU_status(free);
    //bus.at(i)->setFreigabe(free);//verriegelung des BÜs setzen, da diese Methode eig vornehmlich beim Verriegeln aufgerufen wird!-->in evaluateFreigabe wird ja direkt auf die Membervariable zugegriffen ---nicht mehr verwendet
  }
}

bool Block::evaluateFreigabe(){
  if( counter == 0){//zum debuggen
    qDebug()<<"Block::Der Counter =0"<<getName();
    return false;
  }
  if( counter == 1 ){//wenn der Block auf belegt geht, soll getestet werden, ob nicht ein vorheriger block auch belegt ist und ein signal grün ist-->also zugpassiert eingeleitet werden muss
    qDebug()<<"Block::Der Counter =1"<<getName();
    //Block geht auf belegt: haspassiert? --> vorheriger B_status dann? --> Grenzsignal status?
    //trigger: Signal im pair wird auf Hp1 überprüft --> Hp1? if(prevBlock=belegt)-->singalpassiert; Hp0? do nothing
    if( haspassiert ){
        qDebug()<<"                                                                           "<<getName()<<" haspassiert=true";
        for( int i = 0; i < passiert.size(); i++){
          qDebug()<<"size von passiert: "<<passiert.size();
            qDebug()<<"hola, was haben wir denn da?: "<<technik->getS_pass_status( passiert.at(i).first );
        if( technik->getS_pass_status( passiert.at(i).first ) && !passiert.at(i).second->getB_status() ){//falls Signal grün(get Methode auf Stellwerkstechnik) && vorheriger Block belegt
            //eigentlich sollte hier noch 2s gewartet werden
            qDebug()<<"                                                                        Emitting zugpassiert in Block "<<getName();
	    qDebug()<<"       ###############"<<passiert.at(i).first.at(0)<<"mit size"<<passiert.size();
            if(passiert.at(i).first.at(0) == "S"){//je nach dem, ob es ein WSignal oder HSignal ist, wird das entsprechende zugpassiert des jeweiligen Objekts aufgerufen
	      qDebug()<<"                  __BLOCK::__ zugpassiert für HS";
	      emit zugpassiert();//löse zugpassiert aus (hier erst mal Block::zugpassiert, was dann auf HSignal::zugpassiert geht
            }
            if(passiert.at(i).first.at(0) == "W"){//je nach dem, ob es ein WSignal oder HSignal ist, wird das entsprechende zugpassiert des jeweiligen Objekts aufgerufen
	      qDebug()<<"                  __BLOCK::__ zugpassiert für WS";
	      emit zugpassiertW();//löse zugpassiert aus (hier erst mal Block::zugpassiert, was dann auf WSignal::zugpassiertW geht
            }
        }
      }
     }//ansonsten: nichts
    return false;
  }
  if(counter == 2 && !b_status){//Hier wird dann die Freigabe auf true gesetzt und der FS speicher/eingewählt gelöscht
    // Freigabengeschichte
    qDebug()<<"Block::Der COUNTER = 2";
    freigabe = true;//hier freigabe = true -->richtige Zyklenfolge
    changeColor();
    for(  int i = 0 ; i < bus.size() ; i++ ){//Jetzt wird der BU geschlossen/geöffnet
      //bus.at(i)->setFreigabe(true);//zunächst entriegeln des BÜs --- nicht mehr verwendet
      bus.at(i)->setBU_status(true);
    }
    return true;
  }
  return false;
}

void Block::addBus( BU* bu ){
  //qDebug()<<"adding BÜs...";
  bus.push_back(bu);
}

void Block::showBus(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies sind die Bahnübergänge von Block      "<<getName()<<"        ***";
  for(  int i = 0 ; i < bus.size() ; i++ ){//Durchläuft alle BUs
    qDebug()<<"***   "<<bus.at(i)->getName()<<"                                               ***";
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void Block::deleteBus( BU* todelete ){
  int remember = -1;
  //qDebug()<<"remember = -1";
  for(  int i = 0 ; i < bus.size() ; i++ ){//Suche den zu Löschenden BÜ
    if( bus.at(i)->getName().compare( todelete->getName() ) == 0 ){//Wenn es gefunden wird, kann seine Position vermerkt werden
      remember = i;
      //qDebug()<<"remember = "<<i;
    }
  }
  if( remember >= 0 ){//nur wenn der zu löschende BÜ auch existiert, lösche ihn
    bus.erase( bus.begin() + remember );//direktes löschen
  }
}

void Block::addpassiert( QString grenzS, Block* prevBlock ){
  QPair<QString, Block*> hilfspair(grenzS,prevBlock);
  passiert.push_back(hilfspair);
  haspassiert = true;
  qDebug()<<"haspassiert ist jetzt true! mit HSignal(Block)="<<grenzS<<"("<<prevBlock->getName()<<")";
}

void Block::deletepassiert(){
    haspassiert = false;
    passiert.clear();
}

void Block::changeColor(){
    for( int i = 0 ; i < blockitems.size() ; i++ ){
        if(b_status && freigabe){//wenn der Blockstatus frei && nicht reserviert
           blockitems.at(i)->setBrush(QColor(79,79,79));//rgb grau 79,79,79
        }
        if(b_status && !freigabe){//wenn der Blockstatus frei && reserviert
           blockitems.at(i)->setBrush(Qt::white);
        }
        if(!b_status){//wenn der Blockstatus belegt
           blockitems.at(i)->setBrush(Qt::red);
        }
    }

}

Block::~Block(){
  //delete technik; //das ist eigentlich ein pointer, der außerhalb in main noch weiterleben muss, deshalb darf er hier nicht deleted werden --> darum kümmert sich dann main
}
