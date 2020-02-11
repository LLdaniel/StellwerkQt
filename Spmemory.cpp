#include "Spmemory.h"
#include <iostream>

void Spmemory::addFS( HSignal *SPstart, HSignal *SPziel){      // erst mal in den buffer schreiben
  std::pair<HSignal*,HSignal*> hilfspair = std::make_pair(SPstart, SPziel);
  buffer.push_back(hilfspair);
  std::cout<<" AHAAAAAAAAA; DER Buffer wird geladen..."<<std::endl;
}

void Spmemory::showSP(){
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<"*** Dies sind die gespeicherten Fahrstraßen:             ***"<<std::endl;
  for( int i = 0 ; i < speicherlist.size() ; i++ ){//Durchläuft die gesamte Speicherlist
    std::cout<<"***   "<<speicherlist.at(i).first->getS_id()<<" -> "<<speicherlist.at(i).second->getS_id()<<"                                           ***"<<std::endl;
  }
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<""<<std::endl;
}

void Spmemory::quit(){
  trySP = false;
}

void Spmemory::processSpeicher(){ //versuche ständig die FS zu stellen, irgendwann geht sie ja wieder rein: 1) speicherlist abhandeln 2)evtl Vermerke löschen 3) buffer mitaufnehmen 4) neue Tour 
  int i = 0;
  bool delit = false; //true = erfolgreich gestellt, kann aus der Liste gelöscht werden
  while(trySP){
    if( i < speicherlist.size() and !speicherlist.isEmpty() ){ //1)
      delit = speicherlist.at(i).first->setFahrt(speicherlist.at(i).second); // Rückgabewert wird notwendig für das stellen...
      std::cout<<" ich probiere zu stellen"<<std::endl;
      if( delit ){ // bei erfolgreichem stellen, merke man sich die position
	deleter.push_back(i);
	std::cout<<" ich lösche"<<std::endl;
	//erledige das Löschen der Speicher gui (Markierung)
	speicherlist.at(i).first->setSpeicher( false );
	speicherlist.at(i).first->getspeicheritems().first->setBrush(Qt::darkBlue);
      }
      i++;//solange noch nicht alle SP der FS erledigt, fahre fort
      delit = false; // reset delit für den nächsten durchgang
      std::cout<<" ich zähle hoch"<<std::endl;
    }
    if( i == speicherlist.size() and !speicherlist.isEmpty() ){ // 2) lösche alle erfolgreichen Speicher und resette i und 3)
      i = 0;
      for( int j = 0; j < deleter.size(); j++){   //lösche alle Vermerke der schon gestellten FS
	speicherlist.removeAt(deleter.at(j));
      }
      deleter.clear(); //resette den deleter für die nächste Tour
      //3) lade buffer
      std::cout<<" vor schritt 3) ende"<<std::endl;
      QMutex mutex; //erstelle einen mutex, nicht dass in dem Moment der Buffer geändert wird...
      mutex.lock();                     // ---
      speicherlist.append(buffer);      //   |  gesichert
      buffer.clear();                   //   |
      mutex.unlock();                   // ---
      std::cout<<" schritt 3) ende"<<std::endl;
    }
  }
  std::cout<<"                           F E R T I S C H #2 "<<std::endl;
  emit finished();
}
