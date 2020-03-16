/*
 * stellwerkstechnik for main and shunt signal status [- STELLWERKSTECHNIK.H -]
 *******************************************************************************
 * To break ciricular dependencies with HSignal and WSingal, this class is up  *
 * to date and stores the current status of the above kind of signals          *
 *    - note: a transition from HS->WS is saved as true="go" for the main      *
 *      signal                                                                 *
 *******************************************************************************
 */
#ifndef STELLWERKSTECHNIK_H
#define STELLWERKSTECHNIK_H
#include <QObject>
#include <QList>
#include <QPair>
#include <QString>
class Stellwerkstechnik : public QObject{
 Q_OBJECT
  public:
  Stellwerkstechnik(){}
  //
  void delete_HSignal( QString strtodelete );
  void show_Signal();
  bool getS_pass_status( QString statusvonSignal );               //returns S_status von main signal
  QString HSorWS(QString zutesten);                               //test if it is a main or a shunt signal
  ~Stellwerkstechnik(){};
public slots:
  void add_Signal(QString strid , bool status );
 private:
  QList<QPair<int,bool> > hsignale;                               //save main signals with status
  QList<QPair<int,bool> > wsignale;                               //save shunt signals with status
};
#endif
