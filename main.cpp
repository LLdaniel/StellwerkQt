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
#include <QDebug>
#include <iostream>
#include "Block.h"
#include "Weiche.h"
#include "HSignal.h"
#include "VSignal.h"
#include "WSignal.h"
#include "worker.h"
#include "Spmemory.h"
#include <QString>
#include <QPair>
#include <QList>
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
#ifdef __cplusplus
extern "C"{
#endif
#include <wiringPi.h>
#include <sr595.h>
#ifdef __cplusplus
}
#endif

int main( int argc , char *argv[] ){
  //parameter Übergabe
  //bool debug = false;
  printf("argc: %d\n", argc);
  for(int i=0; i < argc; i++) {
    printf("argv[%d]: %s\n", i, argv[i]);
    //-v debug option with different std's
    if( strcmp(argv[i],"-v") == 0 ){
      qDebug() <<"__MAIN__: verbose output mode";
      //debug = true;
    }
    if(strcmp(argv[i],"--help") == 0 ){
      qDebug()<<" #################################################";
      qDebug()<<" ###              StellwerkQt Help             ###";
      qDebug()<<" ### ----------------------------------------- ###";
      qDebug()<<" ### usage: StellwerkQt -[option]              ###";
      qDebug()<<" ### -platform xcb: use when problems with gl  ###";
      qDebug()<<" ### --help:        show this help             ###";
      qDebug()<<" ### -v:            verbose debug mode         ###";
      qDebug()<<" ### -d   :         shows test/debug menu      ###";
      qDebug()<<" #################################################";
      return 0;
    }
    if(strcmp(argv[i],"-d") == 0 ){
      
    }
  }
  //::Eigentlicher Start main
  QApplication a(argc, argv);
  MainWindow w;
  //create scene
  QGraphicsScene *scene = new QGraphicsScene(&w);
  scene->setBackgroundBrush(Qt::black);
  // connect for quiting program
  QObject::connect(&w,&MainWindow::shutdown,&a,QApplication::quit);

  //create view
  QGraphicsView *view = new QGraphicsView(scene);
  view->setWindowTitle("Electronic Signalling Control Center - Model Railway");
  //view->showMaximized();
  w.setCentralWidget(view);//add view to mainwindow
  w.setWindowTitle("Electronic Signalling Control Center - Model Railway");
  view->scale(1.0,1.0);
    
  //Initialisierung der Strecke--------------------------------------------------------
  //Stellwerkstechnik+Block aa as basis
  Stellwerkstechnik stellwerkstec;
  Stellwerkstechnik *stellwerkstecptr = &stellwerkstec;
  Block aa("aa", stellwerkstecptr);
  Block *aaptr = &aa;
  //:::Weichen:::
  Weiche w1(1);
  Weiche *w1ptr = &w1;
  Weiche w2(2);
  Weiche *w2ptr = &w2;
  Weiche w3(3);
  Weiche *w3ptr = &w3;
  Weiche w4(4);
  Weiche *w4ptr = &w4;
  Weiche w5(5);
  Weiche *w5ptr = &w5;
  Weiche w6(6);
  Weiche *w6ptr = &w6;
  Weiche w7(7);
  Weiche *w7ptr = &w7;
  Weiche w8(8);
  Weiche *w8ptr = &w8;
  Weiche w9(9);
  Weiche *w9ptr = &w9;
  Weiche w10(10);
  Weiche *w10ptr = &w10;
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
  Block ah("ah", stellwerkstecptr);
  Block *ahptr = &ah;
  Block ai("ai", stellwerkstecptr);
  Block *aiptr = &ai;
  //:::VS:::
  VSignal v1(1);
  VSignal *v1ptr = &v1;
  VSignal v2(2);
  VSignal *v2ptr = &v2;
  VSignal v3(3);
  VSignal *v3ptr = &v3;
  VSignal v4(4);
  VSignal *v4ptr = &v4;
  VSignal v5(5);
  VSignal *v5ptr = &v5;
  VSignal v6(6);
  VSignal *v6ptr = &v6;
  VSignal v7(7);
  VSignal *v7ptr = &v7;
  VSignal v8(8);
  VSignal *v8ptr = &v8;
  VSignal v9(9);
  VSignal *v9ptr = &v9;
  VSignal v10(10);
  VSignal *v10ptr = &v10;
  v1.setRichtung("S001","S004");
  v2.setRichtung("S013","S002");
  v3.setRichtung("S004","S015");
  v3.setRichtung("S004","S007");
  v4.setRichtung("S008","S002");
  //v4.setRichtung("S0","S002");
  //v4.setRichtung("S00V","S002");
  //v4.setRichtung("S00X","S002");
  //v4.setRichtung("S00Y","S002");
  v5.setRichtung("S004","S014");
  v6.setRichtung("S004","S012");
  v6.setRichtung("S004","S010");
  v6.setRichtung("S005","S012");
  v6.setRichtung("S005","S010");
  v7.setRichtung("S011","S003");
  v7.setRichtung("S009","S003");
  v7.setRichtung("S011","S002");
  v7.setRichtung("S009","S002");
  //v8.setRichtung("S007","");
  //v9.setRichtung("S006","");
  //v10.setRichtung("S00X","S013");
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
  HSignal s6(6);
  HSignal *s6ptr = &s6;
  HSignal s7(7);
  HSignal *s7ptr = &s7;
  HSignal s8(8);
  HSignal *s8ptr = &s8;
  HSignal s9(9);
  HSignal *s9ptr = &s9;
  HSignal s10(10);
  HSignal *s10ptr = &s10;
  HSignal s11(11);
  HSignal *s11ptr = &s11;
  HSignal s12(12);
  HSignal *s12ptr = &s12;
  HSignal s13(13);
  HSignal *s13ptr = &s13;
  HSignal s14(14);
  HSignal *s14ptr = &s14;
  HSignal s15(15);
  HSignal *s15ptr = &s15;
  //:::::::::
  s1.addVSignal(v1ptr,"v");
  //s2.addVSignal(v2ptr,"v");
  //s2.addVSignal(v2ptr,"r");
  //s2.addVSignal(v3ptr,"r");
  //s3.addVSignal(v3ptr,"v");
  //s3.addVSignal(v1ptr,"r");
  s4.addVSignal(v1ptr,"r");
  s4.addVSignal(v3ptr,"v");
  s4.addVSignal(v5ptr,"v");
  //s5.addVSignal(v2ptr,"r");
  s5.addVSignal(v6ptr,"v");
  //s6.addVSignal(v3ptr,"v");
  s6.addVSignal(v4ptr,"r");
  s7.addVSignal(v3ptr,"r");
  s8.addVSignal(v3ptr,"v");
  s8.addVSignal(v4ptr,"v");
  s9.addVSignal(v7ptr,"v");
  s10.addVSignal(v6ptr,"r");
  s11.addVSignal(v7ptr,"v");
  s12.addVSignal(v6ptr,"r");
  s13.addVSignal(v2ptr,"v");
  s13.addVSignal(v10ptr,"r");
  //:::::::::
  //WSignale
  WSignal ww1 (1);
  WSignal *ww1ptr = &ww1;
  ww1.setRichtung("S009","S002");
  ww1.setRichtung("S009","S003");
  ww1.setRichtung("S011","S002");
  ww1.setRichtung("S011","S003");
  s9.addWSignal(ww1ptr,"v");
  s11.addWSignal(ww1ptr,"v");
  s2.addWSignal(ww1ptr,"r");
  s3.addWSignal(ww1ptr,"r");
  //
  QGraphicsSvgItem *item3 = new QGraphicsSvgItem(":/resources/WSfahrt.svg");
  QGraphicsSvgItem *item4 = new QGraphicsSvgItem(":/resources/WShalt.svg");
  scene->addItem(item3);
  scene->addItem(item4);
  item3->setPos(QPointF(-870,815));
  item4->setPos(QPointF(-870,815));
  item3->setRotation(90);
  item4->setRotation(90);
  item3->setScale(0.05);
  item4->setScale(0.05);
  ww1ptr->addVSignalitems(item3, item4);
  QLabel *ww1label = new QLabel();
  scene->addWidget(ww1label);
  //
  WSignal ww2 (2);
  WSignal *ww2ptr = &ww2;
  ww2.setRichtung("S004","S012");
  ww2.setRichtung("S004","S010");
  ww2.setRichtung("S005","S012");
  ww2.setRichtung("S005","S010");
  s4.addWSignal(ww2ptr,"v");
  s5.addWSignal(ww2ptr,"v");
  s10.addWSignal(ww2ptr,"r");
  s12.addWSignal(ww2ptr,"r");
  //
  QGraphicsSvgItem *item5 = new QGraphicsSvgItem(":/resources/WSfahrt.svg");
  QGraphicsSvgItem *item6 = new QGraphicsSvgItem(":/resources/WShalt.svg");
  scene->addItem(item5);
  scene->addItem(item6);
  item5->setPos(QPointF(-950,205));
  item6->setPos(QPointF(-950,205));
  item5->setRotation(90);
  item6->setRotation(90);
  item5->setScale(0.05);
  item6->setScale(0.05);
  ww2ptr->addVSignalitems(item5, item6);
  QLabel *ww2label = new QLabel();
  scene->addWidget(ww2label);

  //Weichenstatus ---------------------------------------------------
  QPair<Weiche*, bool> s1tos4w1(w2ptr,true);
  QList<QPair<Weiche*, bool>> s1tos4;
  s1tos4.push_back(s1tos4w1);
  s1.addWeichenstatus(s4ptr,s1tos4);

  QPair<Weiche*, bool> s4tos7w3(w3ptr,true);
  QPair<Weiche*, bool> s4tos7w5(w5ptr,false);
  QPair<Weiche*, bool> s4tos7w6(w6ptr,true);
  QPair<Weiche*, bool> s4tos7w7(w7ptr,false);
  QList<QPair<Weiche*, bool>> s4tos7;
  s4tos7.push_back(s4tos7w3);
  s4tos7.push_back(s4tos7w5);
  s4tos7.push_back(s4tos7w6);
  s4tos7.push_back(s4tos7w7);
  s4.addWeichenstatus(s7ptr,s4tos7);

  QPair<Weiche*, bool> s4tos15w3(w3ptr,true);
  QPair<Weiche*, bool> s4tos15w5(w5ptr,false);
  QPair<Weiche*, bool> s4tos15w6(w6ptr,true);
  QPair<Weiche*, bool> s4tos15w7(w7ptr,true);
  QList<QPair<Weiche*, bool>> s4tos15;
  s4tos15.push_back(s4tos15w3);
  s4tos15.push_back(s4tos15w5);
  s4tos15.push_back(s4tos15w6);
  s4tos15.push_back(s4tos15w7);
  s4.addWeichenstatus(s15ptr,s4tos15);

  QPair<Weiche*, bool> s4tos14w3(w3ptr,true);
  QPair<Weiche*, bool> s4tos14w5(w5ptr,true);
  QList<QPair<Weiche*, bool>> s4tos14;
  s4tos14.push_back(s4tos14w3);
  s4tos14.push_back(s4tos14w5);
  s4.addWeichenstatus(s14ptr,s4tos14);

  QPair<Weiche*, bool> s4tos10w3(w3ptr,false);
  QPair<Weiche*, bool> s4tos10w4(w4ptr,false);
  QPair<Weiche*, bool> s4tos10w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s4tos10;
  s4tos10.push_back(s4tos10w3);
  s4tos10.push_back(s4tos10w4);
  s4tos10.push_back(s4tos10w9);
  s4.addWeichenstatus(s10ptr,s4tos10);

  QPair<Weiche*, bool> s4tos12w3(w3ptr,false);
  QPair<Weiche*, bool> s4tos12w4(w4ptr,false);
  QPair<Weiche*, bool> s4tos12w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> s4tos12;
  s4tos12.push_back(s4tos12w3);
  s4tos12.push_back(s4tos12w4);
  s4tos12.push_back(s4tos12w9);
  s4.addWeichenstatus(s12ptr,s4tos12);


  QPair<Weiche*, bool> s5tos10w4(w4ptr,true);
  QPair<Weiche*, bool> s5tos10w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s5tos10;
  s5tos10.push_back(s5tos10w4);
  s5tos10.push_back(s5tos10w9);
  s5.addWeichenstatus(s10ptr,s5tos10);

  QPair<Weiche*, bool> s5tos12w4(w4ptr,true);
  QPair<Weiche*, bool> s5tos12w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> s5tos12;
  s5tos12.push_back(s5tos12w4);
  s5tos12.push_back(s5tos12w9);
  s5.addWeichenstatus(s12ptr,s5tos12);

  QPair<Weiche*, bool> s13tos2w5(w5ptr,true);
  QPair<Weiche*, bool> s13tos2w3(w3ptr,true);
  QList<QPair<Weiche*, bool>> s13tos2;
  s13tos2.push_back(s13tos2w5);
  s13tos2.push_back(s13tos2w3);
  s13.addWeichenstatus(s2ptr,s13tos2);

  QPair<Weiche*, bool> s9tos3w4(w4ptr,true);
  QPair<Weiche*, bool> s9tos3w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s9tos3;
  s9tos3.push_back(s9tos3w4);
  s9tos3.push_back(s9tos3w9);
  s9.addWeichenstatus(s3ptr,s9tos3);

  QPair<Weiche*, bool> s11tos3w4(w4ptr,true);
  QPair<Weiche*, bool> s11tos3w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> s11tos3;
  s11tos3.push_back(s11tos3w4);
  s11tos3.push_back(s11tos3w9);
  s11.addWeichenstatus(s3ptr,s11tos3);

  QPair<Weiche*, bool> s9tos2w4(w4ptr,false);
  QPair<Weiche*, bool> s9tos2w3(w3ptr,false);
  QPair<Weiche*, bool> s9tos2w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s9tos2;
  s9tos2.push_back(s9tos2w4);
  s9tos2.push_back(s9tos2w3);
  s9tos2.push_back(s9tos2w9);
  s9.addWeichenstatus(s2ptr,s9tos2);

  QPair<Weiche*, bool> s11tos2w4(w4ptr,false);
  QPair<Weiche*, bool> s11tos2w3(w3ptr,false);
  QPair<Weiche*, bool> s11tos2w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> s11tos2;
  s11tos2.push_back(s11tos2w4);
  s11tos2.push_back(s11tos2w3);
  s11tos2.push_back(s11tos2w9);
  s11.addWeichenstatus(s2ptr,s11tos2);

  QPair<Weiche*, bool> ww1tos2w4(w4ptr,false);
  QPair<Weiche*, bool> ww1tos2w3(w3ptr,false);
  QList<QPair<Weiche*, bool>> ww1tos2;
  ww1tos2.push_back(ww1tos2w4);
  ww1tos2.push_back(ww1tos2w3);
  ww1.addWeichenstatusZuH(s2ptr->getS_id(),ww1tos2);

  QPair<Weiche*, bool> ww1tos3w4(w4ptr,true);
  QList<QPair<Weiche*, bool>> ww1tos3;
  ww1tos3.push_back(ww1tos3w4);
  ww1.addWeichenstatusZuH(s3ptr->getS_id(),ww1tos3);

  QPair<Weiche*, bool> ww2tos12w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> ww2tos12;
  ww2tos12.push_back(ww2tos12w9);
  ww2.addWeichenstatusZuH(s12ptr->getS_id(),ww2tos12);

  QPair<Weiche*, bool> ww2tos10w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> ww2tos10;
  ww2tos10.push_back(ww2tos10w9);
  ww2.addWeichenstatusZuH(s10ptr->getS_id(),ww2tos10);

  QPair<Weiche*, bool> s9toww1w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s9toww1;
  s9toww1.push_back(s9toww1w9);
  s9.addWeichenstatusZuRangier(ww1ptr,s9toww1);

  QPair<Weiche*, bool> s11toww1w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> s11toww1;
  s11toww1.push_back(s11toww1w9);
  s11.addWeichenstatusZuRangier(ww1ptr,s11toww1);
  
  //Blockstatus -----------------------------------------------
  QList<Block*> s1tos4b;
  s1tos4b.push_back(aaptr);
  s1.addBlock(s4ptr,s1tos4b);

  QList<Block*> s13tos2b;
  s13tos2b.push_back(aaptr);
  s13.addBlock(s2ptr,s13tos2b);

  QList<Block*> s4tos7b;
  s4tos7b.push_back(afptr);
  s4.addBlock(s7ptr,s4tos7b);
  QList<Block*> s4tos14b;
  s4tos14b.push_back(acptr);
  s4.addBlock(s14ptr,s4tos14b);
  QList<Block*> s4tos15b;
  s4tos15b.push_back(aeptr);
  s4.addBlock(s15ptr,s4tos15b);
  QList<Block*> s4tos10b;
  s4tos10b.push_back(adptr);
  s4tos10b.push_back(ahptr);
  s4.addBlock(s10ptr,s4tos10b);
  QList<Block*> s4tos12b;
  s4tos12b.push_back(adptr);
  s4tos12b.push_back(aiptr);
  s4.addBlock(s12ptr,s4tos12b);

  QList<Block*> s5tos10b;
  s5tos10b.push_back(adptr);
  s5tos10b.push_back(ahptr);
  s5.addBlock(s10ptr,s5tos10b);
  QList<Block*> s5tos12b;
  s5tos12b.push_back(adptr);
  s5tos12b.push_back(aiptr);
  s5.addBlock(s12ptr,s5tos12b);

  QList<Block*> s9tos3b;
  s9tos3b.push_back(adptr);
  s9tos3b.push_back(abptr);
  s9.addBlock(s3ptr,s9tos3b);
  QList<Block*> s9tos2b;
  s9tos2b.push_back(adptr);
  s9tos2b.push_back(aaptr);
  s9.addBlock(s2ptr,s9tos2b);

  QList<Block*> s11tos3b;
  s11tos3b.push_back(adptr);
  s11tos3b.push_back(abptr);
  s11.addBlock(s3ptr,s11tos3b);
  QList<Block*> s11tos2b;
  s11tos2b.push_back(adptr);
  s11tos2b.push_back(aaptr);
  s11.addBlock(s2ptr,s11tos2b); 

  QList<Block*> ww2tos10b;
  ww2tos10b.push_back(ahptr);
  ww2.addBlockZuH(s10ptr->getS_id(),ww2tos10b);
  QList<Block*> ww2tos12b;
  ww2tos12b.push_back(aiptr);
  ww2.addBlockZuH(s12ptr->getS_id(),ww2tos12b);

  QList<Block*> ww1tos2b;
  ww1tos2b.push_back(aaptr);
  ww1.addBlockZuH(s2ptr->getS_id(),ww1tos2b);
  QList<Block*> ww1tos3b;
  ww1tos3b.push_back(abptr);
  ww1.addBlockZuH(s3ptr->getS_id(),ww1tos3b);

  QList<Block*> s9toww1b;
  s9toww1b.push_back(adptr);
  s9.addBlockZuRangier(ww1ptr,s9toww1b);
  QList<Block*> s11toww1b;
  s11toww1b.push_back(adptr);
  s11.addBlockZuRangier(ww1ptr,s11toww1b);

  // ++++++++++++++++++++++++ muss als erstes mal initialisiert werden...!
  stellwerkstec.add_Signal(s1.getS_id(),false);
  stellwerkstec.add_Signal(s2.getS_id(),false);
  stellwerkstec.add_Signal(s3.getS_id(),false);
  stellwerkstec.add_Signal(s4.getS_id(),false);
  stellwerkstec.add_Signal(s5.getS_id(),false);
  stellwerkstec.add_Signal(s6.getS_id(),false);
  stellwerkstec.add_Signal(s7.getS_id(),false);
  stellwerkstec.add_Signal(s8.getS_id(),false);
  stellwerkstec.add_Signal(s9.getS_id(),false);
  stellwerkstec.add_Signal(s10.getS_id(),false);
  stellwerkstec.add_Signal(s11.getS_id(),false);
  stellwerkstec.add_Signal(s12.getS_id(),false);
  stellwerkstec.add_Signal(s13.getS_id(),false);
  stellwerkstec.add_Signal(s14.getS_id(),false);
  stellwerkstec.add_Signal(s15.getS_id(),false);
  stellwerkstec.add_Signal(ww1.getV_id(),false);
  stellwerkstec.add_Signal(ww2.getV_id(),false);


  //BÜs
  //BU bu1(1);
  //BU *bu1ptr = &bu1;

  //GUI
  //QGraphicsRectItem *bue = new QGraphicsRectItem();
  //bue->setRect(0,0,50,25);
  //bue->setPos(QPointF(-100,-20));
  //bue->setBrush(QColor(QColor(153,50,204)));//lila
  //bue->setRotation(90);
  //scene->addItem(bue);
  //bu1ptr->addBUrect(bue);
  //aa.addBus( bu1ptr );

  
  //signals and slots
  QObject::connect(&s1,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s2,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s3,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s4,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s5,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s6,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s7,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s8,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s9,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s10,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s11,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s12,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s13,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s14,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&s15,&HSignal::refreshStellwerkstechnik,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&ww1,&WSignal::refreshStellwerkstechnikW,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&ww2,&WSignal::refreshStellwerkstechnikW,&stellwerkstec,&Stellwerkstechnik::add_Signal);
  QObject::connect(&aa,&Block::zugpassiert,&s1,&HSignal::zugpassiert);
  QObject::connect(&aa,&Block::zugpassiert,&s13,&HSignal::zugpassiert);
  QObject::connect(&ac,&Block::zugpassiert,&s4,&HSignal::zugpassiert);
  QObject::connect(&ae,&Block::zugpassiert,&s4,&HSignal::zugpassiert);
  QObject::connect(&af,&Block::zugpassiert,&s4,&HSignal::zugpassiert);
  QObject::connect(&ad,&Block::zugpassiert,&s5,&HSignal::zugpassiert);
  QObject::connect(&ad,&Block::zugpassiert,&s9,&HSignal::zugpassiert);
  QObject::connect(&ad,&Block::zugpassiert,&s11,&HSignal::zugpassiert);
  QObject::connect(&af,&Block::zugpassiert,&s8,&HSignal::zugpassiert);
  
  QObject::connect(&ai,&Block::zugpassiertW,&ww2,&WSignal::zugpassiertW);
  QObject::connect(&ah,&Block::zugpassiertW,&ww2,&WSignal::zugpassiertW);
  QObject::connect(&aa,&Block::zugpassiertW,&ww1,&WSignal::zugpassiertW);
  QObject::connect(&ab,&Block::zugpassiertW,&ww1,&WSignal::zugpassiertW);
  
  //Hier wird das Grenzsignal übergeben
  aa.addpassiert(s13.getS_id(),acptr);
  ac.addpassiert(s4.getS_id(),aaptr);
  ae.addpassiert(s4.getS_id(),aaptr);
  af.addpassiert(s4.getS_id(),aaptr);
  ad.addpassiert(s4.getS_id(),aaptr);
  ad.addpassiert(s5.getS_id(),abptr);
  ad.addpassiert(s9.getS_id(),ahptr);
  ad.addpassiert(s11.getS_id(),aiptr);
  
  ah.addpassiert(ww2.getV_id(),adptr);
  ai.addpassiert(ww2.getV_id(),adptr);
  aa.addpassiert(ww1.getV_id(),adptr);
  ab.addpassiert(ww1.getV_id(),adptr);

  //
  //GUI Attribute
  QGraphicsRectItem *w1rectge = new QGraphicsRectItem();
  w1rectge->setRect(0,0,10,75);
  w1rectge->setPos(QPointF(500,800));
  w1rectge->setBrush(QColor(79,79,79));
  w1rectge->setRotation(90);
  scene->addItem(w1rectge);
  QGraphicsRectItem *w1rectab = new QGraphicsRectItem();
  w1rectab->setRect(0,0,10,50);
  w1rectab->setPos(QPointF(500,800));
  w1rectab->setBrush(QColor(79,79,79));
  w1rectab->setRotation(135);
  scene->addItem(w1rectab);
  QLabel *w1label = new QLabel();
  scene->addWidget(w1label);
  w1.addWeichenitem(w1rectab,w1rectge,w1label);
  w1.moveLabel(480,810);
  
  QGraphicsRectItem *abrect = new QGraphicsRectItem();
  abrect->setRect(0,0,10,1200);
  abrect->setPos(QPointF(425,800));
  abrect->setBrush(QColor(79,79,79));
  abrect->setRotation(90);
  ab.addBlockitems(abrect);
  scene->addItem(abrect);
  //
  QGraphicsRectItem *w2rectge = new QGraphicsRectItem();
  w2rectge->setRect(0,0,10,75);
  w2rectge->setPos(QPointF(500,730));
  w2rectge->setBrush(QColor(79,79,79));
  w2rectge->setRotation(90);
  scene->addItem(w2rectge);
  QGraphicsRectItem *w2rectab = new QGraphicsRectItem();
  w2rectab->setRect(0,0,10,50);
  w2rectab->setPos(QPointF(420,735));
  w2rectab->setBrush(QColor(79,79,79));
  w2rectab->setRotation(315);
  scene->addItem(w2rectab);
  QLabel *w2label = new QLabel();
  scene->addWidget(w2label);
  w2.addWeichenitem(w2rectab,w2rectge,w2label);
  w2.moveLabel(420,715);

  QGraphicsRectItem *aarect = new QGraphicsRectItem();
  aarect->setRect(0,0,10,1200);
  aarect->setPos(QPointF(425,730));
  aarect->setBrush(QColor(79,79,79));
  aarect->setRotation(90);
  aa.addBlockitems(aarect);
  scene->addItem(aarect);

  QGraphicsRectItem *w4rectge = new QGraphicsRectItem();
  w4rectge->setRect(0,0,10,75);
  w4rectge->setPos(QPointF(-775,800));
  w4rectge->setBrush(QColor(79,79,79));
  w4rectge->setRotation(90);
  scene->addItem(w4rectge);
  QGraphicsRectItem *w4rectab = new QGraphicsRectItem();
  w4rectab->setRect(0,0,10,50);
  w4rectab->setPos(QPointF(-845,810));
  w4rectab->setBrush(QColor(79,79,79));
  w4rectab->setRotation(-135);
  scene->addItem(w4rectab);
  QLabel *w4label = new QLabel();
  scene->addWidget(w4label);
  w4.addWeichenitem(w4rectab,w4rectge,w4label);
  w4.moveLabel(-845,810);

  QGraphicsRectItem *w3rectge = new QGraphicsRectItem();
  w3rectge->setRect(0,0,10,75);
  w3rectge->setPos(QPointF(-775,730));
  w3rectge->setBrush(QColor(79,79,79));
  w3rectge->setRotation(90);
  scene->addItem(w3rectge);
  QGraphicsRectItem *w3rectab = new QGraphicsRectItem();
  w3rectab->setRect(0,0,10,50);
  w3rectab->setPos(QPointF(-785,735));
  w3rectab->setBrush(QColor(79,79,79));
  w3rectab->setRotation(45);
  scene->addItem(w3rectab);
  QLabel *w3label = new QLabel();
  scene->addWidget(w3label);
  w3.addWeichenitem(w3rectab,w3rectge,w3label);
  w3.moveLabel(-795,715);

  QGraphicsRectItem *w5rectge = new QGraphicsRectItem();
  w5rectge->setRect(0,0,10,75);
  w5rectge->setPos(QPointF(-850,730));
  w5rectge->setBrush(QColor(79,79,79));
  w5rectge->setRotation(90);
  scene->addItem(w5rectge);
  QGraphicsRectItem *w5rectab = new QGraphicsRectItem();
  w5rectab->setRect(0,0,10,80);
  w5rectab->setPos(QPointF(-850,735));
  w5rectab->setBrush(QColor(79,79,79));
  w5rectab->setRotation(135);
  scene->addItem(w5rectab);
  QLabel *w5label = new QLabel();
  scene->addWidget(w5label);
  w5.addWeichenitem(w5rectab,w5rectge,w5label);
  w5.moveLabel(-855,715);

  QGraphicsRectItem *w6rectge = new QGraphicsRectItem();
  w6rectge->setRect(0,0,10,100);
  w6rectge->setPos(QPointF(-918,575));
  w6rectge->setBrush(QColor(79,79,79));
  //w6rectge->setRotation(90);
  scene->addItem(w6rectge);
  QGraphicsRectItem *w6rectab = new QGraphicsRectItem();
  w6rectab->setRect(0,0,10,50);
  w6rectab->setPos(QPointF(-918,580));
  w6rectab->setBrush(QColor(79,79,79));
  w6rectab->setRotation(-45);
  scene->addItem(w6rectab);
  QLabel *w6label = new QLabel();
  scene->addWidget(w6label);
  w6.addWeichenitem(w6rectab,w6rectge,w6label);
  w6.moveLabel(-938,578);

  QGraphicsRectItem *w7rectge = new QGraphicsRectItem();
  w7rectge->setRect(0,0,10,150);
  w7rectge->setPos(QPointF(-918,425));
  w7rectge->setBrush(QColor(79,79,79));
  scene->addItem(w7rectge);
  QGraphicsRectItem *w7rectab = new QGraphicsRectItem();
  w7rectab->setRect(0,0,10,50);
  w7rectab->setPos(QPointF(-910,530));
  w7rectab->setBrush(QColor(79,79,79));
  w7rectab->setRotation(-135);
  scene->addItem(w7rectab);
  QLabel *w7label = new QLabel();
  scene->addWidget(w7label);
  w7.addWeichenitem(w7rectab,w7rectge,w7label);
  w7.moveLabel(-938,508);

  QGraphicsRectItem *aerect = new QGraphicsRectItem();
  aerect->setRect(0,0,10,70);
  aerect->setPos(QPointF(-918,375));
  aerect->setBrush(QColor(79,79,79));
  ae.addBlockitems(aerect);
  scene->addItem(aerect);
  QGraphicsRectItem *aerect2 = new QGraphicsRectItem();
  aerect2->setRect(0,0,10,305);
  aerect2->setPos(QPointF(-918,375));
  aerect2->setBrush(QColor(79,79,79));
  aerect2->setRotation(-90);
  ae.addBlockitems(aerect2);
  scene->addItem(aerect2);

  QGraphicsRectItem *acrect = new QGraphicsRectItem();
  acrect->setRect(0,0,10,70);
  acrect->setPos(QPointF(-925,730));
  acrect->setBrush(QColor(79,79,79));
  acrect->setRotation(90);
  ac.addBlockitems(acrect);
  scene->addItem(acrect);
  QGraphicsRectItem *acrect2 = new QGraphicsRectItem();
  acrect2->setRect(0,0,10,450);
  acrect2->setPos(QPointF(-995,290));
  acrect2->setBrush(QColor(79,79,79));
  ac.addBlockitems(acrect2);
  scene->addItem(acrect2);
  QGraphicsRectItem *acrect3 = new QGraphicsRectItem();
  acrect3->setRect(0,0,10,380);
  acrect3->setPos(QPointF(-995,290));
  acrect3->setBrush(QColor(79,79,79));
  acrect3->setRotation(-90);
  ac.addBlockitems(acrect3);
  scene->addItem(acrect3);

  QGraphicsRectItem *adrect = new QGraphicsRectItem();
  adrect->setRect(0,0,10,225);
  adrect->setPos(QPointF(-850,800));
  adrect->setBrush(QColor(79,79,79));
  adrect->setRotation(90);
  ad.addBlockitems(adrect);
  scene->addItem(adrect);
  QGraphicsRectItem *adrect2 = new QGraphicsRectItem();
  adrect2->setRect(0,0,10,600);
  adrect2->setPos(QPointF(-1075,200));
  adrect2->setBrush(QColor(79,79,79));
  ad.addBlockitems(adrect2);
  scene->addItem(adrect2);
  QGraphicsRectItem *adrect3 = new QGraphicsRectItem();
  adrect3->setRect(0,0,10,140);
  adrect3->setPos(QPointF(-1075,200));
  adrect3->setBrush(QColor(79,79,79));
  adrect3->setRotation(-90);
  ad.addBlockitems(adrect3);
  scene->addItem(adrect3);

  QGraphicsRectItem *w9rectge = new QGraphicsRectItem();
  w9rectge->setRect(0,0,10,75);
  w9rectge->setPos(QPointF(-935,200));
  w9rectge->setBrush(QColor(79,79,79));
  w9rectge->setRotation(-90);
  scene->addItem(w9rectge);
  QGraphicsRectItem *w9rectab = new QGraphicsRectItem();
  w9rectab->setRect(0,0,10,50);
  w9rectab->setPos(QPointF(-935,200));
  w9rectab->setBrush(QColor(79,79,79));
  w9rectab->setRotation(-135);
  scene->addItem(w9rectab);
  QLabel *w9label = new QLabel();
  scene->addWidget(w9label);
  w9.addWeichenitem(w9rectab,w9rectge,w9label);
  w9.moveLabel(-935,200);

  QGraphicsRectItem *airect = new QGraphicsRectItem();
  airect->setRect(0,0,10,1000);
  airect->setPos(QPointF(-860,200));
  airect->setBrush(QColor(79,79,79));
  airect->setRotation(-90);
  ai.addBlockitems(airect);
  scene->addItem(airect);

  QGraphicsRectItem *ahrect = new QGraphicsRectItem();
  ahrect->setRect(0,0,10,55);
  ahrect->setPos(QPointF(-868,120));
  ahrect->setBrush(QColor(79,79,79));
  ahrect->setRotation(45);
  ah.addBlockitems(ahrect);
  scene->addItem(ahrect);
  QGraphicsRectItem *ahrect2 = new QGraphicsRectItem();
  ahrect2->setRect(0,0,10,1003);
  ahrect2->setPos(QPointF(-863,130));
  ahrect2->setBrush(QColor(79,79,79));
  ahrect2->setRotation(-90);
  ah.addBlockitems(ahrect2);
  scene->addItem(ahrect2);
  QGraphicsRectItem *ahrect3 = new QGraphicsRectItem();
  ahrect3->setRect(0,0,10,55);
  ahrect3->setPos(QPointF(140,130));
  ahrect3->setBrush(QColor(79,79,79));
  ahrect3->setRotation(-45);
  ah.addBlockitems(ahrect3);
  scene->addItem(ahrect3);

  QGraphicsRectItem *w10rectge = new QGraphicsRectItem();
  w10rectge->setRect(0,0,10,75);
  w10rectge->setPos(QPointF(215,190));
  w10rectge->setBrush(QColor(79,79,79));
  w10rectge->setRotation(90);
  scene->addItem(w10rectge);
  QGraphicsRectItem *w10rectab = new QGraphicsRectItem();
  w10rectab->setRect(0,0,10,50);
  w10rectab->setPos(QPointF(215,190));
  w10rectab->setBrush(QColor(79,79,79));
  w10rectab->setRotation(135);
  scene->addItem(w10rectab);
  QLabel *w10label = new QLabel();
  scene->addWidget(w10label);
  w10.addWeichenitem(w10rectab,w10rectge,w10label);
  w10.moveLabel(200,200);

  QGraphicsRectItem *afrect = new QGraphicsRectItem();
  afrect->setRect(0,0,10,675);
  afrect->setPos(QPointF(-875,495));
  afrect->setBrush(QColor(79,79,79));
  afrect->setRotation(-90);
  af.addBlockitems(afrect);
  scene->addItem(afrect);

  QGraphicsRectItem *agrect = new QGraphicsRectItem();
  agrect->setRect(0,0,10,630);
  agrect->setPos(QPointF(-875,615));
  agrect->setBrush(QColor(79,79,79));
  agrect->setRotation(-90);
  ag.addBlockitems(agrect);
  scene->addItem(agrect);
  QGraphicsRectItem *agrect2 = new QGraphicsRectItem();
  agrect2->setRect(0,0,10,125);
  agrect2->setPos(QPointF(-160,520));
  agrect2->setBrush(QColor(79,79,79));
  agrect2->setRotation(45);
  ag.addBlockitems(agrect2);
  scene->addItem(agrect2);

  QGraphicsRectItem *w8rectge = new QGraphicsRectItem();
  w8rectge->setRect(0,0,10,75);
  w8rectge->setPos(QPointF(-125,485));
  w8rectge->setBrush(QColor(79,79,79));
  w8rectge->setRotation(90);
  scene->addItem(w8rectge);
  QGraphicsRectItem *w8rectab = new QGraphicsRectItem();
  w8rectab->setRect(0,0,10,50);
  w8rectab->setPos(QPointF(-125,485));
  w8rectab->setBrush(QColor(79,79,79));
  w8rectab->setRotation(45);
  scene->addItem(w8rectab);
  QLabel *w8label = new QLabel();
  scene->addWidget(w8label);
  w8.addWeichenitem(w8rectab,w8rectge,w8label);
  w8.moveLabel(-140,475);

  QGraphicsSvgItem *s4item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s4item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s4item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s4item1);
  scene->addItem(s4item3);
  scene->addItem(s4item2);
  s4item1->setPos(QPointF(-775,720));
  s4item2->setPos(QPointF(-775,720));
  s4item3->setPos(QPointF(-775,720));
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
  sphin4->setPos(QPointF(-715,700));
  QGraphicsRectItem *spweg4 = new QGraphicsRectItem();
  spweg4->setRect(0,0,10,10);
  spweg4->setBrush(Qt::yellow);
  spweg4->setPos(QPointF(-715,710));
  scene->addItem(spweg4);
  scene->addItem(sphin4);
  s4.addHSignalitem(s4item1,s4item2, s4item3, s4label,s4push,spweg4,sphin4);
  s4.moveButton(-755,725);
  s4.moveLabel(-700,705);

  QGraphicsSvgItem *s5item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s5item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s5item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s5item1);
  scene->addItem(s5item3);
  scene->addItem(s5item2);
  s5item1->setPos(QPointF(-775,790));
  s5item2->setPos(QPointF(-775,790));
  s5item3->setPos(QPointF(-775,790));
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
  sphin5->setPos(QPointF(-715,770));
  QGraphicsRectItem *spweg5 = new QGraphicsRectItem();
  spweg5->setRect(0,0,10,10);
  spweg5->setBrush(Qt::yellow);
  spweg5->setPos(QPointF(-715,780));
  scene->addItem(spweg5);
  scene->addItem(sphin5);
  s5.addHSignalitem(s5item1,s5item2, s5item3, s5label,s5push,spweg5,sphin5);
  s5.moveButton(-755,795);
  s5.moveLabel(-700,775);

  QGraphicsSvgItem *s2item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s2item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s2item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s2item1);
  scene->addItem(s2item3);
  scene->addItem(s2item2);
  s2item1->setPos(QPointF(425,750));
  s2item2->setPos(QPointF(425,750));
  s2item3->setPos(QPointF(425,750));
  s2item1->setScale(0.1);
  s2item2->setScale(0.1);
  s2item3->setScale(0.1);
  s2item1->setRotation(90);
  s2item2->setRotation(90);
  s2item3->setRotation(90);
  QPushButton *s2push = new QPushButton();
  scene->addWidget(s2push);
  QLabel *s2label = new QLabel();
  scene->addWidget(s2label);
  //speicher GUIs
  QGraphicsRectItem *sphin2 = new QGraphicsRectItem();
  sphin2->setRect(0,0,10,10);
  sphin2->setBrush(Qt::yellow);
  sphin2->setPos(QPointF(356,749));
  QGraphicsRectItem *spweg2 = new QGraphicsRectItem();
  spweg2->setRect(0,0,10,10);
  spweg2->setBrush(Qt::yellow);
  spweg2->setPos(QPointF(356,759));
  scene->addItem(spweg2);
  scene->addItem(sphin2);
  s2.addHSignalitem(s2item1,s2item2, s2item3, s2label,s2push,spweg2,sphin2);
  s2.moveButton(385,725);
  s2.moveLabel(318,750);

  QGraphicsSvgItem *s3item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s3item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s3item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s3item1);
  scene->addItem(s3item3);
  scene->addItem(s3item2);
  s3item1->setPos(QPointF(425,820));
  s3item2->setPos(QPointF(425,820));
  s3item3->setPos(QPointF(425,820));
  s3item1->setScale(0.1);
  s3item2->setScale(0.1);
  s3item3->setScale(0.1);
  s3item1->setRotation(90);
  s3item2->setRotation(90);
  s3item3->setRotation(90);
  QPushButton *s3push = new QPushButton();
  scene->addWidget(s3push);
  QLabel *s3label = new QLabel();
  scene->addWidget(s3label);
  //speicher GUIs
  QGraphicsRectItem *sphin3 = new QGraphicsRectItem();
  sphin3->setRect(0,0,10,10);
  sphin3->setBrush(Qt::yellow);
  sphin3->setPos(QPointF(356,819));
  QGraphicsRectItem *spweg3 = new QGraphicsRectItem();
  spweg3->setRect(0,0,10,10);
  spweg3->setBrush(Qt::yellow);
  spweg3->setPos(QPointF(356,829));
  scene->addItem(spweg3);
  scene->addItem(sphin3);
  s3.addHSignalitem(s3item1,s3item2, s3item3, s3label,s3push,spweg3,sphin3);
  s3.moveButton(385,795);
  s3.moveLabel(318,820);

  QGraphicsSvgItem *item1 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *item2 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(item1);
  scene->addItem(item2);
  item1->setPos(QPointF(-965,745));
  item2->setPos(QPointF(-965,745));
  item1->setScale(0.1);
  item2->setScale(0.1);
  item1->setRotation(90);
  item2->setRotation(90);
  v2ptr->addVSignalitems(item1, item2);
  
  QGraphicsSvgItem *s13item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s13item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s13item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s13item1);
  scene->addItem(s13item3);
  scene->addItem(s13item2);
  s13item1->setPos(QPointF(-925,750));
  s13item2->setPos(QPointF(-925,750));
  s13item3->setPos(QPointF(-925,750));
  s13item1->setScale(0.1);
  s13item2->setScale(0.1);
  s13item3->setScale(0.1);
  s13item1->setRotation(90);
  s13item2->setRotation(90);
  s13item3->setRotation(90);
  QPushButton *s13push = new QPushButton();
  scene->addWidget(s13push);
  QLabel *s13label = new QLabel();
  scene->addWidget(s13label);
  //speicher GUIs
  QGraphicsRectItem *sphin13 = new QGraphicsRectItem();
  sphin13->setRect(0,0,10,10);
  sphin13->setBrush(Qt::yellow);
  sphin13->setPos(QPointF(-1004,749));
  QGraphicsRectItem *spweg13 = new QGraphicsRectItem();
  spweg13->setRect(0,0,10,10);
  spweg13->setBrush(Qt::yellow);
  spweg13->setPos(QPointF(-1004,759));
  scene->addItem(spweg13);
  scene->addItem(sphin13);
  s13.addHSignalitem(s13item1,s13item2, s13item3, s13label,s13push,spweg13,sphin13);
  s13.moveButton(-965,725);
  s13.moveLabel(-1037,750);

  /////////////NOCH VON WSIGNAL
  //soll on top of scene sein...
  QPushButton *ww1push = new QPushButton();
  scene->addWidget(ww1push);
  ww1.addButtonAndLabel(ww1label,ww1push);
  ww1.moveButton(-900,795);
  ww1.moveLabel(-940,820);
  // und NR 2 W
  QPushButton *ww2push = new QPushButton();
  scene->addWidget(ww2push);
  ww2.addButtonAndLabel(ww2label,ww2push);
  ww2.moveButton(-980,185);
  ww2.moveLabel(-1015,205);
  ///////////////////////////
  

  
  //QPushButtonPart
  clickmanager *c1 = new clickmanager;
  QObject::connect(&s1,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) ); //QOverload wegen zeideutigkeit von recieveFS; C++11, C++14 qOverload<>
  QObject::connect(&s2,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s3,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s4,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s5,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s6,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s7,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s8,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s9,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s10,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s11,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s12,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s13,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s14,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&s15,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  //ebenfalls für WSignal
  QObject::connect(&ww1,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(&ww2,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  //:::::::::


  /////// Thread start
  //initialisiere pins als in/out-put + speichere in worker eine Liste von pins
  QThread* thread = new QThread;
  worker* wrkr = new worker();
  wrkr->moveToThread(thread);
  QObject::connect(thread, &QThread::started, wrkr, &worker::updateBelegt); ////thread start connection 
  QObject::connect(wrkr, &worker::finished, thread, &QThread::quit); //if no pin is registered finished
  QObject::connect(&a, &QApplication::aboutToQuit, wrkr, &worker::quit); // quit updateBelegt on aboutToQuit
  QObject::connect(&a, &QApplication::aboutToQuit, thread, &QThread::quit); // quit thread on aboutToQuit
  thread->start();
  //thread endet theoretisch auch über wrkr.quit() -> updateBelegt() -> emit finished -> oberer slot
      
  QThread* thread2 = new QThread;
  Spmemory *mem = new Spmemory();
  mem->moveToThread(thread2);
  QObject::connect(thread2, &QThread::started, mem, &Spmemory::timing); //thread start connection 
  //  QObject::connect(mem, &Spmemory::finished, thread2, &QThread::quit);
  QObject::connect(&a, &QApplication::aboutToQuit, mem, &Spmemory::quit); // quit timing on aboutToQuit
  QObject::connect(&a, &QApplication::aboutToQuit, thread2, &QThread::quit); // quit thread2 on aboutToQuit
  //spmemory connection 
  
  QObject::connect(&s1, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(&s2, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(&s3, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(&s4, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(&s5, &HSignal::callspmemory,mem, &Spmemory::addFS);
  thread2->start();
 
  w.showMaximized();

      
  // End of program, now delete all resources... all classes with parents are deleted through this chain, addItem, addWidget takes ownership
  int eofprogram = a.exec();
  qDebug() <<"endofprogram";
  //reverse order
  delete mem;
  delete thread2;
      
  delete wrkr;
  delete thread;

  delete c1;
  
  return eofprogram;

}
