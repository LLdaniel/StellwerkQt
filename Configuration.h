/*
 * segments for railroad [- CONFIGURATION.H -]
 *******************************************************************************
 * Configuration parameters which can be passed to other classes               *
 *    - debug, info mode                                                       *
 *    - mode for connected hardware or not                                     *
  *******************************************************************************
 */
#ifndef CONFIGURATION_H
#define CONFIGURATION_H
class Configuration{
 public:
  Configuration() = default;
  //
  void setDebug( bool isDebug){ this->isDebug = isDebug;}
  bool getDebug(){ return isDebug; }
  void setWithHardware( bool withHardware){ this->withHardware = withHardware;}
  bool getWithHardware(){ return withHardware; }
  ~Configuration(){}

 private:
  bool isDebug = false;           // print whole debug messages
  bool withHardware = true;       // mode where real hardware is connected I2C, MCP, shift register...
};
#endif
