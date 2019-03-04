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
unsigned int i;
do{
    i = 0;
    while( i < speicherqueue.size()){
        speicherqueue.at(i).first->setFahrt(speicherqueue.at(i).second);//versuch, die FS zu stellen
        std::cout<<"\t\t\t\t"<<i<<std::endl;
        if(speicherqueue.at(i).first->getS_status()){
            speicherqueue.erase(speicherqueue.begin());
            i = 0;//beginne von vorne, size hat sich wegen erase geändert
        }
        else i++;//fortfahren, bis ein Speicher erfolgreich gestellt wurde
    }
} while (end);
}
