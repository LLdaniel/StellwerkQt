/*
 * configuration for railroad [- CONFIGURATION.H -]
 *******************************************************************************
 * Configuration parameters which can be passed to other classes               *
 *    - debug, info mode (logging)                                             *
 *    - mode for connected hardware or not                                     *
  *******************************************************************************
 */
#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <QObject>
#include <QtLogging>
#include <QLoggingCategory>
#include <QString>
#include "VSignal.h"
class Configuration : public QObject{
  Q_OBJECT
  Q_DECLARE_LOGGING_CATEGORY(StellwerkQt)
 public:
  Configuration();
  //
  void setLogLevel( QString loglevel);
  QString getLogLevel(){ return logLevel; }
  void setWithHardware( bool withHardware){ this->withHardware = withHardware;}
  bool getWithHardware(){ return withHardware; }
  void addVSignals(VSignal *vsignal);
  QtMessageHandler msgHandler;
  ~Configuration();
public slots:
  void changeSignalSymbols(QString mode);
 private:
  QString logLevel = "warning";
  bool withHardware = true;       // mode where real hardware is connected I2C, MCP, shift register...
  QList<VSignal*> *vsignals = new QList<VSignal*>();
};
#endif
