/*
 * segments for railroad [- BLOCK.CXX -]
 **************************************************************************
 */
#include "Block.h"
#include <QDebug>
#include <QBrush>
#include <QTimer>
#include <WiringPi/wiringPi/wiringPi.h>
#include <WiringPi/wiringPi/mcp23017.h>

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
  changeColor();
}

void Block::setFreigabe( bool free ){
  freigabe = free;
  changeColor();
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
	  QString signalInQuestion = passiert.at(i).first;
	  // WSignal or HSignal? trigger zugpassiertW or zugpassiert
	  if(signalInQuestion == 'S'){
	    //emit zugpassiert(passiert.at(i).first);                   //connected to HSignal::zugpassiert/WSignal::zugpassiert
	    //QTimer::singleShot(1000, &myObject, &MyClass::mySlot);
	    QTimer::singleShot(2000, [signalInQuestion, this]() {
	      emit this->zugpassiert(signalInQuestion);
	    });
	    for(int e = 0; e < this->passiert.size(); e++){
	      qDebug()<<"__Block__: Zugpassiert of this: "<<this->getName()<<passiert.at(e).first<<passiert.at(e).second->getName();
	    }
	  }
	  if(passiert.at(i).first.at(0) == 'W'){
	    //emit zugpassiertW(passiert.at(i).first);                  //connected to WSignal::zugpassiertW
	    QTimer::singleShot(2000, [signalInQuestion, this]() {
	      emit this->zugpassiertW(signalInQuestion);
	    });
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
    return true;
  }
  return false;
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

void Block::showPassiert(){
  if(getHaspassiert()){
    qInfo()<<"***************************************************************************";
    qInfo()<<" passiert von "<<getName();
    for( QPair pair : passiert ){
      qInfo()<<"   "<<pair.first<<": -"<<pair.second->getName();
    }
    qInfo()<<"***************************************************************************";
  }
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
