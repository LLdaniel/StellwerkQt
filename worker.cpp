/*
 * worker for constant updated hardware [- WORKER.CXX -]
 **************************************************************************
 */
#include "worker.h"
#include <QDebug>

worker::worker( QList<Block*> allBlocks, QList<Weiche*> allWeichen, Configuration *config ){
  blocklist = allBlocks;
  weichenlist = allWeichen;
  configuration = config;
  t->callOnTimeout(this, &worker::updateBelegt);
}


worker::~worker(){
}

void worker::showBlocks(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies sind die Blöcke, die ständig überwacht werden   ***";
  for(  int i = 0 ; i < blocklist.size() ; i++ ){//Durchläuft alle update Blocks
    qDebug()<<"***   "<<blocklist.at(i)->getGpio()<<" -> "<<blocklist.at(i)->getName()<<"                                           ***";
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void worker::addBlocks(Block* bl){
  blocklist.push_back(bl);
}

void worker::showWeichen(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies sind die Weichen, die ständig überwacht werden  ***";
  for(  int i = 0 ; i < weichenlist.size() ; i++ ){//Durchläuft alle update Blocks
    // qDebug()<<"***   "<<weichenlist.at(i)->getGpio()<<" -> "<<weichenlist.at(i)->getW_id()<<"                                           ***";
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void worker::timing(){                                        //this is a hack: thread connected with processSpeicher directly only executes processSpeicher, but cannot insert updates, therefore timer with 0ms which calls it also repeatedly, but there thread has chance to process event loop and new events see https://github.com/LLdaniel/QThreadExample
  if( update ) t->start();
  if(!update ){
    delete t;
    qDebug()<<" F I N I S H E D: Worker ";
  }
}

void worker::quit(){
  update = false;
}

void worker::updateBelegt(){
  qDebug(" ich mach dann mal was");
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
  while( update and maxindex > 0){
    //qDebug(" w o r k i n g");
    //
    //loop runs parallel for turnout and segments and stops for the smaller list earlier until the reset
    if( i < blocklist.size() ){
      aktuellBlock = blocklist.at(i)->readBlock();
      if( aktuellBlock == 0 ){                                      //read pins false = belegt || true = frei dagegen ist 0 = LOW , 1 = HIGH
	emit callGUIb(blocklist.at(i), false);
	//qDebug("true = frei");
      }
      if( aktuellBlock == 1 ){                                      //read pins
	emit callGUIb(blocklist.at(i), true);
	//qDebug("false = belegt");
      }
    }
    // #### copy above block for Weichen: __________
    //
    //test more segments of the list otherwise start a new run -- maybe interrupt is the better variant
    timing();
    if( i < maxindex ){
      i++;
    }
    if( i == maxindex) i = 0;                                       //otherwise: reset
  }
  //
  //end of loop either there are no entries or quit call: finish worker
  qDebug()<<"                           F E R T I S C H worker";
}
