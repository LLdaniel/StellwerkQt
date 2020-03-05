//*************************************************************************
//Block of control center [- BLOCK.CXX -]
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
  //better exception handling needed --> catch cases likea5 31 ...
  blockname = name;
}

void Block::setB_status( bool status ){
  if( counter == 2){//reset counter
    counter = 0;
  }
  counter++;//every state change will be registered and counted
  evaluateFreigabe();//evaluation if segments can be unlocked or not
  b_status = status;//change status
  changeColor();
}

void Block::setFreigabe( bool free ){
  freigabe = free;
  changeColor();
  for(  int i = 0 ; i < bus.size() ; i++ ){//close/open BU 
    bus.at(i)->setBU_status(free);
    //bus.at(i)->setFreigabe(free);//verriegelung des BÜs setzen, da diese Methode eig vornehmlich beim Verriegeln aufgerufen wird!-->in evaluateFreigabe wird ja direkt auf die Membervariable zugegriffen deprecated
  }
}

bool Block::evaluateFreigabe(){
  if( counter == 0){//do nothing and give a callback false
    //qDebug()<<"Block::Der Counter =0"<<getName();
    return false;
  }
  if( counter == 1 ){//Block is now occupied -> test if previous Block is also occupied and at the same time the signal in between shows green-->trigger zugpassiert
    //qDebug()<<"Block::Der Counter =1"<<getName();
    //Block becomes occupied: haspassiert? --> previous B_status? --> signal in between status?
    //trigger: signal of pair is checked if Hp1 --> Hp1? if(prevBlock=occupied)-->singalpassiert; Hp0? do nothing
    if( haspassiert ){
      //qDebug()<<"                                                                           "<<getName()<<" haspassiert=true";
      for( int i = 0; i < passiert.size(); i++){
	//qDebug()<<"size von passiert: "<<passiert.size();
	//qDebug()<<"hola, was haben wir denn da?: "<<technik->getS_pass_status( passiert.at(i).first );
        if( technik->getS_pass_status( passiert.at(i).first ) && !passiert.at(i).second->getB_status() ){//is signal green? (get method of Stellwerkstechnik) && previous Block occupied
	  //normally: signal goes not immediately from green to red... maybe wait 2s?
	  //qDebug()<<"                                                                        Emitting zugpassiert in Block "<<getName();
	  //qDebug()<<"       ###############"<<passiert.at(i).first.at(0)<<"mit size"<<passiert.size();
	  if(passiert.at(i).first.at(0) == "S"){//if it is a WSignal oder HSignal:  trigger zugpassiertW or zugpassiert
	    //qDebug()<<"                  __BLOCK::__ zugpassiert für HS";
	    emit zugpassiert();//trigger zugpassiert aus (here it is Block::zugpassiert, which is connected to HSignal::zugpassiert
	  }
	  if(passiert.at(i).first.at(0) == "W"){//if it is a WSignal oder HSignal:  trigger zugpassiertW or zugpassiert
	    //qDebug()<<"                  __BLOCK::__ zugpassiert für WS";
	    emit zugpassiertW();//trigger zugpassiert aus (here it is Block::zugpassiert, which is connected to HSignal::zugpassiertlöse 
	  }
        }
      }
    }//ansonsten: nichts
    return false;
  }
  if(counter == 2 && !b_status){//setting Freigabe true 
    // Freigabengeschichte
    //qDebug()<<"Block::Der COUNTER = 2";
    freigabe = true;//here freigabe = true -->right cycle
    changeColor();
    for(  int i = 0 ; i < bus.size() ; i++ ){//changing status of BUs open/close
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
  for(  int i = 0 ; i < bus.size() ; i++ ){//loop over all all BUs
    qDebug()<<"***   "<<bus.at(i)->getName()<<"                                               ***";
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void Block::deleteBus( BU* todelete ){
  int remember = -1;
  //qDebug()<<"remember = -1";
  for(  int i = 0 ; i < bus.size() ; i++ ){//searching for BU to delete
    if( bus.at(i)->getName().compare( todelete->getName() ) == 0 ){//found!: remember position
      remember = i;
      //qDebug()<<"remember = "<<i;
    }
  }
  if( remember >= 0 ){//evaluate remembered value
    bus.erase( bus.begin() + remember );//delete
  }
}

void Block::addpassiert( QString grenzS, Block* prevBlock ){
  QPair<QString, Block*> hilfspair(grenzS,prevBlock);
  passiert.push_back(hilfspair);
  haspassiert = true;
  //qDebug()<<"haspassiert ist jetzt true! mit HSignal(Block)="<<grenzS<<"("<<prevBlock->getName()<<")";
}

void Block::deletepassiert(){
  haspassiert = false;
  passiert.clear();
}

void Block::changeColor(){
  for( int i = 0 ; i < blockitems.size() ; i++ ){
    if(b_status && freigabe){//if Block state free && and not occupied  
      blockitems.at(i)->setBrush(QColor(79,79,79));//rgb grey 79,79,79
    }
    if(b_status && !freigabe){//if Block state free && and locked
      blockitems.at(i)->setBrush(Qt::white);
    }
    if(!b_status){//if Block state occupied
      blockitems.at(i)->setBrush(Qt::red);
    }
  }

}

Block::~Block(){
  //delete technik; //it`s a pointer, yes, but should live outside block --> no delete: happens in main
}
