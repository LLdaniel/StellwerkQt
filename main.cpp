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
  Weiche *w11ptr = new Weiche(11);
  Weiche *w12ptr = new Weiche(12);
  Weiche *w13ptr = new Weiche(13);
  Weiche *w14ptr = new Weiche(14);
  Weiche *w15ptr = new Weiche(15);
  Weiche *w16ptr = new Weiche(16);
  Weiche *w17ptr = new Weiche(17);
  Weiche *w18ptr = new Weiche(18);
  Weiche *w19ptr = new Weiche(19);
  //Weiche *w20ptr = new Weiche(20);
  //Weiche *w21ptr = new Weiche(21);
  //Weiche *w22ptr = new Weiche(22);
  //Weiche *w23ptr = new Weiche(23);
  //:::Block:::
  Block *abptr = new Block("ab", stellwerkstecptr);
  Block *acptr = new Block("ac", stellwerkstecptr);
  Block *adptr = new Block("ad", stellwerkstecptr);
  Block *aeptr = new Block("ae", stellwerkstecptr);
  Block *afptr = new Block("af", stellwerkstecptr);
  Block *agptr = new Block("ag", stellwerkstecptr);
  Block *ahptr = new Block("ah", stellwerkstecptr);
  Block *aiptr = new Block("ai", stellwerkstecptr);
  Block *ajptr = new Block("aj", stellwerkstecptr);
  Block *akptr = new Block("ak", stellwerkstecptr);
  Block *alptr = new Block("al", stellwerkstecptr);
  Block *amptr = new Block("am", stellwerkstecptr);
  Block *anptr = new Block("an", stellwerkstecptr);
  Block *aoptr = new Block("ao", stellwerkstecptr);
  Block *apptr = new Block("ap", stellwerkstecptr);
  Block *aqptr = new Block("aq", stellwerkstecptr);
  Block *arptr = new Block("ar", stellwerkstecptr);
  Block *asptr = new Block("as", stellwerkstecptr);
  Block *atptr = new Block("at", stellwerkstecptr);
  Block *auptr = new Block("au", stellwerkstecptr);
  Block *avptr = new Block("av", stellwerkstecptr);
  Block *awptr = new Block("aw", stellwerkstecptr);
  Block *axptr = new Block("ax", stellwerkstecptr);
  Block *ayptr = new Block("ay", stellwerkstecptr);
  //..
  Block *bmptr = new Block("bm", stellwerkstecptr);
  Block *bnptr = new Block("bn", stellwerkstecptr);
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
  VSignal *v11ptr = new VSignal(11);
  VSignal *v12ptr = new VSignal(12);
  VSignal *v13ptr = new VSignal(13);
  VSignal *v14ptr = new VSignal(14);
  VSignal *v15ptr = new VSignal(15);
  VSignal *v16ptr = new VSignal(16);
  v1ptr->setRichtung("S001","S004");
  v2ptr->setRichtung("S013","S002");
  v3ptr->setRichtung("S004","S015");
  v3ptr->setRichtung("S004","S007");
  v4ptr->setRichtung("S008","S002");
  v4ptr->setRichtung("S008","S002");
  v4ptr->setRichtung("S008","S006");
  v4ptr->setRichtung("S016","S002");
  v4ptr->setRichtung("S017","S002");
  v4ptr->setRichtung("S018","S002");
  v4ptr->setRichtung("S016","S006");
  v4ptr->setRichtung("S017","S006");
  v4ptr->setRichtung("S018","S006");
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
  v10ptr->setRichtung("S016","S013");
  v10ptr->setRichtung("S017","S013");
  v10ptr->setRichtung("S018","S013");
  v11ptr->setRichtung("S015","S019");
  v11ptr->setRichtung("S015","S020");
  v11ptr->setRichtung("S015","S021");
  v11ptr->setRichtung("S014","S019");
  v11ptr->setRichtung("S014","S020");
  v11ptr->setRichtung("S014","S021");
  v12ptr->setRichtung("S019","S022");
  v13ptr->setRichtung("S020","S022");
  v14ptr->setRichtung("S022","S016");
  v15ptr->setRichtung("S021","S023");
  v16ptr->setRichtung("S023","S018");
  v16ptr->setRichtung("S023","S017");
  v16ptr->setRichtung("S023","S016");
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
  HSignal *s16ptr = new HSignal(16);
  HSignal *s17ptr = new HSignal(17);
  HSignal *s18ptr = new HSignal(18);
  HSignal *s19ptr = new HSignal(19);
  HSignal *s20ptr = new HSignal(20);
  HSignal *s21ptr = new HSignal(21);
  HSignal *s22ptr = new HSignal(22);
  HSignal *s23ptr = new HSignal(23);
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
  s14ptr->addVSignal(v5ptr,"r");
  s14ptr->addVSignal(v11ptr,"v");
  s15ptr->addVSignal(v3ptr,"r");
  s15ptr->addVSignal(v11ptr,"v");
  s16ptr->addVSignal(v14ptr,"r");
  s16ptr->addVSignal(v16ptr,"r");
  s16ptr->addVSignal(v4ptr,"v");
  s16ptr->addVSignal(v10ptr,"v");
  s17ptr->addVSignal(v16ptr,"r");
  s17ptr->addVSignal(v4ptr,"v");
  s17ptr->addVSignal(v10ptr,"v");
  s18ptr->addVSignal(v16ptr,"r");
  s18ptr->addVSignal(v4ptr,"v");
  s18ptr->addVSignal(v10ptr,"v");
  s19ptr->addVSignal(v11ptr,"r");
  s19ptr->addVSignal(v12ptr,"v");
  s20ptr->addVSignal(v11ptr,"r");
  s20ptr->addVSignal(v13ptr,"v");
  s21ptr->addVSignal(v11ptr,"r");
  s21ptr->addVSignal(v15ptr,"v");
  s22ptr->addVSignal(v12ptr,"r");
  s22ptr->addVSignal(v13ptr,"r");
  s22ptr->addVSignal(v15ptr,"r");
  s22ptr->addVSignal(v14ptr,"v");
  s23ptr->addVSignal(v15ptr,"r");
  s23ptr->addVSignal(v16ptr,"v");
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

  WSignal *ww3ptr = new WSignal(3);
  ww3ptr->setRichtung("S025","S011");
  ww3ptr->setRichtung("S025","S009");
  //s25ptr->addWSignal(ww3ptr,"v");
  s9ptr->addWSignal(ww3ptr,"r");
  s11ptr->addWSignal(ww3ptr,"r");
  //
  QGraphicsSvgItem *itemw3 = new QGraphicsSvgItem(":/resources/WSfahrt.svg");
  QGraphicsSvgItem *itemw32 = new QGraphicsSvgItem(":/resources/WShalt.svg");
  scene->addItem(itemw3);
  scene->addItem(itemw32);
  itemw3->setPos(QPointF(240,185));
  itemw32->setPos(QPointF(240,185));
  itemw3->setRotation(-90);
  itemw32->setRotation(-90);
  itemw3->setScale(0.05);
  itemw32->setScale(0.05);
  ww3ptr->addVSignalitems(itemw3, itemw32);
  QLabel *ww3label = new QLabel();
  scene->addWidget(ww3label);
  
  
  WSignal *ww4ptr = new WSignal(4);
  ww4ptr->setRichtung("S016","S013");
  ww4ptr->setRichtung("S017","S013");
  ww4ptr->setRichtung("S018","S013");
  ww4ptr->setRichtung("S016","S006");
  ww4ptr->setRichtung("S017","S006");
  ww4ptr->setRichtung("S018","S006");
  ww4ptr->setRichtung("S016","S002");
  ww4ptr->setRichtung("S017","S002");
  ww4ptr->setRichtung("S018","S002");
  s16ptr->addWSignal(ww4ptr,"v");
  s17ptr->addWSignal(ww4ptr,"v");
  s18ptr->addWSignal(ww4ptr,"v");
  s13ptr->addWSignal(ww4ptr,"r");
  s6ptr->addWSignal(ww4ptr,"r");
  s2ptr->addWSignal(ww4ptr,"r");
  //
  QGraphicsSvgItem *itemw4 = new QGraphicsSvgItem(":/resources/WSfahrt.svg");
  QGraphicsSvgItem *itemw42 = new QGraphicsSvgItem(":/resources/WShalt.svg");
  scene->addItem(itemw4);
  scene->addItem(itemw42);
  itemw4->setPos(QPointF(320,-615));
  itemw42->setPos(QPointF(320,-615));
  itemw4->setRotation(-90);
  itemw42->setRotation(-90);
  itemw4->setScale(0.05);
  itemw42->setScale(0.05);
  ww4ptr->addVSignalitems(itemw4, itemw42);
  QLabel *ww4label = new QLabel();
  scene->addWidget(ww4label);
  
  WSignal *ww5ptr = new WSignal(5);
  ww5ptr->setRichtung("S015","S021");
  ww5ptr->setRichtung("S015","S020");
  ww5ptr->setRichtung("S015","S019");
  ww5ptr->setRichtung("S014","S021");
  ww5ptr->setRichtung("S014","S020");
  ww5ptr->setRichtung("S014","S019");
  s15ptr->addWSignal(ww5ptr,"v");
  s14ptr->addWSignal(ww5ptr,"v");
  s20ptr->addWSignal(ww5ptr,"r");
  s21ptr->addWSignal(ww5ptr,"r");
  s19ptr->addWSignal(ww5ptr,"r");  
  //
  QGraphicsSvgItem *itemw5 = new QGraphicsSvgItem(":/resources/WSfahrt.svg");
  QGraphicsSvgItem *itemw52 = new QGraphicsSvgItem(":/resources/WShalt.svg");
  scene->addItem(itemw5);
  scene->addItem(itemw52);
  itemw5->setPos(QPointF(410,-165));
  itemw52->setPos(QPointF(410,-165));
  itemw5->setRotation(180);
  itemw52->setRotation(180);
  itemw5->setScale(0.05);
  itemw52->setScale(0.05);
  ww5ptr->addVSignalitems(itemw5, itemw52);
  QLabel *ww5label = new QLabel();
  scene->addWidget(ww5label);
  
  WSignal *ww6ptr = new WSignal(6);
  //
  QGraphicsSvgItem *itemw6 = new QGraphicsSvgItem(":/resources/WSfahrt.svg");
  QGraphicsSvgItem *itemw62 = new QGraphicsSvgItem(":/resources/WShalt.svg");
  scene->addItem(itemw6);
  scene->addItem(itemw62);
  itemw6->setPos(QPointF(280,-165));
  itemw62->setPos(QPointF(280,-165));
  itemw6->setRotation(-90);
  itemw62->setRotation(-90);
  itemw6->setScale(0.05);
  itemw62->setScale(0.05);
  ww6ptr->addVSignalitems(itemw6, itemw62);
  QLabel *ww6label = new QLabel();
  scene->addWidget(ww6label);
  
  WSignal *ww7ptr = new WSignal(7);
  //
  QGraphicsSvgItem *itemw7 = new QGraphicsSvgItem(":/resources/WSfahrt.svg");
  QGraphicsSvgItem *itemw72 = new QGraphicsSvgItem(":/resources/WShalt.svg");
  scene->addItem(itemw7);
  scene->addItem(itemw72);
  itemw7->setPos(QPointF(400,-145));
  itemw72->setPos(QPointF(400,-145));
  itemw7->setRotation(90);
  itemw72->setRotation(90);
  itemw7->setScale(0.05);
  itemw72->setScale(0.05);
  ww7ptr->addVSignalitems(itemw7, itemw72);
  QLabel *ww7label = new QLabel();
  scene->addWidget(ww7label);
  
  WSignal *ww8ptr = new WSignal(8);
  //
  QGraphicsSvgItem *itemw8 = new QGraphicsSvgItem(":/resources/WSfahrt.svg");
  QGraphicsSvgItem *itemw82 = new QGraphicsSvgItem(":/resources/WShalt.svg");
  scene->addItem(itemw8);
  scene->addItem(itemw82);
  itemw8->setPos(QPointF(180,-165));
  itemw82->setPos(QPointF(180,-165));
  itemw8->setRotation(180);
  itemw82->setRotation(180);
  itemw8->setScale(0.05);
  itemw82->setScale(0.05);
  ww8ptr->addVSignalitems(itemw8, itemw82);
  QLabel *ww8label = new QLabel();
  scene->addWidget(ww8label);
  
  WSignal *ww9ptr = new WSignal(9);
  //
  QGraphicsSvgItem *itemw9 = new QGraphicsSvgItem(":/resources/WSfahrt.svg");
  QGraphicsSvgItem *itemw92 = new QGraphicsSvgItem(":/resources/WShalt.svg");
  scene->addItem(itemw9);
  scene->addItem(itemw92);
  itemw9->setPos(QPointF(-30,-245));
  itemw92->setPos(QPointF(-30,-245));
  itemw9->setRotation(-90);
  itemw92->setRotation(-90);
  itemw9->setScale(0.05);
  itemw92->setScale(0.05);
  ww9ptr->addVSignalitems(itemw9, itemw92);
  QLabel *ww9label = new QLabel();
  scene->addWidget(ww9label);
  
  WSignal *ww10ptr = new WSignal(10);
  //
  QGraphicsSvgItem *itemw10 = new QGraphicsSvgItem(":/resources/WSfahrt.svg");
  QGraphicsSvgItem *itemw102 = new QGraphicsSvgItem(":/resources/WShalt.svg");
  scene->addItem(itemw10);
  scene->addItem(itemw102);
  itemw10->setPos(QPointF(-670,-60));
  itemw102->setPos(QPointF(-670,-60));
  itemw10->setRotation(90);
  itemw102->setRotation(90);
  itemw10->setScale(0.05);
  itemw102->setScale(0.05);
  ww10ptr->addVSignalitems(itemw10, itemw102);
  QLabel *ww10label = new QLabel();
  scene->addWidget(ww10label);
  
  WSignal *ww11ptr = new WSignal(11);
  //
  QGraphicsSvgItem *itemw11 = new QGraphicsSvgItem(":/resources/WSfahrt.svg");
  QGraphicsSvgItem *itemw112 = new QGraphicsSvgItem(":/resources/WShalt.svg");
  scene->addItem(itemw11);
  scene->addItem(itemw112);
  itemw11->setPos(QPointF(-880,-165));
  itemw112->setPos(QPointF(-880,-165));
  itemw11->setRotation(-45);
  itemw112->setRotation(-45);
  itemw11->setScale(0.05);
  itemw112->setScale(0.05);
  ww11ptr->addVSignalitems(itemw11, itemw112);
  QLabel *ww11label = new QLabel();
  scene->addWidget(ww11label);
  
  //Weichenstatus ---------------------------------------------------
  QPair<Weiche*, bool> s1tos4w1(w2ptr,true);
  QList<QPair<Weiche*, bool>> s1tos4;
  s1tos4.push_back(s1tos4w1);
  s1ptr->addWeichenstatus(s4ptr,s1tos4);

  QPair<Weiche*, bool> ww3tos9w10(w10ptr,false);
  QList<QPair<Weiche*, bool>> ww3tos9;
  ww3tos9.push_back(ww3tos9w10);
  ww3ptr->addWeichenstatusZuH(s9ptr->getS_id(),ww3tos9);
  QPair<Weiche*, bool> ww3tos11w10(w10ptr,true);
  QList<QPair<Weiche*, bool>> ww3tos11;
  ww3tos11.push_back(ww3tos11w10);
  ww3ptr->addWeichenstatusZuH(s11ptr->getS_id(),ww3tos11);
  
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

  QPair<Weiche*, bool> s14tos19w11(w11ptr,true);
  QPair<Weiche*, bool> s14tos19w12(w12ptr,false);
  QPair<Weiche*, bool> s14tos19w13(w13ptr,false);
  QPair<Weiche*, bool> s14tos19w14(w14ptr,true);
  QPair<Weiche*, bool> s14tos19w15(w15ptr,true);
  QList<QPair<Weiche*, bool>> s14tos19;
  s14tos19.push_back(s14tos19w11);
  s14tos19.push_back(s14tos19w12);
  s14tos19.push_back(s14tos19w13);
  s14tos19.push_back(s14tos19w14);
  s14tos19.push_back(s14tos19w15);
  s14ptr->addWeichenstatus(s19ptr,s14tos19);

  QPair<Weiche*, bool> s14tos20w11(w11ptr,true);
  QPair<Weiche*, bool> s14tos20w12(w12ptr,false);
  QPair<Weiche*, bool> s14tos20w13(w13ptr,true);
  QList<QPair<Weiche*, bool>> s14tos20;
  s14tos20.push_back(s14tos20w11);
  s14tos20.push_back(s14tos20w12);
  s14tos20.push_back(s14tos20w13);
  s14ptr->addWeichenstatus(s20ptr,s14tos20);

  QPair<Weiche*, bool> s14tos21w11(w11ptr,true);
  QPair<Weiche*, bool> s14tos21w12(w12ptr,true);
  QList<QPair<Weiche*, bool>> s14tos21;
  s14tos21.push_back(s14tos21w11);
  s14tos21.push_back(s14tos21w12);
  s14ptr->addWeichenstatus(s21ptr,s14tos21);

  QPair<Weiche*, bool> s15tos19w11(w11ptr,false);
  QPair<Weiche*, bool> s15tos19w12(w12ptr,false);
  QPair<Weiche*, bool> s15tos19w13(w13ptr,false);
  QPair<Weiche*, bool> s15tos19w14(w14ptr,true);
  QPair<Weiche*, bool> s15tos19w15(w15ptr,true);
  QList<QPair<Weiche*, bool>> s15tos19;
  s15tos19.push_back(s15tos19w11);
  s15tos19.push_back(s15tos19w12);
  s15tos19.push_back(s15tos19w13);
  s15tos19.push_back(s15tos19w14);
  s15tos19.push_back(s15tos19w15);
  s15ptr->addWeichenstatus(s19ptr,s15tos19);

  QPair<Weiche*, bool> s15tos20w11(w11ptr,false);
  QPair<Weiche*, bool> s15tos20w12(w12ptr,false);
  QPair<Weiche*, bool> s15tos20w13(w13ptr,true);
  QList<QPair<Weiche*, bool>> s15tos20;
  s15tos20.push_back(s15tos20w11);
  s15tos20.push_back(s15tos20w12);
  s15tos20.push_back(s15tos20w13);
  s15ptr->addWeichenstatus(s20ptr,s15tos20);

  QPair<Weiche*, bool> s15tos21w11(w11ptr,false);
  QPair<Weiche*, bool> s15tos21w12(w12ptr,true);
  QList<QPair<Weiche*, bool>> s15tos21;
  s15tos21.push_back(s15tos21w11);
  s15tos21.push_back(s15tos21w12);
  s15ptr->addWeichenstatus(s21ptr,s15tos21);

  QPair<Weiche*, bool> s16tos2w12(w12ptr,true);
  QPair<Weiche*, bool> s16tos2w11(w11ptr,false);
  QPair<Weiche*, bool> s16tos2w7(w7ptr,true);
  QPair<Weiche*, bool> s16tos2w6(w6ptr,true);
  QPair<Weiche*, bool> s16tos2w5(w5ptr,false);
  QPair<Weiche*, bool> s16tos2w3(w3ptr,true);
  QList<QPair<Weiche*, bool>> s16tos2;
  s16tos2.push_back(s16tos2w12);
  s16tos2.push_back(s16tos2w11);
  s16tos2.push_back(s16tos2w7);
  s16tos2.push_back(s16tos2w6);
  s16tos2.push_back(s16tos2w5);
  s16tos2.push_back(s16tos2w3);
  s16ptr->addWeichenstatus(s2ptr,s16tos2);

  QPair<Weiche*, bool> s16tos6w12(w12ptr,true);
  QPair<Weiche*, bool> s16tos6w11(w11ptr,false);
  QPair<Weiche*, bool> s16tos6w7(w7ptr,true);
  QPair<Weiche*, bool> s16tos6w6(w6ptr,false);
  QList<QPair<Weiche*, bool>> s16tos6;
  s16tos6.push_back(s16tos6w12);
  s16tos6.push_back(s16tos6w11);
  s16tos6.push_back(s16tos6w7);
  s16tos6.push_back(s16tos6w6);
  s16ptr->addWeichenstatus(s6ptr,s16tos6);

  QPair<Weiche*, bool> s16tos13w12(w12ptr,true);
  QPair<Weiche*, bool> s16tos13w11(w11ptr,true);
  QList<QPair<Weiche*, bool>> s16tos13;
  s16tos13.push_back(s16tos13w12);
  s16tos13.push_back(s16tos13w11);
  s16ptr->addWeichenstatus(s13ptr,s16tos13);
  QPair<Weiche*, bool> s16toww4w12(w12ptr,true);
  QList<QPair<Weiche*, bool>> s16toww4;
  s16toww4.push_back(s16toww4w12);
  s16ptr->addWeichenstatusZuRangier(ww4ptr,s16toww4);


  QPair<Weiche*, bool> s17tos2w13(w13ptr,true);
  QPair<Weiche*, bool> s17tos2w12(w12ptr,false);
  QPair<Weiche*, bool> s17tos2w11(w11ptr,false);
  QPair<Weiche*, bool> s17tos2w7(w7ptr,true);
  QPair<Weiche*, bool> s17tos2w6(w6ptr,true);
  QPair<Weiche*, bool> s17tos2w5(w5ptr,false);
  QPair<Weiche*, bool> s17tos2w3(w3ptr,true);
  QList<QPair<Weiche*, bool>> s17tos2;
  s17tos2.push_back(s17tos2w13);
  s17tos2.push_back(s17tos2w12);
  s17tos2.push_back(s17tos2w11);
  s17tos2.push_back(s17tos2w7);
  s17tos2.push_back(s17tos2w6);
  s17tos2.push_back(s17tos2w5);
  s17tos2.push_back(s17tos2w3);
  s17ptr->addWeichenstatus(s2ptr,s17tos2);

  QPair<Weiche*, bool> s17tos6w13(w13ptr,true);
  QPair<Weiche*, bool> s17tos6w12(w12ptr,false);
  QPair<Weiche*, bool> s17tos6w11(w11ptr,false);
  QPair<Weiche*, bool> s17tos6w7(w7ptr,true);
  QPair<Weiche*, bool> s17tos6w6(w6ptr,false);
  QList<QPair<Weiche*, bool>> s17tos6;
  s17tos6.push_back(s17tos6w13);
  s17tos6.push_back(s17tos6w12);
  s17tos6.push_back(s17tos6w11);
  s17tos6.push_back(s17tos6w7);
  s17tos6.push_back(s17tos6w6);
  s17ptr->addWeichenstatus(s6ptr,s17tos6);

  QPair<Weiche*, bool> s17tos13w13(w13ptr,true);
  QPair<Weiche*, bool> s17tos13w12(w12ptr,false);
  QPair<Weiche*, bool> s17tos13w11(w11ptr,true);
  QList<QPair<Weiche*, bool>> s17tos13;
  s17tos13.push_back(s17tos13w13);
  s17tos13.push_back(s17tos13w12);
  s17tos13.push_back(s17tos13w11);
  s17ptr->addWeichenstatus(s13ptr,s17tos13);
  QPair<Weiche*, bool> s17toww4w13(w13ptr,true);
  QPair<Weiche*, bool> s17toww4w12(w12ptr,false);
  QList<QPair<Weiche*, bool>> s17toww4;
  s17toww4.push_back(s17toww4w13);
  s17toww4.push_back(s17toww4w12);
  s17ptr->addWeichenstatusZuRangier(ww4ptr,s17toww4);

  QPair<Weiche*, bool> s18tos2w15(w15ptr,true);
  QPair<Weiche*, bool> s18tos2w14(w14ptr,true);
  QPair<Weiche*, bool> s18tos2w13(w13ptr,false);
  QPair<Weiche*, bool> s18tos2w12(w12ptr,false);
  QPair<Weiche*, bool> s18tos2w11(w11ptr,false);
  QPair<Weiche*, bool> s18tos2w7(w7ptr,true);
  QPair<Weiche*, bool> s18tos2w6(w6ptr,true);
  QPair<Weiche*, bool> s18tos2w5(w5ptr,false);
  QPair<Weiche*, bool> s18tos2w3(w3ptr,true);
  QList<QPair<Weiche*, bool>> s18tos2;
  s18tos2.push_back(s18tos2w15);
  s18tos2.push_back(s18tos2w14);
  s18tos2.push_back(s18tos2w13);
  s18tos2.push_back(s18tos2w12);
  s18tos2.push_back(s18tos2w11);
  s18tos2.push_back(s18tos2w7);
  s18tos2.push_back(s18tos2w6);
  s18tos2.push_back(s18tos2w5);
  s18tos2.push_back(s18tos2w3);
  s18ptr->addWeichenstatus(s2ptr,s18tos2);

  QPair<Weiche*, bool> s18tos6w15(w15ptr,true);
  QPair<Weiche*, bool> s18tos6w14(w14ptr,true);
  QPair<Weiche*, bool> s18tos6w13(w13ptr,false);
  QPair<Weiche*, bool> s18tos6w12(w12ptr,false);
  QPair<Weiche*, bool> s18tos6w11(w11ptr,false);
  QPair<Weiche*, bool> s18tos6w7(w7ptr,true);
  QPair<Weiche*, bool> s18tos6w6(w6ptr,false);
  QList<QPair<Weiche*, bool>> s18tos6;
  s18tos6.push_back(s18tos6w15);
  s18tos6.push_back(s18tos6w14);
  s18tos6.push_back(s18tos6w13);
  s18tos6.push_back(s18tos6w12);
  s18tos6.push_back(s18tos6w11);
  s18tos6.push_back(s18tos6w7);
  s18tos6.push_back(s18tos6w6);
  s18ptr->addWeichenstatus(s6ptr,s18tos6);

  QPair<Weiche*, bool> s18tos13w15(w15ptr,true);
  QPair<Weiche*, bool> s18tos13w14(w14ptr,true);
  QPair<Weiche*, bool> s18tos13w13(w13ptr,false);
  QPair<Weiche*, bool> s18tos13w12(w12ptr,false);
  QPair<Weiche*, bool> s18tos13w11(w11ptr,true);
  QList<QPair<Weiche*, bool>> s18tos13;
  s18tos13.push_back(s18tos13w15);
  s18tos13.push_back(s18tos13w14);
  s18tos13.push_back(s18tos13w13);
  s18tos13.push_back(s18tos13w12);
  s18tos13.push_back(s18tos13w11);
  s18ptr->addWeichenstatus(s13ptr,s18tos13);

  QPair<Weiche*, bool> s18toww4w15(w15ptr,true);
  QPair<Weiche*, bool> s18toww4w14(w14ptr,true);
  QPair<Weiche*, bool> s18toww4w13(w13ptr,false);
  QPair<Weiche*, bool> s18toww4w12(w12ptr,false);
  QList<QPair<Weiche*, bool>> s18toww4;
  s18toww4.push_back(s18toww4w15);
  s18toww4.push_back(s18toww4w14);
  s18toww4.push_back(s18toww4w13);
  s18toww4.push_back(s18toww4w12);
  s18ptr->addWeichenstatusZuRangier(ww4ptr,s18toww4);
  QPair<Weiche*, bool> s18toww7w15(w15ptr,true);
  QPair<Weiche*, bool> s18toww7w14(w14ptr,false);
  QList<QPair<Weiche*, bool>> s18toww7;
  s18toww7.push_back(s18toww7w15);
  s18toww7.push_back(s18toww7w14);
  s18ptr->addWeichenstatusZuRangier(ww7ptr,s18toww7);

  QPair<Weiche*, bool> s19tos22w16(w16ptr,false);
  QList<QPair<Weiche*, bool>> s19tos22;
  s19tos22.push_back(s19tos22w16);
  s19ptr->addWeichenstatus(s22ptr,s19tos22);

  QPair<Weiche*, bool> s20tos22w18(w18ptr,false);
  QPair<Weiche*, bool> s20tos22w17(w17ptr,true);
  QPair<Weiche*, bool> s20tos22w16(w16ptr,true);
  QList<QPair<Weiche*, bool>> s20tos22;
  s20tos22.push_back(s20tos22w18);
  s20tos22.push_back(s20tos22w17);
  s20tos22.push_back(s20tos22w16);
  s20ptr->addWeichenstatus(s22ptr,s20tos22);
  QPair<Weiche*, bool> s20toww11w18(w18ptr,false);
  QPair<Weiche*, bool> s20toww11w17(w17ptr,false);
  QList<QPair<Weiche*, bool>> s20toww11;
  s20toww11.push_back(s20toww11w18);
  s20toww11.push_back(s20toww11w17);
  s20ptr->addWeichenstatusZuRangier(ww11ptr,s20toww11);

  QPair<Weiche*, bool> s21tos22w19(w19ptr,false);
  QPair<Weiche*, bool> s21tos22w18(w18ptr,true);
  QPair<Weiche*, bool> s21tos22w17(w17ptr,true);
  QPair<Weiche*, bool> s21tos22w16(w16ptr,true);
  QList<QPair<Weiche*, bool>> s21tos22;
  s21tos22.push_back(s21tos22w19);
  s21tos22.push_back(s21tos22w18);
  s21tos22.push_back(s21tos22w17);
  s21tos22.push_back(s21tos22w16);
  s21ptr->addWeichenstatus(s22ptr,s21tos22);

  QPair<Weiche*, bool> s21tos23w19(w19ptr,true);
  QList<QPair<Weiche*, bool>> s21tos23;
  s21tos23.push_back(s21tos23w19);
  s21ptr->addWeichenstatus(s23ptr,s21tos23);
  QPair<Weiche*, bool> s21toww11w19(w19ptr,false);
  QPair<Weiche*, bool> s21toww11w18(w18ptr,true);
  QPair<Weiche*, bool> s21toww11w17(w17ptr,false);
  QList<QPair<Weiche*, bool>> s21toww11;
  s21toww11.push_back(s21toww11w19);
  s21toww11.push_back(s21toww11w18);
  s21toww11.push_back(s21toww11w17);
  s21ptr->addWeichenstatusZuRangier(ww11ptr,s21toww11);

  QPair<Weiche*, bool> s22tos16w19(w19ptr,true);
  QList<QPair<Weiche*, bool>> s22tos16;
  s22tos16.push_back(s22tos16w19);
  s22ptr->addWeichenstatus(s16ptr,s22tos16);

  QPair<Weiche*, bool> s23tos18w16(w16ptr,false);
  QList<QPair<Weiche*, bool>> s23tos18;
  s23tos18.push_back(s23tos18w16);
  s23ptr->addWeichenstatus(s18ptr,s23tos18);

  QPair<Weiche*, bool> s23tos17w16(w16ptr,true);
  QPair<Weiche*, bool> s23tos17w17(w17ptr,true);
  QPair<Weiche*, bool> s23tos17w18(w18ptr,false);
  QList<QPair<Weiche*, bool>> s23tos17;
  s23tos17.push_back(s23tos17w16);
  s23tos17.push_back(s23tos17w17);
  s23tos17.push_back(s23tos17w18);
  s23ptr->addWeichenstatus(s17ptr,s23tos17);

  QPair<Weiche*, bool> s23tos16w16(w16ptr,true);
  QPair<Weiche*, bool> s23tos16w17(w17ptr,true);
  QPair<Weiche*, bool> s23tos16w18(w18ptr,true);
  QPair<Weiche*, bool> s23tos16w19(w19ptr,false);
  QList<QPair<Weiche*, bool>> s23tos16;
  s23tos16.push_back(s23tos16w16);
  s23tos16.push_back(s23tos16w17);
  s23tos16.push_back(s23tos16w18);
  s23tos16.push_back(s23tos16w19);
  s23ptr->addWeichenstatus(s16ptr,s23tos16);

  QPair<Weiche*, bool> ww5tos21w12(w12ptr,true);
  QList<QPair<Weiche*, bool>> ww5tos21;
  ww5tos21.push_back(ww5tos21w12);
  ww5ptr->addWeichenstatusZuH(s21ptr->getS_id(),ww5tos21);
  QPair<Weiche*, bool> ww5tos20w12(w12ptr,false);
  QPair<Weiche*, bool> ww5tos20w13(w13ptr,true);
  QList<QPair<Weiche*, bool>> ww5tos20;
  ww5tos20.push_back(ww5tos20w12);
  ww5tos20.push_back(ww5tos20w13);
  ww5ptr->addWeichenstatusZuH(s20ptr->getS_id(),ww5tos20);
  QPair<Weiche*, bool> ww5tos19w12(w12ptr,false);
  QPair<Weiche*, bool> ww5tos19w13(w13ptr,false);
  QPair<Weiche*, bool> ww5tos19w14(w14ptr,true);
  QPair<Weiche*, bool> ww5tos19w15(w15ptr,true);
  QList<QPair<Weiche*, bool>> ww5tos19;
  ww5tos19.push_back(ww5tos19w12);
  ww5tos19.push_back(ww5tos19w13);
  ww5tos19.push_back(ww5tos19w14);
  ww5tos19.push_back(ww5tos19w15);
  ww5ptr->addWeichenstatusZuH(s19ptr->getS_id(),ww5tos19);
  QPair<Weiche*, bool> ww5toww9w12(w12ptr,false);
  QPair<Weiche*, bool> ww5toww9w13(w13ptr,false);
  QPair<Weiche*, bool> ww5toww9w14(w14ptr,true);
  QPair<Weiche*, bool> ww5toww9w15(w15ptr,false);
  QList<QPair<Weiche*, bool>> ww5toww9;
  ww5toww9.push_back(ww5toww9w12);
  ww5toww9.push_back(ww5toww9w13);
  ww5toww9.push_back(ww5toww9w14);
  ww5toww9.push_back(ww5toww9w15);
  ww5ptr->addWeichenstatus(ww9ptr,ww5toww9);

  QPair<Weiche*, bool> ww6tos19w14(w14ptr,false);
  QPair<Weiche*, bool> ww6tos19w15(w15ptr,true);
  QList<QPair<Weiche*, bool>> ww6tos19;
  ww6tos19.push_back(ww6tos19w14);
  ww6tos19.push_back(ww6tos19w15);
  ww6ptr->addWeichenstatusZuH(s19ptr->getS_id(),ww6tos19);
  QPair<Weiche*, bool> ww6toww9w14(w14ptr,false);
  QPair<Weiche*, bool> ww6toww9w15(w15ptr,false);
  QList<QPair<Weiche*, bool>> ww6toww9;
  ww6toww9.push_back(ww6toww9w14);
  ww6toww9.push_back(ww6toww9w15);
  ww6ptr->addWeichenstatus(ww9ptr,ww6toww9);

  QPair<Weiche*, bool> ww8toww4w12(w12ptr,false);
  QPair<Weiche*, bool> ww8toww4w13(w13ptr,false);
  QPair<Weiche*, bool> ww8toww4w14(w14ptr,true);
  QPair<Weiche*, bool> ww8toww4w15(w15ptr,false);
  QList<QPair<Weiche*, bool>> ww8toww4;
  ww8toww4.push_back(ww8toww4w12);
  ww8toww4.push_back(ww8toww4w13);
  ww8toww4.push_back(ww8toww4w14);
  ww8toww4.push_back(ww8toww4w15);
  ww8ptr->addWeichenstatus(ww4ptr,ww8toww4);
  QPair<Weiche*, bool> ww8toww7w14(w14ptr,false);
  QPair<Weiche*, bool> ww8toww7w15(w15ptr,false);
  QList<QPair<Weiche*, bool>> ww8toww7;
  ww8toww7.push_back(ww8toww7w14);
  ww8toww7.push_back(ww8toww7w15);
  ww8ptr->addWeichenstatus(ww7ptr,ww8toww7);
  
  
  QPair<Weiche*, bool> ww10tos16w17(w17ptr,false);
  QPair<Weiche*, bool> ww10tos16w18(w18ptr,true);
  QPair<Weiche*, bool> ww10tos16w19(w19ptr,false);
  QList<QPair<Weiche*, bool>> ww10tos16;
  ww10tos16.push_back(ww10tos16w17);
  ww10tos16.push_back(ww10tos16w18);
  ww10tos16.push_back(ww10tos16w19);
  ww10ptr->addWeichenstatusZuH(s16ptr->getS_id(),ww10tos16);
  QPair<Weiche*, bool> ww10tos17w17(w17ptr,false);
  QPair<Weiche*, bool> ww10tos17w18(w18ptr,false);
  QList<QPair<Weiche*, bool>> ww10tos17;
  ww10tos17.push_back(ww10tos17w17);
  ww10tos17.push_back(ww10tos17w18);
  ww10ptr->addWeichenstatusZuH(s17ptr->getS_id(),ww10tos17);
  
  //Blockstatus -----------------------------------------------
  QList<Block*> s1tos4b;
  s1tos4b.push_back(aaptr);
  s1ptr->addBlock(s4ptr,s1tos4b);
  
  QList<Block*> ww3tos9b;
  ww3tos9b.push_back(ahptr);
  ww3ptr->addBlockZuH(s9ptr->getS_id(),ww3tos9b);
  QList<Block*> ww3tos11b;
  ww3tos11b.push_back(aiptr);
  ww3ptr->addBlockZuH(s11ptr->getS_id(),ww3tos11b);
  
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

  QList<Block*> s14tos19b;
  s14tos19b.push_back(ajptr);
  s14tos19b.push_back(alptr);
  s14tos19b.push_back(anptr);
  s14ptr->addBlock(s19ptr,s14tos19b);
  QList<Block*> s14tos20b;
  s14tos20b.push_back(ajptr);
  s14tos20b.push_back(alptr);
  s14tos20b.push_back(amptr);
  s14ptr->addBlock(s20ptr,s14tos20b);
  QList<Block*> s14tos21b;
  s14tos21b.push_back(ajptr);
  s14tos21b.push_back(akptr);
  s14ptr->addBlock(s21ptr,s14tos21b);

  QList<Block*> s15tos19b;
  s15tos19b.push_back(ajptr);
  s15tos19b.push_back(alptr);
  s15tos19b.push_back(anptr);
  s15ptr->addBlock(s19ptr,s15tos19b);
  QList<Block*> s15tos20b;
  s15tos20b.push_back(ajptr);
  s15tos20b.push_back(alptr);
  s15tos20b.push_back(amptr);
  s15ptr->addBlock(s20ptr,s15tos20b);
  QList<Block*> s15tos21b;
  s15tos21b.push_back(ajptr);
  s15tos21b.push_back(akptr);
  s15ptr->addBlock(s21ptr,s15tos21b);

  QList<Block*> s18tos2b;
  s18tos2b.push_back(alptr);
  s18tos2b.push_back(ajptr);
  s18tos2b.push_back(aeptr);
  s18tos2b.push_back(aaptr);
  s18ptr->addBlock(s2ptr,s18tos2b);
  QList<Block*> s18tos6b;
  s18tos6b.push_back(alptr);
  s18tos6b.push_back(ajptr);
  s18tos6b.push_back(aeptr);
  s18tos6b.push_back(agptr);
  s18ptr->addBlock(s6ptr,s18tos6b);
  QList<Block*> s18tos13b;
  s18tos13b.push_back(alptr);
  s18tos13b.push_back(ajptr);
  s18tos13b.push_back(acptr);
  s18ptr->addBlock(s13ptr,s18tos13b);
  QList<Block*> s18toww4b;
  s18toww4b.push_back(ajptr);
  s18toww4b.push_back(alptr);
  s18ptr->addBlockZuRangier(ww4ptr,s18toww4b);
  QList<Block*> s18toww7b;
  s18toww7b.push_back(aoptr);
  s18ptr->addBlockZuRangier(ww7ptr,s18toww7b);

  QList<Block*> s17tos2b;
  s17tos2b.push_back(alptr);
  s17tos2b.push_back(ajptr);
  s17tos2b.push_back(aeptr);
  s17tos2b.push_back(aaptr);
  s17ptr->addBlock(s2ptr,s17tos2b);
  QList<Block*> s17tos6b;
  s17tos6b.push_back(alptr);
  s17tos6b.push_back(ajptr);
  s17tos6b.push_back(aeptr);
  s17tos6b.push_back(agptr);
  s17ptr->addBlock(s6ptr,s17tos6b);
  QList<Block*> s17tos13b;
  s17tos13b.push_back(alptr);
  s17tos13b.push_back(ajptr);
  s17tos13b.push_back(acptr);
  s17ptr->addBlock(s13ptr,s17tos13b);
  QList<Block*> s17toww4b;
  s17toww4b.push_back(ajptr);
  s17toww4b.push_back(alptr);
  s17ptr->addBlockZuRangier(ww4ptr,s17toww4b);

  QList<Block*> s16tos2b;
  s16tos2b.push_back(ajptr);
  s16tos2b.push_back(aeptr);
  s16tos2b.push_back(aaptr);
  s16ptr->addBlock(s2ptr,s16tos2b);
  QList<Block*> s16tos6b;
  s16tos6b.push_back(ajptr);
  s16tos6b.push_back(aeptr);
  s16tos6b.push_back(agptr);
  s16ptr->addBlock(s6ptr,s16tos6b);
  QList<Block*> s16tos13b;
  s16tos13b.push_back(ajptr);
  s16tos13b.push_back(acptr);
  s16ptr->addBlock(s13ptr,s16tos13b);
  QList<Block*> s16toww4b;
  s16toww4b.push_back(ajptr);
  s16ptr->addBlockZuRangier(ww4ptr,s16toww4b);


  QList<Block*> s19tos22b;
  s19tos22b.push_back(arptr);
  s19ptr->addBlock(s22ptr,s19tos22b);

  QList<Block*> s20tos22b;
  s20tos22b.push_back(arptr);
  s20ptr->addBlock(s22ptr,s20tos22b);
  QList<Block*> s20toww11b;
  s20toww11b.push_back(aqptr);
  s20ptr->addBlockZuRangier(ww11ptr,s20toww11b);

  QList<Block*> s21tos22b;
  s21tos22b.push_back(arptr);
  s21ptr->addBlock(s22ptr,s21tos22b);
  QList<Block*> s21tos23b;
  s21tos23b.push_back(arptr);
  s21ptr->addBlock(s23ptr,s21tos23b);
  QList<Block*> s21toww11b;
  s21toww11b.push_back(aqptr);
  s21ptr->addBlockZuRangier(ww11ptr,s21toww11b);

  QList<Block*> s22tos16b;
  s22tos16b.push_back(akptr);
  s22ptr->addBlock(s16ptr,s22tos16b);

  QList<Block*> s23tos16b;
  s23tos16b.push_back(akptr);
  s23ptr->addBlock(s16ptr,s23tos16b);
  QList<Block*> s23tos17b;
  s23tos17b.push_back(amptr);
  s23ptr->addBlock(s17ptr,s23tos17b);
  QList<Block*> s23tos18b;
  s23tos18b.push_back(anptr);
  s23ptr->addBlock(s18ptr,s23tos18b);

  QList<Block*> ww5tos21b;
  ww5tos21b.push_back(akptr);
  ww5ptr->addBlockZuH(s21ptr->getS_id(),ww5tos21b);
  QList<Block*> ww5tos20b;
  ww5tos20b.push_back(alptr);
  ww5tos20b.push_back(amptr);
  ww5ptr->addBlockZuH(s20ptr->getS_id(),ww5tos20b);
  QList<Block*> ww5tos19b;
  ww5tos19b.push_back(alptr);
  ww5tos19b.push_back(anptr);
  ww5ptr->addBlockZuH(s19ptr->getS_id(),ww5tos19b);
  QList<Block*> ww5toww9b;
  ww5toww9b.push_back(alptr);
  ww5toww9b.push_back(apptr); 
  ww5ptr->addBlock(ww9ptr,ww5toww9b);

  QList<Block*> ww6tos19b;
  ww6tos19b.push_back(anptr);
  ww6ptr->addBlockZuH(s19ptr->getS_id(),ww6tos19b);
  QList<Block*> ww6toww9b;
  ww6toww9b.push_back(apptr);
  ww6ptr->addBlock(ww9ptr,ww6toww9b);

  QList<Block*> ww8toww4b;
  ww8toww4b.push_back(alptr);
  ww8toww4b.push_back(ajptr);
  ww8ptr->addBlock(ww4ptr,ww8toww4b);
  QList<Block*> ww8toww7b;
  ww8toww7b.push_back(aoptr);
  ww8ptr->addBlock(ww7ptr,ww8toww7b);


  QList<Block*> ww10tos16b;
  ww10tos16b.push_back(akptr);
  ww10ptr->addBlockZuH(s16ptr->getS_id(),ww10tos16b);
  QList<Block*> ww10tos17b;
  ww10tos17b.push_back(amptr);
  ww10ptr->addBlockZuH(s17ptr->getS_id(),ww10tos17b);

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
  stellwerkstecptr->add_Signal(s16ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s17ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s18ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s19ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s20ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s21ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s22ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s23ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(ww1ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww2ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww3ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww4ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww5ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww6ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww7ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww8ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww9ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww10ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww11ptr->getV_id(),false);


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
  QObject::connect(s16ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s17ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s18ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s19ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s20ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s21ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s22ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s23ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww1ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww2ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww3ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww4ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww5ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww6ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww7ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww8ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww9ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww10ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww11ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(aaptr,&Block::zugpassiert,s1ptr,&HSignal::zugpassiert);
  QObject::connect(aaptr,&Block::zugpassiert,s13ptr,&HSignal::zugpassiert);
  QObject::connect(acptr,&Block::zugpassiert,s4ptr,&HSignal::zugpassiert);
  QObject::connect(aeptr,&Block::zugpassiert,s4ptr,&HSignal::zugpassiert);
  QObject::connect(afptr,&Block::zugpassiert,s4ptr,&HSignal::zugpassiert);
  QObject::connect(adptr,&Block::zugpassiert,s5ptr,&HSignal::zugpassiert);
  QObject::connect(adptr,&Block::zugpassiert,s9ptr,&HSignal::zugpassiert);
  QObject::connect(adptr,&Block::zugpassiert,s11ptr,&HSignal::zugpassiert);
  QObject::connect(afptr,&Block::zugpassiert,s8ptr,&HSignal::zugpassiert);
  QObject::connect(ajptr,&Block::zugpassiert,s14ptr,&HSignal::zugpassiert);
  QObject::connect(ajptr,&Block::zugpassiert,s15ptr,&HSignal::zugpassiert);
  QObject::connect(ajptr,&Block::zugpassiert,s16ptr,&HSignal::zugpassiert);
  QObject::connect(alptr,&Block::zugpassiert,s17ptr,&HSignal::zugpassiert);
  QObject::connect(alptr,&Block::zugpassiert,s18ptr,&HSignal::zugpassiert);
  QObject::connect(arptr,&Block::zugpassiert,s19ptr,&HSignal::zugpassiert);
  QObject::connect(arptr,&Block::zugpassiert,s20ptr,&HSignal::zugpassiert);
  QObject::connect(arptr,&Block::zugpassiert,s21ptr,&HSignal::zugpassiert);
  QObject::connect(akptr,&Block::zugpassiert,s22ptr,&HSignal::zugpassiert);
  QObject::connect(anptr,&Block::zugpassiert,s23ptr,&HSignal::zugpassiert);
  QObject::connect(amptr,&Block::zugpassiert,s23ptr,&HSignal::zugpassiert);
  QObject::connect(akptr,&Block::zugpassiert,s23ptr,&HSignal::zugpassiert);
  
  QObject::connect(aiptr,&Block::zugpassiertW,ww2ptr,&WSignal::zugpassiertW);
  QObject::connect(ahptr,&Block::zugpassiertW,ww2ptr,&WSignal::zugpassiertW);
  QObject::connect(aaptr,&Block::zugpassiertW,ww1ptr,&WSignal::zugpassiertW);
  QObject::connect(abptr,&Block::zugpassiertW,ww1ptr,&WSignal::zugpassiertW);
  QObject::connect(akptr,&Block::zugpassiertW,ww5ptr,&WSignal::zugpassiertW);
  QObject::connect(alptr,&Block::zugpassiertW,ww5ptr,&WSignal::zugpassiertW);
  QObject::connect(apptr,&Block::zugpassiertW,ww6ptr,&WSignal::zugpassiertW);
  QObject::connect(anptr,&Block::zugpassiertW,ww6ptr,&WSignal::zugpassiertW);
  QObject::connect(aoptr,&Block::zugpassiertW,ww8ptr,&WSignal::zugpassiertW);
  QObject::connect(alptr,&Block::zugpassiertW,ww8ptr,&WSignal::zugpassiertW);
  QObject::connect(akptr,&Block::zugpassiertW,ww10ptr,&WSignal::zugpassiertW);
  QObject::connect(amptr,&Block::zugpassiertW,ww10ptr,&WSignal::zugpassiertW);
  
  //Hier wird das Grenzsignal übergeben
  aaptr->addpassiert(s13ptr->getS_id(),acptr);
  acptr->addpassiert(s4ptr->getS_id(),aaptr);
  aeptr->addpassiert(s4ptr->getS_id(),aaptr);
  afptr->addpassiert(s4ptr->getS_id(),aaptr);
  adptr->addpassiert(s4ptr->getS_id(),aaptr);
  adptr->addpassiert(s5ptr->getS_id(),abptr);
  adptr->addpassiert(s9ptr->getS_id(),ahptr);
  adptr->addpassiert(s11ptr->getS_id(),aiptr);
  ajptr->addpassiert(s14ptr->getS_id(),acptr);
  ajptr->addpassiert(s15ptr->getS_id(),aeptr);
  ajptr->addpassiert(s16ptr->getS_id(),akptr);
  alptr->addpassiert(s17ptr->getS_id(),amptr);
  alptr->addpassiert(s18ptr->getS_id(),anptr);
  arptr->addpassiert(s19ptr->getS_id(),anptr);
  arptr->addpassiert(s20ptr->getS_id(),amptr);
  arptr->addpassiert(s21ptr->getS_id(),akptr);
  akptr->addpassiert(s22ptr->getS_id(),arptr);
  anptr->addpassiert(s23ptr->getS_id(),arptr);
  amptr->addpassiert(s23ptr->getS_id(),arptr);
  akptr->addpassiert(s23ptr->getS_id(),arptr);
  
  ahptr->addpassiert(ww2ptr->getV_id(),adptr);
  aiptr->addpassiert(ww2ptr->getV_id(),adptr);
  aaptr->addpassiert(ww1ptr->getV_id(),adptr);
  abptr->addpassiert(ww1ptr->getV_id(),adptr);
  alptr->addpassiert(ww5ptr->getV_id(),ajptr);
  akptr->addpassiert(ww5ptr->getV_id(),ajptr);
  apptr->addpassiert(ww6ptr->getV_id(),aoptr);
  anptr->addpassiert(ww6ptr->getV_id(),aoptr);
  aoptr->addpassiert(ww8ptr->getV_id(),apptr);
  alptr->addpassiert(ww8ptr->getV_id(),apptr);
  amptr->addpassiert(ww10ptr->getV_id(),aqptr);
  akptr->addpassiert(ww10ptr->getV_id(),aqptr);
  
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

  QGraphicsRectItem *bmrect = new QGraphicsRectItem();
  bmrect->setRect(0,0,10,150);
  bmrect->setPos(QPointF(650,730));
  bmrect->setBrush(QColor(79,79,79));
  bmrect->setRotation(90);
  bmptr->addBlockitems(bmrect);
  scene->addItem(bmrect);

  QGraphicsRectItem *bnrect = new QGraphicsRectItem();
  bnrect->setRect(0,0,10,150);
  bnrect->setPos(QPointF(650,800));
  bnrect->setBrush(QColor(79,79,79));
  bnrect->setRotation(90);
  bnptr->addBlockitems(bnrect);
  scene->addItem(bnrect);

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

  QGraphicsRectItem *asrect = new QGraphicsRectItem();
  asrect->setRect(0,0,10,200);
  asrect->setPos(QPointF(215,200));
  asrect->setBrush(QColor(79,79,79));
  asrect->setRotation(-90);
  asptr->addBlockitems(asrect);
  scene->addItem(asrect);

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

  QGraphicsRectItem *aurect = new QGraphicsRectItem();
  aurect->setRect(0,0,10,500);
  aurect->setPos(QPointF(-125,495));
  aurect->setBrush(QColor(79,79,79));
  aurect->setRotation(-90);
  auptr->addBlockitems(aurect);
  scene->addItem(aurect);

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
  itemv101->setPos(QPointF(0,-615));
  itemv102->setPos(QPointF(0,-615));
  itemv101->setScale(0.1);
  itemv102->setScale(0.1);
  itemv101->setRotation(-90);
  itemv102->setRotation(-90);
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


  //strecke oben
  QGraphicsRectItem *akrect = new QGraphicsRectItem();
  akrect->setRect(0,0,10,1000);
  akrect->setPos(QPointF(425,0));
  akrect->setBrush(QColor(79,79,79));
  akrect->setRotation(90);
  akptr->addBlockitems(akrect);
  scene->addItem(akrect);
  QGraphicsRectItem *akrect2 = new QGraphicsRectItem();
  akrect2->setRect(0,0,10,35);
  akrect2->setPos(QPointF(415,-35));
  akrect2->setBrush(QColor(79,79,79));
  akptr->addBlockitems(akrect2);
  scene->addItem(akrect2);

  QGraphicsRectItem *w12rectge = new QGraphicsRectItem();
  w12rectge->setRect(0,0,10,75);
  w12rectge->setPos(QPointF(415,-110));
  w12rectge->setBrush(QColor(79,79,79));
  //w12rectge->setRotation(90);
  scene->addItem(w12rectge);
  QGraphicsRectItem *w12rectab = new QGraphicsRectItem();
  w12rectab->setRect(0,0,10,50);
  w12rectab->setPos(QPointF(415,-110));
  w12rectab->setBrush(QColor(79,79,79));
  w12rectab->setRotation(45);
  scene->addItem(w12rectab);
  QLabel *w12label = new QLabel();
  scene->addWidget(w12label);
  w12ptr->addWeichenitem(w12rectab,w12rectge,w12label);
  w12ptr->moveLabel(425,-110);

  QGraphicsRectItem *ajrect = new QGraphicsRectItem();
  ajrect->setRect(0,0,10,500);
  ajrect->setPos(QPointF(415,-610));
  ajrect->setBrush(QColor(79,79,79));
  ajptr->addBlockitems(ajrect);
  scene->addItem(ajrect);
  QGraphicsRectItem *ajrect2 = new QGraphicsRectItem();
  ajrect2->setRect(0,0,10,120);
  ajrect2->setPos(QPointF(415,-610));
  ajrect2->setBrush(QColor(79,79,79));
  ajrect2->setRotation(90);
  ajptr->addBlockitems(ajrect2);
  scene->addItem(ajrect2);
  
  QGraphicsRectItem *w11rectge = new QGraphicsRectItem();
  w11rectge->setRect(0,0,10,75);
  w11rectge->setPos(QPointF(295,-610));
  w11rectge->setBrush(QColor(79,79,79));
  w11rectge->setRotation(90);
  scene->addItem(w11rectge);
  QGraphicsRectItem *w11rectab = new QGraphicsRectItem();
  w11rectab->setRect(0,0,10,50);
  w11rectab->setPos(QPointF(295,-610));
  w11rectab->setBrush(QColor(79,79,79));
  w11rectab->setRotation(45);
  scene->addItem(w11rectab);
  QLabel *w11label = new QLabel();
  scene->addWidget(w11label);
  w11ptr->addWeichenitem(w11rectab,w11rectge,w11label);
  w11ptr->moveLabel(280,-625);

  QGraphicsRectItem *acrect4 = new QGraphicsRectItem();
  acrect4->setRect(0,0,10,500);
  acrect4->setPos(QPointF(220,-610));
  acrect4->setBrush(QColor(79,79,79));
  acrect4->setRotation(90);
  acptr->addBlockitems(acrect4);
  scene->addItem(acrect4);

  QGraphicsRectItem *aerect3 = new QGraphicsRectItem();
  aerect3->setRect(0,0,10,500);
  aerect3->setPos(QPointF(220,-540));
  aerect3->setBrush(QColor(79,79,79));
  aerect3->setRotation(90);
  aeptr->addBlockitems(aerect3);
  scene->addItem(aerect3);
  QGraphicsRectItem *aerect4 = new QGraphicsRectItem();
  aerect4->setRect(0,0,10,55);
  aerect4->setPos(QPointF(260,-575));
  aerect4->setBrush(QColor(79,79,79));
  aerect4->setRotation(45);
  aeptr->addBlockitems(aerect4);
  scene->addItem(aerect4);

  QGraphicsRectItem *alrect = new QGraphicsRectItem();
  alrect->setRect(0,0,10,50);
  alrect->setPos(QPointF(380,-75));
  alrect->setBrush(QColor(79,79,79));
  alrect->setRotation(90);
  alptr->addBlockitems(alrect);
  scene->addItem(alrect);

  QGraphicsRectItem *w13rectge = new QGraphicsRectItem();
  w13rectge->setRect(0,0,10,75);
  w13rectge->setPos(QPointF(330,-75));
  w13rectge->setBrush(QColor(79,79,79));
  w13rectge->setRotation(90);
  scene->addItem(w13rectge);
  QGraphicsRectItem *w13rectab = new QGraphicsRectItem();
  w13rectab->setRect(0,0,10,50);
  w13rectab->setPos(QPointF(330,-75));
  w13rectab->setBrush(QColor(79,79,79));
  w13rectab->setRotation(135);
  scene->addItem(w13rectab);
  QLabel *w13label = new QLabel();
  scene->addWidget(w13label);
  w13ptr->addWeichenitem(w13rectab,w13rectge,w13label);
  w13ptr->moveLabel(340,-90);

  QGraphicsRectItem *w14rectge = new QGraphicsRectItem();
  w14rectge->setRect(0,0,10,75);
  w14rectge->setPos(QPointF(240,-150));
  w14rectge->setBrush(QColor(79,79,79));
  w14rectge->setRotation(-45);
  scene->addItem(w14rectge);
  QGraphicsRectItem *w14rectab = new QGraphicsRectItem();
  w14rectab->setRect(0,0,10,50);
  w14rectab->setPos(QPointF(240,-150));
  w14rectab->setBrush(QColor(79,79,79));
  w14rectab->setRotation(-90);
  scene->addItem(w14rectab);
  QLabel *w14label = new QLabel();
  scene->addWidget(w14label);
  w14ptr->addWeichenitem(w14rectab,w14rectge,w14label);
  w14ptr->moveLabel(250,-150);
  
  QGraphicsRectItem *w15rectge = new QGraphicsRectItem();
  w15rectge->setRect(0,0,10,75);
  w15rectge->setPos(QPointF(240,-160));
  w15rectge->setBrush(QColor(79,79,79));
  w15rectge->setRotation(90);
  scene->addItem(w15rectge);
  QGraphicsRectItem *w15rectab = new QGraphicsRectItem();
  w15rectab->setRect(0,0,10,50);
  w15rectab->setPos(QPointF(240,-160));
  w15rectab->setBrush(QColor(79,79,79));
  w15rectab->setRotation(135);
  scene->addItem(w15rectab);
  QLabel *w15label = new QLabel();
  scene->addWidget(w15label);
  w15ptr->addWeichenitem(w15rectab,w15rectge,w15label);
  w15ptr->moveLabel(240,-175); 
  
  QGraphicsRectItem *aorect = new QGraphicsRectItem();
  aorect->setRect(0,0,10,90);
  aorect->setPos(QPointF(380,-160));
  aorect->setBrush(QColor(79,79,79));
  aorect->setRotation(90);
  aoptr->addBlockitems(aorect);
  scene->addItem(aorect);

  QGraphicsRectItem *aprect = new QGraphicsRectItem();
  aprect->setRect(0,0,10,50);
  aprect->setPos(QPointF(205,-195));
  aprect->setBrush(QColor(79,79,79));
  aprect->setRotation(135);
  apptr->addBlockitems(aprect);
  scene->addItem(aprect);
  QGraphicsRectItem *aprect2 = new QGraphicsRectItem();
  aprect2->setRect(0,0,10,200);
  aprect2->setPos(QPointF(170,-240));
  aprect2->setBrush(QColor(79,79,79));
  aprect2->setRotation(90);
  apptr->addBlockitems(aprect2);
  scene->addItem(aprect2);

  QGraphicsRectItem *w19rectge = new QGraphicsRectItem();
  w19rectge->setRect(0,0,10,75);
  w19rectge->setPos(QPointF(-575,0));
  w19rectge->setBrush(QColor(79,79,79));
  w19rectge->setRotation(90);
  scene->addItem(w19rectge);
  QGraphicsRectItem *w19rectab = new QGraphicsRectItem();
  w19rectab->setRect(0,0,10,50);
  w19rectab->setPos(QPointF(-575,0));
  w19rectab->setBrush(QColor(79,79,79));
  w19rectab->setRotation(135);
  scene->addItem(w19rectab);
  QLabel *w19label = new QLabel();
  scene->addWidget(w19label);
  w19ptr->addWeichenitem(w19rectab,w19rectge,w19label);
  w19ptr->moveLabel(-575,10);

  QGraphicsRectItem *w18rectge = new QGraphicsRectItem();
  w18rectge->setRect(0,0,10,55);
  w18rectge->setPos(QPointF(-650,-60));
  w18rectge->setBrush(QColor(79,79,79));
  w18rectge->setRotation(-45);
  scene->addItem(w18rectge);
  QGraphicsRectItem *w18rectab = new QGraphicsRectItem();
  w18rectab->setRect(0,0,10,50);
  w18rectab->setPos(QPointF(-650,-65));
  w18rectab->setBrush(QColor(79,79,79));
  w18rectab->setRotation(-90);
  scene->addItem(w18rectab);
  QLabel *w18label = new QLabel();
  scene->addWidget(w18label);
  w18ptr->addWeichenitem(w18rectab,w18rectge,w18label);
  w18ptr->moveLabel(-620,-65);

  QGraphicsRectItem *amrect = new QGraphicsRectItem();
  amrect->setRect(0,0,10,855);
  amrect->setPos(QPointF(255,-75));
  amrect->setBrush(QColor(79,79,79));
  amrect->setRotation(90);
  amptr->addBlockitems(amrect);
  scene->addItem(amrect);

  QGraphicsRectItem *anrect = new QGraphicsRectItem();
  anrect->setRect(0,0,10,875);
  anrect->setPos(QPointF(180,-160));
  anrect->setBrush(QColor(79,79,79));
  anrect->setRotation(90);
  anptr->addBlockitems(anrect);
  scene->addItem(anrect);
  
  QGraphicsRectItem *w17rectge = new QGraphicsRectItem();
  w17rectge->setRect(0,0,10,75);
  w17rectge->setPos(QPointF(-650,-70));
  w17rectge->setBrush(QColor(79,79,79));
  w17rectge->setRotation(135);
  scene->addItem(w17rectge);
  QGraphicsRectItem *w17rectab = new QGraphicsRectItem();
  w17rectab->setRect(0,0,10,50);
  w17rectab->setPos(QPointF(-650,-75));
  w17rectab->setBrush(QColor(79,79,79));
  w17rectab->setRotation(90);
  scene->addItem(w17rectab);
  QLabel *w17label = new QLabel();
  scene->addWidget(w17label);
  w17ptr->addWeichenitem(w17rectab,w17rectge,w17label);
  w17ptr->moveLabel(-700,-90);

  QGraphicsRectItem *w16rectge = new QGraphicsRectItem();
  w16rectge->setRect(0,0,10,55);
  w16rectge->setPos(QPointF(-743,-150));
  w16rectge->setBrush(QColor(79,79,79));
  w16rectge->setRotation(-45);
  scene->addItem(w16rectge);
  QGraphicsRectItem *w16rectab = new QGraphicsRectItem();
  w16rectab->setRect(0,0,10,50);
  w16rectab->setPos(QPointF(-743,-150));
  w16rectab->setBrush(QColor(79,79,79));
  w16rectab->setRotation(-90);
  scene->addItem(w16rectab);
  QLabel *w16label = new QLabel();
  scene->addWidget(w16label);
  w16ptr->addWeichenitem(w16rectab,w16rectge,w16label);
  w16ptr->moveLabel(-765,-150);

  QGraphicsRectItem *arrect = new QGraphicsRectItem();
  arrect->setRect(0,0,10,430);
  arrect->setPos(QPointF(-650,0));
  arrect->setBrush(QColor(79,79,79));
  arrect->setRotation(90);
  arptr->addBlockitems(arrect);
  scene->addItem(arrect);
  QGraphicsRectItem *arrect2 = new QGraphicsRectItem();
  arrect2->setRect(0,0,10,300);
  arrect2->setPos(QPointF(-1080,-300));
  arrect2->setBrush(QColor(79,79,79));
  arptr->addBlockitems(arrect2);
  scene->addItem(arrect2);
  QGraphicsRectItem *arrect3 = new QGraphicsRectItem();
  arrect3->setRect(0,0,10,200);
  arrect3->setPos(QPointF(-880,-290));
  arrect3->setBrush(QColor(79,79,79));
  arrect3->setRotation(-45);
  arptr->addBlockitems(arrect3);
  scene->addItem(arrect3);
  QGraphicsRectItem *arrect4 = new QGraphicsRectItem();
  arrect4->setRect(0,0,10,200);
  arrect4->setPos(QPointF(-880,-300));
  arrect4->setBrush(QColor(79,79,79));
  arrect4->setRotation(90);
  arptr->addBlockitems(arrect4);
  scene->addItem(arrect4);

  QGraphicsRectItem *aqrect = new QGraphicsRectItem();
  aqrect->setRect(0,0,10,120);
  aqrect->setPos(QPointF(-700,-75));
  aqrect->setBrush(QColor(79,79,79));
  aqrect->setRotation(90);
  aqptr->addBlockitems(aqrect);
  scene->addItem(aqrect);
  QGraphicsRectItem *aqrect2 = new QGraphicsRectItem();
  aqrect2->setRect(0,0,10,100);
  aqrect2->setPos(QPointF(-890,-145));
  aqrect2->setBrush(QColor(79,79,79));
  aqrect2->setRotation(-45);
  aqptr->addBlockitems(aqrect2);
  scene->addItem(aqrect2);

  QGraphicsSvgItem *s14item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s14item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s14item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s14item1);
  scene->addItem(s14item3);
  scene->addItem(s14item2);
  s14item1->setPos(QPointF(230,-590));
  s14item2->setPos(QPointF(230,-590));
  s14item3->setPos(QPointF(230,-590));
  s14item1->setScale(0.1);
  s14item2->setScale(0.1);
  s14item3->setScale(0.1);
  s14item1->setRotation(90);
  s14item2->setRotation(90);
  s14item3->setRotation(90);
  QPushButton *s14push = new QPushButton();
  scene->addWidget(s14push);
  QLabel *s14label = new QLabel();
  scene->addWidget(s14label);
  //speicher GUIs
  QGraphicsRectItem *sphin14 = new QGraphicsRectItem();
  sphin14->setRect(0,0,10,10);
  sphin14->setBrush(Qt::yellow);
  sphin14->setPos(QPointF(155,-580));
  QGraphicsRectItem *spweg14 = new QGraphicsRectItem();
  spweg14->setRect(0,0,10,10);
  spweg14->setBrush(Qt::yellow);
  spweg14->setPos(QPointF(155,-590));
  scene->addItem(spweg14);
  scene->addItem(sphin14);
  s14ptr->addHSignalitem(s14item1,s14item2, s14item3, s14label,s14push,spweg14,sphin14);
  s14ptr->moveButton(200,-615);
  s14ptr->moveLabel(120,-590);

  QGraphicsSvgItem *s15item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s15item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s15item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s15item1);
  scene->addItem(s15item3);
  scene->addItem(s15item2);
  s15item1->setPos(QPointF(230,-520));
  s15item2->setPos(QPointF(230,-520));
  s15item3->setPos(QPointF(230,-520));
  s15item1->setScale(0.1);
  s15item2->setScale(0.1);
  s15item3->setScale(0.1);
  s15item1->setRotation(90);
  s15item2->setRotation(90);
  s15item3->setRotation(90);
  QPushButton *s15push = new QPushButton();
  scene->addWidget(s15push);
  QLabel *s15label = new QLabel();
  scene->addWidget(s15label);
  //speicher GUIs
  QGraphicsRectItem *sphin15 = new QGraphicsRectItem();
  sphin15->setRect(0,0,10,10);
  sphin15->setBrush(Qt::yellow);
  sphin15->setPos(QPointF(155,-510));
  QGraphicsRectItem *spweg15 = new QGraphicsRectItem();
  spweg15->setRect(0,0,10,10);
  spweg15->setBrush(Qt::yellow);
  spweg15->setPos(QPointF(155,-520));
  scene->addItem(spweg15);
  scene->addItem(sphin15);
  s15ptr->addHSignalitem(s15item1,s15item2, s15item3, s15label,s15push,spweg15,sphin15);
  s15ptr->moveButton(200,-545);
  s15ptr->moveLabel(120,-520);

  QGraphicsSvgItem *itemv111 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *itemv112 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(itemv111);
  scene->addItem(itemv112);
  itemv111->setPos(QPointF(405,-350));
  itemv112->setPos(QPointF(405,-350));
  itemv111->setScale(0.1);
  itemv112->setScale(0.1);
  itemv111->setRotation(180);
  itemv112->setRotation(180);
  v11ptr->addVSignalitems(itemv111, itemv112);

  QGraphicsSvgItem *s18item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s18item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s18item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s18item1);
  scene->addItem(s18item3);
  scene->addItem(s18item2);
  s18item1->setPos(QPointF(220,-140));
  s18item2->setPos(QPointF(220,-140));
  s18item3->setPos(QPointF(220,-140));
  s18item1->setScale(0.1);
  s18item2->setScale(0.1);
  s18item3->setScale(0.1);
  s18item1->setRotation(90);
  s18item2->setRotation(90);
  s18item3->setRotation(90);
  QPushButton *s18push = new QPushButton();
  scene->addWidget(s18push);
  QLabel *s18label = new QLabel();
  scene->addWidget(s18label);
  //speicher GUIs
  QGraphicsRectItem *sphin18 = new QGraphicsRectItem();
  sphin18->setRect(0,0,10,10);
  sphin18->setBrush(Qt::yellow);
  sphin18->setPos(QPointF(145,-130));
  QGraphicsRectItem *spweg18 = new QGraphicsRectItem();
  spweg18->setRect(0,0,10,10);
  spweg18->setBrush(Qt::yellow);
  spweg18->setPos(QPointF(145,-140));
  scene->addItem(spweg18);
  scene->addItem(sphin18);
  s18ptr->addHSignalitem(s18item1,s18item2, s18item3, s18label,s18push,spweg18,sphin18);
  s18ptr->moveButton(170,-165);
  s18ptr->moveLabel(110,-140);

  QGraphicsSvgItem *s17item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s17item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s17item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s17item1);
  scene->addItem(s17item3);
  scene->addItem(s17item2);
  s17item1->setPos(QPointF(270,-55));
  s17item2->setPos(QPointF(270,-55));
  s17item3->setPos(QPointF(270,-55));
  s17item1->setScale(0.1);
  s17item2->setScale(0.1);
  s17item3->setScale(0.1);
  s17item1->setRotation(90);
  s17item2->setRotation(90);
  s17item3->setRotation(90);
  QPushButton *s17push = new QPushButton();
  scene->addWidget(s17push);
  QLabel *s17label = new QLabel();
  scene->addWidget(s17label);
  //speicher GUIs
  QGraphicsRectItem *sphin17 = new QGraphicsRectItem();
  sphin17->setRect(0,0,10,10);
  sphin17->setBrush(Qt::yellow);
  sphin17->setPos(QPointF(185,-45));
  QGraphicsRectItem *spweg17 = new QGraphicsRectItem();
  spweg17->setRect(0,0,10,10);
  spweg17->setBrush(Qt::yellow);
  spweg17->setPos(QPointF(185,-55));
  scene->addItem(spweg17);
  scene->addItem(sphin17);
  s17ptr->addHSignalitem(s17item1,s17item2, s17item3, s17label,s17push,spweg17,sphin17);
  s17ptr->moveButton(235,-80);
  s17ptr->moveLabel(155,-55);

  QGraphicsSvgItem *s16item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s16item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s16item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s16item1);
  scene->addItem(s16item3);
  scene->addItem(s16item2);
  s16item1->setPos(QPointF(380,25));
  s16item2->setPos(QPointF(380,25));
  s16item3->setPos(QPointF(380,25));
  s16item1->setScale(0.1);
  s16item2->setScale(0.1);
  s16item3->setScale(0.1);
  s16item1->setRotation(90);
  s16item2->setRotation(90);
  s16item3->setRotation(90);
  QPushButton *s16push = new QPushButton();
  scene->addWidget(s16push);
  QLabel *s16label = new QLabel();
  scene->addWidget(s16label);
  //speicher GUIs
  QGraphicsRectItem *sphin16 = new QGraphicsRectItem();
  sphin16->setRect(0,0,10,10);
  sphin16->setBrush(Qt::yellow);
  sphin16->setPos(QPointF(300,25));
  QGraphicsRectItem *spweg16 = new QGraphicsRectItem();
  spweg16->setRect(0,0,10,10);
  spweg16->setBrush(Qt::yellow);
  spweg16->setPos(QPointF(300,35));
  scene->addItem(spweg16);
  scene->addItem(sphin16);
  s16ptr->addHSignalitem(s16item1,s16item2, s16item3, s16label,s16push,spweg16,sphin16);
  s16ptr->moveButton(350,-5);
  s16ptr->moveLabel(270,30);

  QGraphicsSvgItem *item141 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *item142 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(item141);
  scene->addItem(item142);
  item141->setPos(QPointF(-660,20));
  item142->setPos(QPointF(-660,20));
  item141->setScale(0.1);
  item142->setScale(0.1);
  item141->setRotation(90);
  item142->setRotation(90);
  v14ptr->addVSignalitems(item141, item142);
  
  QGraphicsSvgItem *s22item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s22item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s22item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s22item1);
  scene->addItem(s22item3);
  scene->addItem(s22item2);
  s22item1->setPos(QPointF(-620,25));
  s22item2->setPos(QPointF(-620,25));
  s22item3->setPos(QPointF(-620,25));
  s22item1->setScale(0.1);
  s22item2->setScale(0.1);
  s22item3->setScale(0.1);
  s22item1->setRotation(90);
  s22item2->setRotation(90);
  s22item3->setRotation(90);
  QPushButton *s22push = new QPushButton();
  scene->addWidget(s22push);
  QLabel *s22label = new QLabel();
  scene->addWidget(s22label);
  //speicher GUIs
  QGraphicsRectItem *sphin22 = new QGraphicsRectItem();
  sphin22->setRect(0,0,10,10);
  sphin22->setBrush(Qt::yellow);
  sphin22->setPos(QPointF(-700,25));
  QGraphicsRectItem *spweg22 = new QGraphicsRectItem();
  spweg22->setRect(0,0,10,10);
  spweg22->setBrush(Qt::yellow);
  spweg22->setPos(QPointF(-700,35));
  scene->addItem(spweg22);
  scene->addItem(sphin22);
  s22ptr->addHSignalitem(s22item1,s22item2, s22item3, s22label,s22push,spweg22,sphin22);
  s22ptr->moveButton(-650,-5);
  s22ptr->moveLabel(-730,30);

  QGraphicsSvgItem *itemv151 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *itemv152 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(itemv151);
  scene->addItem(itemv152);
  itemv151->setPos(QPointF(-545,-5));
  itemv152->setPos(QPointF(-545,-5));
  itemv151->setScale(0.1);
  itemv152->setScale(0.1);
  itemv151->setRotation(-90);
  itemv152->setRotation(-90);
  v15ptr->addVSignalitems(itemv151, itemv152);
  
  QGraphicsSvgItem *s21item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s21item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s21item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s21item1);
  scene->addItem(s21item3);
  scene->addItem(s21item2);
  s21item1->setPos(QPointF(-585,-10));
  s21item2->setPos(QPointF(-585,-10));
  s21item3->setPos(QPointF(-585,-10));
  s21item1->setScale(0.1);
  s21item2->setScale(0.1);
  s21item3->setScale(0.1);
  s21item1->setRotation(-90);
  s21item2->setRotation(-90);
  s21item3->setRotation(-90);
  QPushButton *s21push = new QPushButton();
  scene->addWidget(s21push);
  QLabel *s21label = new QLabel();
  scene->addWidget(s21label);
  //speicher GUIs
  QGraphicsRectItem *sphin21 = new QGraphicsRectItem();
  sphin21->setRect(0,0,10,10);
  sphin21->setBrush(Qt::yellow);
  sphin21->setPos(QPointF(-505,-30));
  QGraphicsRectItem *spweg21 = new QGraphicsRectItem();
  spweg21->setRect(0,0,10,10);
  spweg21->setBrush(Qt::yellow);
  spweg21->setPos(QPointF(-505,-20));
  scene->addItem(spweg21);
  scene->addItem(sphin21);
  s21ptr->addHSignalitem(s21item1,s21item2, s21item3, s21label,s21push,spweg21,sphin21);
  s21ptr->moveButton(-560,-5);
  s21ptr->moveLabel(-490,-25);

  QGraphicsSvgItem *itemv131 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *itemv132 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(itemv131);
  scene->addItem(itemv132);
  itemv131->setPos(QPointF(-585,-80));
  itemv132->setPos(QPointF(-585,-80));
  itemv131->setScale(0.1);
  itemv132->setScale(0.1);
  itemv131->setRotation(-90);
  itemv132->setRotation(-90);
  v13ptr->addVSignalitems(itemv131, itemv132);
  
  QGraphicsSvgItem *s20item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s20item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s20item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s20item1);
  scene->addItem(s20item3);
  scene->addItem(s20item2);
  s20item1->setPos(QPointF(-625,-85));
  s20item2->setPos(QPointF(-625,-85));
  s20item3->setPos(QPointF(-625,-85));
  s20item1->setScale(0.1);
  s20item2->setScale(0.1);
  s20item3->setScale(0.1);
  s20item1->setRotation(-90);
  s20item2->setRotation(-90);
  s20item3->setRotation(-90);
  QPushButton *s20push = new QPushButton();
  scene->addWidget(s20push);
  QLabel *s20label = new QLabel();
  scene->addWidget(s20label);
  //speicher GUIs
  QGraphicsRectItem *sphin20 = new QGraphicsRectItem();
  sphin20->setRect(0,0,10,10);
  sphin20->setBrush(Qt::yellow);
  sphin20->setPos(QPointF(-545,-105));
  QGraphicsRectItem *spweg20 = new QGraphicsRectItem();
  spweg20->setRect(0,0,10,10);
  spweg20->setBrush(Qt::yellow);
  spweg20->setPos(QPointF(-545,-95));
  scene->addItem(spweg20);
  scene->addItem(sphin20);
  s20ptr->addHSignalitem(s20item1,s20item2, s20item3, s20label,s20push,spweg20,sphin20);
  s20ptr->moveButton(-600,-80);
  s20ptr->moveLabel(-530,-100);

  
  QGraphicsSvgItem *itemv121 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *itemv122 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(itemv121);
  scene->addItem(itemv122);
  itemv121->setPos(QPointF(-665,-165));
  itemv122->setPos(QPointF(-665,-165));
  itemv121->setScale(0.1);
  itemv122->setScale(0.1);
  itemv121->setRotation(-90);
  itemv122->setRotation(-90);
  v12ptr->addVSignalitems(itemv121, itemv122);
  
  QGraphicsSvgItem *s19item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s19item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s19item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s19item1);
  scene->addItem(s19item3);
  scene->addItem(s19item2);
  s19item1->setPos(QPointF(-705,-170));
  s19item2->setPos(QPointF(-705,-170));
  s19item3->setPos(QPointF(-705,-170));
  s19item1->setScale(0.1);
  s19item2->setScale(0.1);
  s19item3->setScale(0.1);
  s19item1->setRotation(-90);
  s19item2->setRotation(-90);
  s19item3->setRotation(-90);
  QPushButton *s19push = new QPushButton();
  scene->addWidget(s19push);
  QLabel *s19label = new QLabel();
  scene->addWidget(s19label);
  //speicher GUIs
  QGraphicsRectItem *sphin19 = new QGraphicsRectItem();
  sphin19->setRect(0,0,10,10);
  sphin19->setBrush(Qt::yellow);
  sphin19->setPos(QPointF(-625,-190));
  QGraphicsRectItem *spweg19 = new QGraphicsRectItem();
  spweg19->setRect(0,0,10,10);
  spweg19->setBrush(Qt::yellow);
  spweg19->setPos(QPointF(-625,-180));
  scene->addItem(spweg19);
  scene->addItem(sphin19);
  s19ptr->addHSignalitem(s19item1,s19item2, s19item3, s19label,s19push,spweg19,sphin19);
  s19ptr->moveButton(-680,-165);
  s19ptr->moveLabel(-610,-185);

  QGraphicsSvgItem *itemv161 = new QGraphicsSvgItem(":/resources/VSfahrt.svg");
  QGraphicsSvgItem *itemv162 = new QGraphicsSvgItem(":/resources/VShalt.svg");
  scene->addItem(itemv161);
  scene->addItem(itemv162);
  itemv161->setPos(QPointF(-800,-200));
  itemv162->setPos(QPointF(-800,-200));
  itemv161->setScale(0.1);
  itemv162->setScale(0.1);
  itemv161->setRotation(135);
  itemv162->setRotation(135);
  v16ptr->addVSignalitems(itemv161, itemv162);
  
  QGraphicsSvgItem *s23item1 = new QGraphicsSvgItem(":/resources/HSfahrt.svg");
  QGraphicsSvgItem *s23item2 = new QGraphicsSvgItem(":/resources/HShalt.svg");
  QGraphicsSvgItem *s23item3 = new QGraphicsSvgItem(":/resources/HSrangier.svg");
  scene->addItem(s23item1);
  scene->addItem(s23item3);
  scene->addItem(s23item2);
  s23item1->setPos(QPointF(-775,-170));
  s23item2->setPos(QPointF(-775,-170));
  s23item3->setPos(QPointF(-775,-170));
  s23item1->setScale(0.1);
  s23item2->setScale(0.1);
  s23item3->setScale(0.1);
  s23item1->setRotation(135);
  s23item2->setRotation(135);
  s23item3->setRotation(135);
  QPushButton *s23push = new QPushButton();
  scene->addWidget(s23push);
  QLabel *s23label = new QLabel();
  scene->addWidget(s23label);
  //speicher GUIs
  QGraphicsRectItem *sphin23 = new QGraphicsRectItem();
  sphin23->setRect(0,0,10,10);
  sphin23->setBrush(Qt::yellow);
  sphin23->setPos(QPointF(-835,-210));
  QGraphicsRectItem *spweg23 = new QGraphicsRectItem();
  spweg23->setRect(0,0,10,10);
  spweg23->setBrush(Qt::yellow);
  spweg23->setPos(QPointF(-825,-220));
  spweg23->setRotation(135);
  sphin23->setRotation(135);
  scene->addItem(spweg23);
  scene->addItem(sphin23);
  s23ptr->addHSignalitem(s23item1,s23item2, s23item3, s23label,s23push,spweg23,sphin23);
  s23ptr->moveButton(-770,-185);
  s23ptr->moveLabel(-865,-240);

  //WS
  QPushButton *ww4push = new QPushButton();
  scene->addWidget(ww4push);
  ww4ptr->addButtonAndLabel(ww4label,ww4push);
  ww4ptr->moveButton(320,-615);
  ww4ptr->moveLabel(350,-630);

  QPushButton *ww9push = new QPushButton();
  scene->addWidget(ww9push);
  ww9ptr->addButtonAndLabel(ww9label,ww9push);
  ww9ptr->moveButton(-30,-245);
  ww9ptr->moveLabel(0,-280);

  QPushButton *ww6push = new QPushButton();
  scene->addWidget(ww6push);
  ww6ptr->addButtonAndLabel(ww6label,ww6push);
  ww6ptr->moveButton(280,-165);
  ww6ptr->moveLabel(310,-200);

  QPushButton *ww11push = new QPushButton();
  scene->addWidget(ww11push);
  ww11ptr->addButtonAndLabel(ww11label,ww11push);
  ww11ptr->moveButton(-900,-165);
  ww11ptr->moveLabel(-850,-155);

  QPushButton *ww10push = new QPushButton();
  scene->addWidget(ww10push);
  ww10ptr->addButtonAndLabel(ww10label,ww10push);
  ww10ptr->moveButton(-710,-80);
  ww10ptr->moveLabel(-740,-60);

  QPushButton *ww7push = new QPushButton();
  scene->addWidget(ww7push);
  ww7ptr->addButtonAndLabel(ww7label,ww7push);
  ww7ptr->moveButton(380,-165);
  ww7ptr->moveLabel(335,-140);

  QPushButton *ww5push = new QPushButton();
  scene->addWidget(ww5push);
  ww5ptr->addButtonAndLabel(ww5label,ww5push);
  ww5ptr->moveButton(410,-135);
  ww5ptr->moveLabel(375,-210);

  QPushButton *ww8push = new QPushButton();
  scene->addWidget(ww8push);
  ww8ptr->addButtonAndLabel(ww8label,ww8push);
  ww8ptr->moveButton(190,-205);
  ww8ptr->moveLabel(135,-210);

  QPushButton *ww3push = new QPushButton();
  scene->addWidget(ww3push);
  ww3ptr->addButtonAndLabel(ww3label,ww3push);
  ww3ptr->moveButton(240,185);
  ww3ptr->moveLabel(270,170);

  //neue Bahn ---------------------------------------------------------------------------------------------------
  QGraphicsRectItem *asrect2 = new QGraphicsRectItem();
  asrect2->setRect(0,0,10,300);
  asrect2->setPos(QPointF(600,90));
  asrect2->setBrush(QColor(79,79,79));
  asptr->addBlockitems(asrect2);
  scene->addItem(asrect2);
  
  QGraphicsRectItem *atrect = new QGraphicsRectItem();
  atrect->setRect(0,0,10,700);
  atrect->setPos(QPointF(600,-610));
  atrect->setBrush(QColor(79,79,79));
  atptr->addBlockitems(atrect);
  scene->addItem(atrect);

  QGraphicsRectItem *aurect2 = new QGraphicsRectItem();
  aurect2->setRect(0,0,10,150);
  aurect2->setPos(QPointF(670,240));
  aurect2->setBrush(QColor(79,79,79));
  auptr->addBlockitems(aurect2);
  scene->addItem(aurect2);
  
  QGraphicsRectItem *avrect = new QGraphicsRectItem();
  avrect->setRect(0,0,10,780);
  avrect->setPos(QPointF(670,-540));
  avrect->setBrush(QColor(79,79,79));
  avptr->addBlockitems(avrect);
  scene->addItem(avrect);

  QGraphicsRectItem *atrect2 = new QGraphicsRectItem();
  atrect2->setRect(0,0,10,250);
  atrect2->setPos(QPointF(850,-610));
  atrect2->setBrush(QColor(79,79,79));
  atrect2->setRotation(90);
  atptr->addBlockitems(atrect2);
  scene->addItem(atrect2);

  QGraphicsRectItem *avrect2 = new QGraphicsRectItem();
  avrect2->setRect(0,0,10,180);
  avrect2->setPos(QPointF(850,-540));
  avrect2->setBrush(QColor(79,79,79));
  avrect2->setRotation(90);
  avptr->addBlockitems(avrect2);
  scene->addItem(avrect2);

  QGraphicsRectItem *awrect = new QGraphicsRectItem();
  awrect->setRect(0,0,10,1660);
  awrect->setPos(QPointF(2580,-610));
  awrect->setBrush(QColor(79,79,79));
  awrect->setRotation(90);
  awptr->addBlockitems(awrect);
  scene->addItem(awrect);

  QGraphicsRectItem *ayrect = new QGraphicsRectItem();
  ayrect->setRect(0,0,10,150);
  ayrect->setPos(QPointF(2730,-610));
  ayrect->setBrush(QColor(79,79,79));
  ayrect->setRotation(90);
  ayptr->addBlockitems(ayrect);
  scene->addItem(ayrect);
  
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
  QObject::connect(s16ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s17ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s18ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s19ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s20ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s21ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s22ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s23ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  //ebenfalls für WSignal
  QObject::connect(ww1ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww2ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww3ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww4ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww5ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww6ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww7ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww8ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww9ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww10ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww11ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
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
  QObject::connect(s6ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s7ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s8ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s9ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s10ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s11ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s12ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s13ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s14ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s15ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s16ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s17ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s18ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s19ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s20ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s21ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s22ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s23ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  
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
  delete aaptr; delete abptr; delete acptr; delete adptr; delete aeptr; delete afptr; delete agptr; delete ahptr; delete aiptr; delete ajptr; delete akptr; delete alptr; delete amptr; delete anptr; delete aoptr; delete apptr; delete aqptr; delete arptr; delete asptr; delete atptr; delete avptr; delete awptr; delete axptr; delete ayptr; delete bmptr; delete bnptr;
  delete ww1ptr; delete ww2ptr; delete ww3ptr; delete ww4ptr; delete ww5ptr; delete ww6ptr; delete ww7ptr; delete ww8ptr; delete ww9ptr; delete ww10ptr; delete ww11ptr;
  delete v1ptr; delete v2ptr; delete v3ptr; delete v4ptr; delete v5ptr; delete v6ptr; delete v7ptr; delete v8ptr; delete v9ptr; delete v10ptr; delete v11ptr; delete v12ptr; delete v13ptr; delete v14ptr; delete v15ptr; delete v16ptr;
  delete w1ptr; delete w2ptr; delete w3ptr; delete w4ptr; delete w5ptr; delete w6ptr; delete w7ptr; delete w8ptr; delete w9ptr; delete w10ptr; delete w11ptr; delete w12ptr; delete w13ptr; delete w14ptr; delete w15ptr; delete w16ptr; delete w17ptr; delete w18ptr; delete w19ptr;
  delete s1ptr; delete s2ptr; delete s3ptr; delete s4ptr; delete s5ptr; delete s6ptr; delete s7ptr; delete s8ptr; delete s9ptr; delete s10ptr; delete s11ptr; delete s12ptr; delete s13ptr; delete s14ptr; delete s15ptr; delete s16ptr; delete s17ptr; delete s18ptr; delete s19ptr; delete s20ptr; delete s21ptr; delete s22ptr; delete s23ptr;
  
  return eofprogram;

}
