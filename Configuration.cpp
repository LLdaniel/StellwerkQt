/*
 * configuration for railroad [- CONFIGURATION.CXX -]
 *******************************************************************************
 */
#include "Configuration.h"
#include <QLoggingCategory>

static void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg){
  QString message = qFormatLogMessage(type, context, msg);
  static FILE *f = fopen("StellwerkQt.log", "a");
  fprintf(f, "%s\n", qPrintable(message));
  fflush(f);
}

Configuration::Configuration(){
  msgHandler = qInstallMessageHandler(logToFile);
  setLogLevel("info");
  const QString STELLWERK_LOG_PATTERN="[%{time yyyy-MM-dd h:mm:ss.zzz ttt} %{if-debug}   DEBUG%{endif}%{if-info}    INFO%{endif}%{if-warning} WARNING%{endif}%{if-critical}CRITICAL%{endif}%{if-fatal}   FATAL%{endif}] %{message}";
  qSetMessagePattern(STELLWERK_LOG_PATTERN);
}

void Configuration::setLogLevel(QString loglevel){
  logLevel = loglevel;
  qDebug()<<"__Configuration__: loglevel = "<<loglevel;
  QLoggingCategory category("StellwerkQt");
  if(loglevel == "critical"){
    category.setFilterRules("*.debug=false\n"
			    "*.info=false\n"
			    "*.warning=false");
  }
  else if(loglevel == "warning"){
    category.setFilterRules("*.debug=false\n"
			    "*.info=false");
  }
  else if(loglevel == "info"){
    category.setFilterRules("*.debug=false");
  }
  else{
    category.setFilterRules("StellwerkQt.debug=false");
  }
}

Configuration::~Configuration(){
  //fclose(f);
}
