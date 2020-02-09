#include "worker.h"
#include <iostream>
#include <wiringPi.h>
worker::worker( std::vector<Block*> allBlocks, std::vector<int> allpins ){
  blocklist = allBlocks;
  pins = allpins;
}

worker::~worker(){

}

void worker::addBlockList( Block* moreBlocks ){
  blocklist.push_back( moreBlocks );
}

void worker::showBlocks(){
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<"*** Dies sind die Blöcke, die ständig überwacht werden   ***"<<std::endl;
  for( unsigned int i = 0 ; i < blocklist.size() ; i++ ){//Durchläuft alle update Blocks
    std::cout<<"***   "<<blocklist.at(i)->getName()<<"                                               ***"<<std::endl;
  }
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<""<<std::endl;
}

void worker::addPinList( int morePins ){
  pins.push_back( morePins );
}

void worker::showPins(){
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<"*** Dies sind alle Input-Pins                            ***"<<std::endl;
  for( unsigned int i = 0 ; i < pins.size() ; i++ ){//Durchläuft alle pins
    std::cout<<"***   "<<pins.at(i)<<"                                               ***"<<std::endl;
  }
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<""<<std::endl;
}

void worker::quit(){
  update = false;
}

void worker::updateBelegt(){
  unsigned int i = 0;
  int aktuell = -1;
  while( update and pins.size() > 0){
    //genaue Zuordung muss noch geklärt werden!! hier wäre es aktuell so, dass die pin und blockreihenfolge die zugehörige map ist... at(i) pin --> at(i) block
    aktuell = digitalRead(pins.at(i));
    if( aktuell == 0 ){ //read pins false = belegt || true = frei dagegen ist 0 = LOW , 1 = HIGH
      blocklist.at(i)->setB_status(true);
    }
    if( aktuell == 1 ){ //read pins
      blocklist.at(i)->setB_status(false);
    }
    //gehe weiter in der Liste der Blöcke, und teste die, ansonsten von vorne -- vllt eher mit interrupt
    if( i < pins.size() ){
      i++;
    }
    else i = 0; //ansonsten: reset
  }
}
