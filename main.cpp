//**********************************************************************
//------------------Stellwerk---------------------
//Daniel Adlkofer
//Dieses Stellwerk ist aus einem Projekt nach dem Vorbild von stellwerksim.de enstanden und könnte auch auf einer Modelleisenbahn verwendete werden,
//wenn die hardwaretechnischen Schnittstellen bereit gestellt werden
//--------
//Weichenbenennung von 001-999 möglich = 999
//BU Benennung von 1-99 möglich = 99
//Signalbenennung S,V,W von 001-999 möglich = 999
//Blockbenennung von aa-zz möglich = 676
//***********************************************************************
#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "Block.h"
#include "Weiche.h"
#include "HSignal.h"
#include "VSignal.h"
#include "WSignal.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QMouseEvent>
#include <clickmanager.h>
#ifdef __cplusplus
extern "C"{
#endif
#include <wiringPi.h>
#include <sr595.h>
#ifdef __cplusplus
}
#endif

//all pin definitions here
int in=24;
//Stellwerkstechnik+Block as basis
Stellwerkstechnik stellwerkstec;
Stellwerkstechnik *stellwerkstecptr = &stellwerkstec;
Block aa("aa", stellwerkstecptr);
Block *aaptr = &aa;
//interrupt definitions each for one pin
static void interrupt(){//helper Funktion für Interrupts, wegen wiringPiISR()
    std::cout<<"do interrupt"<<std::endl;
    bool status;
    if(digitalRead(in) == HIGH){status = false;}//false = belegt -|- true = frei
    if(digitalRead(in) == LOW){status = true;}
    aa.setB_status(status);
    //muss halt dann vorher definiert werden:
    //pins, Blöcke und einzelne interrupts
}
//etc... meherere interrupts für die einzelnen pins

int main( int argc , char *argv[] ){
    //parameter Übergabe
    //bool debug = false;
    printf("argc: %d\n", argc);
    for(int i=0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
        //-v debug option with different std's
        if( strcmp(argv[i],"-v") == 0 ){
            std::cout<<"verbose"<<std::endl;
            //debug = true;
        }
    }
    //::Eigentlicher Start main
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow( );
    //create scene
    QGraphicsScene *scene = new QGraphicsScene(w);
    scene->setBackgroundBrush(Qt::black);
    //w.show();
  std::cout<<""<<std::endl;
  std::cout<<"*************************************************************"<<std::endl;
  std::cout<<"***                Testmenü des Stellwerks                ***"<<std::endl;
  std::cout<<"*** 1) Initialisierungen und grundlegende Methodentests   ***"<<std::endl;
  std::cout<<"*** 2) Kleine Simulation einer Fahrt (ohne Zugpassiert)   ***"<<std::endl;
  std::cout<<"*** 3) WSignal Test; grundsätzliches                      ***"<<std::endl;
  std::cout<<"*** 4) Bahnübergang Test (inkl. kleiner Blocktest)        ***"<<std::endl;
  std::cout<<"*** 5) Test für deleteFS() (testet V vor + BÜ)            ***"<<std::endl;
  std::cout<<"*** 6) Test für deleteFS() (testet V rück)                ***"<<std::endl;
  std::cout<<"*** 7) Test für Stellwerkstechnik (vor HS... erstellen)   ***"<<std::endl;
  std::cout<<"*** 8) Test für Farbigkeit der Blöcke und QList_Block     ***"<<std::endl;
  std::cout<<"*** 9) Test für Farbigkeit der Weichen                    ***"<<std::endl;
  std::cout<<"***10) Test für Farbigkeit der VorSignale                 ***"<<std::endl;
  std::cout<<"***11) Test für zugpassiert durch eine Fahrt (s. 2))      ***"<<std::endl;
  std::cout<<"***12) Weichentest                                        ***"<<std::endl;
  std::cout<<"***13) Test: Einwählen des Speichers nach freier FS       ***"<<std::endl;
  std::cout<<"***14) wiringPi Test                                      ***"<<std::endl;
  std::cout<<"***15) ZugpassiertTest -> wegen Neuerungen wie WS->WS...  ***"<<std::endl;
  std::cout<<"***16) HS->WS: was VS macht [nicht TestPlan anwendbar ]   ***"<<std::endl;
  std::cout<<"***17) Beweis, dass deleteFS auch bei HS/WS->WS/HS klappt ***"<<std::endl;
  std::cout<<"*************************************************************"<<std::endl;
  int menue;
  std::cin>>menue;
  std::cout<<"                                    Testplan                                            "<<std::endl;
  std::cout<<"  _S004____af______V003______ad_______S003_____ab_____________V001___WS1__|+|___aa___S001__  "<<std::endl;
  std::cout<<"                                                            /001          |+|                "<<std::endl;
  std::cout<<"  _S005____ag______V002______ae_______S002_____ac__________/              |+|BÜ1          "<<std::endl;
  std::cout<<""<<std::endl;
  //Initialisierung der Strecke--------------------------------------------------------
  //:::Weichen:::
  Weiche w1(1);
  Weiche *w1ptr = &w1;
  //:::Block:::
  Block ab("ab", stellwerkstecptr);
  Block *abptr = &ab;
  Block ac("ac", stellwerkstecptr);
  Block *acptr = &ac;
  Block ad("ad", stellwerkstecptr);
  Block *adptr = &ad;
  Block ae("ae", stellwerkstecptr);
  Block *aeptr = &ae;
  Block af("af", stellwerkstecptr);
  Block *afptr = &af;
  Block ag("ag", stellwerkstecptr);
  Block *agptr = &ag;
  //:::VS:::
  VSignal v1(1);
  VSignal *v1ptr = &v1;
  VSignal v2(2);
  VSignal *v2ptr = &v2;  VSignal v3(3);
  VSignal *v3ptr = &v3;
  VSignal v4(4);
  VSignal *v4ptr = &v4;
  v1.setRichtung("S001","S002");
  v1.setRichtung("S001","S003");
  v2.setRichtung("S002","S005");
  v3.setRichtung("S003","S004");
  //:::HS:::
  HSignal s1(1);
  HSignal *s1ptr = &s1;
  HSignal s2(2);
  HSignal *s2ptr = &s2;
  HSignal s3(3);
  HSignal *s3ptr = &s3;
  HSignal s4(4);
  HSignal *s4ptr = &s4;
  HSignal s5(5);
  HSignal *s5ptr = &s5;
  //:::::::::
  s1.addVSignal(v1ptr,"v");
  s2.addVSignal(v2ptr,"v");
  s2.addVSignal(v1ptr,"r");
  s3.addVSignal(v3ptr,"v");
  s3.addVSignal(v1ptr,"r");
  s4.addVSignal(v3ptr,"r");
  s5.addVSignal(v2ptr,"r");
  //:::::::::
  //WSignale
  WSignal ww1 (1);
  WSignal *ww1ptr = &ww1;
  ww1.setRichtung("S001","S003");
  ww1.setRichtung("S001","S002");
  s1.addWSignal(ww1ptr,"v");
  s3.addWSignal(ww1ptr,"r");
  //
  QGraphicsRectItem *ww1rect2 = new QGraphicsRectItem();
  ww1rect2->setRect(0,0,20,20);
  ww1rect2->setPos(QPointF(-270,-20));
  ww1.addVSignalitems(ww1rect2);
  scene->addItem(ww1rect2);
  QLabel *ww1label = new QLabel();
  scene->addWidget(ww1label);
  //
  WSignal ww2 (2);
  WSignal *ww2ptr = &ww2;
  ww2.setRichtung("S001","S002");
  std::vector<Block*> ww2tos2b;
  ww2tos2b.push_back(acptr);
  ww2.addBlockZuH("S002",ww2tos2b);
  s1.addWSignal(ww2ptr,"v");
  s2.addWSignal(ww2ptr,"r");
  //
  QGraphicsRectItem *ww2rect2 = new QGraphicsRectItem();
  ww2rect2->setRect(0,0,20,20);
  ww2rect2->setPos(QPointF(-605,30));
  ww2.addVSignalitems(ww2rect2);
  scene->addItem(ww2rect2);
  QLabel *ww2label = new QLabel();
  scene->addWidget(ww2label);
  std::pair<Weiche*, bool> s1tos2w1(w1ptr,false);
  std::vector<std::pair<Weiche*, bool>> s1tos2;
  s1tos2.push_back(s1tos2w1);
  s1.addWeichenstatus(s2ptr,s1tos2);
  ww1.addWeichenstatus(ww2ptr,s1tos2);//WSignal inside -->gibt hoffentlich nicht Probleme, den selben Pointe zu verwenden
  //[außerdem stimmt es von den Blöcken nicht ganz...]
  //
  std::vector<Block*> s1tow1b;
  s1tow1b.push_back(aaptr);
  s1.addBlockZuRangier(ww1ptr,s1tow1b);
  std::pair<Weiche*, bool> s1tos3w1(w1ptr,true);
  std::vector<std::pair<Weiche*, bool>> s1tos3;
  s1tos3.push_back(s1tos3w1);
  s1.addWeichenstatus(s3ptr,s1tos3);
  //
  std::vector<Block*> s1tos2b;
  s1tos2b.push_back(aaptr);
  s1tos2b.push_back(acptr);
  s1.addBlock(s2ptr,s1tos2b);
  std::vector<Block*>w1tow2b;
  w1tow2b.push_back(acptr);
  ww1.addBlock(ww2ptr,w1tow2b);//WSignal inside -->gibt hoffentlich nicht Probleme, den selben Pointe zu verwenden
  //[außerdem stimmt es von den Blöcken nicht ganz...]
  std::vector<Block*> s1tos3b;
  s1tos3b.push_back(aaptr);
  s1tos3b.push_back(abptr);
  s1.addBlock(s3ptr,s1tos3b);
  std::vector<Block*> s2tos5b;
  s2tos5b.push_back(aeptr);
  s2tos5b.push_back(agptr);
  s2.addBlock(s5ptr,s2tos5b);
  std::vector<Block*> s3tos4b;
  s3tos4b.push_back(adptr);
  s3tos4b.push_back(afptr);
  s3.addBlock(s4ptr,s3tos4b);
  //Am Anfang muss die Liste in Stellwerkstechnik zum ersten mal initialisiert werden, sonst steht am Anfang nichts drin------------------------------------------muss noch geändert werden?
  stellwerkstec.add_Signal(s1.getS_id(),false);
  stellwerkstec.add_Signal(s2.getS_id(),false);
  stellwerkstec.add_Signal(s3.getS_id(),false);
  stellwerkstec.add_Signal(s4.getS_id(),false);
  stellwerkstec.add_Signal(s5.getS_id(),false);
  stellwerkstec.add_Signal(ww1.getV_id(),false);
  stellwerkstec.add_Signal(ww2.getV_id(),false);
  //BÜs
  BU bu1(1);
  BU *bu1ptr = &bu1;

  //GUI
  QGraphicsRectItem *bue = new QGraphicsRectItem();
  bue->setRect(0,0,50,25);
  bue->setPos(QPointF(-100,-20));
  bue->setBrush(QColor(QColor(153,50,204)));//lila
  bue->setRotation(90);
  scene->addItem(bue);
  bu1ptr->addBUrect(bue);
  aa.addBus( bu1ptr );
  //signals and slots
  QObject::connect(&s1,SIGNAL(refreshStellwerkstechnik(std::string,bool)),&stellwerkstec,SLOT(add_Signal(std::string , bool) ));
  QObject::connect(&s2,SIGNAL(refreshStellwerkstechnik(std::string,bool)),&stellwerkstec,SLOT(add_Signal(std::string , bool) ));
  QObject::connect(&s3,SIGNAL(refreshStellwerkstechnik(std::string,bool)),&stellwerkstec,SLOT(add_Signal(std::string , bool) ));
  QObject::connect(&s4,SIGNAL(refreshStellwerkstechnik(std::string,bool)),&stellwerkstec,SLOT(add_Signal(std::string , bool) ));
  QObject::connect(&s5,SIGNAL(refreshStellwerkstechnik(std::string,bool)),&stellwerkstec,SLOT(add_Signal(std::string , bool) ));
  QObject::connect(&ww1,SIGNAL(refreshStellwerkstechnikW(std::string,bool)),&stellwerkstec,SLOT(add_Signal(std::string , bool) ));
  QObject::connect(&ww2,SIGNAL(refreshStellwerkstechnikW(std::string,bool)),&stellwerkstec,SLOT(add_Signal(std::string , bool) ));
  QObject::connect(&ad,SIGNAL(zugpassiert()),&s3,SLOT(zugpassiert()) );
  QObject::connect(&ae,SIGNAL(zugpassiert()),&s2,SLOT(zugpassiert()) );
  QObject::connect(&ae,SIGNAL(zugpassiertW()),&ww2,SLOT(zugpassiertW()) );
  QObject::connect(&ac,SIGNAL(zugpassiertW()),&ww1,SLOT(zugpassiertW()) );
  //Hier wird das Grenzsignal übergeben
  ad.addpassiert(s3.getS_id(),abptr);
  ae.addpassiert(s2.getS_id(),acptr);
  ae.addpassiert(ww2.getV_id(),acptr);
  ac.addpassiert(ww1.getV_id(),aaptr);
  //
  //GUI Attribute - Testgleisplan
  QGraphicsRectItem *aarect = new QGraphicsRectItem();
  aarect->setRect(0,0,10,350);
  aarect->setPos(QPointF(0,0));
  aarect->setBrush(QColor(79,79,79));
  aarect->setRotation(90);
  aa.addBlockitems(aarect);
  scene->addItem(aarect);
  //
  QGraphicsRectItem *w1rectge = new QGraphicsRectItem();
  w1rectge->setRect(0,0,10,75);
  w1rectge->setPos(QPointF(-350,0));
  w1rectge->setBrush(QColor(79,79,79));
  w1rectge->setRotation(90);
  scene->addItem(w1rectge);
  QGraphicsRectItem *w1rectab = new QGraphicsRectItem();
  w1rectab->setRect(0,0,10,40);
  w1rectab->setPos(QPointF(-350,0));
  w1rectab->setBrush(QColor(79,79,79));
  w1rectab->setRotation(45);
  scene->addItem(w1rectab);
  QLabel *w1label = new QLabel();
  scene->addWidget(w1label);
  w1.addWeichenitem(w1rectab,w1rectge,w1label);
  w1.moveLabel(-350,-10);
  //
  QGraphicsRectItem *abrect = new QGraphicsRectItem();
  abrect->setRect(0,0,10,350);
  abrect->setPos(QPointF(-425,0));
  abrect->setBrush(QColor(79,79,79));
  abrect->setRotation(90);
  ab.addBlockitems(abrect);
  scene->addItem(abrect);
  //
  QGraphicsRectItem *adrect = new QGraphicsRectItem();
  adrect->setRect(0,0,10,350);
  adrect->setPos(QPointF(-775,0));
  adrect->setBrush(QColor(79,79,79));
  adrect->setRotation(90);
  ad.addBlockitems(adrect);
  scene->addItem(adrect);
  //
  QGraphicsRectItem *afrect = new QGraphicsRectItem();
  afrect->setRect(0,0,10,350);
  afrect->setPos(QPointF(-1125,0));
  afrect->setBrush(QColor(79,79,79));
  afrect->setRotation(90);
  af.addBlockitems(afrect);
  scene->addItem(afrect);
  //
  QGraphicsRectItem *acrect = new QGraphicsRectItem();
  acrect->setRect(0,0,10,350);
  acrect->setPos(QPointF(-425,50));
  acrect->setBrush(QColor(79,79,79));
  acrect->setRotation(90);
  ac.addBlockitems(acrect);
  scene->addItem(acrect);
  QGraphicsRectItem *acrect2 = new QGraphicsRectItem();
  acrect2->setRect(0,0,10,40);
  acrect2->setPos(QPointF(-375,25));
  acrect2->setBrush(QColor(79,79,79));
  acrect2->setRotation(45);
  ac.addBlockitems(acrect2);
  scene->addItem(acrect2);
  QGraphicsRectItem *acrect3 = new QGraphicsRectItem();
  acrect3->setRect(0,0,10,30);
  acrect3->setPos(QPointF(-395,50));
  acrect3->setBrush(QColor(79,79,79));
  acrect3->setRotation(90);
  ac.addBlockitems(acrect3);
  scene->addItem(acrect3);
  //
  /////////////NOCH VON WSIGNAL
//soll on top of scene sein...
  QPushButton *ww1push = new QPushButton();
    scene->addWidget(ww1push);
    ww1.addButtonAndLabel(ww1label,ww1push);
    ww1.moveButton(-264,0);
    ww1.moveLabel(-249,-20);
    // und NR 2 W
    QPushButton *ww2push = new QPushButton();
      scene->addWidget(ww2push);
      ww2.addButtonAndLabel(ww2label,ww2push);
      ww2.moveButton(-600,50);
      ww2.moveLabel(-585,30);
  ///////////////////////////
  QGraphicsRectItem *aerect = new QGraphicsRectItem();
  aerect->setRect(0,0,10,350);
  aerect->setPos(QPointF(-775,50));
  aerect->setBrush(QColor(79,79,79));
  aerect->setRotation(90);
  ae.addBlockitems(aerect);
  scene->addItem(aerect);
  //
  QGraphicsRectItem *agrect = new QGraphicsRectItem();
  agrect->setRect(0,0,10,350);
  agrect->setPos(QPointF(-1125,50));
  agrect->setBrush(QColor(79,79,79));
  agrect->setRotation(90);
  ag.addBlockitems(agrect);
  scene->addItem(agrect);
  //
  QGraphicsRectItem *v1rect2 = new QGraphicsRectItem();
  v1rect2->setRect(0,0,10,20);
  v1rect2->setPos(QPointF(-300,-20));
  v1rect2->setBrush(QColor(79,79,79));
  v1rect2->setRotation(90);
  v1.addVSignalitems(v1rect2);
  scene->addItem(v1rect2);
  QGraphicsRectItem *v1rect = new QGraphicsRectItem();
  v1rect->setRect(0,0,30,40);
  v1rect->setPos(QPointF(-320,-50));
  v1rect->setBrush(QColor(79,79,79));
  v1rect->setRotation(45);
  v1.addVSignalitems(v1rect);
  scene->addItem(v1rect);
  //
  QGraphicsRectItem *v3rect2 = new QGraphicsRectItem();
  v3rect2->setRect(0,0,10,20);
  v3rect2->setPos(QPointF(-1085,-20));
  v3rect2->setBrush(QColor(79,79,79));
  v3rect2->setRotation(90);
  v3.addVSignalitems(v3rect2);
  scene->addItem(v3rect2);
  QGraphicsRectItem *v3rect = new QGraphicsRectItem();
  v3rect->setRect(0,0,30,40);
  v3rect->setPos(QPointF(-1105,-50));
  v3rect->setBrush(QColor(79,79,79));
  v3rect->setRotation(45);
  v3.addVSignalitems(v3rect);
  scene->addItem(v3rect);
  //
  QGraphicsRectItem *v2rect2 = new QGraphicsRectItem();
  v2rect2->setRect(0,0,10,20);
  v2rect2->setPos(QPointF(-1085,30));
  v2rect2->setBrush(QColor(79,79,79));
  v2rect2->setRotation(90);
  v2.addVSignalitems(v2rect2);
  scene->addItem(v2rect2);
  QGraphicsRectItem *v2rect = new QGraphicsRectItem();
  v2rect->setRect(0,0,30,40);
  v2rect->setPos(QPointF(-1105,0));
  v2rect->setBrush(QColor(79,79,79));
  v2rect->setRotation(45);
  v2.addVSignalitems(v2rect);
  scene->addItem(v2rect);
  //
  QGraphicsRectItem *s1rect2 = new QGraphicsRectItem();
  s1rect2->setRect(0,0,10,20);
  s1rect2->setPos(QPointF(20,-20));
  s1rect2->setBrush(Qt::blue);
  s1rect2->setRotation(90);
  scene->addItem(s1rect2);
  QGraphicsRectItem *s1rect = new QGraphicsRectItem();
  s1rect->setRect(0,0,30,40);
  s1rect->setPos(QPointF(0,-30));
  s1rect->setBrush(Qt::blue);
  s1rect->setRotation(90);
  scene->addItem(s1rect);
  QPushButton *s1push = new QPushButton();
  scene->addWidget(s1push);
  QLabel *s1label = new QLabel();
  scene->addWidget(s1label);
  //speicher GUIs
  QGraphicsRectItem *sphin = new QGraphicsRectItem();
  sphin->setRect(0,0,10,10);
  sphin->setBrush(Qt::yellow);
  sphin->setPos(QPointF(20,-10));
  QGraphicsRectItem *spweg = new QGraphicsRectItem();
  spweg->setRect(0,0,10,10);
  spweg->setBrush(Qt::yellow);
  spweg->setPos(QPointF(20,-20));
  scene->addItem(spweg);
  scene->addItem(sphin);
  s1.addHSignalitem(s1rect,s1rect2,s1label,s1push,spweg,sphin);
  s1.moveLabel(10,0);
  //
  QGraphicsRectItem *s3rect2 = new QGraphicsRectItem();
  s3rect2->setRect(0,0,10,20);
  s3rect2->setPos(QPointF(-740,-20));
  s3rect2->setBrush(Qt::blue);
  s3rect2->setRotation(90);
  scene->addItem(s3rect2);
  QGraphicsRectItem *s3rect = new QGraphicsRectItem();
  s3rect->setRect(0,0,30,40);
  s3rect->setPos(QPointF(-760,-30));
  s3rect->setBrush(Qt::blue);
  s3rect->setRotation(90);
  scene->addItem(s3rect);
  QPushButton *s3push = new QPushButton();
  scene->addWidget(s3push);
  QLabel *s3label = new QLabel();
  scene->addWidget(s3label);

  //
  //speicher GUIs
  QGraphicsRectItem *sphin3 = new QGraphicsRectItem();
  sphin3->setRect(0,0,10,10);
  sphin3->setBrush(Qt::yellow);
  sphin3->setPos(QPointF(-740,-20));
  QGraphicsRectItem *spweg3 = new QGraphicsRectItem();
  spweg3->setRect(0,0,10,10);
  spweg3->setBrush(Qt::yellow);
  spweg3->setPos(QPointF(-740,-10));
  scene->addItem(spweg3);
  scene->addItem(sphin3);

  s3.addHSignalitem(s3rect,s3rect2,s3label,s3push,sphin3,spweg3);
  s3.moveLabel(-760,0);
  s3.moveButton(-775,0);
  //
  QGraphicsRectItem *s4rect2 = new QGraphicsRectItem();
  s4rect2->setRect(0,0,10,20);
  s4rect2->setPos(QPointF(-1410,-20));
  s4rect2->setBrush(Qt::blue);
  s4rect2->setRotation(90);
  scene->addItem(s4rect2);
  QGraphicsRectItem *s4rect = new QGraphicsRectItem();
  s4rect->setRect(0,0,30,40);
  s4rect->setPos(QPointF(-1430,-30));
  s4rect->setBrush(Qt::blue);
  s4rect->setRotation(90);
  scene->addItem(s4rect);
  QPushButton *s4push = new QPushButton();
  scene->addWidget(s4push);
  QLabel *s4label = new QLabel();
  scene->addWidget(s4label);
  //speicher GUIs
  QGraphicsRectItem *sphin4 = new QGraphicsRectItem();
  sphin4->setRect(0,0,10,10);
  sphin4->setBrush(Qt::yellow);
  sphin4->setPos(QPointF(-1410,-10));
  QGraphicsRectItem *spweg4 = new QGraphicsRectItem();
  spweg4->setRect(0,0,10,10);
  spweg4->setBrush(Qt::yellow);
  spweg4->setPos(QPointF(-1410,-20));
  scene->addItem(spweg4);
  scene->addItem(sphin4);
  s4.addHSignalitem(s4rect,s4rect2,s4label,s4push,spweg4,sphin4);
  s4.moveLabel(-1410,0);
  s4.moveButton(-1420,0);
  //
  QGraphicsRectItem *s2rect2 = new QGraphicsRectItem();
  s2rect2->setRect(0,0,10,20);
  s2rect2->setPos(QPointF(-740,30));
  s2rect2->setBrush(Qt::blue);
  s2rect2->setRotation(90);
  scene->addItem(s2rect2);
  QGraphicsRectItem *s2rect = new QGraphicsRectItem();
  s2rect->setRect(0,0,30,40);
  s2rect->setPos(QPointF(-760,20));
  s2rect->setBrush(Qt::blue);
  s2rect->setRotation(90);
  scene->addItem(s2rect);
  QPushButton *s2push = new QPushButton();
  scene->addWidget(s2push);
  QLabel *s2label = new QLabel();
  scene->addWidget(s2label);
  //speicher GUIs
  QGraphicsRectItem *sphin2 = new QGraphicsRectItem();
  sphin2->setRect(0,0,10,10);
  sphin2->setBrush(Qt::yellow);
  sphin2->setPos(QPointF(-740,40));
  QGraphicsRectItem *spweg2 = new QGraphicsRectItem();
  spweg2->setRect(0,0,10,10);
  spweg2->setBrush(Qt::yellow);
  spweg2->setPos(QPointF(-740,30));
  scene->addItem(spweg2);
  scene->addItem(sphin2);
  s2.addHSignalitem(s2rect,s2rect2,s2label,s2push,spweg2,sphin2);
  s2.moveLabel(-760,50);
  s2.moveButton(-775,50);
  //
  QGraphicsRectItem *s5rect2 = new QGraphicsRectItem();
  s5rect2->setRect(0,0,10,20);
  s5rect2->setPos(QPointF(-1410,30));
  s5rect2->setBrush(Qt::blue);
  s5rect2->setRotation(90);
  scene->addItem(s5rect2);
  QGraphicsRectItem *s5rect = new QGraphicsRectItem();
  s5rect->setRect(0,0,30,40);
  s5rect->setPos(QPointF(-1430,20));
  s5rect->setBrush(Qt::blue);
  s5rect->setRotation(90);
  scene->addItem(s5rect);
  QPushButton *s5push = new QPushButton();
  scene->addWidget(s5push);
  QLabel *s5label = new QLabel();
  scene->addWidget(s5label);
  //speicher GUIs
  QGraphicsRectItem *sphin5 = new QGraphicsRectItem();
  sphin5->setRect(0,0,10,10);
  sphin5->setBrush(Qt::yellow);
  sphin5->setPos(QPointF(-1410,40));
  QGraphicsRectItem *spweg5 = new QGraphicsRectItem();
  spweg5->setRect(0,0,10,10);
  spweg5->setBrush(Qt::yellow);
  spweg5->setPos(QPointF(-1410,30));
  scene->addItem(spweg5);
  scene->addItem(sphin5);
  s5.addHSignalitem(s5rect,s5rect2,s5label,s5push,spweg5,sphin5);
  s5.moveLabel(-1410,50);
  s5.moveButton(-1420,50);

  //QPushButtonPart
  clickmanager *c1 = new clickmanager;
  QObject::connect(s1ptr,SIGNAL(listened(HSignal*)),c1,SLOT(recieveFS(HSignal*)) );
  QObject::connect(s2ptr,SIGNAL(listened(HSignal*)),c1,SLOT(recieveFS(HSignal*)) );
  QObject::connect(s3ptr,SIGNAL(listened(HSignal*)),c1,SLOT(recieveFS(HSignal*)) );
  QObject::connect(s4ptr,SIGNAL(listened(HSignal*)),c1,SLOT(recieveFS(HSignal*)) );
  QObject::connect(s5ptr,SIGNAL(listened(HSignal*)),c1,SLOT(recieveFS(HSignal*)) );
  //ebenfalls für WSignal
  QObject::connect(ww1ptr,SIGNAL(listened(WSignal*)),c1,SLOT(recieveFS(WSignal*)) );
  QObject::connect(ww2ptr,SIGNAL(listened(WSignal*)),c1,SLOT(recieveFS(WSignal*)) );
  //:::::::::
  ///////////////////////////////////////////////////////////////////////////////////////
  if(menue == 1){//1) Initialisierungen und grundlegende Methodentests
    s1.showWeichenstatusALL();
    s1.showBlockALL();
    std::cout<<"zeige Weichenstatus mit Ziel S002"<<std::endl;
    s1.showWeichenstatus(s2ptr);
    std::cout<<"zeige Blockstatus mit Ziel S003"<<std::endl;
    s1.showBlock(s3ptr);
    s5.showVSignalR();
    s1.showVSignalV();
    std::cout<<"Löschen von Status S003 und V002 führt zu:"<<std::endl;
    s1.deleteNachbar(s3ptr);
    s1.deleteVS(v2ptr,"r");
    s1.deleteVS(v2ptr,"v");
    s1.showWeichenstatusALL();
    s1.showBlockALL();
    s1.showWeichenstatus(s2ptr);
    s1.showBlock(s3ptr);
    s5.showVSignalR();
    s1.showVSignalV();
    std::cout<<"Show Richtung von Vorsignalen:"<<std::endl;
    v1.showRichtung();
    v2.showRichtung();
    std::cout<<"Delete Richtung S001 --> S002:"<<std::endl;
    v1.deleteRichtung("S001","S002");
    v2.deleteRichtung(s1.getS_id(),s2.getS_id() );
    v1.showRichtung();
    v2.showRichtung();
  }
  ///////////////////////////////////////////////////////////////////////////////////////////
  if( menue == 2){//Fahrt Simulation
    std::cout<<"FS von S001->S002->S005"<<std::endl;
    s1.setFahrt(s2ptr);
    std::cout<<"Status von "<<s1.getS_id()<<": "<<s1.getS_status()<<std::endl;
    std::cout<<"Status von "<<v1.getV_id()<<": "<<v1.getV_status()<<" mit getinFS: "<<v1.getinFS()<<std::endl;
    std::pair<std::string,std::string> p;
    p = v1.getAktFS();
    std::cout<<"v1 first/second: "<<p.first<<"/"<<p.second<<std::endl;
    std::cout<<"Status von "<<s2.getS_id()<<": "<<s2.getS_status()<<std::endl;
    std::cout<<"Status von "<<v2.getV_id()<<": "<<v2.getV_status()<<" mit getinFS: "<<v2.getinFS()<<std::endl;
    std::pair<std::string,std::string> q;
    q = v2.getAktFS();
    std::cout<<"v2 first/second: "<<q.first<<"/"<<q.second<<std::endl;
    std::cout<<"Status von "<<s5.getS_id()<<": "<<s5.getS_status()<<std::endl;
    std::cout<<"-------------------------------------------------"<<std::endl;
    s2.setFahrt(s5ptr);
    std::cout<<"Status von "<<s1.getS_id()<<": "<<s1.getS_status()<<std::endl;
    std::cout<<"Status von "<<v1.getV_id()<<": "<<v1.getV_status()<<" mit getinFS: "<<v1.getinFS()<<std::endl;
    p = v1.getAktFS();
    std::cout<<"v1 first/second: "<<p.first<<"/"<<p.second<<std::endl;
    std::cout<<"Status von "<<s2.getS_id()<<": "<<s2.getS_status()<<std::endl;
    std::cout<<"Status von "<<v2.getV_id()<<": "<<v2.getV_status()<<" mit getinFS: "<<v2.getinFS()<<std::endl;
    q = v2.getAktFS();
    std::cout<<"v2 first/second: "<<q.first<<"/"<<q.second<<std::endl;
    std::cout<<"Status von "<<s5.getS_id()<<": "<<s5.getS_status()<<std::endl;
    std::cout<<"-------------------------------------------------"<<std::endl;
    std::cout<<"Das ziel von S001: "<<s1.getZiel()<<std::endl;
    }
  //////////////////////////////////////////////////////////////////////////////////////////////
  if( menue == 3 ){//WSignal Test- grundsätzliches
    std::cout<<ww1.getV_id()<<std::endl;
    std::cout<<ww1.getV_status()<<std::endl;
    s1.showWSignalV();
    s1.setFahrt(s3ptr);
    std::cout<<"Richtung §§§ "<<ww1.getRichtung("S001","S003")<<std::endl;
    std::cout<<ww1.isAktFS("S003")<<std::endl;
    aa.setB_status(false);
    ab.setB_status(false);
    aa.setB_status(true);
    s1.zugpassiert();
    s3.setFahrt(s5ptr);
    ab.setB_status(true);
    s3.zugpassiert();
  }
  /////////////////////////////////////////////////////////////////////////////////////////////
  if( menue == 4 ){//Bahnübergang Test
    aa.showBus();
    s1.setFahrt( s2ptr );
    std::cout<<"BU status:"<<std::endl;
    std::cout<<bu1ptr->getBU_status()<<std::endl;
     std::cout<<"Block aa Freigabe:"<<std::endl;
    std::cout<<aa.getFreigabe()<<std::endl;
    //wenn man manuell mit Gewalt manipulieren will:
    //std::cout<<"Aber bei manuellem Öffnen (sollte nicht aufgerufen werden, sondern wid nur intern als Methode wenn dann verwendet):"<<std::endl;
    //bu1ptr->setBU_status(true);
    //std::cout<<"BU status:"<<std::endl;
    //std::cout<<bu1ptr->getBU_status()<<std::endl;
    std::cout<<"setting B_status -->false + true"<<std::endl;
    aa.setB_status( false );
    aa.setB_status( true );
    std::cout<<"BU status:"<<std::endl;
    std::cout<<bu1ptr->getBU_status()<<std::endl;
    std::cout<<"Block aa Freigabe:"<<std::endl;
    std::cout<<aa.getFreigabe()<<std::endl;
    std::cout<<"Löschen von Bü1 in Block aa führt zu:"<<std::endl;
    aa.deleteBus( bu1ptr );
    aa.showBus();
    std::cout<<"------------------------mutwillig falsches Freigeben------------------------------------"<<std::endl;

  }
  ///////////////////////////////////////////////////////////////////////////////////////////
  if( menue == 5 ){//delete FS Test
    std::cout<<"========================= Normales Stellen, wie sonst auch ==========================================================="<<std::endl;
    std::cout<<"FS von S001->S002->S005"<<std::endl;
    s1.setFahrt(s2ptr);
    std::cout<<"Status von "<<s1.getS_id()<<": "<<s1.getS_status()<<std::endl;
    std::cout<<"Status von "<<v1.getV_id()<<": "<<v1.getV_status()<<" mit getinFS: "<<v1.getinFS()<<std::endl;
    std::pair<std::string,std::string> p;
    p = v1.getAktFS();
    std::cout<<"v1 first/second: "<<p.first<<"/"<<p.second<<std::endl;
    std::cout<<"Status von "<<s2.getS_id()<<": "<<s2.getS_status()<<std::endl;
    std::cout<<"Status von "<<v2.getV_id()<<": "<<v2.getV_status()<<" mit getinFS: "<<v2.getinFS()<<std::endl;
    std::pair<std::string,std::string> q;
    q = v2.getAktFS();
    std::cout<<"v2 first/second: "<<q.first<<"/"<<q.second<<std::endl;
    std::cout<<"Status von "<<s5.getS_id()<<": "<<s5.getS_status()<<std::endl;
    std::cout<<"-----------------yyyyyyyyyyyyyyyyyyy-----yyyyyyyyyyyyyyyyy-yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy--------------------------"<<std::endl;
    s2.setFahrt(s5ptr);
    std::cout<<"Status von "<<s1.getS_id()<<": "<<s1.getS_status()<<std::endl;
    std::cout<<"Status von "<<v1.getV_id()<<": "<<v1.getV_status()<<" mit getinFS: "<<v1.getinFS()<<std::endl;
    p = v1.getAktFS();
    std::cout<<"v1 first/second: "<<p.first<<"/"<<p.second<<std::endl;
    std::cout<<"Status von "<<s2.getS_id()<<": "<<s2.getS_status()<<std::endl;
    std::cout<<"Status von "<<v2.getV_id()<<": "<<v2.getV_status()<<" mit getinFS: "<<v2.getinFS()<<std::endl;
    q = v2.getAktFS();
    std::cout<<"v2 first/second: "<<q.first<<"/"<<q.second<<std::endl;
    std::cout<<"Status von "<<s5.getS_id()<<": "<<s5.getS_status()<<std::endl;
    std::cout<<"====================================================================================================================="<<std::endl;
    std::cout<<"deleting FS S001->S002"<<std::endl;
    s1.deleteFS();
    std::cout<<"Status von BÜ1: "<<bu1ptr->getBU_status()<<std::endl;
    std::cout<<"Status von V001: "<<v1.getV_status()<<std::endl;
    std::cout<<"Status von S001: "<<s1.getS_status()<<std::endl;
    std::cout<<"Erneutes Stellen von S001->S002 sollte jetzt wieder möglich sein"<<std::endl;
    s1.setFahrt(s2ptr);
    std::cout<<"Status von S001: "<<s1.getS_status()<<"| mit V001: "<<v1.getV_status()<<"| und BÜ1: "<<bu1.getBU_status()<<std::endl;

  }
  /////////////////////////////////////////////////////////////////////////////////////////////
  if( menue == 6 ){//delete FS test2 [rück]
    std::cout<<"========================= Normales Stellen, wie sonst auch ==========================================================="<<std::endl;
    std::cout<<"FS von S001->S002->S005"<<std::endl;
    s1.setFahrt(s2ptr);
    std::cout<<"Status von "<<s1.getS_id()<<": "<<s1.getS_status()<<std::endl;
    std::cout<<"Status von "<<v1.getV_id()<<": "<<v1.getV_status()<<" mit getinFS: "<<v1.getinFS()<<std::endl;
    std::pair<std::string,std::string> p;
    p = v1.getAktFS();
    std::cout<<"v1 first/second: "<<p.first<<"/"<<p.second<<std::endl;
    std::cout<<"Status von "<<s2.getS_id()<<": "<<s2.getS_status()<<std::endl;
    std::cout<<"Status von "<<v2.getV_id()<<": "<<v2.getV_status()<<" mit getinFS: "<<v2.getinFS()<<std::endl;
    std::pair<std::string,std::string> q;
    q = v2.getAktFS();
    std::cout<<"v2 first/second: "<<q.first<<"/"<<q.second<<std::endl;
    std::cout<<"Status von "<<s5.getS_id()<<": "<<s5.getS_status()<<std::endl;
    std::cout<<"-------------------------------------------------"<<std::endl;
    s2.setFahrt(s5ptr);
    std::cout<<"Status von "<<s1.getS_id()<<": "<<s1.getS_status()<<std::endl;
    std::cout<<"Status von "<<v1.getV_id()<<": "<<v1.getV_status()<<" mit getinFS: "<<v1.getinFS()<<std::endl;
    p = v1.getAktFS();
    std::cout<<"v1 first/second: "<<p.first<<"/"<<p.second<<std::endl;
    std::cout<<"Status von "<<s2.getS_id()<<": "<<s2.getS_status()<<std::endl;
    std::cout<<"Status von "<<v2.getV_id()<<": "<<v2.getV_status()<<" mit getinFS: "<<v2.getinFS()<<std::endl;
    q = v2.getAktFS();
    std::cout<<"v2 first/second: "<<q.first<<"/"<<q.second<<std::endl;
    std::cout<<"Status von "<<s5.getS_id()<<": "<<s5.getS_status()<<std::endl;
    std::cout<<"====================================================================================================================="<<std::endl;
    std::cout<<"deleting FS S002->S005..."<<std::endl;
    s2.deleteFS();
    std::cout<<"Status von V001: "<<v1.getV_status()<<std::endl;
    std::cout<<"Status von S002: "<<s2.getS_status()<<std::endl;
    std::cout<<"Erneutes Stellen von S002->S005 sollte jetzt wieder möglich sein"<<std::endl;
    s2.setFahrt(s5ptr);
    std::cout<<"Status von S002: "<<s2.getS_status()<<"| mit V001: "<<v1.getV_status()<<std::endl;
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////77
  if( menue == 7){//Test Stellwerktechnik
    Stellwerkstechnik Signaltechnik;
    std::cout<<"Filling Stellwerkstechnik with s1ptr and s2ptr..."<<std::endl;
    Signaltechnik.add_Signal( s1ptr->getS_id() , s1ptr->getS_status() );
    Signaltechnik.add_Signal( s2ptr->getS_id() , s2ptr->getS_status() );
    Signaltechnik.show_Signal();
    std::cout<<"deleting s2 from Stellwerkstechnik..."<<std::endl;
    Signaltechnik.delete_HSignal( s2ptr->getS_id() );
    Signaltechnik.show_Signal();
  }
  if(menue == 8){
        //Nr 1
        QGraphicsRectItem *r1 = new QGraphicsRectItem();
        r1->setRect(0,0,5,100);
        r1->setPos(QPointF(-1123,-696));
        r1->setRotation(-45);
        r1->setBrush(Qt::darkGray);
        scene->addItem(r1);
        //Nr 2
        QGraphicsRectItem *r2 = new QGraphicsRectItem();
        r2->setRect(0,0,5,100);
        r2->setPos(QPointF(-1223,-696));
        r2->setBrush(Qt::darkGray);
        scene->addItem(r2);
        //adding to Block "aa" QList
        aa.addBlockitems(r1);
        ab.addBlockitems(r2);
        aaptr->setB_status(true);
        abptr->setFreigabe(false);



  }
  if(menue == 9){
    //Nr 3
    QGraphicsRectItem *r3 = new QGraphicsRectItem();
    r3->setRect(0,0,5,40);
    r3->setPos(QPointF(-1323,-696));
    r3->setRotation(-45);
    r3->setBrush(Qt::darkGray);
    scene->addItem(r3);
    //Nr 4
    QGraphicsRectItem *r4 = new QGraphicsRectItem();
    r4->setRect(0,0,5,100);
    r4->setPos(QPointF(-1323,-696));
    r4->setBrush(Qt::gray);
    scene->addItem(r4);
    QLabel *label = new QLabel();
    scene->addWidget(label);
    //adding to Weiche "W001":
    w1ptr->addWeichenitem(r3,r4,label);
    w1ptr->moveLabel(-1323,-726);
    w1ptr->setW_status(false);
    //w1ptr->setBelegung(false);
    w1ptr->setVerriegelung(true);
  }

  if( menue == 10 ){
      //Nr 5
      QGraphicsRectItem *r5 = new QGraphicsRectItem();
      r5->setRect(0,0,10,20);
      r5->setPos(QPointF(-1323,-696));
      r5->setRotation(45);
      r5->setBrush(Qt::darkGray);
      scene->addItem(r5);
      //Nr 6
      QGraphicsRectItem *r6 = new QGraphicsRectItem();
      r6->setRect(0,0,5,10);
      r6->setPos(QPointF(-1328,-680));
      r6->setBrush(Qt::gray);
      scene->addItem(r6);
      v4ptr->addVSignalitems(r5);
      v4ptr->addVSignalitems(r6);
      v4ptr->setV_status(false);
  }
  if( menue == 11){//Fahrt Simulation mit zugpassiert
    std::cout<<"FS von S001->S002->S005"<<std::endl;
    s1.setFahrt(s2ptr);
    std::cout<<"Status von "<<s1.getS_id()<<": "<<s1.getS_status()<<std::endl;
    std::cout<<"Status von "<<v1.getV_id()<<": "<<v1.getV_status()<<" mit getinFS: "<<v1.getinFS()<<std::endl;
    std::pair<std::string,std::string> p;
    p = v1.getAktFS();
    std::cout<<"Status/Verriegelung von Weiche"<<w1.getW_id()<<" ist: "<<w1.getW_status()<<"/"<<w1.getVerriegelung()<<std::endl;
    std::cout<<"v1 first/second: "<<p.first<<"/"<<p.second<<std::endl;
    std::cout<<"Status von "<<s2.getS_id()<<": "<<s2.getS_status()<<std::endl;
    std::cout<<"Status von "<<v2.getV_id()<<": "<<v2.getV_status()<<" mit getinFS: "<<v2.getinFS()<<std::endl;
    std::pair<std::string,std::string> q;
    q = v2.getAktFS();
    std::cout<<"v2 first/second: "<<q.first<<"/"<<q.second<<std::endl;
    std::cout<<"Status von "<<s5.getS_id()<<": "<<s5.getS_status()<<std::endl;
    std::cout<<"Status/Freigabe von Blöcken aa = "<<aa.getB_status()<<"/"<<aa.getFreigabe()<<"|ab= "<<ab.getB_status()<<"/"<<ab.getFreigabe()<<"|ac= "<<ac.getB_status()<<"/"<<ac.getFreigabe()<<"|ad= "<<ad.getB_status()<<"/"<<ad.getFreigabe()<<"|ae= "<<ae.getB_status()<<"/"<<ae.getFreigabe()<<"|af= "<<af.getB_status()<<"/"<<af.getFreigabe()<<"|ag= "<<ag.getB_status()<<"/"<<ag.getFreigabe()<<std::endl;
    stellwerkstec.show_Signal();
    std::cout<<"-------------------------------------------------"<<std::endl;
    s2.setFahrt(s5ptr);
    std::cout<<"Status von "<<s1.getS_id()<<": "<<s1.getS_status()<<std::endl;
    std::cout<<"Status von "<<v1.getV_id()<<": "<<v1.getV_status()<<" mit getinFS: "<<v1.getinFS()<<std::endl;
    p = v1.getAktFS();
    std::cout<<"Status/Verriegelung von Weiche"<<w1.getW_id()<<" ist: "<<w1.getW_status()<<"/"<<w1.getVerriegelung()<<std::endl;
    std::cout<<"v1 first/second: "<<p.first<<"/"<<p.second<<std::endl;
    std::cout<<"Status von "<<s2.getS_id()<<": "<<s2.getS_status()<<std::endl;
    std::cout<<"Status von "<<v2.getV_id()<<": "<<v2.getV_status()<<" mit getinFS: "<<v2.getinFS()<<std::endl;
    q = v2.getAktFS();
    std::cout<<"v2 first/second: "<<q.first<<"/"<<q.second<<std::endl;
    std::cout<<"Status von "<<s5.getS_id()<<": "<<s5.getS_status()<<std::endl;
    std::cout<<"Status/Freigabe von Blöcken aa = "<<aa.getB_status()<<"/"<<aa.getFreigabe()<<"|ab= "<<ab.getB_status()<<"/"<<ab.getFreigabe()<<"|ac= "<<ac.getB_status()<<"/"<<ac.getFreigabe()<<"|ad= "<<ad.getB_status()<<"/"<<ad.getFreigabe()<<"|ae= "<<ae.getB_status()<<"/"<<ae.getFreigabe()<<"|af= "<<af.getB_status()<<"/"<<af.getFreigabe()<<"|ag= "<<ag.getB_status()<<"/"<<ag.getFreigabe()<<std::endl;
    stellwerkstec.show_Signal();
    std::cout<<"-------------------------------------------------"<<std::endl;
    std::cout<<"Das ziel von S001: "<<s1.getZiel()<<std::endl;
    std::cout<<"-------------------------------------------------"<<std::endl;
    std::cout<<"Zug fährt die FS ab, zunächst belegt er Block aa und Weiche 001:"<<std::endl;
    aa.setB_status(false);
    w1.setBelegung(false);
    std::cout<<"Status von "<<s1.getS_id()<<": "<<s1.getS_status()<<std::endl;
    std::cout<<"Status von "<<v1.getV_id()<<": "<<v1.getV_status()<<" mit getinFS: "<<v1.getinFS()<<std::endl;
    p = v1.getAktFS();
    std::cout<<"Status/Verriegelung von Weiche"<<w1.getW_id()<<" ist: "<<w1.getW_status()<<"/"<<w1.getVerriegelung()<<std::endl;
    std::cout<<"v1 first/second: "<<p.first<<"/"<<p.second<<std::endl;
    std::cout<<"Status von "<<s2.getS_id()<<": "<<s2.getS_status()<<std::endl;
    std::cout<<"Status von "<<v2.getV_id()<<": "<<v2.getV_status()<<" mit getinFS: "<<v2.getinFS()<<std::endl;
    q = v2.getAktFS();
    std::cout<<"v2 first/second: "<<q.first<<"/"<<q.second<<std::endl;
    std::cout<<"Status von "<<s5.getS_id()<<": "<<s5.getS_status()<<std::endl;
    std::cout<<"Status/Freigabe von Blöcken aa = "<<aa.getB_status()<<"/"<<aa.getFreigabe()<<"|ab= "<<ab.getB_status()<<"/"<<ab.getFreigabe()<<"|ac= "<<ac.getB_status()<<"/"<<ac.getFreigabe()<<"|ad= "<<ad.getB_status()<<"/"<<ad.getFreigabe()<<"|ae= "<<ae.getB_status()<<"/"<<ae.getFreigabe()<<"|af= "<<af.getB_status()<<"/"<<af.getFreigabe()<<"|ag= "<<ag.getB_status()<<"/"<<ag.getFreigabe()<<std::endl;
    stellwerkstec.show_Signal();
    std::cout<<"-------------------------------------------------"<<std::endl;
    std::cout<<"Zug fährt die FS ab, nun belegt er auch  Block ac:"<<std::endl;
    ac.setB_status(false);
    std::cout<<"Status von "<<s1.getS_id()<<": "<<s1.getS_status()<<std::endl;
    std::cout<<"Status von "<<v1.getV_id()<<": "<<v1.getV_status()<<" mit getinFS: "<<v1.getinFS()<<std::endl;
    p = v1.getAktFS();
    std::cout<<"Status/Verriegelung von Weiche"<<w1.getW_id()<<" ist: "<<w1.getW_status()<<"/"<<w1.getVerriegelung()<<std::endl;
    std::cout<<"v1 first/second: "<<p.first<<"/"<<p.second<<std::endl;
    std::cout<<"Status von "<<s2.getS_id()<<": "<<s2.getS_status()<<std::endl;
    std::cout<<"Status von "<<v2.getV_id()<<": "<<v2.getV_status()<<" mit getinFS: "<<v2.getinFS()<<std::endl;
    q = v2.getAktFS();
    std::cout<<"v2 first/second: "<<q.first<<"/"<<q.second<<std::endl;
    std::cout<<"Status von "<<s5.getS_id()<<": "<<s5.getS_status()<<std::endl;
    std::cout<<"Status/Freigabe von Blöcken aa = "<<aa.getB_status()<<"/"<<aa.getFreigabe()<<"|ab= "<<ab.getB_status()<<"/"<<ab.getFreigabe()<<"|ac= "<<ac.getB_status()<<"/"<<ac.getFreigabe()<<"|ad= "<<ad.getB_status()<<"/"<<ad.getFreigabe()<<"|ae= "<<ae.getB_status()<<"/"<<ae.getFreigabe()<<"|af= "<<af.getB_status()<<"/"<<af.getFreigabe()<<"|ag= "<<ag.getB_status()<<"/"<<ag.getFreigabe()<<std::endl;
    stellwerkstec.show_Signal();
    std::cout<<"-------------------------------------------------"<<std::endl;
    std::cout<<"Zug fährt die FS ab, nun belegt er auch  Block ae, aa gibt er frei:"<<std::endl;
    aa.setB_status(true);
    ae.setB_status(false);
    std::cout<<"Status von "<<s1.getS_id()<<": "<<s1.getS_status()<<std::endl;
    std::cout<<"Status von "<<v1.getV_id()<<": "<<v1.getV_status()<<" mit getinFS: "<<v1.getinFS()<<std::endl;
    p = v1.getAktFS();
    std::cout<<"Status/Verriegelung von Weiche"<<w1.getW_id()<<" ist: "<<w1.getW_status()<<"/"<<w1.getVerriegelung()<<std::endl;
    std::cout<<"v1 first/second: "<<p.first<<"/"<<p.second<<std::endl;
    std::cout<<"Status von "<<s2.getS_id()<<": "<<s2.getS_status()<<std::endl;
    std::cout<<"Status von "<<v2.getV_id()<<": "<<v2.getV_status()<<" mit getinFS: "<<v2.getinFS()<<std::endl;
    q = v2.getAktFS();
    std::cout<<"v2 first/second: "<<q.first<<"/"<<q.second<<std::endl;
    std::cout<<"Status von "<<s5.getS_id()<<": "<<s5.getS_status()<<std::endl;
    std::cout<<"Status/Freigabe von Blöcken aa = "<<aa.getB_status()<<"/"<<aa.getFreigabe()<<"|ab= "<<ab.getB_status()<<"/"<<ab.getFreigabe()<<"|ac= "<<ac.getB_status()<<"/"<<ac.getFreigabe()<<"|ad= "<<ad.getB_status()<<"/"<<ad.getFreigabe()<<"|ae= "<<ae.getB_status()<<"/"<<ae.getFreigabe()<<"|af= "<<af.getB_status()<<"/"<<af.getFreigabe()<<"|ag= "<<ag.getB_status()<<"/"<<ag.getFreigabe()<<std::endl;
    stellwerkstec.show_Signal();
    std::cout<<"-------------------------------------------------"<<std::endl;
    std::cout<<"Zug fährt die FS ab, nun belegt er auch  Block ag:"<<std::endl;
    ag.setB_status(false);
    std::cout<<"Status von "<<s1.getS_id()<<": "<<s1.getS_status()<<std::endl;
    std::cout<<"Status von "<<v1.getV_id()<<": "<<v1.getV_status()<<" mit getinFS: "<<v1.getinFS()<<std::endl;
    p = v1.getAktFS();
    std::cout<<"Status/Verriegelung von Weiche"<<w1.getW_id()<<" ist: "<<w1.getW_status()<<"/"<<w1.getVerriegelung()<<std::endl;
    std::cout<<"v1 first/second: "<<p.first<<"/"<<p.second<<std::endl;
    std::cout<<"Status von "<<s2.getS_id()<<": "<<s2.getS_status()<<std::endl;
    std::cout<<"Status von "<<v2.getV_id()<<": "<<v2.getV_status()<<" mit getinFS: "<<v2.getinFS()<<std::endl;
    q = v2.getAktFS();
    std::cout<<"v2 first/second: "<<q.first<<"/"<<q.second<<std::endl;
    std::cout<<"Status von "<<s5.getS_id()<<": "<<s5.getS_status()<<std::endl;
    std::cout<<"Status/Freigabe von Blöcken aa = "<<aa.getB_status()<<"/"<<aa.getFreigabe()<<"|ab= "<<ab.getB_status()<<"/"<<ab.getFreigabe()<<"|ac= "<<ac.getB_status()<<"/"<<ac.getFreigabe()<<"|ad= "<<ad.getB_status()<<"/"<<ad.getFreigabe()<<"|ae= "<<ae.getB_status()<<"/"<<ae.getFreigabe()<<"|af= "<<af.getB_status()<<"/"<<af.getFreigabe()<<"|ag= "<<ag.getB_status()<<"/"<<ag.getFreigabe()<<std::endl;
    stellwerkstec.show_Signal();
    std::cout<<"-------------------------------------------------"<<std::endl;
    std::cout<<"Zug fährt die FS ab, nun gibt er auch ac und Weiche 001 frei(eig unreal wegen S005->Hp0) :"<<std::endl;
    ac.setB_status(true);
    w1.setBelegung(true);
    std::cout<<"Status von "<<s1.getS_id()<<": "<<s1.getS_status()<<std::endl;
    std::cout<<"Status von "<<v1.getV_id()<<": "<<v1.getV_status()<<" mit getinFS: "<<v1.getinFS()<<std::endl;
    p = v1.getAktFS();
    std::cout<<"Status/Verriegelung von Weiche"<<w1.getW_id()<<" ist: "<<w1.getW_status()<<"/"<<w1.getVerriegelung()<<std::endl;
    std::cout<<"v1 first/second: "<<p.first<<"/"<<p.second<<std::endl;
    std::cout<<"Status von "<<s2.getS_id()<<": "<<s2.getS_status()<<std::endl;
    std::cout<<"Status von "<<v2.getV_id()<<": "<<v2.getV_status()<<" mit getinFS: "<<v2.getinFS()<<std::endl;
    q = v2.getAktFS();
    std::cout<<"v2 first/second: "<<q.first<<"/"<<q.second<<std::endl;
    std::cout<<"Status von "<<s5.getS_id()<<": "<<s5.getS_status()<<std::endl;
    std::cout<<"Status/Freigabe von Blöcken aa = "<<aa.getB_status()<<"/"<<aa.getFreigabe()<<"|ab= "<<ab.getB_status()<<"/"<<ab.getFreigabe()<<"|ac= "<<ac.getB_status()<<"/"<<ac.getFreigabe()<<"|ad= "<<ad.getB_status()<<"/"<<ad.getFreigabe()<<"|ae= "<<ae.getB_status()<<"/"<<ae.getFreigabe()<<"|af= "<<af.getB_status()<<"/"<<af.getFreigabe()<<"|ag= "<<ag.getB_status()<<"/"<<ag.getFreigabe()<<std::endl;
    stellwerkstec.show_Signal();
    std::cout<<"-------------------------------------------------"<<std::endl;
    }
  if( menue == 12){
      std::cout<<"Status/Verriegelung/Belegung von Weiche "<<w1.getW_id()<<" = "<<w1.getW_status()<<"/"<<w1.getVerriegelung()<<"/"<<w1.getBelegung()<<std::endl;
      std::cout<<"Nach dem Umstellen:"<<std::endl;
      w1.setW_status(false);
      std::cout<<"Status/Verriegelung/Belegung von Weiche "<<w1.getW_id()<<" = "<<w1.getW_status()<<"/"<<w1.getVerriegelung()<<"/"<<w1.getBelegung()<<std::endl;
      std::cout<<"Nach dem Belegen: "<<std::endl;
      w1.setBelegung(false);
      std::cout<<"Status/Verriegelung/Belegung von Weiche "<<w1.getW_id()<<" = "<<w1.getW_status()<<"/"<<w1.getVerriegelung()<<"/"<<w1.getBelegung()<<std::endl;
      std::cout<<"Nach dem Verriegeln: "<<std::endl;
      w1.setVerriegelung(true);
      std::cout<<"Status/Verriegelung/Belegung von Weiche "<<w1.getW_id()<<" = "<<w1.getW_status()<<"/"<<w1.getVerriegelung()<<"/"<<w1.getBelegung()<<std::endl;
      std::cout<<"++++++++++++++++++++++++++++++++++RESET+++++++++++++++++++++++++++++++++++++++++++++++ "<<std::endl;
      w1.setBelegung(true);
      w1.setVerriegelung(false);
      std::cout<<"Status/Verriegelung/Belegung von Weiche "<<w1.getW_id()<<" = "<<w1.getW_status()<<"/"<<w1.getVerriegelung()<<"/"<<w1.getBelegung()<<std::endl;
      std::cout<<"Nach dem Verriegeln: "<<std::endl;
      w1.setVerriegelung(true);
      std::cout<<"Status/Verriegelung/Belegung von Weiche "<<w1.getW_id()<<" = "<<w1.getW_status()<<"/"<<w1.getVerriegelung()<<"/"<<w1.getBelegung()<<std::endl;

  }
  if(menue == 13){
        QObject::connect(&ac,SIGNAL(releaseSpeicher()),&s2,SLOT(processSpeicher()) );//in eine zeile drunter verpacken, dann ist es auch schon wieder weniger arbeit
        ac.addcontrolspeicher(true);
        s1.setFahrt(s2ptr);
        s1.setFahrt(s2ptr);
        aa.setB_status(false);
        w1.setBelegung(false);
        s3.zugpassiert();
        ac.setB_status(false);
        aa.setB_status(true);
        w1.setBelegung(true);
        ww1.zugpassiert();
        //w2.zugpassiert();
        ac.setB_status(true);
        std::cout<<"aa "<<aa.getB_status()<<std::endl;
        std::cout<<"ac "<<ac.getB_status()<<std::endl;
        //s3.showBlockALL();
  }
  if(menue == 14){//Testing wiringPi in the program (wahrscheinlich linken, dann auf den Pi und da ausführbar testen, denn hier hat man ja keine gpio...)
        wiringPiSetupGpio();
        //output test
        //int led = 27;//set the right pin
        //pinMode(led,OUTPUT);
        //digitalWrite(led,HIGH);
        //delay(5000);//[ms]
        //digitalWrite(led,LOW);
        //input test
        pinMode(in,INPUT);//set input pin here
        wiringPiISR(in,INT_EDGE_RISING,*interrupt);

        //Notes ....
        ////////////////////////////////////////////////////////////////
        //von der Pi Seite betrachtet:
      //INPUT = sich ständig ändernder Blockstatus
      //OUTPUT= Weichen; Signale, Vorsignale, BÜs, Verriegelungen usw
      // --> Lösung über stellwerkstechnik, in der alle Block-Status ebenfalls zusammenlaufen und den entsprechenden Blöcken als Änderung (SLOT) aufgerufen wird.

      //wiringpiisr nutzen
      /*int wiringPiISR (int pin, int edgeType,  void (*function)(void)) ;
      Trigger trigger;
      void interrupt()
      {
          emit trigger.triggered();
      }
      wiringPiISR(0, INT_EDGE_BOTH, *interrupt);
      // ----------------------------------------------------------------------
      As Paul said, Interrupts must be defined just in the begin of the program.
      Example:
      wiringPiISR (pinx, INT_EDGE_BOTH, &my_intr_function);

      Then, in the my_intr_function you can read pinx as normal GPIO to find its rising or falling.
      Example:
      if ( digitalRead(pinx) == 1) rising_edge_Interrupt_function(); else falling_edge_Interrupt_function();*/
      //interrupt();//see beginning of main...
      //eher int gpioSetAlertFunc(unsigned user_gpio, gpioAlertFunc_t f) pigpio lib

  }

  if(menue == 15){
    //zugpassiert Test WS->WS
      ww1.setFahrt(ww1ptr);
      aa.setB_status(false);
      ac.setB_status(false);
  }
  if(menue == 16){
    WSignal *ww3ptr = new WSignal(3);
    std::vector<Block*> s2tow3b;
    s2tow3b.push_back(aeptr);
    s2ptr->addBlockZuRangier(ww3ptr,s2tow3b);
    s1.setFahrt(s2ptr);
    s2.setFahrt(ww3ptr);
  }
  if( menue == 17){//HS/WS->WS/HS deleteFS
      s1.setFahrt(ww1ptr);
      s1.deleteFS();
      ww2.setFahrt("S002");
      ww2.deleteFS();
  }

  //create view
      QGraphicsView *view = new QGraphicsView(scene);
      view->setWindowTitle("Elektronisches Stellwerk Modelleisenbahn");
      //view->showMaximized();
      w->setCentralWidget(view);//add view to mainwindow
      w->setWindowTitle("Elektronisches Stellwerk Modelleisenbahn");
      w->showMaximized();
      //SetViewport(new QGLWidget)
  return a.exec();

}


