/*
 * segments for railroad [- BLOCK.CXX -]
 **************************************************************************
 */
#include "Block.h"
#include <QDebug>
#include <QBrush>
#include <wiringPi.h>
#include <mcp23017.h>

Block::Block(QString name , Stellwerkstechnik *signaltechnik, Configuration *config){
  setName( name );
  technik = signaltechnik;
  configuration = config;
}

void Block::setName( QString name ){
  blockname = name;
}

void Block::setB_status( bool status ){
  if( counter == 2){                            //reset counter
    counter = 0;
  }
  counter++;                                     //every state change will be registered and counted
  evaluateFreigabe();                            //evaluation if segments can be unlocked or not
  b_status = status;                             //change status

  // close BU when occupying segments - comment in to change back to reservation
  if( !status ){
    for(  int i = 0 ; i < bus.size() ; i++ ){      //close/open BU 
      bus.at(i)->setBU_status(false);
    }
  }
  changeColor();
}

void Block::setFreigabe( bool free ){
  freigabe = free;
  changeColor();
  /* closing BU when reserving segments - uncomment to change
    for(  int i = 0 ; i < bus.size() ; i++ ){      //close/open BU 
    bus.at(i)->setBU_status(free);
    }*/
}

bool Block::evaluateFreigabe(){
  if( counter == 0){                              //do nothing and give a callback false
    return false;
  }
  if( counter == 1 ){                             //Block is now occupied -> test if previous Block is also occupied and at the same time the signal in between shows green-->trigger zugpassiert
    //Block becomes occupied: haspassiert? --> previous B_status? --> signal in between status?
    //trigger: signal of pair is checked if Hp1 --> Hp1? if(prevBlock=occupied)-->singalpassiert; Hp0? do nothing
    if( haspassiert ){
      for( int i = 0; i < passiert.size(); i++){
        if( technik->getS_pass_status( passiert.at(i).first ) && !passiert.at(i).second->getB_status() ){                //is signal green? (get method of Stellwerkstechnik) && previous Block occupied
	  // WSignal or HSignal? trigger zugpassiertW or zugpassiert
	  if(passiert.at(i).first.at(0) == 'S'){
	    emit zugpassiert();                   //connected to HSignal::zugpassiert/WSignal::zugpassiert
	    for(int e = 0; e < this->passiert.size(); e++){
	      qDebug()<<this->getName()<<passiert.at(e).first<<passiert.at(e).second->getName();
	    }
	  }
	  if(passiert.at(i).first.at(0) == 'W'){
	    emit zugpassiertW();                  //connected to WSignal::zugpassiertW 
	  }
        }
      }
    }                                             //otherwise: do nothing
    return false;
  }
  // setting Freigabe 
  if(counter == 2 && !b_status){
    freigabe = true;                              //here freigabe = true -->right cycle
    changeColor();
    for(  int i = 0 ; i < bus.size() ; i++ ){     //changing status of BUs open/close
      bus.at(i)->setBU_status(true);
    }
    return true;
  }
  return false;
}

void Block::addBus( BU* bu ){
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
  for(  int i = 0 ; i < bus.size() ; i++ ){       //searching for BU to delete
    if( bus.at(i)->getName().compare( todelete->getName() ) == 0 ){//found!: remember position
      remember = i;
    }
  }
  if( remember >= 0 ){                            //evaluate remembered value
    bus.erase( bus.begin() + remember );          //delete
  }
}

void Block::addpassiert( QString grenzS, Block* prevBlock ){
  QPair<QString, Block*> hilfspair(grenzS,prevBlock);
  passiert.push_back(hilfspair);
  haspassiert = true;
}

void Block::deletepassiert(){
  haspassiert = false;
  passiert.clear();
}

void Block::changeColor(){
  for( int i = 0 ; i < blockitems.size() ; i++ ){
    if(b_status && freigabe){                      //if Block state free && and not occupied  
      blockitems.at(i)->setBrush(QColor(79,79,79));//rgb grey 79,79,79
    }
    if(b_status && !freigabe){                     //if Block state free && and locked
      blockitems.at(i)->setBrush(Qt::white);
    }
    if(!b_status){                                 //if Block state occupied
      blockitems.at(i)->setBrush(Qt::red);
    }
  }

}

void Block::setGpio(int blockpin){
  pin = blockpin;
}

int Block::getGpio(){
  return pin;
}

int Block::readBlock(){
  if( configuration->getWithHardware()){
    return digitalRead(pin);
  }
  else return 1;
}

Block::~Block(){
  //delete technik;                                //it`s a pointer, yes, but should live outside block --> no delete: happens in main
}
