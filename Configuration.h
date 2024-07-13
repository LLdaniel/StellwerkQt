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
#include <QtLogging>
#include <QLoggingCategory>
#include <QString>
class Configuration{
  Q_DECLARE_LOGGING_CATEGORY(StellwerkQt)
 public:
  Configuration();
  //
  void setLogLevel( QString loglevel);
  QString getLogLevel(){ return logLevel; }
  void setWithHardware( bool withHardware){ this->withHardware = withHardware;}
  bool getWithHardware(){ return withHardware; }
  QtMessageHandler msgHandler;
  ~Configuration();

 private:
  QString logLevel = "warning";
  bool withHardware = true;       // mode where real hardware is connected I2C, MCP, shift register...
};
#endif
