//*************************************************************************
// clickmanager of control center [- CLICKMANAGER.CPP -]
//*************************************************************************
#include "clickmanager.h"
#include <QDebug>
void clickmanager::recieveFS(HSignal *toFS){
  counter++;
  if( counter == 3){//reset-step at first
    counter = 1;
  }
  if(counter == 1){
    clickList.first = toFS;//recieve first entry and wait for the second one
    //::: HS->WS resistered----------------------------------------------------------------------
    counterForHW++;//                                                                           |
    if(counterForHW == 3){ counterForHW = 1; }//reset counter for HS->WS
    if(counterForHW == 1){
      qDebug()<<"<HS,WS>";
      qDebug()<<"<X,>";
      clickListHW.first = toFS;
      toW = true;//flag, this could be a transition from HS to WS 
    }//                                                                                         |
    //:::----------------------------------------------------------------------------------------
    //::: WS->HS registered------------------------------------------------------------------
    counterForWH++;//                                                                       |
    if(counterForWH == 3){ counterForWH = 2; }//reset counters for WS->HS
    if(counterForWH == 2 && toH){// =2, because reciveFS(WSignal*) has set it to =1 
      qDebug()<<"<WS,HS>";
      qDebug()<<"<X,X>";
      clickListWH.second = toFS;
      clickListWH.first->setFahrt(clickListWH.second->getS_id());
      //reset all counter:
      reset();
    }//
  }
  if(counter == 2){
    clickList.second = toFS;//second click happend --> <first>.setFahrt(<second>)
    clickList.first->setFahrt(clickList.second);
    //reset all counter:
    reset();
    qDebug()<<"<HS,HS>";
    qDebug()<<"<X,X>";
    if(clickList.first->getS_id().compare(clickList.second->getS_id()) == 0){//double click?-->delete FS
      if(clickList.first->getS_status() == true){//If there is a route delete deleteFS, but only then otherwise deleteFS will fail
	qDebug()<<"<HS,HS>";
	qDebug()<<"<X_same,X_same>";
	clickList.first->deleteFS();
	//reset all counter:
	reset();
      }
    }
  }
}

void clickmanager::recieveFS(WSignal *toFS){
  qDebug()<<"recived Click from WSIgnal";
  counterForW++;
  if( counterForW == 3){//reset step at first
    counterForW = 1;
  }
  if(counterForW == 1){
    clickListW.first = toFS;//save first entry and wait for the second one
    qDebug()<<"<WS,WS>";
    qDebug()<<"<X,>";
    //::: HS->WS registered------------------------------------------------------------------
    counterForHW++;//                                                                       |
    if(counterForHW == 3){ counterForHW = 2; }//reset counters for HS->WS
    if(counterForHW == 2 && toW){// =2, because reciveFS(HSignal*) set it to =1 
      qDebug()<<"<HS,WS>";
      qDebug()<<"<X,X>";
      clickListHW.second = toFS;
      clickListHW.first->setFahrt(clickListHW.second);
      //reset all counter:
      reset();
    }//                                                                                     |
    //:::------------------------------------------------------------------------------------
    //::: WS->HS Registrierung-------------------------------------------------------------------
    counterForWH++;//                                                                           |
    if(counterForWH == 3){ counterForWH = 1; }//reset counter für WS->HS
    if(counterForWH == 1){
      qDebug()<<"<WS,HS>";
      qDebug()<<"<X,>";
      clickListWH.first = toFS;
      toH = true;//flag, this could be a transition from WS to HS
    }//                                                                                         |
    //:::----------------------------------------------------------------------------------------
  }
  if(counterForW == 2){
    clickListW.second = toFS;//second click recieved--> <first>.setFahrt(<second>)
    clickListW.first->setFahrt(clickListW.second);
    //reset all counter
    reset();
    qDebug()<<"<WS,WS>";
    qDebug()<<"<X,X>";
    if(clickListW.first->getV_id().compare(clickListW.second->getV_id()) == 0){//double click?-->deleteFS
      if(clickListW.first->getV_status() == true){//if there exists a route, delete it, only then. Otherwise delete FS will fail
	clickListW.first->deleteFS();
	qDebug()<<"<WS,WS>";
	qDebug()<<"<X_same,X_same>";
	//reset all counter:
	reset();
      }
    }
  }
}

void clickmanager::reset(){
  counter = 2;
  counterForW = 2;
  counterForHW = 2;
  counterForWH = 2;
  toH = false;
  toW = false;
}
