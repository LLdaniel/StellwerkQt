#include "clickmanager.h"
#include <iostream>
clickmanager::clickmanager(){

}

void clickmanager::recieveFS(HSignal *toFS){
    counter++;
    if( counter == 3){//reset step at first
       counter = 1;
    }
    if(counter == 1){
        clickList.first = toFS;//ersten Eintrag speichern und auf den 2. Klick warten
        //::: HS->WS Registrierung-------------------------------------------------------------------
        counterForHW++;//                                                                           |
        if(counterForHW == 3){ counterForHW = 1; }//resetten des Counters für HS->WS
        if(counterForHW == 1){
            std::cout<<"<HS,WS>"<<std::endl;
            std::cout<<"<X,>"<<std::endl;
            clickListHW.first = toFS;
            toW = true;//Vermerk, das könnte ein Übergang zu einer Rangierfahrt werden
        }//                                                                                         |
        //:::----------------------------------------------------------------------------------------
        //::: WS->HS Registrierung --------------------------------------------------------------
        counterForWH++;//                                                                       |
        if(counterForWH == 3){ counterForWH = 2; }//resetten des Counters für WS->HS
        if(counterForWH == 2 && toH){// =2, da er von reciveFS(WSignal*) auf =1 gesetzt wird
            std::cout<<"<WS,HS>"<<std::endl;
            std::cout<<"<X,X>"<<std::endl;
            clickListWH.second = toFS;
            clickListWH.first->setFahrt(clickListWH.second->getS_id());
            //resetten aller counter:
            reset();
        }//
    }
    if(counter == 2){
        clickList.second = toFS;//zweiter Klick erfolgt --> <first>.setFahrt(<second>)
        clickList.first->setFahrt(clickList.second);
        //resetten aller counter:
        reset();
        std::cout<<"<HS,HS>"<<std::endl;
        std::cout<<"<X,X>"<<std::endl;
        if(clickList.first->getS_id().compare(clickList.second->getS_id()) == 0){//Doppelklick?-->FS löschen
            if(clickList.first->getS_status() == true){//Nur wenn auch eine FS gestellt ist, soll deleteFS aufgerufen werden!
                std::cout<<"<HS,HS>"<<std::endl;
                std::cout<<"<X_same,X_same>"<<std::endl;
                clickList.first->deleteFS();
                //resetten aller counter:
                reset();
            }
        }
    }
}

void clickmanager::recieveFS(WSignal *toFS){
     std::cout<<"recived Click from WSIgnal"<<std::endl;
    counterForW++;
    if( counterForW == 3){//reset step at first
       counterForW = 1;
    }
    if(counterForW == 1){
        clickListW.first = toFS;//ersten Eintrag speichern und auf den 2. Klick warten
        std::cout<<"<WS,WS>"<<std::endl;
        std::cout<<"<X,>"<<std::endl;
        //::: HS->WS Registrierung --------------------------------------------------------------
        counterForHW++;//                                                                       |
        if(counterForHW == 3){ counterForHW = 2; }//resetten des Counters für HS->WS
        if(counterForHW == 2 && toW){// =2, da er von reciveFS(HSignal*) auf =1 gesetzt wird
            std::cout<<"<HS,WS>"<<std::endl;
            std::cout<<"<X,X>"<<std::endl;
            clickListHW.second = toFS;
            clickListHW.first->setFahrt(clickListHW.second);
            //resetten aller counter:
            reset();
        }//                                                                                     |
        //:::------------------------------------------------------------------------------------
        //::: WS->HS Registrierung-------------------------------------------------------------------
        counterForWH++;//                                                                           |
        if(counterForWH == 3){ counterForWH = 1; }//resetten des Counters für WS->HS
        if(counterForWH == 1){
            std::cout<<"<WS,HS>"<<std::endl;
            std::cout<<"<X,>"<<std::endl;
            clickListWH.first = toFS;
            toH = true;//Vermerk, das könnte ein Übergang zu einer Haupt FS werden
        }//                                                                                         |
        //:::----------------------------------------------------------------------------------------
    }
    if(counterForW == 2){
        clickListW.second = toFS;//zweiter Klick erfolgt --> <first>.setFahrt(<second>)
        clickListW.first->setFahrt(clickListW.second);
        //resetten aller counter
        reset();
        std::cout<<"<WS,WS>"<<std::endl;
        std::cout<<"<X,X>"<<std::endl;
        if(clickListW.first->getV_id().compare(clickListW.second->getV_id()) == 0){//Doppelklick?-->FS löschen
            if(clickListW.first->getV_status() == true){//Nur wenn auch eine FS gestellt ist, soll deleteFS aufgerufen werden!
                clickListW.first->deleteFS();
                std::cout<<"<WS,WS>"<<std::endl;
                std::cout<<"<X_same,X_same>"<<std::endl;
                //resetten aller counter:
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
//Anmerkung1: für den Speicher ist eine Erweiterung ganz einfach möglich, in dem man ein 2. Pair anlegt zB
