/*
 * signal aspect [- SIGNALASPECT.CXX -]
 **************************************************************************
 */
#include "SignalAspect.h"

SignalAspect::SignalAspect(signalType sigType){
  this->sigType = sigType;
  switch(sigType){
  case signalType::HS:
    aspects.append("Hp0");
    aspects.append("Hp1");
    aspects.append("Hp2");
    aspects.append("Sh1");
    break;
  case signalType::VS:
    aspects.append("Vr0");
    aspects.append("Vr1");
    aspects.append("Vr2");
    break;
  case signalType::WS:
    aspects.append("Sh0");
    aspects.append("Sh1");
    break;
  }
}

std::vector<unsigned char> SignalAspect::calculateAspect(QString name, QString aspect){
  //unsigned char data[2];
  std::vector<unsigned char> data = {0, 0};
  //data.append(QByteArray::number(0)); // t.b.d. signal/gipo? reference
  //data[0] = 0; //t.b.d. signal/gipo? reference
  switch(sigType){
  case signalType::HS:
    //data.append(QByteArray::number(aspects.indexOf(aspect)));
    data[1] = aspects.indexOf(aspect);
    break;
  case signalType::VS:
    //data.append(QByteArray::number(aspects.indexOf(aspect) + 10));
    data[1] = aspects.indexOf(aspect) + 10;
    break;
  case signalType::WS:
    //data.append(QByteArray::number(aspects.indexOf(aspect) + 20));
    data[1] = aspects.indexOf(aspect) + 20;
    break;
  }
  return data;
}
