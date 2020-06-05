/*
 * two coupled turnouts [- CONNECTOR.CPP -]
 */
#include "Connector.h"
#include <QDebug>
#ifdef __cplusplus
extern "C"{
#endif
#include <wiringPi.h>
#include <sr595.h>
#ifdef __cplusplus
}
#endif
//
Connector::Connector(QString name){
  id = name;
}

void Connector::recieveCall(bool state){
  counter++;
  if(counter == 1){                      //first recieved call, save it and wait for second
    turnout1 = state;
  }
  if(counter == 2){                      //second recieved call, save it and act = not XOR
    turnout2 = state;
    act();
    counter = 0;
  }
}

void Connector::setGpio(int pinGerade, int pinAbknickend){
  pin0 = pinGerade;
  pin1 = pinAbknickend;
}

int Connector::getGpio(bool linksrechts){
  int aktuellerPin = -1;
  if(linksrechts){
    aktuellerPin = pin0;
  }
  if(!linksrechts){
    aktuellerPin = pin1;
  }
  return aktuellerPin;
}

void Connector::act(){
  if(pin0 > 0 and pin1 > 0){                                 
    //calculate the physical state with not XOR
    bool changeit = turnout1 == turnout2;
    // changing hardware status
    if(changeit){
      digitalWrite(pin0, HIGH);
      delay(25);//[ms]
      digitalWrite(pin0, LOW);
      //qDebug()<<"Hardware change Kreuzung: true";
    }
    if(!changeit){
      digitalWrite(pin1, HIGH);
      delay(25);//[ms]
      digitalWrite(pin1, LOW);
      //qDebug()<<"Hardware change Kreuzung: false";
    }
  }
  else{
    qDebug()<<"Kreuzungsweiche mit Kennung "<<getID()<<" ist nicht als pin initialisiert";
  }
}
