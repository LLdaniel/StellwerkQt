/*
 * some turnouts are physically two coupled turnouts [- CONNECTOR.H -]
 * ___________________________________________________________________
 * To be able to display the four different directions, coupled turnouts
 * are treated as two turnouts with the additional feature to change 
 * the physical status correctly - hence connector
 */
#ifndef CONNECTOR_H
#define CONNECTOR_H
#include <QObject>
#include <QString>
class Connector : public QObject{
  Q_OBJECT
 public:
  Connector( QString name);
  //
  QString getID(){ return id; }
 public slots:
  void recieveCall(bool state);
  //
  //+++ GPIO +++
  //
  void setGpio( int pinGerade, int pinAbknickend);
  int getGpio( bool linksrechts );
 private:
   QString id;
   int pin0 = -1;
   int pin1 = -1;
   bool turnout1;
   bool turnout2;
   int counter = 0;
  //
  //+++ GPIO +++
  //
  void act();
  
};
#endif
