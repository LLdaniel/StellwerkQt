#include "Spmemory.h"
#include <QDebug>

void Spmemory::addFS( HSignal *SPstart, HSignal *SPziel){      // erst mal in den buffer schreiben
  QPair<HSignal*,HSignal*> hilfspair = qMakePair(SPstart, SPziel);
  buffer.push_back(hilfspair);
  qDebug()<<" AHAAAAAAAAA; DER Buffer wird geladen...";
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

void Spmemory::timing(){ //this is a trick: thread conncted with processSpeicher directly only executes processSpeicher, but cannot insert updates, therefore timer with 0ms which calls it also repeatedly, but there thread has chance to process event loop and new events see https://github.com/LLdaniel/QThreadExample
  if( trySP ) t->start();
  if(!trySP ){
    delete t;
    qDebug()<<" F I N I S H E D # 2 ";
    //emit finished();
  }
}

void Spmemory::processSpeicher(){ //versuche ständig die FS zu stellen, irgendwann geht sie ja wieder rein: 1) speicherlist abhandeln 2)evtl Vermerke löschen 3) buffer mitaufnehmen 4) neue Tour 
  //qDebug()<<".";
  bool delit = false;
  if(!speicherlist.isEmpty() ){                        //1) speicherlist abhandeln
    for( int i = 0; i < speicherlist.size(); i++){
      delit = speicherlist.at(i).first->setFahrt(speicherlist.at(i).second); // Rückgabewert wird notwendig für das stellen...
      qDebug()<<" ich probiere zu stellen, dabei ist delit = "<<delit;
      if( delit ){ // bei erfolgreichem stellen, merke man sich die position
	deleter.push_back(i);
	qDebug()<<" ich lösche";
	//erledige das Löschen der Speicher gui (Markierung)
	speicherlist.at(i).first->setSpeicher( false );
	//speicherlist.at(i).first->getspeicheritems().first->setBrush(Qt::darkBlue); --> vergisst ziel HS Speicheritems --> deshalb mache untere Zeile: ziel Speicheritems löschen, dann zurück auf Start speicheritems
	speicherlist.at(i).second->recieveSpeicher(false, speicherlist.at(i).second->getS_id());
      }
    }
    delit = false; // reset delit für den nächsten durchgang
    qDebug()<<" eine Tour Stellversuch ist durch";
    for( int j = 0; j < deleter.size(); j++){   // 2) lösche alle erfolgreichen Speicher
      speicherlist.removeAt(deleter.at(j));
    }
    deleter.clear(); //resette den deleter für die nächste Tour
    
    // ::::::
  }
  if(!buffer.isEmpty() ){//3) lade buffer
    qDebug()<<" load buffer ...";
    speicherlist.append(buffer);      
    buffer.clear();               
    qDebug()<<" buffer loaded.schritt 3) ende";
  }
  timing(); // 4)
}
