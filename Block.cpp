//*************************************************************************
//Block des Stellwerks  [- BLOCK.CXX -]
//*************************************************************************
#include "Block.h"
#include <iostream>
#include <QBrush>
//
Block::Block(std::string name , Stellwerkstechnik *signaltechnik){
  setName( name );
  technik = signaltechnik;
}

void Block::setName( std::string name ){
  //Bessere überprüfung noch nötig!-->Absicherung gegen zB. a5 31 ...
  if( name.length() == 2){//ist schon mal zweistellig
    bool kontrolle1 = false;
    bool kontrolle2 = false;
    switch( name[0] ){
    case 'a' : kontrolle1 = true; break;
    case 'b' : kontrolle1 = true; break;
    case 'c' : kontrolle1 = true; break;
    case 'd' : kontrolle1 = true; break;
    case 'e' : kontrolle1 = true; break;
    case 'f' : kontrolle1 = true; break;
    case 'g' : kontrolle1 = true; break;
    case 'h' : kontrolle1 = true; break;
    case 'i' : kontrolle1 = true; break;
    case 'j' : kontrolle1 = true; break;
    case 'k' : kontrolle1 = true; break;
    case 'l' : kontrolle1 = true; break;
    case 'm' : kontrolle1 = true; break;
    case 'n' : kontrolle1 = true; break;
    case 'o' : kontrolle1 = true; break;
    case 'p' : kontrolle1 = true; break;
    case 'q' : kontrolle1 = true; break;
    case 'r' : kontrolle1 = true; break;
    case 's' : kontrolle1 = true; break;
    case 't' : kontrolle1 = true; break;
    case 'u' : kontrolle1 = true; break;
    case 'v' : kontrolle1 = true; break;
    case 'w' : kontrolle1 = true; break;
    case 'x' : kontrolle1 = true; break;
    case 'y' : kontrolle1 = true; break;
    case 'z' : kontrolle1 = true; break;
    }
    switch( name[1] ){
    case 'a' : kontrolle2 = true; break;
    case 'b' : kontrolle2 = true; break;
    case 'c' : kontrolle2 = true; break;
    case 'd' : kontrolle2 = true; break;
    case 'e' : kontrolle2 = true; break;
    case 'f' : kontrolle2 = true; break;
    case 'g' : kontrolle2 = true; break;
    case 'h' : kontrolle2 = true; break;
    case 'i' : kontrolle2 = true; break;
    case 'j' : kontrolle2 = true; break;
    case 'k' : kontrolle2 = true; break;
    case 'l' : kontrolle2 = true; break;
    case 'm' : kontrolle2 = true; break;
    case 'n' : kontrolle2 = true; break;
    case 'o' : kontrolle2 = true; break;
    case 'p' : kontrolle2 = true; break;
    case 'q' : kontrolle2 = true; break;
    case 'r' : kontrolle2 = true; break;
    case 's' : kontrolle2 = true; break;
    case 't' : kontrolle2 = true; break;
    case 'u' : kontrolle2 = true; break;
    case 'v' : kontrolle2 = true; break;
    case 'w' : kontrolle2 = true; break;
    case 'x' : kontrolle2 = true; break;
    case 'y' : kontrolle2 = true; break;
    case 'z' : kontrolle2 = true; break;
    }
    if( kontrolle1 && kontrolle2 ){//nur aplphabetisch ist erlaubt!
      blockname = name;
    }
    else{
      std::cout<<"Block::Falsche Benennung: Kleinbuchstaben aus dem Alphabet nötig."<<std::endl;
    }
  }
  else{ std::cout<<"Block::Falsche Benennung: zwei Kleinbuchstaben aus dem Alphabet nötig."<<std::endl;
  }
}

void Block::setB_status( bool status ){
  if( counter == 2){//resetten des Counters
    counter = 0;
  }
  counter++;//jede statusänderung wird registriert und entsprechend hochgezählt
  bool test;
  test = evaluateFreigabe();//Die Freigeabe (evtl) wird errechnet
  b_status = status;//Jetzt erst wird der Status gesetzt
  //if(test && controlspeicher){std::cout<<"<--TEST VAR"<<std::endl; emit releaseSpeicher(); }//siehe Anmerkung in Anmerkungsliste HSignal
  changeColor();
}

void Block::setFreigabe( bool free ){
  freigabe = free;
  changeColor();
  for( unsigned int i = 0 ; i < bus.size() ; i++ ){//Jetzt wird der BU geschlossen/geöffnet
    bus.at(i)->setBU_status(free);
    //bus.at(i)->setFreigabe(free);//verriegelung des BÜs setzen, da diese Methode eig vornehmlich beim Verriegeln aufgerufen wird!-->in evaluateFreigabe wird ja direkt auf die Membervariable zugegriffen ---nicht mehr verwendet
  }
}

bool Block::evaluateFreigabe(){
  if( counter == 0){//zum debuggen
    std::cout<<"Block::Der Counter =0"<<getName()<<std::endl;
    return false;
  }
  if( counter == 1 ){//wenn der Block auf belegt geht, soll getestet werden, ob nicht ein vorheriger block auch belegt ist und ein signal grün ist-->also zugpassiert eingeleitet werden muss
    std::cout<<"Block::Der Counter =1"<<getName()<<std::endl;
    //Block geht auf belegt: haspassiert? --> vorheriger B_status dann? --> Grenzsignal status?
    //trigger: Signal im pair wird auf Hp1 überprüft --> Hp1? if(prevBlock=belegt)-->singalpassiert; Hp0? do nothing
    if( haspassiert ){
        std::cout<<"                                                                           "<<getName()<<" haspassiert=true"<<std::endl;
        for(unsigned int i = 0; i < passiert.size(); i++){
          std::cout<<"size von passiert: "<<passiert.size()<<std::endl;
            std::cout<<"hola, was haben wir denn da?: "<<technik->getS_pass_status( passiert.at(i).first )<<std::endl;
        if( technik->getS_pass_status( passiert.at(i).first ) && !passiert.at(i).second->getB_status() ){//falls Signal grün(get Methode auf Stellwerkstechnik) && vorheriger Block belegt
            //eigentlich sollte hier noch 2s gewartet werden
            std::cout<<"                                                                        Emitting zugpassiert in Block "<<getName()<<std::endl;
            if(passiert.at(i).first.at(0) == 'S'){//je nach dem, ob es ein WSignal oder HSignal ist, wird das entsprechende zugpassiert des jeweiligen Objekts aufgerufen
                emit zugpassiert();//löse zugpassiert aus (hier erst mal Block::zugpassiert, was dann auf HSignal::zugpassiert geht
            }
            if(passiert.at(i).first.at(0) == 'W'){//je nach dem, ob es ein WSignal oder HSignal ist, wird das entsprechende zugpassiert des jeweiligen Objekts aufgerufen
                emit zugpassiertW();//löse zugpassiert aus (hier erst mal Block::zugpassiert, was dann auf WSignal::zugpassiertW geht
            }
        }
      }
     }//ansonsten: nichts
    return false;
  }
  if(counter == 2 && !b_status){//Hier wird dann die Freigabe auf true gesetzt und der FS speicher/eingewählt gelöscht
    // Freigabengeschichte
    std::cout<<"Block::Der COUNTER = 2"<<std::endl;
    freigabe = true;//hier freigabe = true -->richtige Zyklenfolge
    changeColor();
    for( unsigned int i = 0 ; i < bus.size() ; i++ ){//Jetzt wird der BU geschlossen/geöffnet
      //bus.at(i)->setFreigabe(true);//zunächst entriegeln des BÜs --- nicht mehr verwendet
      bus.at(i)->setBU_status(true);
    }
    return true;
  }
  return false;
}

void Block::addBus( BU* bu ){
  //std::cout<<"adding BÜs..."<<std::endl;
  bus.push_back(bu);
}

void Block::showBus(){
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<"*** Dies sind die Bahnübergänge von Block      "<<getName()<<"        ***"<<std::endl;
  for( unsigned int i = 0 ; i < bus.size() ; i++ ){//Durchläuft alle BUs
    std::cout<<"***   "<<bus.at(i)->getName()<<"                                               ***"<<std::endl;
  }
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<""<<std::endl;
}

void Block::deleteBus( BU* todelete ){
  int remember = -1;
  //std::cout<<"remember = -1"<<std::endl;
  for( unsigned int i = 0 ; i < bus.size() ; i++ ){//Suche den zu Löschenden BÜ
    if( bus.at(i)->getName().compare( todelete->getName() ) == 0 ){//Wenn es gefunden wird, kann seine Position vermerkt werden
      remember = i;
      //std::cout<<"remember = "<<i<<std::endl;
    }
  }
  if( remember >= 0 ){//nur wenn der zu löschende BÜ auch existiert, lösche ihn
    bus.erase( bus.begin() + remember );//direktes löschen
  }
}

void Block::addpassiert( std::string grenzS, Block* prevBlock ){
  std::pair<std::string,Block*> hilfspair(grenzS,prevBlock);
  passiert.push_back(hilfspair);
  haspassiert = true;
  std::cout<<"haspassiert ist jetzt true! mit HSignal(Block)="<<grenzS<<"("<<prevBlock->getName()<<")"<<std::endl;
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

