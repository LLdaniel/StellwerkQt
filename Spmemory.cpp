/*
 * route memory [- SPMEMORY.CXX -]
 **************************************************************************
 */
#include "Spmemory.h"
#include <QDebug>

void Spmemory::addFS( HSignal *SPstart, HSignal *SPziel){        // buffer the route memory request
  QPair<HSignal*,HSignal*> hilfspair = qMakePair(SPstart, SPziel);
  buffer.push_back(hilfspair);
}

void Spmemory::showSP(){
  qDebug()<<"************************************************************";
  qDebug()<<"*** Dies sind die gespeicherten Fahrstraßen:             ***";
  for( int i = 0 ; i < speicherlist.size() ; i++ ){//Durchläuft die gesamte Speicherlist
    qDebug()<<"***   "<<speicherlist.at(i).first->getS_id()<<" -> "<<speicherlist.at(i).second->getS_id()<<"                                           ***";
  }
  qDebug()<<"************************************************************";
  qDebug()<<"";
}

void Spmemory::quit(){
  trySP = false;
}

void Spmemory::timing(){                                        //this is a hack: thread connected with processSpeicher directly only executes processSpeicher, but cannot insert updates, therefore timer with 0ms which calls it also repeatedly, but there thread has chance to process event loop and new events see https://github.com/LLdaniel/QThreadExample
  if( trySP ) t->start();
  if(!trySP ){
    delete t;
    qDebug()<<" F I N I S H E D # 2 ";
  }
}

void Spmemory::processSpeicher(){                              //try to set the memorized route, sooner or later this will be possible: 1) go through speicherlist 2)eventually delete routes 3) include recently buffered routes 4) repeat a new run 
  bool delit = false;
  if(!speicherlist.isEmpty() ){                               //1) 
    for( int i = 0; i < speicherlist.size(); i++){
      delit = speicherlist.at(i).first->setFahrt(speicherlist.at(i).second); //successful to setFahrt?
      if( delit ){                                                           //for successful route, remember position
	deleter.push_back(i);
	//
	//darken squares on plan for the memory
	speicherlist.at(i).first->setSpeicher( false );
	speicherlist.at(i).second->recieveSpeicher(false, speicherlist.at(i).second->getS_id());       //recieving the end signal is efficient: because it calls also the start signal memory square
      }
    }
    delit = false;                                                          // reset delit for next run
    for( int j = 0; j < deleter.size(); j++){                  // 2)
      speicherlist.removeAt(deleter.at(j));
    }
    deleter.clear();                                           //resette den deleter for next run
    // ::::::
  }
  if(!buffer.isEmpty() ){                                     //3) 
    //qDebug()<<" load buffer ...";
    speicherlist.append(buffer);      
    buffer.clear();               
  }
  timing();                                                   // 4)
}
