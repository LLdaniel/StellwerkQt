//****************************************************************************
//Speicherliste des Stellwerks von Stellwerkstechnik [- STELLWERKSTECHNIK.H -]
//****************************************************************************
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
  bool getS_pass_status( QString statusvonSignal );//gibt S_status von HS zurück
  QString HSorWS(QString zutesten);//testet, ob es sich um ein W oder S handelt
  ~Stellwerkstechnik(){};
public slots:
  void add_Signal(QString strid , bool status );
 private:
  QList<QPair<int,bool> > hsignale;//Hier weden alle HSignale der Anlage mit status gespeichert gespeichert
  QList<QPair<int,bool> > wsignale;//Hier weden alle HSignale der Anlage mit status gespeichert gespeichert
};
#endif
//Vorteilhaft wäre jetzt noch: sobald ein Signal-Pointer kreiert wird-->automatischer Eintrag in obiger Liste
//Stellwerkstechnik erweiterbar: kann auch Blockliste beinhalten, oder auch Weichenliste
