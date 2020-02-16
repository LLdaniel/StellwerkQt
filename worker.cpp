#include "worker.h"
#include <QDebug>
#include <wiringPi.h>
worker::worker( QList<QPair<int,Block*>> allBlocks, QList<QPair<int,Weiche*>> allWeichen ){
  blocklist = allBlocks;
  weichenlist = allWeichen;
}

worker::~worker(){

}

void worker::addBlockList( int morePins, Block* moreBlocks ){
  QPair<int,Block*> hilfspair = qMakePair(morePins,moreBlocks);
  blocklist.push_back( hilfspair );
}

void worker::showBlocks(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies sind die Blöcke, die ständig überwacht werden   ***";
  for(  int i = 0 ; i < blocklist.size() ; i++ ){//Durchläuft alle update Blocks
    qDebug()<<"***   "<<blocklist.at(i).first<<" -> "<<blocklist.at(i).second->getName()<<"                                           ***";
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void worker::showWeichen(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies sind die Blöcke, die ständig überwacht werden   ***";
  for(  int i = 0 ; i < blocklist.size() ; i++ ){//Durchläuft alle update Blocks
    qDebug()<<"***   "<<weichenlist.at(i).first<<" -> "<<weichenlist.at(i).second->getW_id()<<"                                           ***";
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void worker::addWeichenList( int morePins, Weiche* moreWeichen ){
  QPair<int,Weiche*> hilfspair = qMakePair(morePins,moreWeichen);
  weichenlist.push_back( hilfspair );
}

void worker::quit(){
  update = false;
}

void worker::updateBelegt(){
  int i = 0;
  int aktuellBlock = -1;
  int aktuellWeiche = -1;
  int maxindex = 0;
  // welcher vector ist größer?
  if( blocklist.size() >= weichenlist.size() ){
    maxindex = blocklist.size();
  }
  if( weichenlist.size() > blocklist.size() ){
    maxindex = weichenlist.size();
  }
  
  // check, if there are pins für weichen oder blocks at all  
  while( update and maxindex > 0){
    //Zuordung direkt 1 zu 1 über das pair, schleif läuft parallel und stoppt je nach size etwas früher, bis es zum reset kommt (leerlauf für diesen vector)
    if( i < blocklist.size() ){
      aktuellBlock = digitalRead(blocklist.at(i).first);
      if( aktuellBlock == 0 ){ //read pins false = belegt || true = frei dagegen ist 0 = LOW , 1 = HIGH
	blocklist.at(i).second->setB_status(true);
      }
      if( aktuellBlock == 1 ){ //read pins
	blocklist.at(i).second->setB_status(false);
      }
    }
    //////////////////////////
    if( i < weichenlist.size() ){
      aktuellWeiche = digitalRead(weichenlist.at(i).first);
      if( aktuellWeiche == 0 ){ //read pins false = belegt || true = frei dagegen ist 0 = LOW , 1 = HIGH
	weichenlist.at(i).second->setBelegung(true);
      }
      if( aktuellWeiche == 1 ){ //read pins
	weichenlist.at(i).second->setBelegung(false);
      }
    }
    //gehe weiter in der Liste der Blöcke, und teste die, ansonsten von vorne -- vllt eher mit interruptfunktion, nur wenn sich was ändert?
    if( i < maxindex ){
      i++;
    }
    if( i == maxindex) i = 0; //ansonsten: reset
  }
  //falls die schleife zu Ende kommt (entweder keine Einträge, oder quit Aufruf, dann beende den Worker)
  qDebug()<<"                           F E R T I S C H ";
  emit finished();
}
