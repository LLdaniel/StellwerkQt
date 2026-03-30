/*
 * signal aspect [- SIGNALASPECT.H -]
 *******************************************************************************
 * Signal aspect collection depending on the signal type                       *
 *******************************************************************************
 */
#ifndef SIGNALASPECT_H
#define SIGNALASPECT_H
#include <QList>
#include <QString>
#include <QByteArray>
#include <vector>
class SignalAspect {
 public:
  enum class signalType {
    HS,
    VS,
    WS
  };

  SignalAspect(signalType sigtype);
  QList<QString> getAspects(){ return aspects; }
  std::vector<unsigned char> calculateAspect(QString name, QString aspect);
 protected:
  QList<QString> aspects;
  signalType sigType;
};
#endif
