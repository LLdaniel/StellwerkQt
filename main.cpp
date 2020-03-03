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
    
  //Initialisierung der Strecke--------------------------------------------------------
  //Stellwerkstechnik+Block aa as basis
  Stellwerkstechnik *stellwerkstecptr = new Stellwerkstechnik();
  Block *aaptr = new Block("aa", stellwerkstecptr);
  //:::Weichen:::
  Weiche *w1ptr = new Weiche(1);
  Weiche *w2ptr = new Weiche(2);
  Weiche *w3ptr = new Weiche(3);
  Weiche *w4ptr = new Weiche(4);
  Weiche *w5ptr = new Weiche(5);
  Weiche *w6ptr = new Weiche(6);
  Weiche *w7ptr = new Weiche(7);
  Weiche *w8ptr = new Weiche(8);
  Weiche *w9ptr = new Weiche(9);
  Weiche *w10ptr = new Weiche(10);
  //:::Block:::
  Block *abptr = new Block("ab", stellwerkstecptr);
  Block *acptr = new Block("ac", stellwerkstecptr);
  Block *adptr = new Block("ad", stellwerkstecptr);
  Block *aeptr = new Block("ae", stellwerkstecptr);
  Block *afptr = new Block("af", stellwerkstecptr);
  Block *agptr = new Block("ag", stellwerkstecptr);
  Block *ahptr = new Block("ah", stellwerkstecptr);
  Block *aiptr = new Block("ai", stellwerkstecptr);
  //:::VS:::
  VSignal *v1ptr = new VSignal(1);
  VSignal *v2ptr = new VSignal(2);
  VSignal *v3ptr = new VSignal(3);
  VSignal *v4ptr = new VSignal(4);
  VSignal *v5ptr = new VSignal(5);
  VSignal *v6ptr = new VSignal(6);
  VSignal *v7ptr = new VSignal(7);
  VSignal *v8ptr = new VSignal(8);
  VSignal *v9ptr = new VSignal(9);
  VSignal *v10ptr = new VSignal(10);
  v1ptr->setRichtung("S001","S004");
  v2ptr->setRichtung("S013","S002");
  v3ptr->setRichtung("S004","S015");
  v3ptr->setRichtung("S004","S007");
  v4ptr->setRichtung("S008","S002");
  v4ptr->setRichtung("S008","S002");
  v4ptr->setRichtung("S008","S006");
  //v4ptr->setRichtung("S00X","S002");
  //v4ptr->setRichtung("S00Y","S002");
  v5ptr->setRichtung("S004","S014");
  v6ptr->setRichtung("S004","S012");
  v6ptr->setRichtung("S004","S010");
  v6ptr->setRichtung("S005","S012");
  v6ptr->setRichtung("S005","S010");
  v7ptr->setRichtung("S011","S003");
  v7ptr->setRichtung("S009","S003");
  v7ptr->setRichtung("S011","S002");
  v7ptr->setRichtung("S009","S002");
  v8ptr->setRichtung("S008","S002");
  //v9ptr->setRichtung("S006","");
  //v10ptr->setRichtung("S00X","S013");
  //:::HS:::
  HSignal *s1ptr = new HSignal(1);
  HSignal *s2ptr = new HSignal(2);
  HSignal *s3ptr = new HSignal(3);
  HSignal *s4ptr = new HSignal(4);
  HSignal *s5ptr = new HSignal(5);
  HSignal *s6ptr = new HSignal(6);
  HSignal *s7ptr = new HSignal(7);
  HSignal *s8ptr = new HSignal(8);
  HSignal *s9ptr = new HSignal(9);
  HSignal *s10ptr = new HSignal(10);
  HSignal *s11ptr = new HSignal(11);
  HSignal *s12ptr = new HSignal(12);
  HSignal *s13ptr = new HSignal(13);
  HSignal *s14ptr = new HSignal(14);
  HSignal *s15ptr = new HSignal(15);
  //:::::::::
  
  s1ptr->addVSignal(v1ptr,"v");
  //s2ptr->addVSignal(v2ptr,"v");
  //s2ptr->addVSignal(v2ptr,"r");
  //s2ptr->addVSignal(v3ptr,"r");
  //s3ptr->addVSignal(v3ptr,"v");
  //s3ptr->addVSignal(v1ptr,"r");
  s4ptr->addVSignal(v1ptr,"r");
  s4ptr->addVSignal(v3ptr,"v");
  s4ptr->addVSignal(v5ptr,"v");
  //s5ptr->addVSignal(v2ptr,"r");
  s5ptr->addVSignal(v6ptr,"v");
  //s6ptr->addVSignal(v3ptr,"v");
  s6ptr->addVSignal(v4ptr,"r");
  s7ptr->addVSignal(v3ptr,"r");
  s8ptr->addVSignal(v3ptr,"v");
  s8ptr->addVSignal(v4ptr,"v");
  s9ptr->addVSignal(v7ptr,"v");
  s10ptr->addVSignal(v6ptr,"r");
  s11ptr->addVSignal(v7ptr,"v");
  s12ptr->addVSignal(v6ptr,"r");
  s13ptr->addVSignal(v2ptr,"v");
  s13ptr->addVSignal(v10ptr,"r");
  //:::::::::
  //WSignale
  WSignal *ww1ptr = new WSignal(1);
  ww1ptr->setRichtung("S009","S002");
  ww1ptr->setRichtung("S009","S003");
  ww1ptr->setRichtung("S011","S002");
  ww1ptr->setRichtung("S011","S003");
  s9ptr->addWSignal(ww1ptr,"v");
  s11ptr->addWSignal(ww1ptr,"v");
  s2ptr->addWSignal(ww1ptr,"r");
  s3ptr->addWSignal(ww1ptr,"r");
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
  WSignal *ww2ptr = new WSignal(2);
  ww2ptr->setRichtung("S004","S012");
  ww2ptr->setRichtung("S004","S010");
  ww2ptr->setRichtung("S005","S012");
  ww2ptr->setRichtung("S005","S010");
  s4ptr->addWSignal(ww2ptr,"v");
  s5ptr->addWSignal(ww2ptr,"v");
  s10ptr->addWSignal(ww2ptr,"r");
  s12ptr->addWSignal(ww2ptr,"r");
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
  s1ptr->addWeichenstatus(s4ptr,s1tos4);

  QPair<Weiche*, bool> s4tos7w3(w3ptr,true);
  QPair<Weiche*, bool> s4tos7w5(w5ptr,false);
  QPair<Weiche*, bool> s4tos7w6(w6ptr,true);
  QPair<Weiche*, bool> s4tos7w7(w7ptr,false);
  QList<QPair<Weiche*, bool>> s4tos7;
  s4tos7.push_back(s4tos7w3);
  s4tos7.push_back(s4tos7w5);
  s4tos7.push_back(s4tos7w6);
  s4tos7.push_back(s4tos7w7);
  s4ptr->addWeichenstatus(s7ptr,s4tos7);

  QPair<Weiche*, bool> s4tos15w3(w3ptr,true);
  QPair<Weiche*, bool> s4tos15w5(w5ptr,false);
  QPair<Weiche*, bool> s4tos15w6(w6ptr,true);
  QPair<Weiche*, bool> s4tos15w7(w7ptr,true);
  QList<QPair<Weiche*, bool>> s4tos15;
  s4tos15.push_back(s4tos15w3);
  s4tos15.push_back(s4tos15w5);
  s4tos15.push_back(s4tos15w6);
  s4tos15.push_back(s4tos15w7);
  s4ptr->addWeichenstatus(s15ptr,s4tos15);

  QPair<Weiche*, bool> s4tos14w3(w3ptr,true);
  QPair<Weiche*, bool> s4tos14w5(w5ptr,true);
  QList<QPair<Weiche*, bool>> s4tos14;
  s4tos14.push_back(s4tos14w3);
  s4tos14.push_back(s4tos14w5);
  s4ptr->addWeichenstatus(s14ptr,s4tos14);

  QPair<Weiche*, bool> s4tos10w3(w3ptr,false);
  QPair<Weiche*, bool> s4tos10w4(w4ptr,false);
  QPair<Weiche*, bool> s4tos10w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s4tos10;
  s4tos10.push_back(s4tos10w3);
  s4tos10.push_back(s4tos10w4);
  s4tos10.push_back(s4tos10w9);
  s4ptr->addWeichenstatus(s10ptr,s4tos10);

  QPair<Weiche*, bool> s4tos12w3(w3ptr,false);
  QPair<Weiche*, bool> s4tos12w4(w4ptr,false);
  QPair<Weiche*, bool> s4tos12w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> s4tos12;
  s4tos12.push_back(s4tos12w3);
  s4tos12.push_back(s4tos12w4);
  s4tos12.push_back(s4tos12w9);
  s4ptr->addWeichenstatus(s12ptr,s4tos12);


  QPair<Weiche*, bool> s5tos10w4(w4ptr,true);
  QPair<Weiche*, bool> s5tos10w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s5tos10;
  s5tos10.push_back(s5tos10w4);
  s5tos10.push_back(s5tos10w9);
  s5ptr->addWeichenstatus(s10ptr,s5tos10);

  QPair<Weiche*, bool> s5tos12w4(w4ptr,true);
  QPair<Weiche*, bool> s5tos12w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> s5tos12;
  s5tos12.push_back(s5tos12w4);
  s5tos12.push_back(s5tos12w9);
  s5ptr->addWeichenstatus(s12ptr,s5tos12);

  QPair<Weiche*, bool> s13tos2w5(w5ptr,true);
  QPair<Weiche*, bool> s13tos2w3(w3ptr,true);
  QList<QPair<Weiche*, bool>> s13tos2;
  s13tos2.push_back(s13tos2w5);
  s13tos2.push_back(s13tos2w3);
  s13ptr->addWeichenstatus(s2ptr,s13tos2);

  QPair<Weiche*, bool> s9tos3w4(w4ptr,true);
  QPair<Weiche*, bool> s9tos3w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s9tos3;
  s9tos3.push_back(s9tos3w4);
  s9tos3.push_back(s9tos3w9);
  s9ptr->addWeichenstatus(s3ptr,s9tos3);

  QPair<Weiche*, bool> s11tos3w4(w4ptr,true);
  QPair<Weiche*, bool> s11tos3w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> s11tos3;
  s11tos3.push_back(s11tos3w4);
  s11tos3.push_back(s11tos3w9);
  s11ptr->addWeichenstatus(s3ptr,s11tos3);

  QPair<Weiche*, bool> s9tos2w4(w4ptr,false);
  QPair<Weiche*, bool> s9tos2w3(w3ptr,false);
  QPair<Weiche*, bool> s9tos2w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s9tos2;
  s9tos2.push_back(s9tos2w4);
  s9tos2.push_back(s9tos2w3);
  s9tos2.push_back(s9tos2w9);
  s9ptr->addWeichenstatus(s2ptr,s9tos2);

  QPair<Weiche*, bool> s11tos2w4(w4ptr,false);
  QPair<Weiche*, bool> s11tos2w3(w3ptr,false);
  QPair<Weiche*, bool> s11tos2w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> s11tos2;
  s11tos2.push_back(s11tos2w4);
  s11tos2.push_back(s11tos2w3);
  s11tos2.push_back(s11tos2w9);
  s11ptr->addWeichenstatus(s2ptr,s11tos2);

  QPair<Weiche*, bool> ww1tos2w4(w4ptr,false);
  QPair<Weiche*, bool> ww1tos2w3(w3ptr,false);
  QList<QPair<Weiche*, bool>> ww1tos2;
  ww1tos2.push_back(ww1tos2w4);
  ww1tos2.push_back(ww1tos2w3);
  ww1ptr->addWeichenstatusZuH(s2ptr->getS_id(),ww1tos2);

  QPair<Weiche*, bool> ww1tos3w4(w4ptr,true);
  QList<QPair<Weiche*, bool>> ww1tos3;
  ww1tos3.push_back(ww1tos3w4);
  ww1ptr->addWeichenstatusZuH(s3ptr->getS_id(),ww1tos3);

  QPair<Weiche*, bool> ww2tos12w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> ww2tos12;
  ww2tos12.push_back(ww2tos12w9);
  ww2ptr->addWeichenstatusZuH(s12ptr->getS_id(),ww2tos12);

  QPair<Weiche*, bool> ww2tos10w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> ww2tos10;
  ww2tos10.push_back(ww2tos10w9);
  ww2ptr->addWeichenstatusZuH(s10ptr->getS_id(),ww2tos10);

  QPair<Weiche*, bool> s9toww1w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s9toww1;
  s9toww1.push_back(s9toww1w9);
  s9ptr->addWeichenstatusZuRangier(ww1ptr,s9toww1);

  QPair<Weiche*, bool> s11toww1w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> s11toww1;
  s11toww1.push_back(s11toww1w9);
  s11ptr->addWeichenstatusZuRangier(ww1ptr,s11toww1);

  QPair<Weiche*, bool> s8tos2w8(w8ptr,true);
  QPair<Weiche*, bool> s8tos2w7(w7ptr,false);
  QPair<Weiche*, bool> s8tos2w6(w6ptr,true);
  QPair<Weiche*, bool> s8tos2w5(w5ptr,false);
  QPair<Weiche*, bool> s8tos2w3(w3ptr,true);
  QList<QPair<Weiche*, bool>> s8tos2;
  s8tos2.push_back(s8tos2w8);
  s8tos2.push_back(s8tos2w7);
  s8tos2.push_back(s8tos2w6);
  s8tos2.push_back(s8tos2w5);
  s8tos2.push_back(s8tos2w3);
  s8ptr->addWeichenstatus(s2ptr,s8tos2);

  QPair<Weiche*, bool> s8tos6w8(w8ptr,true);
  QPair<Weiche*, bool> s8tos6w7(w7ptr,false);
  QPair<Weiche*, bool> s8tos6w6(w6ptr,false);
  QList<QPair<Weiche*, bool>> s8tos6;
  s8tos6.push_back(s8tos6w8);
  s8tos6.push_back(s8tos6w7);
  s8tos6.push_back(s8tos6w6);
  s8ptr->addWeichenstatus(s6ptr,s8tos6);
  
  //Blockstatus -----------------------------------------------
  QList<Block*> s1tos4b;
  s1tos4b.push_back(aaptr);
  s1ptr->addBlock(s4ptr,s1tos4b);

  QList<Block*> s13tos2b;
  s13tos2b.push_back(aaptr);
  s13ptr->addBlock(s2ptr,s13tos2b);

  QList<Block*> s4tos7b;
  s4tos7b.push_back(afptr);
  s4ptr->addBlock(s7ptr,s4tos7b);
  QList<Block*> s4tos14b;
  s4tos14b.push_back(acptr);
  s4ptr->addBlock(s14ptr,s4tos14b);
  QList<Block*> s4tos15b;
  s4tos15b.push_back(aeptr);
  s4ptr->addBlock(s15ptr,s4tos15b);
  QList<Block*> s4tos10b;
  s4tos10b.push_back(adptr);
  s4tos10b.push_back(ahptr);
  s4ptr->addBlock(s10ptr,s4tos10b);
  QList<Block*> s4tos12b;
  s4tos12b.push_back(adptr);
  s4tos12b.push_back(aiptr);
  s4ptr->addBlock(s12ptr,s4tos12b);

  QList<Block*> s5tos10b;
  s5tos10b.push_back(adptr);
  s5tos10b.push_back(ahptr);
  s5ptr->addBlock(s10ptr,s5tos10b);
  QList<Block*> s5tos12b;
  s5tos12b.push_back(adptr);
  s5tos12b.push_back(aiptr);
  s5ptr->addBlock(s12ptr,s5tos12b);

  QList<Block*> s9tos3b;
  s9tos3b.push_back(adptr);
  s9tos3b.push_back(abptr);
  s9ptr->addBlock(s3ptr,s9tos3b);
  QList<Block*> s9tos2b;
  s9tos2b.push_back(adptr);
  s9tos2b.push_back(aaptr);
  s9ptr->addBlock(s2ptr,s9tos2b);

  QList<Block*> s11tos3b;
  s11tos3b.push_back(adptr);
  s11tos3b.push_back(abptr);
  s11ptr->addBlock(s3ptr,s11tos3b);
  QList<Block*> s11tos2b;
  s11tos2b.push_back(adptr);
  s11tos2b.push_back(aaptr);
  s11ptr->addBlock(s2ptr,s11tos2b); 

  QList<Block*> ww2tos10b;
  ww2tos10b.push_back(ahptr);
  ww2ptr->addBlockZuH(s10ptr->getS_id(),ww2tos10b);
  QList<Block*> ww2tos12b;
  ww2tos12b.push_back(aiptr);
  ww2ptr->addBlockZuH(s12ptr->getS_id(),ww2tos12b);

  QList<Block*> ww1tos2b;
  ww1tos2b.push_back(aaptr);
  ww1ptr->addBlockZuH(s2ptr->getS_id(),ww1tos2b);
  QList<Block*> ww1tos3b;
  ww1tos3b.push_back(abptr);
  ww1ptr->addBlockZuH(s3ptr->getS_id(),ww1tos3b);

  QList<Block*> s9toww1b;
  s9toww1b.push_back(adptr);
  s9ptr->addBlockZuRangier(ww1ptr,s9toww1b);
  QList<Block*> s11toww1b;
  s11toww1b.push_back(adptr);
  s11ptr->addBlockZuRangier(ww1ptr,s11toww1b);

  QList<Block*> s8tos2b;
  s8tos2b.push_back(afptr);
  s8tos2b.push_back(aaptr);
  s8ptr->addBlock(s2ptr,s8tos2b);
  QList<Block*> s8tos6b;
  s8tos6b.push_back(afptr);
  s8tos6b.push_back(agptr);
  s8ptr->addBlock(s6ptr,s8tos6b);

  // ++++++++++++++++++++++++ muss als erstes mal initialisiert werden...!
  stellwerkstecptr->add_Signal(s1ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s2ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s3ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s4ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s5ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s6ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s7ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s8ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s9ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s10ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s11ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s12ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s13ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s14ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s15ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(ww1ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww2ptr->getV_id(),false);


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
  //aaptr->addBus( bu1ptr );

  
  //signals and slots
  QObject::connect(s1ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s2ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s3ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s4ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s5ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s6ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s7ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s8ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s9ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s10ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s11ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s12ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s13ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s14ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s15ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww1ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww2ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(aaptr,&Block::zugpassiert,s1ptr,&HSignal::zugpassiert);
  QObject::connect(aaptr,&Block::zugpassiert,s13ptr,&HSignal::zugpassiert);
  QObject::connect(acptr,&Block::zugpassiert,s4ptr,&HSignal::zugpassiert);
  QObject::connect(aeptr,&Block::zugpassiert,s4ptr,&HSignal::zugpassiert);
  QObject::connect(afptr,&Block::zugpassiert,s4ptr,&HSignal::zugpassiert);
  QObject::connect(adptr,&Block::zugpassiert,s5ptr,&HSignal::zugpassiert);
  QObject::connect(adptr,&Block::zugpassiert,s9ptr,&HSignal::zugpassiert);
  QObject::connect(adptr,&Block::zugpassiert,s11ptr,&HSignal::zugpassiert);
  QObject::connect(afptr,&Block::zugpassiert,s8ptr,&HSignal::zugpassiert);
  
  QObject::connect(aiptr,&Block::zugpassiertW,ww2ptr,&WSignal::zugpassiertW);
  QObject::connect(ahptr,&Block::zugpassiertW,ww2ptr,&WSignal::zugpassiertW);
  QObject::connect(aaptr,&Block::zugpassiertW,ww1ptr,&WSignal::zugpassiertW);
  QObject::connect(abptr,&Block::zugpassiertW,ww1ptr,&WSignal::zugpassiertW);
  
  //Hier wird das Grenzsignal übergeben
  aaptr->addpassiert(s13ptr->getS_id(),acptr);
  acptr->addpassiert(s4ptr->getS_id(),aaptr);
  aeptr->addpassiert(s4ptr->getS_id(),aaptr);
  afptr->addpassiert(s4ptr->getS_id(),aaptr);
  adptr->addpassiert(s4ptr->getS_id(),aaptr);
  adptr->addpassiert(s5ptr->getS_id(),abptr);
  adptr->addpassiert(s9ptr->getS_id(),ahptr);
  adptr->addpassiert(s11ptr->getS_id(),aiptr);
  
  ahptr->addpassiert(ww2ptr->getV_id(),adptr);
  aiptr->addpassiert(ww2ptr->getV_id(),adptr);
  aaptr->addpassiert(ww1ptr->getV_id(),adptr);
  abptr->addpassiert(ww1ptr->getV_id(),adptr);
  
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
  w1ptr->addWeichenitem(w1rectab,w1rectge,w1label);
  w1ptr->moveLabel(480,810);
  
  QGraphicsRectItem *abrect = new QGraphicsRectItem();
  abrect->setRect(0,0,10,1200);
  abrect->setPos(QPointF(425,800));
  abrect->setBrush(QColor(79,79,79));
  abrect->setRotation(90);
  abptr->addBlockitems(abrect);
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
  w2ptr->addWeichenitem(w2rectab,w2rectge,w2label);
  w2ptr->moveLabel(420,715);

  QGraphicsRectItem *aarect = new QGraphicsRectItem();
  aarect->setRect(0,0,10,1200);
  aarect->setPos(QPointF(425,730));
  aarect->setBrush(QColor(79,79,79));
  aarect->setRotation(90);
  aaptr->addBlockitems(aarect);
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
  w4ptr->addWeichenitem(w4rectab,w4rectge,w4label);
  w4ptr->moveLabel(-845,810);

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
  w3ptr->addWeichenitem(w3rectab,w3rectge,w3label);
  w3ptr->moveLabel(-795,715);

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
  w5ptr->addWeichenitem(w5rectab,w5rectge,w5label);
  w5ptr->moveLabel(-855,715);

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
  w6ptr->addWeichenitem(w6rectab,w6rectge,w6label);
  w6ptr->moveLabel(-938,578);

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
  w7ptr->addWeichenitem(w7rectab,w7rectge,w7label);
  w7ptr->moveLabel(-938,508);

  QGraphicsRectItem *aerect = new QGraphicsRectItem();
  aerect->setRect(0,0,10,70);
  aerect->setPos(QPointF(-918,375));
  aerect->setBrush(QColor(79,79,79));
  aeptr->addBlockitems(aerect);
  scene->addItem(aerect);
  QGraphicsRectItem *aerect2 = new QGraphicsRectItem();
  aerect2->setRect(0,0,10,305);
  aerect2->setPos(QPointF(-918,375));
  aerect2->setBrush(QColor(79,79,79));
  aerect2->setRotation(-90);
  aeptr->addBlockitems(aerect2);
  scene->addItem(aerect2);

  QGraphicsRectItem *acrect = new QGraphicsRectItem();
  acrect->setRect(0,0,10,70);
  acrect->setPos(QPointF(-925,730));
  acrect->setBrush(QColor(79,79,79));
  acrect->setRotation(90);
  acptr->addBlockitems(acrect);
  scene->addItem(acrect);
  QGraphicsRectItem *acrect2 = new QGraphicsRectItem();
  acrect2->setRect(0,0,10,450);
  acrect2->setPos(QPointF(-995,290));
  acrect2->setBrush(QColor(79,79,79));
  acptr->addBlockitems(acrect2);
  scene->addItem(acrect2);
  QGraphicsRectItem *acrect3 = new QGraphicsRectItem();
  acrect3->setRect(0,0,10,380);
  acrect3->setPos(QPointF(-995,290));
  acrect3->setBrush(QColor(79,79,79));
  acrect3->setRotation(-90);
  acptr->addBlockitems(acrect3);
  scene->addItem(acrect3);

  QGraphicsRectItem *adrect = new QGraphicsRectItem();
  adrect->setRect(0,0,10,225);
  adrect->setPos(QPointF(-850,800));
  adrect->setBrush(QColor(79,79,79));
  adrect->setRotation(90);
  adptr->addBlockitems(adrect);
  scene->addItem(adrect);
  QGraphicsRectItem *adrect2 = new QGraphicsRectItem();
  adrect2->setRect(0,0,10,600);
  adrect2->setPos(QPointF(-1075,200));
  adrect2->setBrush(QColor(79,79,79));
  adptr->addBlockitems(adrect2);
  scene->addItem(adrect2);
  QGraphicsRectItem *adrect3 = new QGraphicsRectItem();
  adrect3->setRect(0,0,10,140);
  adrect3->setPos(QPointF(-1075,200));
  adrect3->setBrush(QColor(79,79,79));
  adrect3->setRotation(-90);
  adptr->addBlockitems(adrect3);
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
  w9ptr->addWeichenitem(w9rectab,w9rectge,w9label);
  w9ptr->moveLabel(-935,200);

  QGraphicsRectItem *airect = new QGraphicsRectItem();
  airect->setRect(0,0,10,1000);
  airect->setPos(QPointF(-860,200));
  airect->setBrush(QColor(79,79,79));
  airect->setRotation(-90);
  aiptr->addBlockitems(airect);
  scene->addItem(airect);

  QGraphicsRectItem *ahrect = new QGraphicsRectItem();
  ahrect->setRect(0,0,10,55);
  ahrect->setPos(QPointF(-868,120));
  ahrect->setBrush(QColor(79,79,79));
  ahrect->setRotation(45);
  ahptr->addBlockitems(ahrect);
  scene->addItem(ahrect);
  QGraphicsRectItem *ahrect2 = new QGraphicsRectItem();
  ahrect2->setRect(0,0,10,1003);
  ahrect2->setPos(QPointF(-863,130));
  ahrect2->setBrush(QColor(79,79,79));
  ahrect2->setRotation(-90);
  ahptr->addBlockitems(ahrect2);
  scene->addItem(ahrect2);
  QGraphicsRectItem *ahrect3 = new QGraphicsRectItem();
  ahrect3->setRect(0,0,10,55);
  ahrect3->setPos(QPointF(140,130));
  ahrect3->setBrush(QColor(79,79,79));
  ahrect3->setRotation(-45);
  ahptr->addBlockitems(ahrect3);
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
  w10ptr->addWeichenitem(w10rectab,w10rectge,w10label);
  w10ptr->moveLabel(200,200);

  QGraphicsRectItem *afrect = new QGraphicsRectItem();
  afrect->setRect(0,0,10,675);
  afrect->setPos(QPointF(-875,495));
  afrect->setBrush(QColor(79,79,79));
  afrect->setRotation(-90);
  afptr->addBlockitems(afrect);
  scene->addItem(afrect);

  QGraphicsRectItem *agrect = new QGraphicsRectItem();
  agrect->setRect(0,0,10,630);
  agrect->setPos(QPointF(-875,615));
  agrect->setBrush(QColor(79,79,79));
  agrect->setRotation(-90);
  agptr->addBlockitems(agrect);
  scene->addItem(agrect);
  QGraphicsRectItem *agrect2 = new QGraphicsRectItem();
  agrect2->setRect(0,0,10,125);
  agrect2->setPos(QPointF(-160,520));
  agrect2->setBrush(QColor(79,79,79));
  agrect2->setRotation(45);
  agptr->addBlockitems(agrect2);
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
  w8ptr->addWeichenitem(w8rectab,w8rectge,w8label);
  w8ptr->moveLabel(-140,475);

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
  s4ptr->addHSignalitem(s4item1,s4item2, s4item3, s4label,s4push,spweg4,sphin4);
  s4ptr->moveButton(-755,725);
  s4ptr->moveLabel(-700,705);

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
  s5ptr->addHSignalitem(s5item1,s5item2, s5item3, s5label,s5push,spweg5,sphin5);
  s5ptr->moveButton(-755,795);
  s5ptr->moveLabel(-700,775);

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
  s2ptr->addHSignalitem(s2item1,s2item2, s2item3, s2label,s2push,spweg2,sphin2);
  s2ptr->moveButton(385,725);
  s2ptr->moveLabel(318,750);

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
  s3ptr->addHSignalitem(s3item1,s3item2, s3item3, s3label,s3push,spweg3,sphin3);
  s3ptr->moveButton(385,795);
  s3ptr->moveLabel(318,820);

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
  s13ptr->addHSignalitem(s13item1,s13item2, s13item3, s13label,s13push,spweg13,sphin13);
  s13ptr->moveButton(-965,725);
  s13ptr->moveLabel(-1037,750);

  QGraphicsSvgItem *itemv11 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *itemv12 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(itemv11);
  scene->addItem(itemv12);
  itemv11->setPos(QPointF(515,725));
  itemv12->setPos(QPointF(515,725));
  itemv11->setScale(0.1);
  itemv12->setScale(0.1);
  itemv11->setRotation(-90);
  itemv12->setRotation(-90);
  v1ptr->addVSignalitems(itemv11, itemv12);

  QGraphicsSvgItem *s1item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s1item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s1item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s1item1);
  scene->addItem(s1item3);
  scene->addItem(s1item2);
  s1item1->setPos(QPointF(475,720));
  s1item2->setPos(QPointF(475,720));
  s1item3->setPos(QPointF(475,720));
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
  QGraphicsRectItem *sphin1 = new QGraphicsRectItem();
  sphin1->setRect(0,0,10,10);
  sphin1->setBrush(Qt::yellow);
  sphin1->setPos(QPointF(555,700));
  QGraphicsRectItem *spweg1 = new QGraphicsRectItem();
  spweg1->setRect(0,0,10,10);
  spweg1->setBrush(Qt::yellow);
  spweg1->setPos(QPointF(555,710));
  scene->addItem(spweg1);
  scene->addItem(sphin1);
  s1ptr->addHSignalitem(s1item1,s1item2, s1item3, s1label,s1push,spweg1,sphin1);
  s1ptr->moveButton(500,725);
  s1ptr->moveLabel(570,705);
  
  /////////////NOCH VON WSIGNAL
  //soll on top of scene sein...
  QPushButton *ww1push = new QPushButton();
  scene->addWidget(ww1push);
  ww1ptr->addButtonAndLabel(ww1label,ww1push);
  ww1ptr->moveButton(-900,795);
  ww1ptr->moveLabel(-940,820);
  // und NR 2 W
  QPushButton *ww2push = new QPushButton();
  scene->addWidget(ww2push);
  ww2ptr->addButtonAndLabel(ww2label,ww2push);
  ww2ptr->moveButton(-980,185);
  ww2ptr->moveLabel(-1015,205);
  ///////////////////////////

  QGraphicsSvgItem *itemv31 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *itemv32 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(itemv31);
  scene->addItem(itemv32);
  itemv31->setPos(QPointF(-900,520));
  itemv32->setPos(QPointF(-900,520));
  itemv31->setScale(0.1);
  itemv32->setScale(0.1);
  v3ptr->addVSignalitems(itemv31, itemv32);

  QGraphicsSvgItem *itemv51 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *itemv52 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(itemv51);
  scene->addItem(itemv52);
  itemv51->setPos(QPointF(-980,520));
  itemv52->setPos(QPointF(-980,520));
  itemv51->setScale(0.1);
  itemv52->setScale(0.1);
  v5ptr->addVSignalitems(itemv51, itemv52);

  QGraphicsSvgItem *itemv61 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *itemv62 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(itemv61);
  scene->addItem(itemv62);
  itemv61->setPos(QPointF(-1060,520));
  itemv62->setPos(QPointF(-1060,520));
  itemv61->setScale(0.1);
  itemv62->setScale(0.1);
  v6ptr->addVSignalitems(itemv61, itemv62);

  QGraphicsSvgItem *itemv71 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *itemv72 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(itemv71);
  scene->addItem(itemv72);
  itemv71->setPos(QPointF(-1080,620));
  itemv72->setPos(QPointF(-1080,620));
  itemv71->setScale(0.1);
  itemv72->setScale(0.1);
  itemv71->setRotation(180);
  itemv72->setRotation(180);
  v7ptr->addVSignalitems(itemv71, itemv72);

  QGraphicsSvgItem *itemv101 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *itemv102 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(itemv101);
  scene->addItem(itemv102);
  itemv101->setPos(QPointF(-1000,620));
  itemv102->setPos(QPointF(-1000,620));
  itemv101->setScale(0.1);
  itemv102->setScale(0.1);
  itemv101->setRotation(180);
  itemv102->setRotation(180);
  v10ptr->addVSignalitems(itemv101, itemv102);

  QGraphicsSvgItem *itemv41 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *itemv42 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(itemv41);
  scene->addItem(itemv42);
  itemv41->setPos(QPointF(-920,620));
  itemv42->setPos(QPointF(-920,620));
  itemv41->setScale(0.1);
  itemv42->setScale(0.1);
  itemv41->setRotation(180);
  itemv42->setRotation(180);
  v4ptr->addVSignalitems(itemv41, itemv42);

  QGraphicsSvgItem *s9item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s9item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s9item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s9item1);
  scene->addItem(s9item3);
  scene->addItem(s9item2);
  s9item1->setPos(QPointF(-865,110));
  s9item2->setPos(QPointF(-865,110));
  s9item3->setPos(QPointF(-865,110));
  s9item1->setScale(0.1);
  s9item2->setScale(0.1);
  s9item3->setScale(0.1);
  s9item1->setRotation(-90);
  s9item2->setRotation(-90);
  s9item3->setRotation(-90);
  QPushButton *s9push = new QPushButton();
  scene->addWidget(s9push);
  QLabel *s9label = new QLabel();
  scene->addWidget(s9label);
  //speicher GUIs
  QGraphicsRectItem *sphin9 = new QGraphicsRectItem();
  sphin9->setRect(0,0,10,10);
  sphin9->setBrush(Qt::yellow);
  sphin9->setPos(QPointF(-805,90));
  QGraphicsRectItem *spweg9 = new QGraphicsRectItem();
  spweg9->setRect(0,0,10,10);
  spweg9->setBrush(Qt::yellow);
  spweg9->setPos(QPointF(-805,100));
  scene->addItem(spweg9);
  scene->addItem(sphin9);
  s9ptr->addHSignalitem(s9item1,s9item2, s9item3, s9label,s9push,spweg9,sphin9);
  s9ptr->moveButton(-840,115);
  s9ptr->moveLabel(-790,90);

  QGraphicsSvgItem *s11item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s11item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s11item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s11item1);
  scene->addItem(s11item3);
  scene->addItem(s11item2);
  s11item1->setPos(QPointF(-865,180));
  s11item2->setPos(QPointF(-865,180));
  s11item3->setPos(QPointF(-865,180));
  s11item1->setScale(0.1);
  s11item2->setScale(0.1);
  s11item3->setScale(0.1);
  s11item1->setRotation(-90);
  s11item2->setRotation(-90);
  s11item3->setRotation(-90);
  QPushButton *s11push = new QPushButton();
  scene->addWidget(s11push);
  QLabel *s11label = new QLabel();
  scene->addWidget(s11label);
  //speicher GUIs
  QGraphicsRectItem *sphin11 = new QGraphicsRectItem();
  sphin11->setRect(0,0,10,10);
  sphin11->setBrush(Qt::yellow);
  sphin11->setPos(QPointF(-805,160));
  QGraphicsRectItem *spweg11 = new QGraphicsRectItem();
  spweg11->setRect(0,0,10,10);
  spweg11->setBrush(Qt::yellow);
  spweg11->setPos(QPointF(-805,170));
  scene->addItem(spweg11);
  scene->addItem(sphin11);
  s11ptr->addHSignalitem(s11item1,s11item2, s11item3, s11label,s11push,spweg11,sphin11);
  s11ptr->moveButton(-840,185);
  s11ptr->moveLabel(-790,160);

  QGraphicsSvgItem *s10item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s10item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s10item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s10item1);
  scene->addItem(s10item3);
  scene->addItem(s10item2);
  s10item1->setPos(QPointF(150,140));
  s10item2->setPos(QPointF(150,140));
  s10item3->setPos(QPointF(150,140));
  s10item1->setScale(0.1);
  s10item2->setScale(0.1);
  s10item3->setScale(0.1);
  s10item1->setRotation(90);
  s10item2->setRotation(90);
  s10item3->setRotation(90);
  QPushButton *s10push = new QPushButton();
  scene->addWidget(s10push);
  QLabel *s10label = new QLabel();
  scene->addWidget(s10label);
  //speicher GUIs
  QGraphicsRectItem *sphin10 = new QGraphicsRectItem();
  sphin10->setRect(0,0,10,10);
  sphin10->setBrush(Qt::yellow);
  sphin10->setPos(QPointF(75,150));
  QGraphicsRectItem *spweg10 = new QGraphicsRectItem();
  spweg10->setRect(0,0,10,10);
  spweg10->setBrush(Qt::yellow);
  spweg10->setPos(QPointF(75,140));
  scene->addItem(spweg10);
  scene->addItem(sphin10);
  s10ptr->addHSignalitem(s10item1,s10item2, s10item3, s10label,s10push,spweg10,sphin10);
  s10ptr->moveButton(110,115);
  s10ptr->moveLabel(45,140);

  QGraphicsSvgItem *s12item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s12item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s12item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s12item1);
  scene->addItem(s12item3);
  scene->addItem(s12item2);
  s12item1->setPos(QPointF(150,210));
  s12item2->setPos(QPointF(150,210));
  s12item3->setPos(QPointF(150,210));
  s12item1->setScale(0.1);
  s12item2->setScale(0.1);
  s12item3->setScale(0.1);
  s12item1->setRotation(90);
  s12item2->setRotation(90);
  s12item3->setRotation(90);
  QPushButton *s12push = new QPushButton();
  scene->addWidget(s12push);
  QLabel *s12label = new QLabel();
  scene->addWidget(s12label);
  //speicher GUIs
  QGraphicsRectItem *sphin12 = new QGraphicsRectItem();
  sphin12->setRect(0,0,10,10);
  sphin12->setBrush(Qt::yellow);
  sphin12->setPos(QPointF(75,220));
  QGraphicsRectItem *spweg12 = new QGraphicsRectItem();
  spweg12->setRect(0,0,10,10);
  spweg12->setBrush(Qt::yellow);
  spweg12->setPos(QPointF(75,210));
  scene->addItem(spweg12);
  scene->addItem(sphin12);
  s12ptr->addHSignalitem(s12item1,s12item2, s12item3, s12label,s12push,spweg12,sphin12);
  s12ptr->moveButton(110,185);
  s12ptr->moveLabel(45,210);
  
  QGraphicsSvgItem *itemv81 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *itemv82 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(itemv81);
  scene->addItem(itemv82);
  itemv81->setPos(QPointF(-215,500));
  itemv82->setPos(QPointF(-215,500));
  itemv81->setScale(0.1);
  itemv82->setScale(0.1);
  itemv81->setRotation(90);
  itemv82->setRotation(90);
  v8ptr->addVSignalitems(itemv81, itemv82);
  
  QGraphicsSvgItem *s7item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s7item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s7item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s7item1);
  scene->addItem(s7item3);
  scene->addItem(s7item2);
  s7item1->setPos(QPointF(-175,505));
  s7item2->setPos(QPointF(-175,505));
  s7item3->setPos(QPointF(-175,505));
  s7item1->setScale(0.1);
  s7item2->setScale(0.1);
  s7item3->setScale(0.1);
  s7item1->setRotation(90);
  s7item2->setRotation(90);
  s7item3->setRotation(90);
  QPushButton *s7push = new QPushButton();
  scene->addWidget(s7push);
  QLabel *s7label = new QLabel();
  scene->addWidget(s7label);
  //speicher GUIs
  QGraphicsRectItem *sphin7 = new QGraphicsRectItem();
  sphin7->setRect(0,0,10,10);
  sphin7->setBrush(Qt::yellow);
  sphin7->setPos(QPointF(-255,505));
  QGraphicsRectItem *spweg7 = new QGraphicsRectItem();
  spweg7->setRect(0,0,10,10);
  spweg7->setBrush(Qt::yellow);
  spweg7->setPos(QPointF(-255,515));
  scene->addItem(spweg7);
  scene->addItem(sphin7);
  s7ptr->addHSignalitem(s7item1,s7item2, s7item3, s7label,s7push,spweg7,sphin7);
  s7ptr->moveButton(-205,480);
  s7ptr->moveLabel(-290,510);

  QGraphicsSvgItem *itemv91 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *itemv92 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(itemv91);
  scene->addItem(itemv92);
  itemv91->setPos(QPointF(-185,565));
  itemv92->setPos(QPointF(-185,565));
  itemv91->setScale(0.1);
  itemv92->setScale(0.1);
  itemv91->setRotation(45);
  itemv92->setRotation(45);
  v9ptr->addVSignalitems(itemv91, itemv92);
  
  QGraphicsSvgItem *s6item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s6item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s6item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s6item1);
  scene->addItem(s6item3);
  scene->addItem(s6item2);
  s6item1->setPos(QPointF(-150,535));
  s6item2->setPos(QPointF(-150,535));
  s6item3->setPos(QPointF(-150,535));
  s6item1->setScale(0.1);
  s6item2->setScale(0.1);
  s6item3->setScale(0.1);
  s6item1->setRotation(45);
  s6item2->setRotation(45);
  s6item3->setRotation(45);
  QPushButton *s6push = new QPushButton();
  scene->addWidget(s6push);
  QLabel *s6label = new QLabel();
  scene->addWidget(s6label);
  //speicher GUIs
  QGraphicsRectItem *sphin6 = new QGraphicsRectItem();
  sphin6->setRect(0,0,10,10);
  sphin6->setBrush(Qt::yellow);
  sphin6->setPos(QPointF(-215,600));
  QGraphicsRectItem *spweg6 = new QGraphicsRectItem();
  spweg6->setRect(0,0,10,10);
  spweg6->setBrush(Qt::yellow);
  spweg6->setPos(QPointF(-203,603));
  spweg6->setRotation(45);
  sphin6->setRotation(45);
  scene->addItem(spweg6);
  scene->addItem(sphin6);
  s6ptr->addHSignalitem(s6item1,s6item2, s6item3, s6label,s6push,spweg6,sphin6);
  s6ptr->moveButton(-180,525);
  s6ptr->moveLabel(-240,615);

  QGraphicsSvgItem *s8item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s8item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s8item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s8item1);
  scene->addItem(s8item3);
  scene->addItem(s8item2);
  s8item1->setPos(QPointF(-150,478));
  s8item2->setPos(QPointF(-150,478));
  s8item3->setPos(QPointF(-150,478));
  s8item1->setScale(0.1);
  s8item2->setScale(0.1);
  s8item3->setScale(0.1);
  s8item1->setRotation(-90);
  s8item2->setRotation(-90);
  s8item3->setRotation(-90);
  QPushButton *s8push = new QPushButton();
  scene->addWidget(s8push);
  QLabel *s8label = new QLabel();
  scene->addWidget(s8label);
  //speicher GUIs
  QGraphicsRectItem *sphin8 = new QGraphicsRectItem();
  sphin8->setRect(0,0,10,10);
  sphin8->setBrush(Qt::yellow);
  sphin8->setPos(QPointF(-715,700));
  QGraphicsRectItem *spweg8 = new QGraphicsRectItem();
  spweg8->setRect(0,0,10,10);
  spweg8->setBrush(Qt::yellow);
  spweg8->setPos(QPointF(-715,710));
  scene->addItem(spweg8);
  scene->addItem(sphin8);
  s8ptr->addHSignalitem(s8item1,s8item2, s8item3, s8label,s8push,spweg8,sphin8);
  s8ptr->moveButton(-110,480);
  s8ptr->moveLabel(-70,465);

  
  //QPushButtonPart
  clickmanager *c1 = new clickmanager;
  QObject::connect(s1ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) ); //QOverload wegen zeideutigkeit von recieveFS; C++11, C++14 qOverload<>
  QObject::connect(s2ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s3ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s4ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s5ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s6ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s7ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s8ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s9ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s10ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s11ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s12ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s13ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s14ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s15ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  //ebenfalls für WSignal
  QObject::connect(ww1ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww2ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
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
  
  QObject::connect(s1ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s2ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s3ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s4ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s5ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  
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
  
  delete stellwerkstecptr;
  delete aaptr; delete abptr; delete acptr; delete adptr; delete aeptr; delete afptr; delete agptr; delete ahptr; delete aiptr;
  delete ww1ptr; delete ww2ptr;
  delete v1ptr; delete v2ptr; delete v3ptr; delete v4ptr; delete v5ptr; delete v6ptr; delete v7ptr; delete v8ptr; delete v9ptr; delete v10ptr;
  delete w1ptr; delete w2ptr; delete w3ptr; delete w4ptr; delete w5ptr; delete w6ptr; delete w7ptr; delete w8ptr; delete w9ptr; delete w10ptr;
  delete s1ptr; delete s2ptr; delete s3ptr; delete s4ptr; delete s5ptr; delete s6ptr; delete s7ptr; delete s8ptr; delete s9ptr; delete s10ptr; delete s11ptr; delete s12ptr; delete s13ptr; //delete s14ptr; delete s15ptr;
  
  return eofprogram;

}
