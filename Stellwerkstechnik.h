//****************************************************************************
//Speicherliste des Stellwerks von Stellwerkstechnik [- STELLWERKSTECHNIK.H -]
//****************************************************************************
#ifndef STELLWERKSTECHNIK_H
#define STELLWERKSTECHNIK_H
#include <QObject>
#include <vector>
class Stellwerkstechnik : public QObject{
 Q_OBJECT
  public:
  Stellwerkstechnik(){}
  //
  void delete_HSignal( std::string strtodelete );
  void show_Signal();
  bool getS_pass_status( std::string statusvonSignal );//gibt S_status von HS zurück
  std::string HSorWS(std::string zutesten);//testet, ob es sich um ein W oder S handelt
public slots:
  void add_Signal(std::string strid , bool status );
 private:
  std::vector<std::pair<int,bool>> hsignale;//Hier weden alle HSignale der Anlage mit status gespeichert gespeichert
  std::vector<std::pair<int,bool>> wsignale;//Hier weden alle HSignale der Anlage mit status gespeichert gespeichert
};
#endif
//Vorteilhaft wäre jetzt noch: sobald ein Signal-Pointer kreiert wird-->automatischer Eintrag in obiger Liste
//Stellwerkstechnik erweiterbar: kann auch Blockliste beinhalten, oder auch Weichenliste
