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
#include <thread>
#include "myqueue.h"
#include "Block.h"
#include "Weiche.h"
#include "HSignal.h"
#include "VSignal.h"
#include "WSignal.h"
#include "worker.h"
#include "Spmemory.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QtSvg>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QThread>
#include <QtGlobal> // for QOverload
#include "clickmanager.h"
#include "util.cpp"
#ifdef __cplusplus
extern "C"{
#endif
#include <wiringPi.h>
#include <sr595.h>
#ifdef __cplusplus
}
#endif


//Stellwerkstechnik+Block as basis
Stellwerkstechnik stellwerkstec;
Stellwerkstechnik *stellwerkstecptr = &stellwerkstec;
Block aa("aa", stellwerkstecptr);
Block *aaptr = &aa;


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
  std::cout<<"***10) normale Initialisierung                            ***"<<std::endl;
  std::cout<<"***11) Test für zugpassiert durch eine Fahrt (s. 2))      ***"<<std::endl;
  std::cout<<"***12) Weichentest                                        ***"<<std::endl;
  std::cout<<"***13) Test: Einwählen des Speichers nach freier FS       ***"<<std::endl;
  std::cout<<"***14) wiringPi Test                                      ***"<<std::endl;
  std::cout<<"***15) ZugpassiertTest -> wegen Neuerungen wie WS->WS...  ***"<<std::endl;
  std::cout<<"***16) HS->WS: was VS macht [nicht TestPlan anwendbar ]   ***"<<std::endl;
  std::cout<<"***17) Beweis, dass deleteFS auch bei HS/WS->WS/HS klappt ***"<<std::endl;
  std::cout<<"***18) ThreadTest: userInput + Statusänderungen+ Speicher ***"<<std::endl;
  std::cout<<"***19) QueueTest                                          ***"<<std::endl;
  std::cout<<"***20) Test für Stellwerkstechnik und WSignale            ***"<<std::endl;
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
  VSignal *v2ptr = &v2;
  VSignal v3(3);
  VSignal *v3ptr = &v3;
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
  QGraphicsSvgItem *item3 = new QGraphicsSvgItem(":/resources/WSfahrt.svg");
  QGraphicsSvgItem *item4 = new QGraphicsSvgItem(":/resources/WShalt.svg");
  scene->addItem(item3);
  scene->addItem(item4);
  item3->setPos(QPointF(-280,0));
  item4->setPos(QPointF(-280,0));
  item3->setRotation(-90);
  item4->setRotation(-90);
  item3->setScale(0.05);
  item4->setScale(0.05);
  ww1ptr->addVSignalitems(item3, item4);
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
  s2.addWSignal(ww1ptr,"r");
  //
  QGraphicsSvgItem *item5 = new QGraphicsSvgItem(":/resources/WSfahrt.svg");
  QGraphicsSvgItem *item6 = new QGraphicsSvgItem(":/resources/WShalt.svg");
  scene->addItem(item5);
  scene->addItem(item6);
  item5->setPos(QPointF(-615,50));
  item6->setPos(QPointF(-615,50));
  item5->setRotation(-90);
  item6->setRotation(-90);
  item5->setScale(0.05);
  item6->setScale(0.05);
  ww2ptr->addVSignalitems(item5, item6);
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

  QObject::connect(&s1,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s2,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s3,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s4,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s5,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&ww1,&WSignal::refreshStellwerkstechnikW,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&ww2,&WSignal::refreshStellwerkstechnikW,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&ad,&Block::zugpassiert,&s3,&HSignal::zugpassiert);
  QObject::connect(&ae,&Block::zugpassiert,&s2,&HSignal::zugpassiert);
  QObject::connect(&ae,&Block::zugpassiertW,&ww2,&WSignal::zugpassiertW);
  QObject::connect(&ac,&Block::zugpassiertW,&ww1,&WSignal::zugpassiertW);
  /*QObject::connect(&s1,SIGNAL(refreshStellwerkstechnik(std::string,bool)),&stellwerkstec,SLOT(add_Signal(std::string , bool) ));
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
  */
  //Hier wird das Grenzsignal übergeben
  ad.addpassiert(s3.getS_id(),abptr);
  ae.addpassiert(s2.getS_id(),acptr);
  ae.addpassiert(ww2.getV_id(),acptr);
  ac.addpassiert(ww1.getV_id(),aaptr);
  //connect(
  // sender, &Sender::valueChanged,
  // receiver, &Receiver::updateValue
  //);

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
  QGraphicsSvgItem *item1 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *item2 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(item1);
  scene->addItem(item2);
  item1->setPos(QPointF(-340,0));
  item2->setPos(QPointF(-340,0));
  item1->setScale(0.1);
  item2->setScale(0.1);
  item1->setRotation(-90);
  item2->setRotation(-90);
  v1ptr->addVSignalitems(item1, item2);
  //
  QGraphicsSvgItem *item7 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *item8 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(item7);
  scene->addItem(item8);
  item7->setPos(QPointF(-1150,0));
  item8->setPos(QPointF(-1150,0));
  item7->setScale(0.1);
  item8->setScale(0.1);
  item7->setRotation(-90);
  item8->setRotation(-90);
  v3ptr->addVSignalitems(item7, item8);
  //
  QGraphicsSvgItem *item9 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *item10 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(item9);
  scene->addItem(item10);
  item9->setPos(QPointF(-1150,50));
  item10->setPos(QPointF(-1150,50));
  item9->setScale(0.1);
  item10->setScale(0.1);
  item9->setRotation(-90);
  item10->setRotation(-90);
  v2ptr->addVSignalitems(item9, item10);
  //
  QGraphicsSvgItem *s1item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s1item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s1item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s1item1);
  scene->addItem(s1item3);
  scene->addItem(s1item2);
  s1item1->setPos(QPointF(-30,0));
  s1item2->setPos(QPointF(-30,0));
  s1item3->setPos(QPointF(-30,0));
  s1item1->setScale(0.1);
  s1item2->setScale(0.1);
  s1item3->setScale(0.1);
  s1item1->setRotation(-90);
  s1item2->setRotation(-90);
  s1item3->setRotation(-90);
  
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
  s1.addHSignalitem(s1item1, s1item2, s1item3, s1label,s1push,spweg,sphin);
  s1.moveLabel(10,0);
  //
  QGraphicsSvgItem *s3item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s3item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s3item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s3item1);
  scene->addItem(s3item3);
  scene->addItem(s3item2);
  s3item1->setPos(QPointF(-790,0));
  s3item2->setPos(QPointF(-790,0));
  s3item3->setPos(QPointF(-790,0));
  s3item1->setScale(0.1);
  s3item2->setScale(0.1);
  s3item3->setScale(0.1);
  s3item1->setRotation(-90);
  s3item2->setRotation(-90);
  s3item3->setRotation(-90);
  
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

  s3.addHSignalitem(s3item1, s3item2, s3item3, s3label,s3push,sphin3,spweg3);
  s3.moveLabel(-760,0);
  s3.moveButton(-775,0);
  //
  QGraphicsSvgItem *s4item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s4item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s4item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s4item1);
  scene->addItem(s4item3);
  scene->addItem(s4item2);
  s4item1->setPos(QPointF(-1460,0));
  s4item2->setPos(QPointF(-1460,0));
  s4item3->setPos(QPointF(-1460,0));
  s4item1->setScale(0.1);
  s4item2->setScale(0.1);
  s4item3->setScale(0.1);
  s4item1->setRotation(-90);
  s4item2->setRotation(-90);
  s4item3->setRotation(-90);
  
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
  s4.addHSignalitem(s4item1,s4item2, s4item3, s4label,s4push,spweg4,sphin4);
  s4.moveLabel(-1410,0);
  s4.moveButton(-1420,0);
  //
  QGraphicsSvgItem *s2item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s2item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s2item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s2item1);
  scene->addItem(s2item3);
  scene->addItem(s2item2);
  s2item1->setPos(QPointF(-790,50));
  s2item2->setPos(QPointF(-790,50));
  s2item3->setPos(QPointF(-790,50));
  s2item1->setScale(0.1);
  s2item2->setScale(0.1);
  s2item3->setScale(0.1);
  s2item1->setRotation(-90);
  s2item2->setRotation(-90);
  s2item3->setRotation(-90);
  
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
  s2.addHSignalitem(s2item1, s2item2, s2item3, s2label,s2push,spweg2,sphin2);
  s2.moveLabel(-760,50);
  s2.moveButton(-775,50);
  //
  QGraphicsSvgItem *s5item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s5item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s5item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s5item1);
  scene->addItem(s5item3);
  scene->addItem(s5item2);
  s5item1->setPos(QPointF(-1460,50));
  s5item2->setPos(QPointF(-1460,50));
  s5item3->setPos(QPointF(-1460,50));
  s5item1->setScale(0.1);
  s5item2->setScale(0.1);
  s5item3->setScale(0.1);
  s5item1->setRotation(-90);
  s5item2->setRotation(-90);
  s5item3->setRotation(-90);
  
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
  s5.addHSignalitem(s5item1, s5item2, s5item3, s5label,s5push,spweg5,sphin5);
  s5.moveLabel(-1410,50);
  s5.moveButton(-1420,50);

  //QPushButtonPart
  clickmanager *c1 = new clickmanager;
  QObject::connect(&s1,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) ); //QOverload wegen zeideutigkeit von recieveFS; C++11, C++14 qOverload<>
  QObject::connect(&s2,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s3,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s4,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s5,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  //ebenfalls für WSignal
  QObject::connect(&ww1,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&ww2,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  //:::::::::


  /////// Thread start
  //initialisiere pins als in/out-put + speichere in worker eine Liste von pins
  QThread* thread = new QThread;
  worker* wrkr = new worker();
  wrkr->moveToThread(thread);
  QObject::connect(thread, &QThread::started, wrkr, &worker::updateBelegt);
  QObject::connect(wrkr, &worker::finished, thread, &QThread::quit);
  thread->start();
  //thread endet theoretisch auch über wrkr.quit() -> updateBelegt() -> emit finished -> oberer slot
      
  QThread* thread2 = new QThread;
  Spmemory *mem = new Spmemory();
  mem->moveToThread(thread2); //erst thread affinity -> also erst movetothread, dann connections
  QObject::connect(thread2, &QThread::started, mem, &Spmemory::timing);
  QObject::connect(mem, &Spmemory::finished, thread2, &QThread::quit);
  //spmemory connection 
  bool habsgetestet = false;
  
  habsgetestet = QObject::connect(&s1, &HSignal::callspmemory,mem, &Spmemory::addFS);
  std::cout<<" HABS GETESTET = "<<habsgetestet<<std::endl;
  QObject::connect(&s2, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(&s3, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(&s4, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(&s5, &HSignal::callspmemory,mem, &Spmemory::addFS);
  thread2->start();
  //mem->quit(); // zu testzwecken
  
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
    stellwerkstec.show_Signal();
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
        s1.setFahrt(s2ptr);
	s2.setFahrt(s5ptr);
	s1.setFahrt(s2ptr);
	aa.setB_status(false);
        w1.setBelegung(false);
        s1.zugpassiert();
        ac.setB_status(false);
        aa.setB_status(true);
        w1.setBelegung(true);
	ae.setB_status(false);
        ww1.zugpassiert();
        ww2.zugpassiert();
        ac.setB_status(true);
	ag.setB_status(false);
	ae.setB_status(true);
	ag.setB_status(true);
	std::cout<<"aa "<<aa.getB_status()<<std::endl;
        std::cout<<"ac "<<ac.getB_status()<<std::endl;	
	//mem->quit();
  }
  if(menue == 14){//Testing wiringPi in the program 
    wiringPiSetupGpio();//BCM numbering
        //output test
        //int led = 27;//set the right pin
        //pinMode(led,OUTPUT);
        //digitalWrite(led,HIGH);
        //delay(5000);//[ms]
        //digitalWrite(led,LOW);
        //input test
        //pinMode(in,INPUT);//set input pin here
        //wiringPiISR(in,INT_EDGE_RISING,*interrupt);

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
      stellwerkstec.show_Signal();
      s1.deleteFS();
      ww2.setFahrt("S002");
      ww2.deleteFS();
  }
  if(menue == 18){//Thread Test
      std::thread blockthread( actrandomly );
      std::thread userthread ( user );
      userthread.join();
      blockthread.join();
  }
  if(menue == 19){//Queue Test
      qRegisterMetaType < std::string >("std::string");//connected to signal and slots in threads...
      myqueue *q1 = new myqueue();
      q1->add(s1ptr,s2ptr);
      q1->add(s2ptr,s5ptr);
      q1->start();
      //q1->terminate();
  }
  if( menue == 20){
    ww1ptr->setFahrt(ww2ptr);
    stellwerkstec.show_Signal();
    aa.setB_status(false);
    ac.setB_status(false);    
    //ww1ptr->deleteFS();
    stellwerkstec.show_Signal();
    
  } 
      //create view
      QGraphicsView *view = new QGraphicsView(scene);
      view->setWindowTitle("Electronic Signalling Control Center - Model Railway");
      //view->showMaximized();
      w->setCentralWidget(view);//add view to mainwindow
      w->setWindowTitle("Electronic Signalling Control Center - Model Railway");
      w->showMaximized();
      //SetViewport(new QGLWidget)
  return a.exec();

}
