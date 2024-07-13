/*
 * worker for constant updated hardware [- WORKER.CXX -]
 **************************************************************************
 */
#include "worker.h"
#include <QDebug>

worker::worker( QList<Block*> allBlocks, QList<Weiche*> allWeichen, QList<BU*> allBUs, Configuration *config ){
  blocklist = allBlocks;
  weichenlist = allWeichen;
  bulist = allBUs;
  configuration = config;
  t->callOnTimeout(this, &worker::updateBelegt);
}


worker::~worker(){
}

void worker::showBlocks(){
  qInfo()<<"************************************************************";
  qInfo()<<"*** Dies sind die Blöcke, die ständig überwacht werden   ***";
  for(  int i = 0 ; i < blocklist.size() ; i++ ){//Durchläuft alle update Blocks
    qInfo()<<"***   "<<blocklist.at(i)->getGpio()<<" -> "<<blocklist.at(i)->getName()<<"                                           ***";
  }
  qInfo()<<"************************************************************";
  qInfo()<<"";
}

void worker::addBlocks(Block* bl){
  blocklist.push_back(bl);
}

void worker::addBUs(BU* bu){
  bulist.push_back(bu);
}

void worker::showWeichen(){
  qInfo()<<"************************************************************";
  qInfo()<<"*** Dies sind die Weichen, die ständig überwacht werden  ***";
  //for(  int i = 0 ; i < weichenlist.size() ; i++ ){//Durchläuft alle update Blocks
  //  qDebug()<<"***   "<<weichenlist.at(i)->getGpio()<<" -> "<<weichenlist.at(i)->getW_id()<<"                                           ***";
  //}
  qInfo()<<"************************************************************";
  qInfo()<<"";
}

void worker::timing(){                                        //this is a hack: thread connected with processSpeicher directly only executes processSpeicher, but cannot insert updates, therefore timer with 0ms which calls it also repeatedly, but there thread has chance to process event loop and new events see https://github.com/LLdaniel/QThreadExample
  if( update ) t->start();
  if(!update ){
    delete t;
    qDebug()<<"__worker__: Worker has finished!";
  }
}

void worker::quit(){
  update = false;
}

void worker::updateBelegt(){
  qDebug("__worker__: Updated occupation state...");
  int i = 0;
  int aktuellBlock = -1;
  //int aktuellWeiche = -1;
  int maxindex = 0;
  //
  // which list is greater?
  if( configuration->getWithHardware() ){//leave max index with =0
    if( blocklist.size() >= weichenlist.size() ){
      maxindex = blocklist.size();
    }
    if( weichenlist.size() > blocklist.size() ){
      maxindex = weichenlist.size();
    }
  }
  //
  // check, if there are pins für weichen oder blocks at all  
  while( update and maxindex > 0 ){
    qDebug("__worker__: Worker started...");
    //
    //loop runs parallel for turnout and segments and stops for the smaller list earlier until the reset
    if( i < blocklist.size() ){
      aktuellBlock = blocklist.at(i)->readBlock();
      if( aktuellBlock == 0 ){                                      //read pins false = belegt || true = frei dagegen ist 0 = LOW , 1 = HIGH
	emit callGUIb(blocklist.at(i), false);
      }
      if( aktuellBlock == 1 ){                                      //read pins
	emit callGUIb(blocklist.at(i), true);
      }
    }
    //
    // in every run check BUs at the end (lesser prio is ok for longer time scales BU)
    if( i == maxindex - 1){
      for( int u = 0; u < bulist.size(); u++ ){
	emit callGUIbu( bulist.at(u), bulist.at(u)->evaluateBU() );
      }
    }
    
    timing();
    if( i < maxindex ){
      i++;
    }
    if( i == maxindex) i = 0;                                       //otherwise: reset
  }
  //
  //end of loop either there are no entries or quit call: finish worker
  qDebug("__worker__: Worker finished!");
}
