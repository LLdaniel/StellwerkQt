#include "myqueue.h"
#include <iostream>
myqueue::myqueue(){
    end = true;
}

void myqueue::add(HSignal *hs1, HSignal *hs2){
    std::pair<HSignal*,HSignal*> pair;
    pair.first = hs1;
    pair.second = hs2;
    speicherqueue.push_back(pair);
}

void myqueue::run(){
unsigned int i = 0;
do{
    while( i < speicherqueue.size()){
        speicherqueue.at(i).first->setFahrt(speicherqueue.at(i).second);//versuch, die FS zu stellen
        std::cout<<"\t\t\t\t"<<i<<std::endl;
        if(speicherqueue.at(i).first->getS_status()){
            speicherqueue.at(i).first->setSpeicher(false);
            speicherqueue.erase(speicherqueue.begin()+i);
            std::cout<<"\t\t\t\t delete"<<std::endl;
            i = 0;//beginne von vorne, size hat sich wegen erase geändert
        }
        else i++; //fortfahren, bis ein Speicher erfolgreich gestellt wurde
        if(i == speicherqueue.size()) i = 0;//Ohne -1, da vorher ja i++)
        std::cout<<"\t\t\t\t nochmal i="<<i<<std::endl;
    }
} while (end);
}
