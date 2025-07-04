/*
 * main routine for control center [- MAIN.CXX -]
 *******************************************************************************
 * Different steps are needed for the control center                           *
 *    1) initialize all control center objects                                 *
 *    2) set pins for hardware connection                                      *
 *    3) create thread for route memory and updates of the segment occupation  *
 *    4) properly set all connection for the interaction of control center     *
 *                                   [ turnouts from 001-999: all = 999 ]      *  
 *                                   [ railroad crossings from 1-99: all = 99 ]*
 *                                   [ signals from SWV001-SWV999: all = 999 ] *
 *                                   [ segments from aa-zz: all = 676 ]        *
 *******************************************************************************
 *
 * control center
 * __________________
 * control center after the german "ESTW" (electronic signalling control center) for model train:
 * hardware with raspberry pi, wiringPi
 * Gui and main routine in C++/Qt
 */
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
#include "Connector.h"
#include "Spmemory.h"
#include "filemanager.h"
#include "WeicheQPushButton.h"
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
#include <QStandardPaths>
#include "clickmanager.h"
#include "Configuration.h"
#ifdef __cplusplus
extern "C"{
#endif
#include <WiringPi/wiringPi/wiringPi.h>
#include <WiringPi/wiringPi/sr595.h>
#include <WiringPi/wiringPi/mcp23017.h>
#ifdef __cplusplus
}
#endif

int main( int argc , char *argv[] ){
  //
  //command line arguments
  //
  Configuration *config = new Configuration();
  for(int i=0; i < argc; i++) {
    if( strcmp(argv[i],"-w") == 0 ){
      config->setLogLevel("warning");
    }
    if(strcmp(argv[i],"--help") == 0 ){
      qInfo()<<" #################################################";
      qInfo()<<" ###              StellwerkQt Help             ###";
      qInfo()<<" ### ----------------------------------------- ###";
      qInfo()<<" ### usage: StellwerkQt -[option]              ###";
      qInfo()<<" ### -platform xcb: use when problems with gl  ###";
      qInfo()<<" ### --help:        show this help             ###";
      qInfo()<<" ### -w   :         verbose debug mode         ###";
      qInfo()<<" ### -d   :         enable debug log level     ###";
      qInfo()<<" ### -n   :         mode without hardware      ###";
      qInfo()<<" #################################################";
      return 0;
    }
    if(strcmp(argv[i],"-d") == 0 ){
      config->setLogLevel("debug");
    }
    if(strcmp(argv[i],"akito") == 0 ){
      qInfo()<<"";
      qInfo()<<"    /)/)";
      qInfo()<<"    (^.^)  ";
      qInfo()<<"   C(\")(\")  >o¤»";
      qInfo()<<"";
      qInfo()<<"";
      qInfo()<<"     ...*nom*...";
      return 0;
    }
    if(strcmp(argv[i],"-n") == 0 ){
      config->setWithHardware(false);
    }
  }
 
  // starting QT application
  //**************************************************************************

  wiringPiSetupGpio(); // is needed by mainwindow already, therefore set it up now
  qDebug()<<"__main__: Setting up wiringPi...";
 
  //pinMode(13,OUTPUT);  // power turnouts on/off with GPIO13                                                                          !!!!!!!!!!!!!!!!
  //digitalWrite(13,LOW);
  
  QApplication a(argc, argv);
  MainWindow w;
  //
  //create scene
  QGraphicsScene *scene = new QGraphicsScene(&w);
  scene->setBackgroundBrush(Qt::darkGray);
  //
  //connect for quiting program
  QObject::connect(&w,&MainWindow::shutdown,&a,&QApplication::quit);
  //
  //create view
  QGraphicsView *view = new QGraphicsView(scene);
  view->setWindowTitle("Electronic Signalling Control Center - Model Railway");
  w.setCentralWidget(view);                                                             //add view to mainwindow
  w.setWindowTitle("Electronic Signalling Control Center - Model Railway");

  // create a filemanager and read turnout state from previous session
  //**************************************************************************
  QString filepath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  filemanager fmngr(filepath + "/.config/StellwerkQt/turnouts.txt");
  fmngr.read();
  
  // initialaizing the specific model railway plan
  //**************************************************************************
  //
  //Stellwerkstechnik+Block aa as base
  Stellwerkstechnik *stellwerkstecptr = new Stellwerkstechnik();
  Block *aaptr = new Block("aa", stellwerkstecptr, config);
  //:::turnouts:::
  Weiche *w1ptr = new Weiche(1,config,fmngr.passstate(1)); 
  Weiche *w2ptr = new Weiche(2,config,fmngr.passstate(2));
  Weiche *w3ptr = new Weiche(3,config,fmngr.passstate(3));
  Weiche *w4ptr = new Weiche(4,config,fmngr.passstate(4));
  Weiche *w5ptr = new Weiche(5,config,fmngr.passstate(5));
  Weiche *w6ptr = new Weiche(6,config,fmngr.passstate(6));
  Weiche *w7ptr = new Weiche(7,config,fmngr.passstate(7));
  Weiche *w8ptr = new Weiche(8,config,fmngr.passstate(8));
  Weiche *w9ptr = new Weiche(9,config,fmngr.passstate(9));
  Weiche *w10ptr = new Weiche(10,config,fmngr.passstate(10));
  Weiche *w11ptr = new Weiche(11,config,fmngr.passstate(11));
  Weiche *w12ptr = new Weiche(12,config,fmngr.passstate(12));
  Weiche *w13ptr = new Weiche(13,config,fmngr.passstate(13));
  Weiche *w14ptr = new Weiche(14,config,fmngr.passstate(14),true);
  Weiche *w15ptr = new Weiche(15,config,fmngr.passstate(15),true);
  Weiche *w16ptr = new Weiche(16,config,fmngr.passstate(16));
  Weiche *w17ptr = new Weiche(17,config,fmngr.passstate(17),true);
  Weiche *w18ptr = new Weiche(18,config,fmngr.passstate(18),true);
  Weiche *w19ptr = new Weiche(19,config,fmngr.passstate(19));
  Weiche *w20ptr = new Weiche(20,config,fmngr.passstate(20));
  Weiche *w21ptr = new Weiche(21,config,fmngr.passstate(21));
  Weiche *w22ptr = new Weiche(22,config,fmngr.passstate(22));
  Weiche *w23ptr = new Weiche(23,config,fmngr.passstate(23));
  Weiche *w24ptr = new Weiche(24,config,fmngr.passstate(24));
  Weiche *w25ptr = new Weiche(25,config,fmngr.passstate(25));
  Weiche *w26ptr = new Weiche(26,config,fmngr.passstate(26));
  Weiche *w27ptr = new Weiche(27,config,fmngr.passstate(27));
  Weiche *w28ptr = new Weiche(28,config,fmngr.passstate(28));
  Weiche *w29ptr = new Weiche(29,config,fmngr.passstate(29));
  Weiche *w30ptr = new Weiche(30,config,fmngr.passstate(30));
  Weiche *w31ptr = new Weiche(31,config,fmngr.passstate(31));
  Weiche *w40ptr = new Weiche(40,config,fmngr.passstate(40));
  Weiche *w41ptr = new Weiche(41,config,fmngr.passstate(41));
  Weiche *w42ptr = new Weiche(42,config,fmngr.passstate(42));
  Weiche *w43ptr = new Weiche(43,config,fmngr.passstate(43));
  Weiche *w44ptr = new Weiche(44,config,fmngr.passstate(44));
  Weiche *w45ptr = new Weiche(45,config,fmngr.passstate(45));
  Weiche *w46ptr = new Weiche(46,config,fmngr.passstate(46));
  Weiche *w47ptr = new Weiche(47,config,fmngr.passstate(47));
  Weiche *w48ptr = new Weiche(48,config,fmngr.passstate(48));
  Weiche *w49ptr = new Weiche(49,config,fmngr.passstate(49));
  Weiche *w50ptr = new Weiche(50,config,fmngr.passstate(50));
  Weiche *w51ptr = new Weiche(51,config,fmngr.passstate(51));
  Weiche *w52ptr = new Weiche(52,config,fmngr.passstate(52));
  Weiche *w53ptr = new Weiche(53,config,fmngr.passstate(53));
  //:::double turnouts:::
  Connector *connector1 = new Connector("014015", config);
  Connector *connector2 = new Connector("017018", config);
  QObject::connect(w14ptr,&Weiche::kreuzungsweiche,connector1,&Connector::recieveCall);
  QObject::connect(w15ptr,&Weiche::kreuzungsweiche,connector1,&Connector::recieveCall);
  QObject::connect(w17ptr,&Weiche::kreuzungsweiche,connector2,&Connector::recieveCall);
  QObject::connect(w18ptr,&Weiche::kreuzungsweiche,connector2,&Connector::recieveCall);
  //:::segments:::
  Block *abptr = new Block("ab", stellwerkstecptr, config);
  Block *acptr = new Block("ac", stellwerkstecptr, config);
  Block *adptr = new Block("ad", stellwerkstecptr, config);
  Block *aeptr = new Block("ae", stellwerkstecptr, config);
  Block *afptr = new Block("af", stellwerkstecptr, config);
  Block *agptr = new Block("ag", stellwerkstecptr, config);
  Block *ahptr = new Block("ah", stellwerkstecptr, config);
  Block *aiptr = new Block("ai", stellwerkstecptr, config);
  Block *ajptr = new Block("aj", stellwerkstecptr, config);
  Block *akptr = new Block("ak", stellwerkstecptr, config);
  Block *alptr = new Block("al", stellwerkstecptr, config);
  Block *amptr = new Block("am", stellwerkstecptr, config);
  Block *anptr = new Block("an", stellwerkstecptr, config);
  Block *aoptr = new Block("ao", stellwerkstecptr, config);
  Block *apptr = new Block("ap", stellwerkstecptr, config);
  Block *aqptr = new Block("aq", stellwerkstecptr, config);
  Block *arptr = new Block("ar", stellwerkstecptr, config);
  Block *asptr = new Block("as", stellwerkstecptr, config);
  Block *atptr = new Block("at", stellwerkstecptr, config);
  Block *auptr = new Block("au", stellwerkstecptr, config);
  Block *avptr = new Block("av", stellwerkstecptr, config);
  Block *awptr = new Block("aw", stellwerkstecptr, config);
  Block *axptr = new Block("ax", stellwerkstecptr, config);
  Block *ayptr = new Block("ay", stellwerkstecptr, config);
  Block *azptr = new Block("az", stellwerkstecptr, config);
  Block *baptr = new Block("ba", stellwerkstecptr, config);
  Block *bbptr = new Block("bb", stellwerkstecptr, config);
  Block *bcptr = new Block("bc", stellwerkstecptr, config);
  Block *bdptr = new Block("bd", stellwerkstecptr, config);
  Block *beptr = new Block("be", stellwerkstecptr, config);
  Block *bfptr = new Block("bf", stellwerkstecptr, config);
  Block *bgptr = new Block("bg", stellwerkstecptr, config);
  Block *bhptr = new Block("bh", stellwerkstecptr, config);
  Block *biptr = new Block("bi", stellwerkstecptr, config);
  Block *bjptr = new Block("bj", stellwerkstecptr, config);
  Block *bkptr = new Block("bk", stellwerkstecptr, config);
  Block *blptr = new Block("bl", stellwerkstecptr, config);
  Block *bmptr = new Block("bm", stellwerkstecptr, config);
  Block *bnptr = new Block("bn", stellwerkstecptr, config);
  Block *boptr = new Block("bo", stellwerkstecptr, config);
  Block *bpptr = new Block("bp", stellwerkstecptr, config);
  Block *bqptr = new Block("bq", stellwerkstecptr, config);
  Block *brptr = new Block("br", stellwerkstecptr, config);
  Block *bsptr = new Block("bs", stellwerkstecptr, config);
  Block *btptr = new Block("bt", stellwerkstecptr, config);
  Block *buptr = new Block("bu", stellwerkstecptr, config);
  Block *bvptr = new Block("bv", stellwerkstecptr, config);
  Block *bwptr = new Block("bw", stellwerkstecptr, config);
  Block *bxptr = new Block("bx", stellwerkstecptr, config);
  Block *byptr = new Block("by", stellwerkstecptr, config);
  Block *bzptr = new Block("bz", stellwerkstecptr, config);
  Block *captr = new Block("ca", stellwerkstecptr, config);
  Block *cbptr = new Block("cb", stellwerkstecptr, config);
  //:::distant signals:::
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
  VSignal *v17ptr = new VSignal(17);
  VSignal *v18ptr = new VSignal(18);
  VSignal *v19ptr = new VSignal(19);
  VSignal *v20ptr = new VSignal(20);
  VSignal *v21ptr = new VSignal(21);
  VSignal *v22ptr = new VSignal(22);
  VSignal *v23ptr = new VSignal(23);
  VSignal *v24ptr = new VSignal(24);
  VSignal *v25ptr = new VSignal(25);
  VSignal *v26ptr = new VSignal(26);
  VSignal *v27ptr = new VSignal(27);
  VSignal *v28ptr = new VSignal(28);
  VSignal *v29ptr = new VSignal(29);
  VSignal *v30ptr = new VSignal(30);
  VSignal *v31ptr = new VSignal(31);
  VSignal *v32ptr = new VSignal(32);
  VSignal *v33ptr = new VSignal(33);
  VSignal *v34ptr = new VSignal(34);
  VSignal *v35ptr = new VSignal(35);
  VSignal *v36ptr = new VSignal(36);
  VSignal *v37ptr = new VSignal(37);
  VSignal *v38ptr = new VSignal(38);
  VSignal *v39ptr = new VSignal(39);
  VSignal *v40ptr = new VSignal(40);
  VSignal *v41ptr = new VSignal(41);
  VSignal *v42ptr = new VSignal(42);
  VSignal *v43ptr = new VSignal(43);
  VSignal *v44ptr = new VSignal(44);
  v1ptr->setRichtung("S001","S004");
  v2ptr->setRichtung("S013","S002");
  v3ptr->setRichtung("S004","S015");
  v3ptr->setRichtung("S008","S015");
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
  v6ptr->setRichtung("S004","S050");
  v6ptr->setRichtung("S005","S012");
  v6ptr->setRichtung("S005","S050");
  v7ptr->setRichtung("S011","S003");
  v7ptr->setRichtung("S049","S003");
  v7ptr->setRichtung("S011","S002");
  v7ptr->setRichtung("S049","S002");
  v8ptr->setRichtung("S007","S024");
  v9ptr->setRichtung("S006","S024");
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
  v17ptr->setRichtung("S025","S009");
  v17ptr->setRichtung("S025","S011");
  v18ptr->setRichtung("S030","S008");
  v19ptr->setRichtung("S010","S028");
  v19ptr->setRichtung("S012","S028");
  v20ptr->setRichtung("S024","S027");
  v21ptr->setRichtung("S029","S025");
  v21ptr->setRichtung("S030","S025");
  v22ptr->setRichtung("S027","S039");
  v23ptr->setRichtung("S028","S039");
  v23ptr->setRichtung("S028","S046");
  v24ptr->setRichtung("S031","S043");
  v24ptr->setRichtung("S031","S045");
  v25ptr->setRichtung("S032","S047");
  v26ptr->setRichtung("S033","S047");
  v27ptr->setRichtung("S034","S047");
  v28ptr->setRichtung("S026","S038");
  v29ptr->setRichtung("S040","S029");
  v30ptr->setRichtung("S041","S030");
  v30ptr->setRichtung("S041","S031");
  v31ptr->setRichtung("S039","S048");
  v32ptr->setRichtung("S042","S031");
  v33ptr->setRichtung("S038","S044");
  v33ptr->setRichtung("S038","S048");
  v34ptr->setRichtung("S002","S032");
  v34ptr->setRichtung("S002","S033");
  v34ptr->setRichtung("S043","S032");
  v34ptr->setRichtung("S043","S033");
  v35ptr->setRichtung("S028","S046");
  v36ptr->setRichtung("S036","S026");
  v36ptr->setRichtung("S036","S001");
  v36ptr->setRichtung("S037","S026");
  v36ptr->setRichtung("S037","S001");
  v37ptr->setRichtung("S002","S042");
  v37ptr->setRichtung("S003","S042");
  v38ptr->setRichtung("S048","S035");
  v38ptr->setRichtung("S048","S036");
  v38ptr->setRichtung("S048","S037");
  v39ptr->setRichtung("S047","S041");
  v40ptr->setRichtung("S046","S038");
  v41ptr->setRichtung("S045","S040");
  v42ptr->setRichtung("S044","S004");
  v42ptr->setRichtung("S044","S005");
  v43ptr->setRichtung("S051","S049");
  v44ptr->setRichtung("S052","S010");
  //:::main signals:::
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
  HSignal *s24ptr = new HSignal(24);
  HSignal *s25ptr = new HSignal(25);
  HSignal *s26ptr = new HSignal(26);
  HSignal *s27ptr = new HSignal(27);
  HSignal *s28ptr = new HSignal(28);
  HSignal *s29ptr = new HSignal(29);
  HSignal *s30ptr = new HSignal(30);
  HSignal *s31ptr = new HSignal(31);
  HSignal *s32ptr = new HSignal(32);
  HSignal *s33ptr = new HSignal(33);
  HSignal *s34ptr = new HSignal(34);
  HSignal *s35ptr = new HSignal(35);
  HSignal *s36ptr = new HSignal(36);
  HSignal *s37ptr = new HSignal(37);
  HSignal *s38ptr = new HSignal(38);
  HSignal *s39ptr = new HSignal(39);
  HSignal *s40ptr = new HSignal(40);
  HSignal *s41ptr = new HSignal(41);
  HSignal *s42ptr = new HSignal(42);
  HSignal *s43ptr = new HSignal(43);
  HSignal *s44ptr = new HSignal(44);
  HSignal *s45ptr = new HSignal(45);
  HSignal *s46ptr = new HSignal(46);
  HSignal *s47ptr = new HSignal(47);
  HSignal *s48ptr = new HSignal(48);
  HSignal *s49ptr = new HSignal(49);
  HSignal *s50ptr = new HSignal(50);
  HSignal *s51ptr = new HSignal(51);
  HSignal *s52ptr = new HSignal(52);
  
  //:::::::::
  s1ptr->addVSignal(v1ptr,"v");
  s1ptr->addVSignal(v36ptr,"r");
  s2ptr->addVSignal(v34ptr,"v");
  s2ptr->addVSignal(v37ptr,"v");
  s2ptr->addVSignal(v2ptr,"r");
  s2ptr->addVSignal(v4ptr,"r");
  s2ptr->addVSignal(v7ptr,"r");
  s3ptr->addVSignal(v37ptr,"v");
  s3ptr->addVSignal(v7ptr,"r");
  s4ptr->addVSignal(v1ptr,"r");
  s4ptr->addVSignal(v3ptr,"v");
  s4ptr->addVSignal(v5ptr,"v");
  s5ptr->addVSignal(v42ptr,"r");
  s5ptr->addVSignal(v6ptr,"v");
  s6ptr->addVSignal(v4ptr,"r");
  s6ptr->addVSignal(v9ptr,"v");
  s7ptr->addVSignal(v3ptr,"r");
  s7ptr->addVSignal(v8ptr,"v");
  s8ptr->addVSignal(v3ptr,"v");
  s8ptr->addVSignal(v4ptr,"v");
  s8ptr->addVSignal(v18ptr,"r");
  s9ptr->addVSignal(v17ptr,"r");
  s10ptr->addVSignal(v6ptr,"r");
  s10ptr->addVSignal(v19ptr,"v");
  s11ptr->addVSignal(v7ptr,"v");
  s11ptr->addVSignal(v17ptr,"r");
  s12ptr->addVSignal(v6ptr,"r");
  s12ptr->addVSignal(v19ptr,"v");
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
  s24ptr->addVSignal(v8ptr,"r");
  s24ptr->addVSignal(v9ptr,"r");
  s24ptr->addVSignal(v20ptr,"v");
  s25ptr->addVSignal(v17ptr,"v");
  s25ptr->addVSignal(v21ptr,"r");
  s26ptr->addVSignal(v28ptr,"v");
  s26ptr->addVSignal(v36ptr,"r");
  s27ptr->addVSignal(v20ptr,"r");
  s27ptr->addVSignal(v22ptr,"v");
  s28ptr->addVSignal(v19ptr,"r");
  s28ptr->addVSignal(v23ptr,"v");
  s28ptr->addVSignal(v35ptr,"v");
  s29ptr->addVSignal(v21ptr,"v");
  s29ptr->addVSignal(v29ptr,"r");
  s30ptr->addVSignal(v18ptr,"v");
  s30ptr->addVSignal(v21ptr,"v");
  s30ptr->addVSignal(v30ptr,"r");
  s31ptr->addVSignal(v24ptr,"v");
  s31ptr->addVSignal(v32ptr,"r");
  s31ptr->addVSignal(v30ptr,"r");
  s32ptr->addVSignal(v25ptr,"v");
  s32ptr->addVSignal(v34ptr,"r");
  s33ptr->addVSignal(v26ptr,"v");
  s33ptr->addVSignal(v34ptr,"r");
  s34ptr->addVSignal(v27ptr,"v");
  s34ptr->addVSignal(v34ptr,"r");
  s35ptr->addVSignal(v38ptr,"r");
  s36ptr->addVSignal(v36ptr,"v");
  s36ptr->addVSignal(v38ptr,"r");
  s37ptr->addVSignal(v36ptr,"v");
  s37ptr->addVSignal(v38ptr,"r");
  s38ptr->addVSignal(v33ptr,"v");
  s38ptr->addVSignal(v28ptr,"r");
  s38ptr->addVSignal(v40ptr,"r");
  s39ptr->addVSignal(v22ptr,"r");
  s39ptr->addVSignal(v23ptr,"r");
  s39ptr->addVSignal(v31ptr,"v");
  s40ptr->addVSignal(v29ptr,"v");
  s40ptr->addVSignal(v41ptr,"r");
  s41ptr->addVSignal(v30ptr,"v");
  s41ptr->addVSignal(v39ptr,"r");
  s42ptr->addVSignal(v32ptr,"v");
  s42ptr->addVSignal(v37ptr,"r");
  s43ptr->addVSignal(v34ptr,"v");
  s43ptr->addVSignal(v24ptr,"r");
  s44ptr->addVSignal(v42ptr,"v");
  s44ptr->addVSignal(v33ptr,"r");
  s45ptr->addVSignal(v41ptr,"v");
  s45ptr->addVSignal(v24ptr,"r");
  s46ptr->addVSignal(v40ptr,"v");
  s46ptr->addVSignal(v35ptr,"r");
  s46ptr->addVSignal(v23ptr,"r");
  s47ptr->addVSignal(v39ptr,"v");
  s47ptr->addVSignal(v25ptr,"r");
  s47ptr->addVSignal(v26ptr,"r");
  s47ptr->addVSignal(v27ptr,"r");
  s48ptr->addVSignal(v38ptr,"v");
  s48ptr->addVSignal(v31ptr,"r");
  s48ptr->addVSignal(v33ptr,"r");
  s49ptr->addVSignal(v43ptr,"r");
  s51ptr->addVSignal(v43ptr,"v");
  s10ptr->addVSignal(v44ptr,"r");
  s52ptr->addVSignal(v44ptr,"v");
  //:::::::::
  //shunt signals
  WSignal *ww1ptr = new WSignal(1);
  ww1ptr->setRichtung("S049","S002");
  ww1ptr->setRichtung("S049","S003");
  ww1ptr->setRichtung("S011","S002");
  ww1ptr->setRichtung("S011","S003");
  s11ptr->addWSignal(ww1ptr,"v");
  s2ptr->addWSignal(ww1ptr,"r");
  s3ptr->addWSignal(ww1ptr,"r");
  //
  QGraphicsSvgItem *item3 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(item3);
  item3->setPos(QPointF(-870,895));
  item3->setRotation(90);
  item3->setScale(0.18);
  item3->setElementId("basic_sh0");
  item3->setObjectName("basic_sh0");
  ww1ptr->addVSignalitems(item3);
  QLabel *ww1label = new QLabel();
  scene->addWidget(ww1label);
  //
  WSignal *ww2ptr = new WSignal(2);
  ww2ptr->setRichtung("S004","S012");
  ww2ptr->setRichtung("S004","S050");
  ww2ptr->setRichtung("S005","S012");
  ww2ptr->setRichtung("S005","S050");
  s4ptr->addWSignal(ww2ptr,"v");
  s5ptr->addWSignal(ww2ptr,"v");
  s50ptr->addWSignal(ww2ptr,"r");
  s12ptr->addWSignal(ww2ptr,"r");
  //
  QGraphicsSvgItem *item5 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(item5);
  item5->setPos(QPointF(-950,285));
  item5->setRotation(90);
  item5->setScale(0.18);
  item5->setElementId("basic_sh0");
  item5->setObjectName("basic_sh0");
  ww2ptr->addVSignalitems(item5);
  QLabel *ww2label = new QLabel();
  scene->addWidget(ww2label);

  WSignal *ww3ptr = new WSignal(3);
  ww3ptr->setRichtung("S025","S011");
  ww3ptr->setRichtung("S025","S009");
  s25ptr->addWSignal(ww3ptr,"v");
  s9ptr->addWSignal(ww3ptr,"r");
  s11ptr->addWSignal(ww3ptr,"r");
  //
  QGraphicsSvgItem *itemw3 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw3);
  itemw3->setPos(QPointF(240,265));
  itemw3->setRotation(-90);
  itemw3->setScale(0.18);
  itemw3->setElementId("basic_sh0");
  itemw3->setObjectName("basic_sh0");
  ww3ptr->addVSignalitems(itemw3);
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
  QGraphicsSvgItem *itemw4 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw4);
  itemw4->setPos(QPointF(320,-615));
  itemw4->setRotation(-90);
  itemw4->setScale(0.18);
  itemw4->setElementId("basic_sh0");
  itemw4->setObjectName("basic_sh0");
  ww4ptr->addVSignalitems(itemw4);
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
  QGraphicsSvgItem *itemw5 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw5);
  itemw5->setPos(QPointF(410,-165));
  itemw5->setRotation(180);
  itemw5->setScale(0.18);
  itemw5->setElementId("basic_sh0");
  itemw5->setObjectName("basic_sh0");
  ww5ptr->addVSignalitems(itemw5);
  QLabel *ww5label = new QLabel();
  scene->addWidget(ww5label);
  
  WSignal *ww6ptr = new WSignal(6);
  //
  QGraphicsSvgItem *itemw6 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw6);
  itemw6->setPos(QPointF(280,-165));
  itemw6->setRotation(-90);
  itemw6->setScale(0.18);
  itemw6->setElementId("basic_sh0");
  itemw6->setObjectName("basic_sh0");
  ww6ptr->addVSignalitems(itemw6);
  QLabel *ww6label = new QLabel();
  scene->addWidget(ww6label);
  
  WSignal *ww7ptr = new WSignal(7);
  //
  QGraphicsSvgItem *itemw7 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw7);
  itemw7->setPos(QPointF(400,-145));
  itemw7->setRotation(90);
  itemw7->setScale(0.18);
  itemw7->setElementId("basic_sh0");
  itemw7->setObjectName("basic_sh0");
  ww7ptr->addVSignalitems(itemw7);
  QLabel *ww7label = new QLabel();
  scene->addWidget(ww7label);
  
  WSignal *ww8ptr = new WSignal(8);
  //
  QGraphicsSvgItem *itemw8 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw8);
  itemw8->setPos(QPointF(180,-165));
  itemw8->setRotation(180);
  itemw8->setScale(0.18);
  itemw8->setElementId("basic_sh0");
  itemw8->setObjectName("basic_sh0");
  ww8ptr->addVSignalitems(itemw8);
  QLabel *ww8label = new QLabel();
  scene->addWidget(ww8label);
  
  WSignal *ww9ptr = new WSignal(9);
  //
  QGraphicsSvgItem *itemw9 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw9);
  itemw9->setPos(QPointF(-30,-245));
  itemw9->setRotation(-90);
  itemw9->setScale(0.18);
  itemw9->setElementId("basic_sh0");
  itemw9->setObjectName("basic_sh0");
  ww9ptr->addVSignalitems(itemw9);
  QLabel *ww9label = new QLabel();
  scene->addWidget(ww9label);
  
  WSignal *ww10ptr = new WSignal(10);
  //
  QGraphicsSvgItem *itemw10 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw10);
  itemw10->setPos(QPointF(-670,-60));
  itemw10->setRotation(90);
  itemw10->setScale(0.18);
  itemw10->setElementId("basic_sh0");
  itemw10->setObjectName("basic_sh0");
  ww10ptr->addVSignalitems(itemw10);
  QLabel *ww10label = new QLabel();
  scene->addWidget(ww10label);
  
  WSignal *ww11ptr = new WSignal(11);
  //
  QGraphicsSvgItem *itemw11 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw11);
  itemw11->setPos(QPointF(-880,-165));
  itemw11->setRotation(-45);
  itemw11->setScale(0.18);
  itemw11->setElementId("basic_sh0");
  itemw11->setObjectName("basic_sh0");
  ww11ptr->addVSignalitems(itemw11);
  QLabel *ww11label = new QLabel();
  scene->addWidget(ww11label);

  WSignal *ww12ptr = new WSignal(12);
  ww12ptr->setRichtung("S010","S028");
  ww12ptr->setRichtung("S012","S028");
  s10ptr->addWSignal(ww12ptr,"v");
  s12ptr->addWSignal(ww12ptr,"v");
  s28ptr->addWSignal(ww12ptr,"r"); 
  //
  QGraphicsSvgItem *itemw12 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw12);
  itemw12->setPos(QPointF(620,220));
  itemw12->setScale(0.18);
  itemw12->setElementId("basic_sh0");
  itemw12->setObjectName("basic_sh0");
  ww12ptr->addVSignalitems(itemw12);
  QLabel *ww12label = new QLabel();
  scene->addWidget(ww12label);

  WSignal *ww13ptr = new WSignal(13);
  ww13ptr->setRichtung("S048","S035");
  ww13ptr->setRichtung("S048","S036");
  ww13ptr->setRichtung("S048","S037");
  s48ptr->addWSignal(ww13ptr,"v");
  s35ptr->addWSignal(ww13ptr,"r");
  s36ptr->addWSignal(ww13ptr,"r");
  s37ptr->addWSignal(ww13ptr,"r");
  //
  QGraphicsSvgItem *itemw13 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw13);
  itemw13->setPos(QPointF(1065,-345));
  itemw13->setScale(0.18);
  itemw13->setRotation(90);
  itemw13->setElementId("basic_sh0");
  itemw13->setObjectName("basic_sh0");
  ww13ptr->addVSignalitems(itemw13);
  QLabel *ww13label = new QLabel();
  scene->addWidget(ww13label);
  
  WSignal *ww14ptr = new WSignal(14);
  //
  QGraphicsSvgItem *itemw14 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw14);
  itemw14->setPos(QPointF(945,-230));
  itemw14->setScale(0.18);
  itemw14->setRotation(-90);
  itemw14->setElementId("basic_sh0");
  itemw14->setObjectName("basic_sh0");
  ww14ptr->addVSignalitems(itemw14);
  QLabel *ww14label = new QLabel();
  scene->addWidget(ww14label);

  WSignal *ww15ptr = new WSignal(15);
  //
  QGraphicsSvgItem *itemw15 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw15);
  itemw15->setPos(QPointF(1145,-200));
  itemw15->setScale(0.18);
  itemw15->setRotation(90);
  itemw15->setElementId("basic_sh0");
  itemw15->setObjectName("basic_sh0");
  ww15ptr->addVSignalitems(itemw15);
  QLabel *ww15label = new QLabel();
  scene->addWidget(ww15label);

  WSignal *ww16ptr = new WSignal(16);
  //
  QGraphicsSvgItem *itemw16 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw16);
  itemw16->setPos(QPointF(2065,-225));
  itemw16->setScale(0.18);
  itemw16->setRotation(-90);
  itemw16->setElementId("basic_sh0");
  itemw16->setObjectName("basic_sh0");
  ww16ptr->addVSignalitems(itemw16);
  QLabel *ww16label = new QLabel();
  scene->addWidget(ww16label);
  
  WSignal *ww17ptr = new WSignal(17);
  //
  QGraphicsSvgItem *itemw17 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw17);
  itemw17->setPos(QPointF(2395,-200));
  itemw17->setScale(0.18);
  itemw17->setRotation(90);
  itemw17->setElementId("basic_sh0");
  itemw17->setObjectName("basic_sh0");
  ww17ptr->addVSignalitems(itemw17);
  QLabel *ww17label = new QLabel();
  scene->addWidget(ww17label);
  
  WSignal *ww18ptr = new WSignal(18);
  //
  QGraphicsSvgItem *itemw18 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw18);
  itemw18->setPos(QPointF(2055,-190));
  itemw18->setScale(0.18);
  itemw18->setRotation(-45);
  itemw18->setElementId("basic_sh0");
  itemw18->setObjectName("basic_sh0");
  ww18ptr->addVSignalitems(itemw18);
  QLabel *ww18label = new QLabel();
  scene->addWidget(ww18label);

  WSignal *ww19ptr = new WSignal(19);
  //
  QGraphicsSvgItem *itemw19 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw19);
  itemw19->setPos(QPointF(2262,50));
  itemw19->setScale(0.18);
  itemw19->setRotation(135);
  itemw19->setElementId("basic_sh0");
  itemw19->setObjectName("basic_sh0");
  ww19ptr->addVSignalitems(itemw19);
  QLabel *ww19label = new QLabel();
  scene->addWidget(ww19label);
  
  WSignal *ww20ptr = new WSignal(20);
  //
  QGraphicsSvgItem *itemw20 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw20);
  itemw20->setPos(QPointF(2430,-275));
  itemw20->setScale(0.18);
  itemw20->setRotation(90);
  itemw20->setElementId("basic_sh0");
  itemw20->setObjectName("basic_sh0");
  ww20ptr->addVSignalitems(itemw20);
  QLabel *ww20label = new QLabel();
  scene->addWidget(ww20label);

  WSignal *ww21ptr = new WSignal(21);
  //
  QGraphicsSvgItem *itemw21 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw21);
  itemw21->setPos(QPointF(2200,-305));
  itemw21->setScale(0.18);
  itemw21->setRotation(-90);
  itemw21->setElementId("basic_sh0");
  itemw21->setObjectName("basic_sh0");
  ww21ptr->addVSignalitems(itemw21);
  QLabel *ww21label = new QLabel();
  scene->addWidget(ww21label);
  
  WSignal *ww22ptr = new WSignal(22);
  ww22ptr->setRichtung("S002","S034");
  ww22ptr->setRichtung("S002","S033");
  ww22ptr->setRichtung("S002","S032");
  s2ptr->addWSignal(ww22ptr,"v");
  s34ptr->addWSignal(ww22ptr,"r");
  s33ptr->addWSignal(ww22ptr,"r");
  s32ptr->addWSignal(ww22ptr,"r");
  //
  QGraphicsSvgItem *itemw22 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw22);
  itemw22->setPos(QPointF(2210,-375));
  itemw22->setScale(0.18);
  itemw22->setRotation(-90);
  itemw22->setElementId("basic_sh0");
  itemw22->setObjectName("basic_sh0");
  ww22ptr->addVSignalitems(itemw22);
  QLabel *ww22label = new QLabel();
  scene->addWidget(ww22label);
  
  WSignal *ww23ptr = new WSignal(23);
  ww23ptr->setRichtung("S036","S001");
  ww23ptr->setRichtung("S036","S026");
  ww23ptr->setRichtung("S037","S001");
  ww23ptr->setRichtung("S037","S026");
  s36ptr->addWSignal(ww23ptr,"v");
  s37ptr->addWSignal(ww23ptr,"v");
  s1ptr->addWSignal(ww23ptr,"r");
  s26ptr->addWSignal(ww23ptr,"r");
  //
  QGraphicsSvgItem *itemw23 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw23);
  itemw23->setPos(QPointF(1825,220));
  itemw23->setScale(0.18);
  itemw23->setRotation(-90);
  itemw23->setElementId("basic_sh0");
  itemw23->setObjectName("basic_sh0");
  ww23ptr->addVSignalitems(itemw23);
  QLabel *ww23label = new QLabel();
  scene->addWidget(ww23label);

  WSignal *ww24ptr = new WSignal(24);
  //
  QGraphicsSvgItem *itemw24 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw24);
  itemw24->setPos(QPointF(-865,1785));
  itemw24->setRotation(-90);
  itemw24->setScale(0.18);
  itemw24->setElementId("basic_sh0");
  itemw24->setObjectName("basic_sh0");
  ww24ptr->addVSignalitems(itemw24);
  QLabel *ww24label = new QLabel();
  scene->addWidget(ww24label);

  WSignal *ww25ptr = new WSignal(25);
  //
  QGraphicsSvgItem *itemw25 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw25);
  itemw25->setPos(QPointF(-815,1715));
  itemw25->setRotation(-90);
  itemw25->setScale(0.18);
  itemw25->setElementId("basic_sh0");
  itemw25->setObjectName("basic_sh0");
  ww25ptr->addVSignalitems(itemw25);
  QLabel *ww25label = new QLabel();
  scene->addWidget(ww25label);

  WSignal *ww26ptr = new WSignal(26);
  //
  QGraphicsSvgItem *itemw26 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw26);
  itemw26->setPos(QPointF(-745,1645));
  itemw26->setRotation(-90);
  itemw26->setScale(0.18);
  itemw26->setElementId("basic_sh0");
  itemw26->setObjectName("basic_sh0");
  ww26ptr->addVSignalitems(itemw26);
  QLabel *ww26label = new QLabel();
  scene->addWidget(ww26label);

  WSignal *ww27ptr = new WSignal(27);
  //
  QGraphicsSvgItem *itemw27 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw27);
  itemw27->setPos(QPointF(-675,1575));
  itemw27->setRotation(-90);
  itemw27->setScale(0.18);
  itemw27->setElementId("basic_sh0");
  itemw27->setObjectName("basic_sh0");
  ww27ptr->addVSignalitems(itemw27);
  QLabel *ww27label = new QLabel();
  scene->addWidget(ww27label);

  WSignal *ww28ptr = new WSignal(28);
  //
  QGraphicsSvgItem *itemw28 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw28);
  itemw28->setPos(QPointF(-605,1505));
  itemw28->setRotation(-90);
  itemw28->setScale(0.18);
  itemw28->setElementId("basic_sh0");
  itemw28->setObjectName("basic_sh0");
  ww28ptr->addVSignalitems(itemw28);
  QLabel *ww28label = new QLabel();
  scene->addWidget(ww28label);

  WSignal *ww29ptr = new WSignal(29);
  //
  QGraphicsSvgItem *itemw29 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw29);
  itemw29->setPos(QPointF(-535,1435));
  itemw29->setRotation(-90);
  itemw29->setScale(0.18);
  itemw29->setElementId("basic_sh0");
  itemw29->setObjectName("basic_sh0");
  ww29ptr->addVSignalitems(itemw29);
  QLabel *ww29label = new QLabel();
  scene->addWidget(ww29label);

  WSignal *ww30ptr = new WSignal(30);
  //
  QGraphicsSvgItem *itemw30 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw30);
  itemw30->setPos(QPointF(-465,1365));
  itemw30->setRotation(-90);
  itemw30->setScale(0.18);
  itemw30->setElementId("basic_sh0");
  itemw30->setObjectName("basic_sh0");
  ww30ptr->addVSignalitems(itemw30);
  QLabel *ww30label = new QLabel();
  scene->addWidget(ww30label);

  WSignal *ww31ptr = new WSignal(31);
  //
  QGraphicsSvgItem *itemw311 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw311);
  itemw311->setPos(QPointF(-445,1295));
  itemw311->setRotation(-90);
  itemw311->setScale(0.18);
  itemw311->setElementId("basic_sh0");
  itemw311->setObjectName("basic_sh0");
  ww31ptr->addVSignalitems(itemw311);
  QLabel *ww31label = new QLabel();
  scene->addWidget(ww31label);

  WSignal *ww32ptr = new WSignal(32);
  //
  QGraphicsSvgItem *itemw321 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw321);
  itemw321->setPos(QPointF(310,1315));
  itemw321->setRotation(90);
  itemw321->setScale(0.18);
  itemw321->setElementId("basic_sh0");
  itemw321->setObjectName("basic_sh0");
  ww32ptr->addVSignalitems(itemw321);
  QLabel *ww32label = new QLabel();
  scene->addWidget(ww32label);

  WSignal *ww33ptr = new WSignal(33);
  //
  QGraphicsSvgItem *itemw33 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw33);
  itemw33->setPos(QPointF(255,1385));
  itemw33->setRotation(90);
  itemw33->setScale(0.18);
  itemw33->setElementId("basic_sh0");
  itemw33->setObjectName("basic_sh0");
  ww33ptr->addVSignalitems(itemw33);
  QLabel *ww33label = new QLabel();
  scene->addWidget(ww33label);

  WSignal *ww34ptr = new WSignal(34);
  //
  QGraphicsSvgItem *itemw34 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw34);
  itemw34->setPos(QPointF(185,1455));
  itemw34->setRotation(90);
  itemw34->setScale(0.18);
  itemw34->setElementId("basic_sh0");
  itemw34->setObjectName("basic_sh0");
  ww34ptr->addVSignalitems(itemw34);
  QLabel *ww34label = new QLabel();
  scene->addWidget(ww34label);

  WSignal *ww35ptr = new WSignal(35);
  //
  QGraphicsSvgItem *itemw35 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw35);
  itemw35->setPos(QPointF(115,1525));
  itemw35->setRotation(90);
  itemw35->setScale(0.18);
  itemw35->setElementId("basic_sh0");
  itemw35->setObjectName("basic_sh0");
  ww35ptr->addVSignalitems(itemw35);
  QLabel *ww35label = new QLabel();
  scene->addWidget(ww35label);

  WSignal *ww36ptr = new WSignal(36);
  //
  QGraphicsSvgItem *itemw36 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw36);
  itemw36->setPos(QPointF(45,1595));
  itemw36->setRotation(90);
  itemw36->setScale(0.18);
  itemw36->setElementId("basic_sh0");
  itemw36->setObjectName("basic_sh0");
  ww36ptr->addVSignalitems(itemw36);
  QLabel *ww36label = new QLabel();
  scene->addWidget(ww36label);

  WSignal *ww37ptr = new WSignal(37);
  //
  QGraphicsSvgItem *itemw37 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw37);
  itemw37->setPos(QPointF(-25,1665));
  itemw37->setRotation(90);
  itemw37->setScale(0.18);
  itemw37->setElementId("basic_sh0");
  itemw37->setObjectName("basic_sh0");
  ww37ptr->addVSignalitems(itemw37);
  QLabel *ww37label = new QLabel();
  scene->addWidget(ww37label);

  WSignal *ww38ptr = new WSignal(38);
  //
  QGraphicsSvgItem *itemw38 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw38);
  itemw38->setPos(QPointF(-95,1735));
  itemw38->setRotation(90);
  itemw38->setScale(0.18);
  itemw38->setElementId("basic_sh0");
  itemw38->setObjectName("basic_sh0");
  ww38ptr->addVSignalitems(itemw38);
  QLabel *ww38label = new QLabel();
  scene->addWidget(ww38label);
  
  WSignal *ww39ptr = new WSignal(39);
  //
  QGraphicsSvgItem *itemw39 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw39);
  itemw39->setPos(QPointF(-975,1805));
  itemw39->setRotation(90);
  itemw39->setScale(0.18);
  itemw39->setElementId("basic_sh0");
  itemw39->setObjectName("basic_sh0");
  ww39ptr->addVSignalitems(itemw39);
  QLabel *ww39label = new QLabel();
  scene->addWidget(ww39label);

  WSignal *ww40ptr = new WSignal(40);
  //
  QGraphicsSvgItem *itemw40 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw40);
  itemw40->setPos(QPointF(391,1269));
  itemw40->setRotation(-135);
  itemw40->setScale(0.18);
  itemw40->setElementId("basic_sh0");
  itemw40->setObjectName("basic_sh0");
  ww40ptr->addVSignalitems(itemw40);
  QLabel *ww40label = new QLabel();
  scene->addWidget(ww40label);

  WSignal *ww41ptr = new WSignal(41);
  //
  QGraphicsSvgItem *itemw41 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw41);
  itemw41->setPos(QPointF(-115,1805));
  itemw41->setRotation(90);
  itemw41->setScale(0.18);
  itemw41->setElementId("basic_sh0");
  itemw41->setObjectName("basic_sh0");
  ww41ptr->addVSignalitems(itemw41);
  QLabel *ww41label = new QLabel();
  scene->addWidget(ww41label);
  
  WSignal *ww42ptr = new WSignal(42);
  //
  ww42ptr->setRichtung("S051","S049");
  s51ptr->addWSignal(ww42ptr,"v");
  s49ptr->addWSignal(ww42ptr,"r");
  //
  QGraphicsSvgItem *itemw421 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw421);
  itemw421->setPos(QPointF(145,115));
  itemw421->setRotation(-90);
  itemw421->setScale(0.18);
  itemw421->setElementId("basic_sh0");
  itemw421->setObjectName("basic_sh0");
  ww42ptr->addVSignalitems(itemw421);
  QLabel *ww42label = new QLabel();
  scene->addWidget(ww42label);
  
  WSignal *ww43ptr = new WSignal(43);
  //
  ww43ptr->setRichtung("S052","S010");
  s52ptr->addWSignal(ww43ptr,"v");
  s10ptr->addWSignal(ww43ptr,"r");
  //
  QGraphicsSvgItem *itemw431 = new QGraphicsSvgItem(":/resources/WS.svg");
  scene->addItem(itemw431);
  itemw431->setPos(QPointF(-980,220));
  itemw431->setRotation(90);
  itemw431->setScale(0.18);
  itemw431->setElementId("basic_sh0");
  itemw431->setObjectName("basic_sh0");
  ww43ptr->addVSignalitems(itemw431);
  QLabel *ww43label = new QLabel();
  scene->addWidget(ww43label);
  
  //turnout status ---------------------------------------------------
  QPair<Weiche*, bool> s1tos4w2(w2ptr,true);
  QList<QPair<Weiche*, bool>> s1tos4;
  s1tos4.push_back(s1tos4w2);
  s1ptr->addWeichenstatus(s4ptr,s1tos4);

  QPair<Weiche*, bool> s2tos42w1(w1ptr,false);
  QPair<Weiche*, bool> s2tos42w2(w2ptr,false);
  QList<QPair<Weiche*, bool>> s2tos42;
  s2tos42.push_back(s2tos42w1);
  s2tos42.push_back(s2tos42w2);
  s2ptr->addWeichenstatus(s42ptr,s2tos42);
  QPair<Weiche*, bool> s2tos33w31(w31ptr,false);
  QPair<Weiche*, bool> s2tos33w2(w2ptr,true);
  QPair<Weiche*, bool> s2tos33w25(w25ptr,false);
  QPair<Weiche*, bool> s2tos33w26(w26ptr,false);
  QList<QPair<Weiche*, bool>> s2tos33;
  s2tos33.push_back(s2tos33w31);
  s2tos33.push_back(s2tos33w2);
  s2tos33.push_back(s2tos33w25);
  s2tos33.push_back(s2tos33w26);
  s2ptr->addWeichenstatus(s33ptr,s2tos33);
  QPair<Weiche*, bool> s2tos32w31(w31ptr,false);
  QPair<Weiche*, bool> s2tos32w2(w2ptr,true);
  QPair<Weiche*, bool> s2tos32w25(w25ptr,true);
  QList<QPair<Weiche*, bool>> s2tos32;
  s2tos32.push_back(s2tos32w31);
  s2tos32.push_back(s2tos32w2);
  s2tos32.push_back(s2tos32w25);
  s2ptr->addWeichenstatus(s32ptr,s2tos32);

  QPair<Weiche*, bool> s3tos42w1(w1ptr,true);
  QList<QPair<Weiche*, bool>> s3tos42;
  s3tos42.push_back(s3tos42w1);
  s3ptr->addWeichenstatus(s42ptr,s3tos42);

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

  QPair<Weiche*, bool> s4tos50w3(w3ptr,false);
  QPair<Weiche*, bool> s4tos50w4(w4ptr,false);
  QPair<Weiche*, bool> s4tos50w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s4tos50;
  s4tos50.push_back(s4tos50w3);
  s4tos50.push_back(s4tos50w4);
  s4tos50.push_back(s4tos50w9);
  s4ptr->addWeichenstatus(s50ptr,s4tos50);

  QPair<Weiche*, bool> s4tos12w3(w3ptr,false);
  QPair<Weiche*, bool> s4tos12w4(w4ptr,false);
  QPair<Weiche*, bool> s4tos12w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> s4tos12;
  s4tos12.push_back(s4tos12w3);
  s4tos12.push_back(s4tos12w4);
  s4tos12.push_back(s4tos12w9);
  s4ptr->addWeichenstatus(s12ptr,s4tos12);


  QPair<Weiche*, bool> s5tos50w4(w4ptr,true);
  QPair<Weiche*, bool> s5tos50w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s5tos50;
  s5tos50.push_back(s5tos50w4);
  s5tos50.push_back(s5tos50w9);
  s5ptr->addWeichenstatus(s50ptr,s5tos50);

  QPair<Weiche*, bool> s6tos24w8(w8ptr,false);
  QList<QPair<Weiche*, bool>> s6tos24;
  s6tos24.push_back(s6tos24w8);
  s6ptr->addWeichenstatus(s24ptr,s6tos24);

  QPair<Weiche*, bool> s7tos24w8(w8ptr,true);
  QList<QPair<Weiche*, bool>> s7tos24;
  s7tos24.push_back(s7tos24w8);
  s7ptr->addWeichenstatus(s24ptr,s7tos24);

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

  QPair<Weiche*, bool> s49tos3w4(w4ptr,true);
  QPair<Weiche*, bool> s49tos3w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s49tos3;
  s49tos3.push_back(s49tos3w4);
  s49tos3.push_back(s49tos3w9);
  s49ptr->addWeichenstatus(s3ptr,s49tos3);

  QPair<Weiche*, bool> s10tos28w10(w10ptr,false);
  QList<QPair<Weiche*, bool>> s10tos28;
  s10tos28.push_back(s10tos28w10);
  s10ptr->addWeichenstatus(s28ptr,s10tos28);
  QPair<Weiche*, bool> s10toww12w10(w10ptr,false);
  QList<QPair<Weiche*, bool>> s10toww12;
  s10toww12.push_back(s10toww12w10);
  s10ptr->addWeichenstatusZuRangier(ww12ptr,s10toww12);

  QPair<Weiche*, bool> s11tos3w4(w4ptr,true);
  QPair<Weiche*, bool> s11tos3w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> s11tos3;
  s11tos3.push_back(s11tos3w4);
  s11tos3.push_back(s11tos3w9);
  s11ptr->addWeichenstatus(s3ptr,s11tos3);

  QPair<Weiche*, bool> s49tos2w4(w4ptr,false);
  QPair<Weiche*, bool> s49tos2w3(w3ptr,false);
  QPair<Weiche*, bool> s49tos2w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s49tos2;
  s49tos2.push_back(s49tos2w4);
  s49tos2.push_back(s49tos2w3);
  s49tos2.push_back(s49tos2w9);
  s49ptr->addWeichenstatus(s2ptr,s49tos2);

  QPair<Weiche*, bool> s11tos2w4(w4ptr,false);
  QPair<Weiche*, bool> s11tos2w3(w3ptr,false);
  QPair<Weiche*, bool> s11tos2w9(w9ptr,true);
  QList<QPair<Weiche*, bool>> s11tos2;
  s11tos2.push_back(s11tos2w4);
  s11tos2.push_back(s11tos2w3);
  s11tos2.push_back(s11tos2w9);
  s11ptr->addWeichenstatus(s2ptr,s11tos2);

  QPair<Weiche*, bool> s12tos28w10(w10ptr,true);
  QList<QPair<Weiche*, bool>> s12tos28;
  s12tos28.push_back(s12tos28w10);
  s12ptr->addWeichenstatus(s28ptr,s12tos28);
  QPair<Weiche*, bool> s12toww12w10(w10ptr,true);
  QList<QPair<Weiche*, bool>> s12toww12;
  s12toww12.push_back(s12toww12w10);
  s12ptr->addWeichenstatusZuRangier(ww12ptr,s12toww12);

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

  QPair<Weiche*, bool> ww2tos50w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> ww2tos50;
  ww2tos50.push_back(ww2tos50w9);
  ww2ptr->addWeichenstatusZuH(s50ptr->getS_id(),ww2tos50);

  QPair<Weiche*, bool> s49toww1w9(w9ptr,false);
  QList<QPair<Weiche*, bool>> s49toww1;
  s49toww1.push_back(s49toww1w9);
  s49ptr->addWeichenstatusZuRangier(ww1ptr,s49toww1);

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

  QPair<Weiche*, bool> s8tos15w8(w8ptr,false);
  QPair<Weiche*, bool> s8tos15w7(w7ptr,true);
  QPair<Weiche*, bool> s8tos15w6(w6ptr,false);
  QList<QPair<Weiche*, bool>> s8tos15;
  s8tos15.push_back(s8tos15w8);
  s8tos15.push_back(s8tos15w7);
  s8tos15.push_back(s8tos15w6);
  s8ptr->addWeichenstatus(s15ptr,s8tos15);
  
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

  
  QPair<Weiche*, bool> s25tos9w10(w10ptr,false);
  QList<QPair<Weiche*, bool>> s25tos9;
  s25tos9.push_back(s25tos9w10);
  s25ptr->addWeichenstatus(s9ptr,s25tos9);

  QPair<Weiche*, bool> s25tos11w10(w10ptr,true);
  QList<QPair<Weiche*, bool>> s25tos11;
  s25tos11.push_back(s25tos11w10);
  s25ptr->addWeichenstatus(s11ptr,s25tos11);

  QPair<Weiche*, bool> s29tos25w20(w20ptr,true);
  QList<QPair<Weiche*, bool>> s29tos25;
  s29tos25.push_back(s29tos25w20);
  s29ptr->addWeichenstatus(s25ptr,s29tos25);

  QPair<Weiche*, bool> s30tos25w20(w20ptr,false);
  QPair<Weiche*, bool> s30tos25w21(w21ptr,false);
  QList<QPair<Weiche*, bool>> s30tos25;
  s30tos25.push_back(s30tos25w20);
  s30tos25.push_back(s30tos25w21);
  s30ptr->addWeichenstatus(s25ptr,s30tos25);
  QPair<Weiche*, bool> s30tos8w21(w21ptr,true);
  QList<QPair<Weiche*, bool>> s30tos8;
  s30tos8.push_back(s30tos8w21);
  s30ptr->addWeichenstatus(s8ptr,s30tos8);

  QPair<Weiche*, bool> s26tos38w22(w22ptr,true);
  QList<QPair<Weiche*, bool>> s26tos38;
  s26tos38.push_back(s26tos38w22);
  s26ptr->addWeichenstatus(s38ptr,s26tos38);

  QPair<Weiche*, bool> s27tos39w21(w21ptr,true);
  QList<QPair<Weiche*, bool>> s27tos39;
  s27tos39.push_back(s27tos39w21);
  s27ptr->addWeichenstatus(s39ptr,s27tos39);

  QPair<Weiche*, bool> s28tos39w21(w21ptr,false);
  QPair<Weiche*, bool> s28tos39w20(w20ptr,false);
  QList<QPair<Weiche*, bool>> s28tos39;
  s28tos39.push_back(s28tos39w21);
  s28tos39.push_back(s28tos39w20);
  s28ptr->addWeichenstatus(s39ptr,s28tos39);
  QPair<Weiche*, bool> s28tos46w20(w20ptr,true);
  QList<QPair<Weiche*, bool>> s28tos46;
  s28tos46.push_back(s28tos46w20);
  s28ptr->addWeichenstatus(s46ptr,s28tos46);

  QPair<Weiche*, bool> s39tos48w23(w23ptr,true);
  QList<QPair<Weiche*, bool>> s39tos48;
  s39tos48.push_back(s39tos48w23);
  s39ptr->addWeichenstatus(s48ptr,s39tos48);

  QPair<Weiche*, bool> s41tos30w23(w23ptr,true);
  QList<QPair<Weiche*, bool>> s41tos30;
  s41tos30.push_back(s41tos30w23);
  s41ptr->addWeichenstatus(s30ptr,s41tos30);
  QPair<Weiche*, bool> s41tos31w23(w23ptr,false);
  QPair<Weiche*, bool> s41tos31w24(w24ptr,false);
  QList<QPair<Weiche*, bool>> s41tos31;
  s41tos31.push_back(s41tos31w23);
  s41tos31.push_back(s41tos31w24);
  s41ptr->addWeichenstatus(s31ptr,s41tos31);

  QPair<Weiche*, bool> s31tos45w22(w22ptr,false);
  QList<QPair<Weiche*, bool>> s31tos45;
  s31tos45.push_back(s31tos45w22);
  s31ptr->addWeichenstatus(s45ptr,s31tos45);
  QPair<Weiche*, bool> s31tos43w22(w22ptr,true);
  QList<QPair<Weiche*, bool>> s31tos43;
  s31tos43.push_back(s31tos43w22);
  s31ptr->addWeichenstatus(s43ptr,s31tos43);

  QPair<Weiche*, bool> s32tos47w30(w30ptr,true);
  QList<QPair<Weiche*, bool>> s32tos47;
  s32tos47.push_back(s32tos47w30);
  s32ptr->addWeichenstatus(s47ptr,s32tos47);

  QPair<Weiche*, bool> s33tos47w30(w30ptr,false);
  QPair<Weiche*, bool> s33tos47w29(w29ptr,true);
  QList<QPair<Weiche*, bool>> s33tos47;
  s33tos47.push_back(s33tos47w30);
  s33tos47.push_back(s33tos47w29);
  s33ptr->addWeichenstatus(s47ptr,s33tos47);

  QPair<Weiche*, bool> s34tos47w30(w30ptr,false);
  QPair<Weiche*, bool> s34tos47w29(w29ptr,false);
  QPair<Weiche*, bool> s34tos47w28(w28ptr,false);
  QList<QPair<Weiche*, bool>> s34tos47;
  s34tos47.push_back(s34tos47w30);
  s34tos47.push_back(s34tos47w29);
  s34tos47.push_back(s34tos47w28);
  s34ptr->addWeichenstatus(s47ptr,s34tos47);
  QPair<Weiche*, bool> s34toww14w28(w28ptr,true);
  QList<QPair<Weiche*, bool>> s34toww14;
  s34toww14.push_back(s34toww14w28);
  s34ptr->addWeichenstatusZuRangier(ww14ptr,s34toww14);

  QPair<Weiche*, bool> s35toww17w27(w27ptr,true);
  QList<QPair<Weiche*, bool>> s35toww17;
  s35toww17.push_back(s35toww17w27);
  s35ptr->addWeichenstatusZuRangier(ww17ptr,s35toww17);
  QPair<Weiche*, bool> s35toww19w27(w27ptr,false);
  QList<QPair<Weiche*, bool>> s35toww19;
  s35toww19.push_back(s35toww19w27);
  s35ptr->addWeichenstatusZuRangier(ww19ptr,s35toww19);

  QPair<Weiche*, bool> s36tos1w25(w25ptr,false);
  QPair<Weiche*, bool> s36tos1w26(w26ptr,false);
  QPair<Weiche*, bool> s36tos1w31(w31ptr,false);
  QList<QPair<Weiche*, bool>> s36tos1;
  s36tos1.push_back(s36tos1w25);
  s36tos1.push_back(s36tos1w26);
  s36tos1.push_back(s36tos1w31);
  s36ptr->addWeichenstatus(s1ptr,s36tos1);
  QPair<Weiche*, bool> s36tos26w26(w26ptr,false);
  QPair<Weiche*, bool> s36tos26w25(w25ptr,false);
  QPair<Weiche*, bool> s36tos26w31(w31ptr,true);
  QList<QPair<Weiche*, bool>> s36tos26;
  s36tos26.push_back(s36tos26w26);
  s36tos26.push_back(s36tos26w25);
  s36tos26.push_back(s36tos26w31);
  s36ptr->addWeichenstatus(s26ptr,s36tos26);
  QPair<Weiche*, bool> s36toww23w26(w26ptr,false);
  QPair<Weiche*, bool> s36toww23w25(w25ptr,false);
  QList<QPair<Weiche*, bool>> s36toww23;
  s36toww23.push_back(s36toww23w26);
  s36toww23.push_back(s36toww23w25);
  s36ptr->addWeichenstatusZuRangier(ww23ptr,s36toww23);
  QPair<Weiche*, bool> s36toww20w26(w26ptr,true);
  QList<QPair<Weiche*, bool>> s36toww20;
  s36toww20.push_back(s36toww20w26);
  s36ptr->addWeichenstatusZuRangier(ww20ptr,s36toww20);

  QPair<Weiche*, bool> s37tos1w25(w25ptr,true);
  QPair<Weiche*, bool> s37tos1w31(w31ptr,false);
  QList<QPair<Weiche*, bool>> s37tos1;
  s37tos1.push_back(s37tos1w25);
  s37tos1.push_back(s37tos1w31);
  s37ptr->addWeichenstatus(s1ptr,s37tos1);
  QPair<Weiche*, bool> s37tos26w25(w25ptr,true);
  QPair<Weiche*, bool> s37tos26w31(w31ptr,true);
  QList<QPair<Weiche*, bool>> s37tos26;
  s37tos26.push_back(s37tos26w25);
  s37tos26.push_back(s37tos26w31);
  s37ptr->addWeichenstatus(s26ptr,s37tos26);
  QPair<Weiche*, bool> s37toww23w25(w25ptr,true);
  QList<QPair<Weiche*, bool>> s37toww23;
  s37toww23.push_back(s37toww23w25);
  s37ptr->addWeichenstatusZuRangier(ww23ptr,s37toww23);

  QPair<Weiche*, bool> s38tos44w24(w24ptr,true);
  QList<QPair<Weiche*, bool>> s38tos44;
  s38tos44.push_back(s38tos44w24);
  s38ptr->addWeichenstatus(s44ptr,s38tos44);
  QPair<Weiche*, bool> s38tos48w24(w24ptr,false);
  QPair<Weiche*, bool> s38tos48w23(w23ptr,false);
  QList<QPair<Weiche*, bool>> s38tos48;
  s38tos48.push_back(s38tos48w24);
  s38tos48.push_back(s38tos48w23);
  s38ptr->addWeichenstatus(s48ptr,s38tos48);

  QPair<Weiche*, bool> s42tos31w24(w24ptr,true);
  QList<QPair<Weiche*, bool>> s42tos31;
  s42tos31.push_back(s42tos31w24);
  s42ptr->addWeichenstatus(s31ptr,s42tos31);

  QPair<Weiche*, bool> s43tos32w31(w31ptr,true);
  QPair<Weiche*, bool> s43tos32w25(w25ptr,true);
  QList<QPair<Weiche*, bool>> s43tos32;
  s43tos32.push_back(s43tos32w31);
  s43tos32.push_back(s43tos32w25);
  s43ptr->addWeichenstatus(s32ptr,s43tos32);
  QPair<Weiche*, bool> s43tos33w31(w31ptr,true);
  QPair<Weiche*, bool> s43tos33w25(w25ptr,false);
  QPair<Weiche*, bool> s43tos33w26(w26ptr,false);
  QList<QPair<Weiche*, bool>> s43tos33;
  s43tos33.push_back(s43tos33w31);
  s43tos33.push_back(s43tos33w25);
  s43tos33.push_back(s43tos33w26);
  s43ptr->addWeichenstatus(s33ptr,s43tos33);
  QPair<Weiche*, bool> s43toww22w24(w24ptr,true);
  QList<QPair<Weiche*, bool>> s43toww22;
  s43toww22.push_back(s43toww22w24);
  s43ptr->addWeichenstatusZuRangier(ww22ptr,s43toww22);

  QPair<Weiche*, bool> s44tos4w1(w1ptr,false);
  QPair<Weiche*, bool> s44tos4w2(w2ptr,false);
  QList<QPair<Weiche*, bool>> s44tos4;
  s44tos4.push_back(s44tos4w1);
  s44tos4.push_back(s44tos4w2);
  s44ptr->addWeichenstatus(s4ptr,s44tos4);
  QPair<Weiche*, bool> s44tos5w1(w1ptr,true);
  QList<QPair<Weiche*, bool>> s44tos5;
  s44tos5.push_back(s44tos5w1);
  s44ptr->addWeichenstatus(s5ptr,s44tos5);

  QPair<Weiche*, bool> s46tos38w22(w22ptr,false);
  QList<QPair<Weiche*, bool>> s46tos38;
  s46tos38.push_back(s46tos38w22);
  s46ptr->addWeichenstatus(s38ptr,s46tos38);

  QPair<Weiche*, bool> s48tos35w30(w30ptr,false);
  QPair<Weiche*, bool> s48tos35w29(w29ptr,false);
  QPair<Weiche*, bool> s48tos35w28(w28ptr,false);
  QList<QPair<Weiche*, bool>> s48tos35;
  s48tos35.push_back(s48tos35w30);
  s48tos35.push_back(s48tos35w29);
  s48tos35.push_back(s48tos35w28);
  s48ptr->addWeichenstatus(s35ptr,s48tos35);
  QPair<Weiche*, bool> s48tos36w30(w30ptr,false);
  QPair<Weiche*, bool> s48tos36w29(w29ptr,true);
  QList<QPair<Weiche*, bool>> s48tos36;
  s48tos36.push_back(s48tos36w30);
  s48tos36.push_back(s48tos36w29);
  s48ptr->addWeichenstatus(s36ptr,s48tos36);
  QPair<Weiche*, bool> s48tos37w30(w30ptr,true);
  QList<QPair<Weiche*, bool>> s48tos37;
  s48tos37.push_back(s48tos37w30);
  s48ptr->addWeichenstatus(s37ptr,s48tos37);

  QPair<Weiche*, bool> ww13tos37w30(w30ptr,true);
  QList<QPair<Weiche*, bool>> ww13tos37;
  ww13tos37.push_back(ww13tos37w30);
  ww13ptr->addWeichenstatusZuH(s37ptr->getS_id(),ww13tos37);
  QPair<Weiche*, bool> ww13tos36w30(w30ptr,false);
  QPair<Weiche*, bool> ww13tos36w29(w29ptr,true);
  QList<QPair<Weiche*, bool>> ww13tos36;
  ww13tos36.push_back(ww13tos36w30);
  ww13tos36.push_back(ww13tos36w29);
  ww13ptr->addWeichenstatusZuH(s36ptr->getS_id(),ww13tos36);
  QPair<Weiche*, bool> ww13tos35w30(w30ptr,false);
  QPair<Weiche*, bool> ww13tos35w29(w29ptr,false);
  QPair<Weiche*, bool> ww13tos35w28(w28ptr,false);
  QList<QPair<Weiche*, bool>> ww13tos35;
  ww13tos35.push_back(ww13tos35w30);
  ww13tos35.push_back(ww13tos35w29);
  ww13tos35.push_back(ww13tos35w28);
  ww13ptr->addWeichenstatusZuH(s35ptr->getS_id(),ww13tos35);

  QPair<Weiche*, bool> ww15tos35w28(w28ptr,true);
  QList<QPair<Weiche*, bool>> ww15tos35;
  ww15tos35.push_back(ww15tos35w28);
  ww15ptr->addWeichenstatusZuH(s35ptr->getS_id(),ww15tos35);

  QPair<Weiche*, bool> ww16tos34w27(w27ptr,true);
  QList<QPair<Weiche*, bool>> ww16tos34;
  ww16tos34.push_back(ww16tos34w27);
  ww16ptr->addWeichenstatusZuH(s34ptr->getS_id(),ww16tos34);

  QPair<Weiche*, bool> ww18tos34w27(w27ptr,false);
  QList<QPair<Weiche*, bool>> ww18tos34;
  ww18tos34.push_back(ww18tos34w27);
  ww18ptr->addWeichenstatusZuH(s34ptr->getS_id(),ww18tos34);

  QPair<Weiche*, bool> ww21tos33w26(w26ptr,true);
  QList<QPair<Weiche*, bool>> ww21tos33;
  ww21tos33.push_back(ww21tos33w26);
  ww21ptr->addWeichenstatusZuH(s33ptr->getS_id(),ww21tos33);

  QPair<Weiche*, bool> ww22tos32w25(w25ptr,true);
  QList<QPair<Weiche*, bool>> ww22tos32;
  ww22tos32.push_back(ww22tos32w25);
  ww22ptr->addWeichenstatusZuH(s32ptr->getS_id(),ww22tos32);
  QPair<Weiche*, bool> ww22tos33w25(w25ptr,false);
  QPair<Weiche*, bool> ww22tos33w26(w26ptr,false);
  QList<QPair<Weiche*, bool>> ww22tos33;
  ww22tos33.push_back(ww22tos33w25);
  ww22tos33.push_back(ww22tos33w26);
  ww22ptr->addWeichenstatusZuH(s33ptr->getS_id(),ww22tos33);
  
  
  //segment status ----------------------------------------------------------------
  QList<Block*> s1tos4b;
  s1tos4b.push_back(aaptr);
  s1ptr->addBlock(s4ptr,s1tos4b);

  QList<Block*> s2tos42b;
  s2tos42b.push_back(bmptr);
  s2tos42b.push_back(blptr);
  s2tos42b.push_back(cbptr);
  s2ptr->addBlock(s42ptr,s2tos42b);
  QList<Block*> s2tos33b;
  s2tos33b.push_back(bnptr);
  s2tos33b.push_back(bkptr);
  s2tos33b.push_back(beptr);
  s2tos33b.push_back(captr);
  s2ptr->addBlock(s33ptr,s2tos33b);
  QList<Block*> s2tos32b;
  s2tos32b.push_back(bnptr);
  s2tos32b.push_back(bkptr);
  s2tos32b.push_back(bdptr);
  s2tos32b.push_back(captr);
  s2ptr->addBlock(s32ptr,s2tos32b);

  QList<Block*> s3tos42b;
  s3tos42b.push_back(bmptr);
  s3tos42b.push_back(blptr);
  s3tos42b.push_back(cbptr);
  s3ptr->addBlock(s42ptr,s3tos42b);
  
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
  QList<Block*> s4tos50b;
  s4tos50b.push_back(adptr);
  s4tos50b.push_back(bxptr);
  s4ptr->addBlock(s50ptr,s4tos50b);
  QList<Block*> s4tos12b;
  s4tos12b.push_back(adptr);
  s4tos12b.push_back(aiptr);
  s4ptr->addBlock(s12ptr,s4tos12b);

  QList<Block*> s5tos50b;
  s5tos50b.push_back(adptr);
  s5tos50b.push_back(bxptr);
  s5ptr->addBlock(s50ptr,s5tos50b);
  QList<Block*> s5tos12b;
  s5tos12b.push_back(adptr);
  s5tos12b.push_back(aiptr);
  s5ptr->addBlock(s12ptr,s5tos12b);

  QList<Block*> s6tos24b;
  s6tos24b.push_back(auptr);
  s6ptr->addBlock(s24ptr,s6tos24b);

  QList<Block*> s7tos24b;
  s7tos24b.push_back(auptr);
  s7ptr->addBlock(s24ptr,s7tos24b);

  QList<Block*> s49tos3b;
  s49tos3b.push_back(adptr);
  s49tos3b.push_back(abptr);
  s49ptr->addBlock(s3ptr,s49tos3b);
  QList<Block*> s49tos2b;
  s49tos2b.push_back(adptr);
  s49tos2b.push_back(aaptr);
  s49ptr->addBlock(s2ptr,s49tos2b);

  QList<Block*> s10tos28b;
  s10tos28b.push_back(asptr);
  s10tos28b.push_back(atptr);
  s10ptr->addBlock(s28ptr,s10tos28b);
  QList<Block*> s10toww12b;
  s10toww12b.push_back(asptr);
  s10ptr->addBlockZuRangier(ww12ptr,s10toww12b);

  QList<Block*> s12tos28b;
  s12tos28b.push_back(asptr);
  s12tos28b.push_back(atptr);
  s12ptr->addBlock(s28ptr,s12tos28b);
  QList<Block*> s12toww12b;
  s12toww12b.push_back(asptr);
  s12ptr->addBlockZuRangier(ww12ptr,s12toww12b);

  QList<Block*> s11tos3b;
  s11tos3b.push_back(adptr);
  s11tos3b.push_back(abptr);
  s11ptr->addBlock(s3ptr,s11tos3b);
  QList<Block*> s11tos2b;
  s11tos2b.push_back(adptr);
  s11tos2b.push_back(aaptr);
  s11ptr->addBlock(s2ptr,s11tos2b); 

  QList<Block*> ww2tos50b;
  ww2tos50b.push_back(bxptr);
  ww2ptr->addBlockZuH(s50ptr->getS_id(),ww2tos50b);
  QList<Block*> ww2tos12b;
  ww2tos12b.push_back(aiptr);
  ww2ptr->addBlockZuH(s12ptr->getS_id(),ww2tos12b);

  QList<Block*> ww1tos2b;
  ww1tos2b.push_back(aaptr);
  ww1ptr->addBlockZuH(s2ptr->getS_id(),ww1tos2b);
  QList<Block*> ww1tos3b;
  ww1tos3b.push_back(abptr);
  ww1ptr->addBlockZuH(s3ptr->getS_id(),ww1tos3b);

  QList<Block*> s49toww1b;
  s49toww1b.push_back(adptr);
  s49ptr->addBlockZuRangier(ww1ptr,s49toww1b);
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
  QList<Block*> s8tos15b;
  s8tos15b.push_back(aeptr);
  s8tos15b.push_back(agptr);
  s8ptr->addBlock(s15ptr,s8tos15b);

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

  QList<Block*> s24tos27b;
  s24tos27b.push_back(avptr);
  s24ptr->addBlock(s27ptr,s24tos27b);
  
  QList<Block*> s25tos9b;
  s25tos9b.push_back(asptr);
  s25tos9b.push_back(ahptr);
  s25ptr->addBlock(s9ptr,s25tos9b);

  QList<Block*> s25tos11b;
  s25tos11b.push_back(asptr);
  s25tos11b.push_back(aiptr);
  s25ptr->addBlock(s11ptr,s25tos11b);

  QList<Block*> s26tos38b;
  s26tos38b.push_back(bcptr);
  s26ptr->addBlock(s38ptr,s26tos38b);

  QList<Block*> s29tos25b;
  s29tos25b.push_back(atptr);
  s29ptr->addBlock(s25ptr,s29tos25b);

  QList<Block*> s30tos25b;
  s30tos25b.push_back(atptr);
  s30ptr->addBlock(s25ptr,s30tos25b);
  QList<Block*> s30tos8b;
  s30tos8b.push_back(avptr);
  s30tos8b.push_back(auptr);
  s30ptr->addBlock(s8ptr,s30tos8b); 

  QList<Block*> s27tos39b;
  s27tos39b.push_back(axptr);
  s27ptr->addBlock(s39ptr,s27tos39b);

  QList<Block*> s28tos39b;
  s28tos39b.push_back(axptr);
  s28ptr->addBlock(s39ptr,s28tos39b);
  QList<Block*> s28tos46b;
  s28tos46b.push_back(awptr);
  s28tos46b.push_back(ayptr);
  s28ptr->addBlock(s46ptr,s28tos46b);

  QList<Block*> s39tos48b;
  s39tos48b.push_back(azptr);
  s39ptr->addBlock(s48ptr,s39tos48b);

  QList<Block*> s41tos30b;
  s41tos30b.push_back(axptr);
  s41ptr->addBlock(s30ptr,s41tos30b);
  QList<Block*> s41tos31b;
  s41tos31b.push_back(bcptr);
  s41ptr->addBlock(s31ptr,s41tos31b);

  QList<Block*> s40tos29b;
  s40tos29b.push_back(awptr);
  s40ptr->addBlock(s29ptr,s40tos29b);

  QList<Block*> s45tos40b;
  s45tos40b.push_back(ayptr);
  s45ptr->addBlock(s40ptr,s45tos40b);

  QList<Block*> s47tos41b;
  s47tos41b.push_back(azptr);
  s47ptr->addBlock(s41ptr,s47tos41b);

  QList<Block*> s31tos45b;
  s31tos45b.push_back(bbptr);
  s31ptr->addBlock(s45ptr,s31tos45b);
  QList<Block*> s31tos43b;
  s31tos43b.push_back(boptr);
  s31ptr->addBlock(s43ptr,s31tos43b);

  QList<Block*> s32tos47b;
  s32tos47b.push_back(baptr);
  s32ptr->addBlock(s47ptr,s32tos47b);

  QList<Block*> s33tos47b;
  s33tos47b.push_back(baptr);
  s33ptr->addBlock(s47ptr,s33tos47b);

  QList<Block*> s34tos47b;
  s34tos47b.push_back(baptr);
  s34ptr->addBlock(s47ptr,s34tos47b);
  QList<Block*> s34toww14b;
  s34toww14b.push_back(bgptr);
  s34ptr->addBlockZuRangier(ww14ptr,s34toww14b);

  QList<Block*> s35toww19b;
  s35toww19b.push_back(bhptr);
  s35ptr->addBlockZuRangier(ww19ptr,s35toww19b);
  QList<Block*> s35toww17b;
  s35toww17b.push_back(biptr);
  s35ptr->addBlockZuRangier(ww17ptr,s35toww17b);

  QList<Block*> s36tos1b;
  s36tos1b.push_back(bkptr);
  s36tos1b.push_back(bnptr);
  s36tos1b.push_back(captr);
  s36ptr->addBlock(s1ptr,s36tos1b);
  QList<Block*> s36tos26b;
  s36tos26b.push_back(bkptr);
  s36tos26b.push_back(boptr);
  s36tos26b.push_back(captr);
  s36ptr->addBlock(s26ptr,s36tos26b);
  QList<Block*> s36toww23b;
  s36toww23b.push_back(bkptr);
  s36toww23b.push_back(captr);
  s36ptr->addBlockZuRangier(ww23ptr,s36toww23b);
  QList<Block*> s36toww20b;
  s36toww20b.push_back(bjptr);
  s36ptr->addBlockZuRangier(ww20ptr,s36toww20b);

  QList<Block*> s37tos1b;
  s37tos1b.push_back(bkptr);
  s37tos1b.push_back(bnptr);
  s37tos1b.push_back(captr);
  s37ptr->addBlock(s1ptr,s37tos1b);
  QList<Block*> s37tos26b;
  s37tos26b.push_back(bkptr);
  s37tos26b.push_back(boptr);
  s37tos26b.push_back(captr);
  s37ptr->addBlock(s26ptr,s37tos26b);
  QList<Block*> s37toww23b;
  s37toww23b.push_back(bkptr);
  s37toww23b.push_back(captr);
  s37ptr->addBlockZuRangier(ww23ptr,s37toww23b);

  QList<Block*> s38tos44b;
  s38tos44b.push_back(blptr);
  s38tos44b.push_back(cbptr);
  s38ptr->addBlock(s44ptr,s38tos44b);
  QList<Block*> s38tos48b;
  s38tos48b.push_back(azptr);
  s38ptr->addBlock(s48ptr,s38tos48b);

  QList<Block*> s42tos31b;
  s42tos31b.push_back(bcptr);
  s42ptr->addBlock(s31ptr,s42tos31b);

  QList<Block*> s43tos32b;
  s43tos32b.push_back(bkptr);
  s43tos32b.push_back(bdptr);
  s43tos32b.push_back(captr);
  s43ptr->addBlock(s32ptr,s43tos32b);
  QList<Block*> s43tos33b;
  s43tos33b.push_back(bkptr);
  s43tos33b.push_back(beptr);
  s43ptr->addBlock(s33ptr,s43tos33b);
  QList<Block*> s43toww22b;
  s43toww22b.push_back(bkptr);
  s43toww22b.push_back(captr);
  s43ptr->addBlockZuRangier(ww22ptr,s43toww22b);

  QList<Block*> s44tos4b;
  s44tos4b.push_back(bmptr);
  s44tos4b.push_back(aaptr);
  s44ptr->addBlock(s4ptr,s44tos4b);
  QList<Block*> s44tos5b;
  s44tos5b.push_back(bmptr);
  s44tos5b.push_back(abptr);
  s44ptr->addBlock(s5ptr,s44tos5b);

  QList<Block*> s46tos38b;
  s46tos38b.push_back(bbptr);
  s46tos38b.push_back(bcptr);
  s46ptr->addBlock(s38ptr,s46tos38b);

  QList<Block*> s48tos35b;
  s48tos35b.push_back(baptr);
  s48tos35b.push_back(bfptr);
  s48ptr->addBlock(s35ptr,s48tos35b);
  QList<Block*> s48tos36b;
  s48tos36b.push_back(baptr);
  s48tos36b.push_back(beptr);
  s48ptr->addBlock(s36ptr,s48tos36b);
  QList<Block*> s48tos37b;
  s48tos37b.push_back(baptr);
  s48tos37b.push_back(bdptr);
  s48ptr->addBlock(s37ptr,s48tos37b);

  QList<Block*> ww13tos37b;
  ww13tos37b.push_back(bdptr);
  ww13ptr->addBlockZuH(s37ptr->getS_id(),ww13tos37b);
  QList<Block*> ww13tos36b;
  ww13tos36b.push_back(beptr);
  ww13ptr->addBlockZuH(s36ptr->getS_id(),ww13tos36b);
  QList<Block*> ww13tos35b;
  ww13tos35b.push_back(bfptr);
  ww13ptr->addBlockZuH(s35ptr->getS_id(),ww13tos35b);

  QList<Block*> ww15tos35b;
  ww15tos35b.push_back(bfptr);
  ww15ptr->addBlockZuH(s35ptr->getS_id(),ww15tos35b);

  QList<Block*> ww18tos34b;
  ww18tos34b.push_back(bfptr);
  ww18ptr->addBlockZuH(s34ptr->getS_id(),ww18tos34b);

  QList<Block*> ww16tos34b;
  ww16tos34b.push_back(bfptr);
  ww16ptr->addBlockZuH(s34ptr->getS_id(),ww16tos34b);

  QList<Block*> ww21tos33b;
  ww21tos33b.push_back(beptr);
  ww21ptr->addBlockZuH(s33ptr->getS_id(),ww21tos33b);

  QList<Block*> ww22tos33b;
  ww22tos33b.push_back(beptr);
  ww22ptr->addBlockZuH(s33ptr->getS_id(),ww22tos33b);
  QList<Block*> ww22tos32b;
  ww22tos32b.push_back(bdptr);
  ww22ptr->addBlockZuH(s32ptr->getS_id(),ww22tos32b);

  // first initalization for stellwekstechnik, necessary! ----------------------------
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
  stellwerkstecptr->add_Signal(s24ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s25ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s26ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s27ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s28ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s29ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s30ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s31ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s32ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s33ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s34ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s35ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s36ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s37ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s38ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s39ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s40ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s41ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s42ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s43ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s44ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s45ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s46ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s47ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s48ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s49ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s50ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s51ptr->getS_id(),false);
  stellwerkstecptr->add_Signal(s52ptr->getS_id(),false);
  
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
  stellwerkstecptr->add_Signal(ww12ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww13ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww14ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww15ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww16ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww17ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww18ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww19ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww20ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww21ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww22ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww23ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww24ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww25ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww26ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww27ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww28ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww29ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww30ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww31ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww32ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww33ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww34ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww35ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww36ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww37ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww38ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww39ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww40ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww41ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww42ptr->getV_id(),false);
  stellwerkstecptr->add_Signal(ww43ptr->getV_id(),false);

  //BÜs
  BU bu1(1, config);
  BU *bu1ptr = &bu1;
  pinMode(24, OUTPUT);
  
  QGraphicsRectItem *bue = new QGraphicsRectItem();
  bue->setRect(0,0,140,35);
  bue->setPos(QPointF(2310,200));
  bue->setBrush(QColor(Qt::gray));
  bue->setRotation(90);
  scene->addItem(bue);
  bu1ptr->addBUrect(bue);
  bu1ptr->setGpio(24);
  bu1ptr->addSegments(captr);
  bu1ptr->addSegments(cbptr);

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
  QObject::connect(s24ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s25ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s26ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s27ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s28ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s29ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s30ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s31ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s32ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s33ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s34ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s35ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s36ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s37ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s38ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s39ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s40ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s41ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s42ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s43ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s44ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s45ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s46ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s47ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s48ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s49ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s50ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s51ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(s52ptr,&HSignal::refreshStellwerkstechnik,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  
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
  QObject::connect(ww12ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww13ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww14ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww15ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww16ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww17ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww18ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww19ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww20ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww21ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww22ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww23ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww24ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww25ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww26ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww27ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww28ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww29ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww30ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww31ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww32ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww33ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww34ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww35ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww36ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww37ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww38ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww39ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww40ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww41ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww42ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  QObject::connect(ww43ptr,&WSignal::refreshStellwerkstechnikW,stellwerkstecptr,&Stellwerkstechnik::add_Signal);
  //structure: --X---->----<signal>   (segement before signal)
  QObject::connect(bnptr,&Block::zugpassiert,s1ptr,&HSignal::zugpassiert);
  QObject::connect(aaptr,&Block::zugpassiert,s2ptr,&HSignal::zugpassiert);
  QObject::connect(abptr,&Block::zugpassiert,s3ptr,&HSignal::zugpassiert);
  QObject::connect(aaptr,&Block::zugpassiert,s4ptr,&HSignal::zugpassiert);
  QObject::connect(abptr,&Block::zugpassiert,s5ptr,&HSignal::zugpassiert);
  QObject::connect(agptr,&Block::zugpassiert,s6ptr,&HSignal::zugpassiert);
  QObject::connect(afptr,&Block::zugpassiert,s7ptr,&HSignal::zugpassiert);
  QObject::connect(auptr,&Block::zugpassiert,s8ptr,&HSignal::zugpassiert);
  QObject::connect(ahptr,&Block::zugpassiert,s10ptr,&HSignal::zugpassiert);
  QObject::connect(aiptr,&Block::zugpassiert,s11ptr,&HSignal::zugpassiert);
  QObject::connect(aiptr,&Block::zugpassiert,s12ptr,&HSignal::zugpassiert);
  QObject::connect(acptr,&Block::zugpassiert,s13ptr,&HSignal::zugpassiert);
  QObject::connect(acptr,&Block::zugpassiert,s14ptr,&HSignal::zugpassiert);
  QObject::connect(aeptr,&Block::zugpassiert,s15ptr,&HSignal::zugpassiert);
  QObject::connect(akptr,&Block::zugpassiert,s16ptr,&HSignal::zugpassiert);
  QObject::connect(amptr,&Block::zugpassiert,s17ptr,&HSignal::zugpassiert);
  QObject::connect(anptr,&Block::zugpassiert,s18ptr,&HSignal::zugpassiert);
  QObject::connect(anptr,&Block::zugpassiert,s19ptr,&HSignal::zugpassiert);
  QObject::connect(amptr,&Block::zugpassiert,s20ptr,&HSignal::zugpassiert);
  QObject::connect(akptr,&Block::zugpassiert,s21ptr,&HSignal::zugpassiert);
  QObject::connect(arptr,&Block::zugpassiert,s22ptr,&HSignal::zugpassiert);
  QObject::connect(arptr,&Block::zugpassiert,s23ptr,&HSignal::zugpassiert);
  QObject::connect(auptr,&Block::zugpassiert,s24ptr,&HSignal::zugpassiert);
  QObject::connect(atptr,&Block::zugpassiert,s25ptr,&HSignal::zugpassiert);
  QObject::connect(boptr,&Block::zugpassiert,s26ptr,&HSignal::zugpassiert);
  QObject::connect(avptr,&Block::zugpassiert,s27ptr,&HSignal::zugpassiert);
  QObject::connect(atptr,&Block::zugpassiert,s28ptr,&HSignal::zugpassiert);
  QObject::connect(awptr,&Block::zugpassiert,s29ptr,&HSignal::zugpassiert);
  QObject::connect(axptr,&Block::zugpassiert,s30ptr,&HSignal::zugpassiert);
  QObject::connect(bcptr,&Block::zugpassiert,s31ptr,&HSignal::zugpassiert);
  QObject::connect(bdptr,&Block::zugpassiert,s32ptr,&HSignal::zugpassiert);
  QObject::connect(beptr,&Block::zugpassiert,s33ptr,&HSignal::zugpassiert);
  QObject::connect(bfptr,&Block::zugpassiert,s34ptr,&HSignal::zugpassiert);
  QObject::connect(bfptr,&Block::zugpassiert,s35ptr,&HSignal::zugpassiert);
  QObject::connect(beptr,&Block::zugpassiert,s36ptr,&HSignal::zugpassiert);
  QObject::connect(bdptr,&Block::zugpassiert,s37ptr,&HSignal::zugpassiert);
  QObject::connect(bcptr,&Block::zugpassiert,s38ptr,&HSignal::zugpassiert);
  QObject::connect(axptr,&Block::zugpassiert,s39ptr,&HSignal::zugpassiert);
  QObject::connect(ayptr,&Block::zugpassiert,s40ptr,&HSignal::zugpassiert);
  QObject::connect(azptr,&Block::zugpassiert,s41ptr,&HSignal::zugpassiert);
  QObject::connect(blptr,&Block::zugpassiert,s42ptr,&HSignal::zugpassiert);
  QObject::connect(boptr,&Block::zugpassiert,s43ptr,&HSignal::zugpassiert);
  QObject::connect(cbptr,&Block::zugpassiert,s44ptr,&HSignal::zugpassiert);
  QObject::connect(bbptr,&Block::zugpassiert,s45ptr,&HSignal::zugpassiert);
  QObject::connect(ayptr,&Block::zugpassiert,s46ptr,&HSignal::zugpassiert);
  QObject::connect(baptr,&Block::zugpassiert,s47ptr,&HSignal::zugpassiert);
  QObject::connect(azptr,&Block::zugpassiert,s48ptr,&HSignal::zugpassiert);
  QObject::connect(bxptr,&Block::zugpassiert,s49ptr,&HSignal::zugpassiert);
  QObject::connect(bxptr,&Block::zugpassiert,s50ptr,&HSignal::zugpassiert);
  QObject::connect(bpptr,&Block::zugpassiert,s51ptr,&HSignal::zugpassiert);
  QObject::connect(bqptr,&Block::zugpassiert,s51ptr,&HSignal::zugpassiert);
  QObject::connect(brptr,&Block::zugpassiert,s51ptr,&HSignal::zugpassiert);
  QObject::connect(bsptr,&Block::zugpassiert,s51ptr,&HSignal::zugpassiert);
  QObject::connect(btptr,&Block::zugpassiert,s51ptr,&HSignal::zugpassiert);
  QObject::connect(buptr,&Block::zugpassiert,s51ptr,&HSignal::zugpassiert);
  QObject::connect(bvptr,&Block::zugpassiert,s51ptr,&HSignal::zugpassiert);
  QObject::connect(bwptr,&Block::zugpassiert,s51ptr,&HSignal::zugpassiert);
  QObject::connect(bwptr,&Block::zugpassiert,s52ptr,&HSignal::zugpassiert);
  QObject::connect(bvptr,&Block::zugpassiert,s52ptr,&HSignal::zugpassiert);
  QObject::connect(buptr,&Block::zugpassiert,s52ptr,&HSignal::zugpassiert);
  QObject::connect(btptr,&Block::zugpassiert,s52ptr,&HSignal::zugpassiert);
  QObject::connect(bsptr,&Block::zugpassiert,s52ptr,&HSignal::zugpassiert);
  QObject::connect(brptr,&Block::zugpassiert,s52ptr,&HSignal::zugpassiert);
  QObject::connect(bqptr,&Block::zugpassiert,s52ptr,&HSignal::zugpassiert);
  QObject::connect(bpptr,&Block::zugpassiert,s52ptr,&HSignal::zugpassiert);
  
  QObject::connect(adptr,&Block::zugpassiertW,ww1ptr,&WSignal::zugpassiertW);
  QObject::connect(adptr,&Block::zugpassiertW,ww2ptr,&WSignal::zugpassiertW);
  QObject::connect(asptr,&Block::zugpassiertW,ww3ptr,&WSignal::zugpassiertW);
  QObject::connect(ajptr,&Block::zugpassiertW,ww4ptr,&WSignal::zugpassiertW);
  QObject::connect(ajptr,&Block::zugpassiertW,ww5ptr,&WSignal::zugpassiertW);
  QObject::connect(aoptr,&Block::zugpassiertW,ww6ptr,&WSignal::zugpassiertW);
  QObject::connect(aeptr,&Block::zugpassiertW,ww7ptr,&WSignal::zugpassiertW);
  QObject::connect(apptr,&Block::zugpassiertW,ww8ptr,&WSignal::zugpassiertW);
  QObject::connect(apptr,&Block::zugpassiertW,ww9ptr,&WSignal::zugpassiertW);
  QObject::connect(aqptr,&Block::zugpassiertW,ww10ptr,&WSignal::zugpassiertW);
  QObject::connect(aqptr,&Block::zugpassiertW,ww11ptr,&WSignal::zugpassiertW);
  QObject::connect(asptr,&Block::zugpassiertW,ww12ptr,&WSignal::zugpassiertW);
  QObject::connect(baptr,&Block::zugpassiertW,ww13ptr,&WSignal::zugpassiertW);
  QObject::connect(bgptr,&Block::zugpassiertW,ww14ptr,&WSignal::zugpassiertW);
  QObject::connect(bgptr,&Block::zugpassiertW,ww15ptr,&WSignal::zugpassiertW);
  QObject::connect(biptr,&Block::zugpassiertW,ww16ptr,&WSignal::zugpassiertW);
  QObject::connect(biptr,&Block::zugpassiertW,ww17ptr,&WSignal::zugpassiertW);
  QObject::connect(bhptr,&Block::zugpassiertW,ww18ptr,&WSignal::zugpassiertW);
  QObject::connect(bhptr,&Block::zugpassiertW,ww19ptr,&WSignal::zugpassiertW);
  QObject::connect(bjptr,&Block::zugpassiertW,ww20ptr,&WSignal::zugpassiertW);
  QObject::connect(bjptr,&Block::zugpassiertW,ww21ptr,&WSignal::zugpassiertW);
  QObject::connect(bkptr,&Block::zugpassiertW,ww22ptr,&WSignal::zugpassiertW);
  QObject::connect(captr,&Block::zugpassiertW,ww23ptr,&WSignal::zugpassiertW);
  QObject::connect(bwptr,&Block::zugpassiertW,ww24ptr,&WSignal::zugpassiertW);
  QObject::connect(bvptr,&Block::zugpassiertW,ww25ptr,&WSignal::zugpassiertW);
  QObject::connect(buptr,&Block::zugpassiertW,ww26ptr,&WSignal::zugpassiertW);
  QObject::connect(btptr,&Block::zugpassiertW,ww27ptr,&WSignal::zugpassiertW);
  QObject::connect(bsptr,&Block::zugpassiertW,ww28ptr,&WSignal::zugpassiertW);
  QObject::connect(brptr,&Block::zugpassiertW,ww29ptr,&WSignal::zugpassiertW);
  QObject::connect(bqptr,&Block::zugpassiertW,ww30ptr,&WSignal::zugpassiertW);
  QObject::connect(bpptr,&Block::zugpassiertW,ww31ptr,&WSignal::zugpassiertW);
  QObject::connect(bpptr,&Block::zugpassiertW,ww32ptr,&WSignal::zugpassiertW);
  QObject::connect(bqptr,&Block::zugpassiertW,ww33ptr,&WSignal::zugpassiertW);
  QObject::connect(brptr,&Block::zugpassiertW,ww34ptr,&WSignal::zugpassiertW);
  QObject::connect(bsptr,&Block::zugpassiertW,ww35ptr,&WSignal::zugpassiertW);
  QObject::connect(btptr,&Block::zugpassiertW,ww36ptr,&WSignal::zugpassiertW);
  QObject::connect(buptr,&Block::zugpassiertW,ww37ptr,&WSignal::zugpassiertW);
  QObject::connect(bvptr,&Block::zugpassiertW,ww38ptr,&WSignal::zugpassiertW);
  QObject::connect(bzptr,&Block::zugpassiertW,ww39ptr,&WSignal::zugpassiertW);
  QObject::connect(byptr,&Block::zugpassiertW,ww40ptr,&WSignal::zugpassiertW);
  QObject::connect(bwptr,&Block::zugpassiertW,ww41ptr,&WSignal::zugpassiertW);
  QObject::connect(byptr,&Block::zugpassiertW,ww42ptr,&WSignal::zugpassiertW);
  QObject::connect(bzptr,&Block::zugpassiertW,ww43ptr,&WSignal::zugpassiertW);
    
  
  //segments which are at a border for signals - structure: -----X--->---<signal>----Y-->---
  //implementation: X->addpassiert(<signal>,Y)
  acptr->addpassiert(s13ptr->getS_id(),aaptr);
  bnptr->addpassiert(s1ptr->getS_id(),aaptr);
  aaptr->addpassiert(s2ptr->getS_id(),bnptr);
  aaptr->addpassiert(s2ptr->getS_id(),bmptr);
  abptr->addpassiert(s3ptr->getS_id(),bmptr);
  aaptr->addpassiert(s4ptr->getS_id(),acptr);
  aaptr->addpassiert(s4ptr->getS_id(),aeptr);
  aaptr->addpassiert(s4ptr->getS_id(),afptr);
  aaptr->addpassiert(s4ptr->getS_id(),adptr);
  abptr->addpassiert(s5ptr->getS_id(),adptr);
  agptr->addpassiert(s6ptr->getS_id(),auptr);
  afptr->addpassiert(s7ptr->getS_id(),auptr);
  ahptr->addpassiert(s9ptr->getS_id(),bzptr);
  auptr->addpassiert(s8ptr->getS_id(),afptr);
  auptr->addpassiert(s8ptr->getS_id(),agptr);
  bxptr->addpassiert(s49ptr->getS_id(),adptr);
  ahptr->addpassiert(s10ptr->getS_id(),asptr);
  aiptr->addpassiert(s12ptr->getS_id(),asptr);
  aiptr->addpassiert(s11ptr->getS_id(),adptr);
  acptr->addpassiert(s14ptr->getS_id(),ajptr);
  aeptr->addpassiert(s15ptr->getS_id(),ajptr);
  akptr->addpassiert(s16ptr->getS_id(),ajptr);
  amptr->addpassiert(s17ptr->getS_id(),alptr);
  anptr->addpassiert(s18ptr->getS_id(),alptr);
  anptr->addpassiert(s19ptr->getS_id(),arptr);
  amptr->addpassiert(s20ptr->getS_id(),arptr);
  amptr->addpassiert(s20ptr->getS_id(),aqptr);
  akptr->addpassiert(s21ptr->getS_id(),arptr);
  arptr->addpassiert(s22ptr->getS_id(),akptr);
  arptr->addpassiert(s23ptr->getS_id(),anptr);
  arptr->addpassiert(s23ptr->getS_id(),amptr);
  arptr->addpassiert(s23ptr->getS_id(),akptr);
  auptr->addpassiert(s24ptr->getS_id(),avptr);
  atptr->addpassiert(s25ptr->getS_id(),asptr);
  boptr->addpassiert(s26ptr->getS_id(),bcptr);
  avptr->addpassiert(s27ptr->getS_id(),axptr);
  atptr->addpassiert(s28ptr->getS_id(),axptr);
  atptr->addpassiert(s28ptr->getS_id(),awptr);
  awptr->addpassiert(s29ptr->getS_id(),atptr);
  axptr->addpassiert(s30ptr->getS_id(),atptr);
  axptr->addpassiert(s30ptr->getS_id(),avptr);
  bcptr->addpassiert(s31ptr->getS_id(),boptr);
  bcptr->addpassiert(s31ptr->getS_id(),bbptr);
  bdptr->addpassiert(s32ptr->getS_id(),baptr);
  beptr->addpassiert(s33ptr->getS_id(),baptr);
  bfptr->addpassiert(s34ptr->getS_id(),baptr);
  bfptr->addpassiert(s34ptr->getS_id(),bgptr);
  bfptr->addpassiert(s35ptr->getS_id(),biptr);
  bfptr->addpassiert(s35ptr->getS_id(),bhptr);
  beptr->addpassiert(s36ptr->getS_id(),bjptr);
  beptr->addpassiert(s36ptr->getS_id(),bkptr);
  bdptr->addpassiert(s37ptr->getS_id(),bkptr);
  bcptr->addpassiert(s38ptr->getS_id(),blptr);
  bcptr->addpassiert(s38ptr->getS_id(),azptr);
  axptr->addpassiert(s39ptr->getS_id(),azptr);
  ayptr->addpassiert(s40ptr->getS_id(),awptr);
  azptr->addpassiert(s41ptr->getS_id(),axptr);
  azptr->addpassiert(s41ptr->getS_id(),bcptr);
  blptr->addpassiert(s42ptr->getS_id(),bcptr);
  boptr->addpassiert(s43ptr->getS_id(),bkptr);
  blptr->addpassiert(s44ptr->getS_id(),bmptr);
  bbptr->addpassiert(s45ptr->getS_id(),ayptr);
  ayptr->addpassiert(s46ptr->getS_id(),bbptr);
  baptr->addpassiert(s47ptr->getS_id(),azptr);
  azptr->addpassiert(s48ptr->getS_id(),baptr);
  bxptr->addpassiert(s50ptr->getS_id(),byptr);
  bpptr->addpassiert(s51ptr->getS_id(),byptr);
  bqptr->addpassiert(s51ptr->getS_id(),byptr);
  brptr->addpassiert(s51ptr->getS_id(),byptr);
  bsptr->addpassiert(s51ptr->getS_id(),byptr);
  btptr->addpassiert(s51ptr->getS_id(),byptr);
  buptr->addpassiert(s51ptr->getS_id(),byptr);
  bvptr->addpassiert(s51ptr->getS_id(),byptr);
  bwptr->addpassiert(s51ptr->getS_id(),byptr);
  bpptr->addpassiert(s52ptr->getS_id(),bzptr);
  bqptr->addpassiert(s52ptr->getS_id(),bzptr);
  brptr->addpassiert(s52ptr->getS_id(),bzptr);
  bsptr->addpassiert(s52ptr->getS_id(),bzptr);
  btptr->addpassiert(s52ptr->getS_id(),bzptr);
  buptr->addpassiert(s52ptr->getS_id(),bzptr);
  bvptr->addpassiert(s52ptr->getS_id(),bzptr);
  bwptr->addpassiert(s52ptr->getS_id(),bzptr);

  adptr->addpassiert(ww2ptr->getV_id(),bxptr);
  adptr->addpassiert(ww2ptr->getV_id(),aiptr);
  adptr->addpassiert(ww1ptr->getV_id(),aaptr);
  adptr->addpassiert(ww1ptr->getV_id(),abptr);
  asptr->addpassiert(ww3ptr->getV_id(),aiptr);
  asptr->addpassiert(ww3ptr->getV_id(),ahptr);
  ajptr->addpassiert(ww5ptr->getV_id(),alptr);
  ajptr->addpassiert(ww5ptr->getV_id(),akptr);
  aoptr->addpassiert(ww6ptr->getV_id(),apptr);
  aoptr->addpassiert(ww6ptr->getV_id(),anptr);
  apptr->addpassiert(ww8ptr->getV_id(),aoptr);
  apptr->addpassiert(ww8ptr->getV_id(),alptr);
  aqptr->addpassiert(ww10ptr->getV_id(),amptr);
  aqptr->addpassiert(ww10ptr->getV_id(),akptr);
  asptr->addpassiert(ww12ptr->getV_id(),atptr);
  baptr->addpassiert(ww13ptr->getV_id(),bdptr);
  baptr->addpassiert(ww13ptr->getV_id(),beptr);
  baptr->addpassiert(ww13ptr->getV_id(),bfptr);
  bgptr->addpassiert(ww15ptr->getV_id(),bfptr);
  biptr->addpassiert(ww16ptr->getV_id(),bfptr);
  bhptr->addpassiert(ww18ptr->getV_id(),bfptr);
  bjptr->addpassiert(ww21ptr->getV_id(),beptr);
  bkptr->addpassiert(ww22ptr->getV_id(),beptr);
  bkptr->addpassiert(ww22ptr->getV_id(),bdptr);
  bkptr->addpassiert(ww23ptr->getV_id(),boptr);
  bkptr->addpassiert(ww23ptr->getV_id(),bnptr);
  bwptr->addpassiert(ww24ptr->getV_id(),bzptr);
  bvptr->addpassiert(ww25ptr->getV_id(),bzptr);
  buptr->addpassiert(ww26ptr->getV_id(),bzptr);
  btptr->addpassiert(ww27ptr->getV_id(),bzptr);
  bsptr->addpassiert(ww28ptr->getV_id(),bzptr);
  brptr->addpassiert(ww29ptr->getV_id(),bzptr);
  bqptr->addpassiert(ww30ptr->getV_id(),brptr);
  bpptr->addpassiert(ww31ptr->getV_id(),bzptr);
  bpptr->addpassiert(ww32ptr->getV_id(),byptr);
  bqptr->addpassiert(ww33ptr->getV_id(),byptr);
  brptr->addpassiert(ww34ptr->getV_id(),byptr);
  bsptr->addpassiert(ww35ptr->getV_id(),byptr);
  btptr->addpassiert(ww36ptr->getV_id(),byptr);
  buptr->addpassiert(ww37ptr->getV_id(),byptr);
  bvptr->addpassiert(ww38ptr->getV_id(),byptr);
  bwptr->addpassiert(ww41ptr->getV_id(),byptr);
  bzptr->addpassiert(ww39ptr->getV_id(),bpptr);
  bzptr->addpassiert(ww39ptr->getV_id(),bqptr);
  bzptr->addpassiert(ww39ptr->getV_id(),brptr);
  bzptr->addpassiert(ww39ptr->getV_id(),bsptr);
  bzptr->addpassiert(ww39ptr->getV_id(),btptr);
  bzptr->addpassiert(ww39ptr->getV_id(),buptr);
  bzptr->addpassiert(ww39ptr->getV_id(),bvptr);
  bzptr->addpassiert(ww39ptr->getV_id(),bwptr);
  byptr->addpassiert(ww40ptr->getV_id(),bwptr);
  byptr->addpassiert(ww40ptr->getV_id(),bvptr);
  byptr->addpassiert(ww40ptr->getV_id(),buptr);
  byptr->addpassiert(ww40ptr->getV_id(),btptr);
  byptr->addpassiert(ww40ptr->getV_id(),bsptr);
  byptr->addpassiert(ww40ptr->getV_id(),brptr);
  byptr->addpassiert(ww40ptr->getV_id(),bqptr);
  byptr->addpassiert(ww40ptr->getV_id(),bpptr);
  byptr->addpassiert(ww42ptr->getV_id(),bxptr);
  bzptr->addpassiert(ww43ptr->getV_id(),ahptr);
  
  //
  //GUI attributes

  //markings
  QGraphicsRectItem *acae = new QGraphicsRectItem();
  acae->setRect(0,0,140,50);
  acae->setPos(QPointF(-595,335));
  acae->setBrush(QColor(183,84,64));
  acae->setRotation(90);
  scene->addItem(acae);

  QGraphicsRectItem *acae2 = new QGraphicsRectItem();
  acae2->setRect(0,0,140,50);
  acae2->setPos(QPointF(-260,-640));
  acae2->setBrush(QColor(183,84,64));
  acae2->setRotation(90);
  scene->addItem(acae2);
  
  QGraphicsRectItem *by = new QGraphicsRectItem();
  by->setRect(0,0,50,50);
  by->setPos(QPointF(322,155));
  by->setBrush(QColor(64,104,183));
  scene->addItem(by);

  QGraphicsRectItem *by2 = new QGraphicsRectItem();
  by2->setRect(0,0,50,50);
  by2->setPos(QPointF(453,1033));
  by2->setBrush(QColor(64,104,183));
  scene->addItem(by2);

  QGraphicsRectItem *bz = new QGraphicsRectItem();
  bz->setRect(0,0,50,50);
  bz->setPos(QPointF(-1095,180));
  bz->setBrush(QColor(107,64,183));
  scene->addItem(bz);
  
  QGraphicsRectItem *bz2 = new QGraphicsRectItem();
  bz2->setRect(0,0,50,50);
  bz2->setPos(QPointF(-1095,1033));
  bz2->setBrush(QColor(107,64,183));
  scene->addItem(bz2);
  
  
  QGraphicsRectItem *w1rectge = new QGraphicsRectItem();
  w1rectge->setRect(0,0,10,75);
  w1rectge->setPos(QPointF(500,880));
  w1rectge->setBrush(QColor(79,79,79));
  w1rectge->setRotation(90);
  scene->addItem(w1rectge);
  QGraphicsRectItem *w1rectab = new QGraphicsRectItem();
  w1rectab->setRect(0,0,10,50);
  w1rectab->setPos(QPointF(500,880));
  w1rectab->setBrush(QColor(79,79,79));
  w1rectab->setRotation(135);
  scene->addItem(w1rectab);
  QLabel *w1label = new QLabel();
  scene->addWidget(w1label);
  WeicheQPushButton *w1push = new WeicheQPushButton();
  scene->addWidget(w1push);
  w1ptr->addWeichenitem(w1rectab,w1rectge,w1label,w1push);
  w1ptr->moveButton(480,875);
  w1ptr->moveLabel(480,890);
  
  QGraphicsRectItem *abrect = new QGraphicsRectItem();
  abrect->setRect(0,0,10,1200);
  abrect->setPos(QPointF(425,880));
  abrect->setBrush(QColor(79,79,79));
  abrect->setRotation(90);
  abptr->addBlockitems(abrect);
  scene->addItem(abrect);
  //
  QGraphicsRectItem *w2rectge = new QGraphicsRectItem();
  w2rectge->setRect(0,0,10,75);
  w2rectge->setPos(QPointF(500,810));
  w2rectge->setBrush(QColor(79,79,79));
  w2rectge->setRotation(90);
  scene->addItem(w2rectge);
  QGraphicsRectItem *w2rectab = new QGraphicsRectItem();
  w2rectab->setRect(0,0,10,50);
  w2rectab->setPos(QPointF(425,820));
  w2rectab->setBrush(QColor(79,79,79));
  w2rectab->setRotation(315);
  scene->addItem(w2rectab);
  QLabel *w2label = new QLabel();
  scene->addWidget(w2label);
  WeicheQPushButton *w2push = new WeicheQPushButton();
  scene->addWidget(w2push);
  w2ptr->addWeichenitem(w2rectab,w2rectge,w2label,w2push);
  w2ptr->moveButton(425,805);
  w2ptr->moveLabel(435,795);

  QGraphicsRectItem *aarect = new QGraphicsRectItem();
  aarect->setRect(0,0,10,1200);
  aarect->setPos(QPointF(425,810));
  aarect->setBrush(QColor(79,79,79));
  aarect->setRotation(90);
  aaptr->addBlockitems(aarect);
  scene->addItem(aarect);

  QGraphicsRectItem *bnrect = new QGraphicsRectItem();
  bnrect->setRect(0,0,10,1200);
  bnrect->setPos(QPointF(1720,810));
  bnrect->setBrush(QColor(79,79,79));
  bnrect->setRotation(90);
  bnptr->addBlockitems(bnrect);
  scene->addItem(bnrect);

  QGraphicsRectItem *bmrect = new QGraphicsRectItem();
  bmrect->setRect(0,0,10,1300);
  bmrect->setPos(QPointF(1800,880));
  bmrect->setBrush(QColor(79,79,79));
  bmrect->setRotation(90);
  bmptr->addBlockitems(bmrect);
  scene->addItem(bmrect);

  QGraphicsRectItem *w4rectge = new QGraphicsRectItem();
  w4rectge->setRect(0,0,10,75);
  w4rectge->setPos(QPointF(-775,880));
  w4rectge->setBrush(QColor(79,79,79));
  w4rectge->setRotation(90);
  scene->addItem(w4rectge);
  QGraphicsRectItem *w4rectab = new QGraphicsRectItem();
  w4rectab->setRect(0,0,10,50);
  w4rectab->setPos(QPointF(-845,890));
  w4rectab->setBrush(QColor(79,79,79));
  w4rectab->setRotation(-135);
  scene->addItem(w4rectab);
  QLabel *w4label = new QLabel();
  scene->addWidget(w4label);

  QGraphicsRectItem *w3rectge = new QGraphicsRectItem();
  w3rectge->setRect(0,0,10,75);
  w3rectge->setPos(QPointF(-775,810));
  w3rectge->setBrush(QColor(79,79,79));
  w3rectge->setRotation(90);
  scene->addItem(w3rectge);
  QGraphicsRectItem *w3rectab = new QGraphicsRectItem();
  w3rectab->setRect(0,0,10,50);
  w3rectab->setPos(QPointF(-785,815));
  w3rectab->setBrush(QColor(79,79,79));
  w3rectab->setRotation(45);
  scene->addItem(w3rectab);
  QLabel *w3label = new QLabel();
  scene->addWidget(w3label);
  WeicheQPushButton *w3push = new WeicheQPushButton();
  scene->addWidget(w3push);
  w3ptr->addWeichenitem(w3rectab,w3rectge,w3label,w3push);
  w3ptr->moveButton(-795,805);
  w3ptr->moveLabel(-795,795);

  QGraphicsRectItem *w5rectge = new QGraphicsRectItem();
  w5rectge->setRect(0,0,10,75);
  w5rectge->setPos(QPointF(-850,810));
  w5rectge->setBrush(QColor(79,79,79));
  w5rectge->setRotation(90);
  scene->addItem(w5rectge);
  QGraphicsRectItem *w5rectab = new QGraphicsRectItem();
  w5rectab->setRect(0,0,10,85);
  w5rectab->setPos(QPointF(-850,815));
  w5rectab->setBrush(QColor(79,79,79));
  w5rectab->setRotation(135);
  scene->addItem(w5rectab);
  QLabel *w5label = new QLabel();
  scene->addWidget(w5label);
  WeicheQPushButton *w5push = new WeicheQPushButton();
  scene->addWidget(w5push);
  w5ptr->addWeichenitem(w5rectab,w5rectge,w5label,w5push);
  w5ptr->moveButton(-865,805);
  w5ptr->moveLabel(-855,795);

  QGraphicsRectItem *w6rectge = new QGraphicsRectItem();
  w6rectge->setRect(0,0,10,100);
  w6rectge->setPos(QPointF(-918,655));
  w6rectge->setBrush(QColor(79,79,79));
  scene->addItem(w6rectge);
  QGraphicsRectItem *w6rectab = new QGraphicsRectItem();
  w6rectab->setRect(0,0,10,50);
  w6rectab->setPos(QPointF(-918,660));
  w6rectab->setBrush(QColor(79,79,79));
  w6rectab->setRotation(-45);
  scene->addItem(w6rectab);
  QLabel *w6label = new QLabel();
  scene->addWidget(w6label);
  WeicheQPushButton *w6push = new WeicheQPushButton();
  scene->addWidget(w6push);
  w6ptr->addWeichenitem(w6rectab,w6rectge,w6label,w6push);
  w6ptr->moveButton(-922,655);
  w6ptr->moveLabel(-938,658);

  QGraphicsRectItem *w7rectge = new QGraphicsRectItem();
  w7rectge->setRect(0,0,10,150);
  w7rectge->setPos(QPointF(-918,505));
  w7rectge->setBrush(QColor(79,79,79));
  scene->addItem(w7rectge);
  QGraphicsRectItem *w7rectab = new QGraphicsRectItem();
  w7rectab->setRect(0,0,10,50);
  w7rectab->setPos(QPointF(-910,610));
  w7rectab->setBrush(QColor(79,79,79));
  w7rectab->setRotation(-135);
  scene->addItem(w7rectab);
  QLabel *w7label = new QLabel();
  scene->addWidget(w7label);
  WeicheQPushButton *w7push = new WeicheQPushButton();
  scene->addWidget(w7push);
  w7ptr->addWeichenitem(w7rectab,w7rectge,w7label,w7push);
  w7ptr->moveButton(-922,590);
  w7ptr->moveLabel(-938,588);

  QGraphicsRectItem *aerect = new QGraphicsRectItem();
  aerect->setRect(0,0,10,70);
  aerect->setPos(QPointF(-918,455));
  aerect->setBrush(QColor(79,79,79));
  aeptr->addBlockitems(aerect);
  scene->addItem(aerect);
  QGraphicsRectItem *aerect2 = new QGraphicsRectItem();
  aerect2->setRect(0,0,10,305);
  aerect2->setPos(QPointF(-918,455));
  aerect2->setBrush(QColor(79,79,79));
  aerect2->setRotation(-90);
  aeptr->addBlockitems(aerect2);
  scene->addItem(aerect2);

  QGraphicsRectItem *acrect = new QGraphicsRectItem();
  acrect->setRect(0,0,10,70);
  acrect->setPos(QPointF(-925,810));
  acrect->setBrush(QColor(79,79,79));
  acrect->setRotation(90);
  acptr->addBlockitems(acrect);
  scene->addItem(acrect);
  QGraphicsRectItem *acrect2 = new QGraphicsRectItem();
  acrect2->setRect(0,0,10,450);
  acrect2->setPos(QPointF(-995,370));
  acrect2->setBrush(QColor(79,79,79));
  acptr->addBlockitems(acrect2);
  scene->addItem(acrect2);
  QGraphicsRectItem *acrect3 = new QGraphicsRectItem();
  acrect3->setRect(0,0,10,380);
  acrect3->setPos(QPointF(-995,370));
  acrect3->setBrush(QColor(79,79,79));
  acrect3->setRotation(-90);
  acptr->addBlockitems(acrect3);
  scene->addItem(acrect3);

  QGraphicsRectItem *adrect = new QGraphicsRectItem();
  adrect->setRect(0,0,10,225);
  adrect->setPos(QPointF(-850,880));
  adrect->setBrush(QColor(79,79,79));
  adrect->setRotation(90);
  adptr->addBlockitems(adrect);
  scene->addItem(adrect);
  QGraphicsRectItem *adrect2 = new QGraphicsRectItem();
  adrect2->setRect(0,0,10,600);
  adrect2->setPos(QPointF(-1075,280));
  adrect2->setBrush(QColor(79,79,79));
  adptr->addBlockitems(adrect2);
  scene->addItem(adrect2);
  QGraphicsRectItem *adrect3 = new QGraphicsRectItem();
  adrect3->setRect(0,0,10,140);
  adrect3->setPos(QPointF(-1075,280));
  adrect3->setBrush(QColor(79,79,79));
  adrect3->setRotation(-90);
  adptr->addBlockitems(adrect3);
  scene->addItem(adrect3);

  WeicheQPushButton *w4push = new WeicheQPushButton();
  scene->addWidget(w4push);
  w4ptr->addWeichenitem(w4rectab,w4rectge,w4label,w4push);
  w4ptr->moveButton(-855,875);
  w4ptr->moveLabel(-845,890);
  
  QGraphicsRectItem *w9rectge = new QGraphicsRectItem();
  w9rectge->setRect(0,0,10,75);
  w9rectge->setPos(QPointF(-935,280));
  w9rectge->setBrush(QColor(79,79,79));
  w9rectge->setRotation(-90);
  scene->addItem(w9rectge);
  QGraphicsRectItem *w9rectab = new QGraphicsRectItem();
  w9rectab->setRect(0,0,10,50);
  w9rectab->setPos(QPointF(-935,280));
  w9rectab->setBrush(QColor(79,79,79));
  w9rectab->setRotation(-135);
  scene->addItem(w9rectab);
  QLabel *w9label = new QLabel();
  scene->addWidget(w9label);
  WeicheQPushButton *w9push = new WeicheQPushButton();
  scene->addWidget(w9push);
  w9ptr->addWeichenitem(w9rectab,w9rectge,w9label,w9push);
  w9ptr->moveButton(-940,265);
  w9ptr->moveLabel(-935,280);

  QGraphicsRectItem *airect = new QGraphicsRectItem();
  airect->setRect(0,0,10,1000);
  airect->setPos(QPointF(-860,280));
  airect->setBrush(QColor(79,79,79));
  airect->setRotation(-90);
  aiptr->addBlockitems(airect);
  scene->addItem(airect);

  QGraphicsRectItem *bzrect = new QGraphicsRectItem();
  bzrect->setRect(0,0,10,105);
  bzrect->setPos(QPointF(-1075,210));
  bzrect->setBrush(QColor(79,79,79));
  bzrect->setRotation(-90);
  bzptr->addBlockitems(bzrect);
  scene->addItem(bzrect);

  QGraphicsRectItem *ahrect2 = new QGraphicsRectItem();
  ahrect2->setRect(0,0,10,1120);
  ahrect2->setPos(QPointF(-980,210));
  ahrect2->setBrush(QColor(79,79,79));
  ahrect2->setRotation(-90);
  ahptr->addBlockitems(ahrect2);
  scene->addItem(ahrect2);
  QGraphicsRectItem *ahrect3 = new QGraphicsRectItem();
  ahrect3->setRect(0,0,10,55);
  ahrect3->setPos(QPointF(140,210));
  ahrect3->setBrush(QColor(79,79,79));
  ahrect3->setRotation(-45);
  ahptr->addBlockitems(ahrect3);
  scene->addItem(ahrect3);

  QGraphicsRectItem *bxrect = new QGraphicsRectItem();
  bxrect->setRect(0,0,10,913);
  bxrect->setPos(QPointF(-788,130));
  bxrect->setBrush(QColor(79,79,79));
  bxrect->setRotation(-90);
  bxptr->addBlockitems(bxrect);
  scene->addItem(bxrect);
  QGraphicsRectItem *bxrect2 = new QGraphicsRectItem();
  bxrect2->setRect(0,0,10,166);
  bxrect2->setPos(QPointF(-788,120));
  bxrect2->setBrush(QColor(79,79,79));
  bxrect2->setRotation(45);
  bxptr->addBlockitems(bxrect2);
  scene->addItem(bxrect2);

  QGraphicsRectItem *byrect = new QGraphicsRectItem();
  byrect->setRect(0,0,10,217);
  byrect->setPos(QPointF(125,130));
  byrect->setBrush(QColor(79,79,79));
  byrect->setRotation(-90);
  byptr->addBlockitems(byrect);
  scene->addItem(byrect);
  QGraphicsRectItem *byrect2 = new QGraphicsRectItem();
  byrect2->setRect(0,0,10,70);
  byrect2->setPos(QPointF(342,120));
  byrect2->setBrush(QColor(79,79,79));
  byptr->addBlockitems(byrect2);
  scene->addItem(byrect2);
  
  
  QGraphicsRectItem *asrect = new QGraphicsRectItem();
  asrect->setRect(0,0,10,385);
  asrect->setPos(QPointF(215,280));
  asrect->setBrush(QColor(79,79,79));
  asrect->setRotation(-90);
  asptr->addBlockitems(asrect);
  scene->addItem(asrect);

  QGraphicsRectItem *w10rectge = new QGraphicsRectItem();
  w10rectge->setRect(0,0,10,75);
  w10rectge->setPos(QPointF(215,270));
  w10rectge->setBrush(QColor(79,79,79));
  w10rectge->setRotation(90);
  scene->addItem(w10rectge);
  QGraphicsRectItem *w10rectab = new QGraphicsRectItem();
  w10rectab->setRect(0,0,10,50);
  w10rectab->setPos(QPointF(215,270));
  w10rectab->setBrush(QColor(79,79,79));
  w10rectab->setRotation(135);
  scene->addItem(w10rectab);
  QLabel *w10label = new QLabel();
  scene->addWidget(w10label);
  WeicheQPushButton *w10push = new WeicheQPushButton();
  scene->addWidget(w10push);
  w10ptr->addWeichenitem(w10rectab,w10rectge,w10label,w10push);
  w10ptr->moveButton(200,265);
  w10ptr->moveLabel(200,280);

  QGraphicsRectItem *afrect = new QGraphicsRectItem();
  afrect->setRect(0,0,10,675);
  afrect->setPos(QPointF(-875,575));
  afrect->setBrush(QColor(79,79,79));
  afrect->setRotation(-90);
  afptr->addBlockitems(afrect);
  scene->addItem(afrect);

  QGraphicsRectItem *aurect = new QGraphicsRectItem();
  aurect->setRect(0,0,10,795);
  aurect->setPos(QPointF(-125,575));
  aurect->setBrush(QColor(79,79,79));
  aurect->setRotation(-90);
  auptr->addBlockitems(aurect);
  scene->addItem(aurect);

  QGraphicsRectItem *agrect = new QGraphicsRectItem();
  agrect->setRect(0,0,10,630);
  agrect->setPos(QPointF(-875,695));
  agrect->setBrush(QColor(79,79,79));
  agrect->setRotation(-90);
  agptr->addBlockitems(agrect);
  scene->addItem(agrect);
  QGraphicsRectItem *agrect2 = new QGraphicsRectItem();
  agrect2->setRect(0,0,10,125);
  agrect2->setPos(QPointF(-160,600));
  agrect2->setBrush(QColor(79,79,79));
  agrect2->setRotation(45);
  agptr->addBlockitems(agrect2);
  scene->addItem(agrect2);

  QGraphicsRectItem *w8rectge = new QGraphicsRectItem();
  w8rectge->setRect(0,0,10,75);
  w8rectge->setPos(QPointF(-125,565));
  w8rectge->setBrush(QColor(79,79,79));
  w8rectge->setRotation(90);
  scene->addItem(w8rectge);
  QGraphicsRectItem *w8rectab = new QGraphicsRectItem();
  w8rectab->setRect(0,0,10,50);
  w8rectab->setPos(QPointF(-125,565));
  w8rectab->setBrush(QColor(79,79,79));
  w8rectab->setRotation(45);
  scene->addItem(w8rectab);
  QLabel *w8label = new QLabel();
  scene->addWidget(w8label);
  WeicheQPushButton *w8push = new WeicheQPushButton();
  scene->addWidget(w8push);
  w8ptr->addWeichenitem(w8rectab,w8rectge,w8label,w8push);
  w8ptr->moveButton(-135,560);
  w8ptr->moveLabel(-180,555);

  QGraphicsSvgItem *s4item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s4item1);
  s4item1->setPos(QPointF(-775,800));
  s4item1->setScale(0.36);
  s4item1->setRotation(-90);
  s4item1->setElementId("basic_hp0");
  s4item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s4push = new HSignalQPushButton();
  scene->addWidget(s4push);
  QLabel *s4label = new QLabel();
  scene->addWidget(s4label);
  //speicher GUIs
  QGraphicsRectItem *sphin4 = new QGraphicsRectItem();
  sphin4->setRect(0,0,10,10);
  sphin4->setBrush(Qt::yellow);
  sphin4->setPos(QPointF(-715,790));
  QGraphicsRectItem *spweg4 = new QGraphicsRectItem();
  spweg4->setRect(0,0,10,10);
  spweg4->setBrush(Qt::yellow);
  spweg4->setPos(QPointF(-715,780));
  scene->addItem(spweg4);
  scene->addItem(sphin4);
  s4ptr->addHSignalitem(s4item1, s4label,s4push,spweg4,sphin4);
  s4ptr->moveButton(-775,805);
  s4ptr->moveLabel(-700,785);

  QGraphicsSvgItem *s5item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s5item1);
  s5item1->setPos(QPointF(-775,870));
  s5item1->setScale(0.36);
  s5item1->setRotation(-90);
  s5item1->setElementId("basic_hp0");
  s5item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s5push = new HSignalQPushButton();
  scene->addWidget(s5push);
  QLabel *s5label = new QLabel();
  scene->addWidget(s5label);
  //speicher GUIs
  QGraphicsRectItem *sphin5 = new QGraphicsRectItem();
  sphin5->setRect(0,0,10,10);
  sphin5->setBrush(Qt::yellow);
  sphin5->setPos(QPointF(-715,860));
  QGraphicsRectItem *spweg5 = new QGraphicsRectItem();
  spweg5->setRect(0,0,10,10);
  spweg5->setBrush(Qt::yellow);
  spweg5->setPos(QPointF(-715,850));
  scene->addItem(spweg5);
  scene->addItem(sphin5);
  s5ptr->addHSignalitem(s5item1, s5label,s5push,spweg5,sphin5);
  s5ptr->moveButton(-775,875);
  s5ptr->moveLabel(-700,855);

  QGraphicsSvgItem *s2item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s2item1);
  s2item1->setPos(QPointF(425,830));
  s2item1->setScale(0.36);
  s2item1->setRotation(90);
  s2item1->setElementId("basic_hp0");
  s2item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s2push = new HSignalQPushButton();
  scene->addWidget(s2push);
  QLabel *s2label = new QLabel();
  scene->addWidget(s2label);
  //speicher GUIs
  QGraphicsRectItem *sphin2 = new QGraphicsRectItem();
  sphin2->setRect(0,0,10,10);
  sphin2->setBrush(Qt::yellow);
  sphin2->setPos(QPointF(356,829));
  QGraphicsRectItem *spweg2 = new QGraphicsRectItem();
  spweg2->setRect(0,0,10,10);
  spweg2->setBrush(Qt::yellow);
  spweg2->setPos(QPointF(356,839));
  scene->addItem(spweg2);
  scene->addItem(sphin2);
  s2ptr->addHSignalitem(s2item1, s2label,s2push,spweg2,sphin2);
  s2ptr->moveButton(405,805);
  s2ptr->moveLabel(318,830);

  QGraphicsSvgItem *s3item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s3item1);
  s3item1->setPos(QPointF(425,900));
  s3item1->setScale(0.36);
  s3item1->setRotation(90);
  s3item1->setElementId("basic_hp0");
  s3item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s3push = new HSignalQPushButton();
  scene->addWidget(s3push);
  QLabel *s3label = new QLabel();
  scene->addWidget(s3label);
  //speicher GUIs
  QGraphicsRectItem *sphin3 = new QGraphicsRectItem();
  sphin3->setRect(0,0,10,10);
  sphin3->setBrush(Qt::yellow);
  sphin3->setPos(QPointF(356,899));
  QGraphicsRectItem *spweg3 = new QGraphicsRectItem();
  spweg3->setRect(0,0,10,10);
  spweg3->setBrush(Qt::yellow);
  spweg3->setPos(QPointF(356,909));
  scene->addItem(spweg3);
  scene->addItem(sphin3);
  s3ptr->addHSignalitem(s3item1, s3label,s3push,spweg3,sphin3);
  s3ptr->moveButton(405,875);
  s3ptr->moveLabel(318,900);

  QGraphicsSvgItem *item1 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(item1);
  item1->setPos(QPointF(-965,825));
  item1->setScale(0.36);
  item1->setRotation(90);
  item1->setElementId("basic_vr0");
  item1->setObjectName("basic_vr0");
  v2ptr->addVSignalitems(item1);
  
  QGraphicsSvgItem *s13item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s13item1);
  s13item1->setPos(QPointF(-925,830));
  s13item1->setScale(0.36);
  s13item1->setRotation(90);
  s13item1->setElementId("basic_hp0");
  s13item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s13push = new HSignalQPushButton();
  scene->addWidget(s13push);
  QLabel *s13label = new QLabel();
  scene->addWidget(s13label);
  //speicher GUIs
  QGraphicsRectItem *sphin13 = new QGraphicsRectItem();
  sphin13->setRect(0,0,10,10);
  sphin13->setBrush(Qt::yellow);
  sphin13->setPos(QPointF(-1004,829));
  QGraphicsRectItem *spweg13 = new QGraphicsRectItem();
  spweg13->setRect(0,0,10,10);
  spweg13->setBrush(Qt::yellow);
  spweg13->setPos(QPointF(-1004,839));
  scene->addItem(spweg13);
  scene->addItem(sphin13);
  s13ptr->addHSignalitem(s13item1, s13label,s13push,spweg13,sphin13);
  s13ptr->moveButton(-945,805);
  s13ptr->moveLabel(-1037,830);

  QGraphicsSvgItem *itemv11 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv11);
  itemv11->setPos(QPointF(515,805));
  itemv11->setScale(0.36);
  itemv11->setRotation(-90);
  itemv11->setElementId("basic_vr0");
  itemv11->setObjectName("basic_vr0");
  v1ptr->addVSignalitems(itemv11);

  QGraphicsSvgItem *s1item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s1item1);
  s1item1->setPos(QPointF(475,800));
  s1item1->setScale(0.36);
  s1item1->setRotation(-90);
  s1item1->setElementId("basic_hp0");
  s1item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s1push = new HSignalQPushButton();
  scene->addWidget(s1push);
  QLabel *s1label = new QLabel();
  scene->addWidget(s1label);
  //speicher GUIs
  QGraphicsRectItem *sphin1 = new QGraphicsRectItem();
  sphin1->setRect(0,0,10,10);
  sphin1->setBrush(Qt::yellow);
  sphin1->setPos(QPointF(555,790));
  QGraphicsRectItem *spweg1 = new QGraphicsRectItem();
  spweg1->setRect(0,0,10,10);
  spweg1->setBrush(Qt::yellow);
  spweg1->setPos(QPointF(555,780));
  scene->addItem(spweg1);
  scene->addItem(sphin1);
  s1ptr->addHSignalitem(s1item1, s1label,s1push,spweg1,sphin1);
  s1ptr->moveButton(500,805);
  s1ptr->moveLabel(570,785);
  
  /////////////NOCH VON WSIGNAL
  //soll on top of scene sein...
  WSignalQPushButton *ww1push = new WSignalQPushButton();
  scene->addWidget(ww1push);
  ww1ptr->addButtonAndLabel(ww1label,ww1push);
  ww1ptr->moveButton(-900,875);
  ww1ptr->moveLabel(-940,900);
  // und NR 2 W
  WSignalQPushButton *ww2push = new WSignalQPushButton();
  scene->addWidget(ww2push);
  ww2ptr->addButtonAndLabel(ww2label,ww2push);
  ww2ptr->moveButton(-980,265);
  ww2ptr->moveLabel(-1015,285);
  ///////////////////////////

  QGraphicsSvgItem *itemv31 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv31);
  itemv31->setPos(QPointF(-900,600));
  itemv31->setScale(0.36);
  itemv31->setElementId("basic_vr0");
  itemv31->setObjectName("basic_vr0");
  v3ptr->addVSignalitems(itemv31);

  QGraphicsSvgItem *itemv51 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv51);
  itemv51->setPos(QPointF(-980,600));
  itemv51->setScale(0.36);
  itemv51->setElementId("basic_vr0");
  itemv51->setObjectName("basic_vr0");
  v5ptr->addVSignalitems(itemv51);

  QGraphicsSvgItem *itemv61 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv61);
  itemv61->setPos(QPointF(-1060,600));
  itemv61->setScale(0.36);
  itemv61->setElementId("basic_vr0");
  itemv61->setObjectName("basic_vr0");
  v6ptr->addVSignalitems(itemv61);

  QGraphicsSvgItem *itemv71 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv71);
  itemv71->setPos(QPointF(-1080,700));
  itemv71->setScale(0.36);
  itemv71->setRotation(180);
  itemv71->setElementId("basic_vr0");
  itemv71->setObjectName("basic_vr0");
  v7ptr->addVSignalitems(itemv71);

  QGraphicsSvgItem *itemv101 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv101);
  itemv101->setPos(QPointF(0,-615));
  itemv101->setScale(0.36);
  itemv101->setRotation(-90);
  itemv101->setElementId("basic_vr0");
  itemv101->setObjectName("basic_vr0");
  v10ptr->addVSignalitems(itemv101);

  QGraphicsSvgItem *itemv41 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv41);
  itemv41->setPos(QPointF(-920,700));
  itemv41->setScale(0.36);
  itemv41->setRotation(180);
  itemv41->setElementId("basic_vr0");
  itemv41->setObjectName("basic_vr0");
  v4ptr->addVSignalitems(itemv41);

  QGraphicsSvgItem *s9item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s9item1);
  s9item1->setPos(QPointF(-980,190));
  s9item1->setScale(0.36);
  s9item1->setRotation(-90);
  s9item1->setElementId("basic_hp0");
  s9item1->setObjectName("basic_hp0_ks");
  HSignalQPushButton *s9push = new HSignalQPushButton();
  scene->addWidget(s9push);
  QLabel *s9label = new QLabel();
  scene->addWidget(s9label);
  //speicher GUIs
  QGraphicsRectItem *sphin9 = new QGraphicsRectItem();
  sphin9->setRect(0,0,10,10);
  sphin9->setBrush(Qt::yellow);
  sphin9->setPos(QPointF(-920,180));
  QGraphicsRectItem *spweg9 = new QGraphicsRectItem();
  spweg9->setRect(0,0,10,10);
  spweg9->setBrush(Qt::yellow);
  spweg9->setPos(QPointF(-920,170));
  scene->addItem(spweg9);
  scene->addItem(sphin9);
  s9ptr->addHSignalitem(s9item1, s9label,s9push,spweg9,sphin9);
  s9ptr->moveButton(-975,195);
  s9ptr->moveLabel(-905,170);

  QGraphicsSvgItem *s11item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s11item1);
  s11item1->setPos(QPointF(-865,260));
  s11item1->setScale(0.36);
  s11item1->setRotation(-90);
  s11item1->setElementId("basic_vr0");
  s11item1->setObjectName("basic_hp0_ks");
  HSignalQPushButton *s11push = new HSignalQPushButton();
  scene->addWidget(s11push);
  QLabel *s11label = new QLabel();
  scene->addWidget(s11label);
  //speicher GUIs
  QGraphicsRectItem *sphin11 = new QGraphicsRectItem();
  sphin11->setRect(0,0,10,10);
  sphin11->setBrush(Qt::yellow);
  sphin11->setPos(QPointF(-805,250));
  QGraphicsRectItem *spweg11 = new QGraphicsRectItem();
  spweg11->setRect(0,0,10,10);
  spweg11->setBrush(Qt::yellow);
  spweg11->setPos(QPointF(-805,240));
  scene->addItem(spweg11);
  scene->addItem(sphin11);
  s11ptr->addHSignalitem(s11item1, s11label,s11push,spweg11,sphin11);
  s11ptr->moveButton(-860,265);
  s11ptr->moveLabel(-790,240);

  QGraphicsSvgItem *s49item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s49item1);
  s49item1->setPos(QPointF(-790,110));
  s49item1->setScale(0.36);
  s49item1->setRotation(-90);
  s49item1->setElementId("basic_hp0");
  s49item1->setObjectName("basic_hp0_ks");
  HSignalQPushButton *s49push = new HSignalQPushButton();
  scene->addWidget(s49push);
  QLabel *s49label = new QLabel();
  scene->addWidget(s49label);
  //speicher GUIs
  QGraphicsRectItem *sphin49 = new QGraphicsRectItem();
  sphin49->setRect(0,0,10,10);
  sphin49->setBrush(Qt::yellow);
  sphin49->setPos(QPointF(-730,100));
  QGraphicsRectItem *spweg49 = new QGraphicsRectItem();
  spweg49->setRect(0,0,10,10);
  spweg49->setBrush(Qt::yellow);
  spweg49->setPos(QPointF(-730,90));
  scene->addItem(spweg49);
  scene->addItem(sphin49);
  s49ptr->addHSignalitem(s49item1, s49label,s49push,spweg49,sphin49);
  s49ptr->moveButton(-785,115);
  s49ptr->moveLabel(-715,90);

  QGraphicsSvgItem *s10item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s10item1);
  s10item1->setPos(QPointF(150,220));
  s10item1->setScale(0.36);
  s10item1->setRotation(90);
  s10item1->setElementId("basic_hp0");
  s10item1->setObjectName("basic_hp0_ks");
  HSignalQPushButton *s10push = new HSignalQPushButton();
  scene->addWidget(s10push);
  QLabel *s10label = new QLabel();
  scene->addWidget(s10label);
  //speicher GUIs
  QGraphicsRectItem *sphin10 = new QGraphicsRectItem();
  sphin10->setRect(0,0,10,10);
  sphin10->setBrush(Qt::yellow);
  sphin10->setPos(QPointF(75,220));
  QGraphicsRectItem *spweg10 = new QGraphicsRectItem();
  spweg10->setRect(0,0,10,10);
  spweg10->setBrush(Qt::yellow);
  spweg10->setPos(QPointF(75,230));
  scene->addItem(spweg10);
  scene->addItem(sphin10);
  s10ptr->addHSignalitem(s10item1, s10label,s10push,spweg10,sphin10);
  s10ptr->moveButton(120,195);
  s10ptr->moveLabel(45,220);

  QGraphicsSvgItem *s50item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s50item1);
  s50item1->setPos(QPointF(150,140));
  s50item1->setScale(0.36);
  s50item1->setRotation(90);
  s50item1->setElementId("basic_hp0");
  s50item1->setObjectName("basic_hp0_ks");
  HSignalQPushButton *s50push = new HSignalQPushButton();
  scene->addWidget(s50push);
  QLabel *s50label = new QLabel();
  scene->addWidget(s50label);
  //speicher GUIs
  QGraphicsRectItem *sphin50 = new QGraphicsRectItem();
  sphin50->setRect(0,0,10,10);
  sphin50->setBrush(Qt::yellow);
  sphin50->setPos(QPointF(75,140));
  QGraphicsRectItem *spweg50 = new QGraphicsRectItem();
  spweg50->setRect(0,0,10,10);
  spweg50->setBrush(Qt::yellow);
  spweg50->setPos(QPointF(75,150));
  scene->addItem(spweg50);
  scene->addItem(sphin50);
  s50ptr->addHSignalitem(s50item1, s50label,s50push,spweg50,sphin50);
  s50ptr->moveButton(120,115);
  s50ptr->moveLabel(45,140);

  QGraphicsSvgItem *s12item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s12item1);
  s12item1->setPos(QPointF(150,290));
  s12item1->setScale(0.36);
  s12item1->setRotation(90);
  s12item1->setElementId("basic_hp0");
  s12item1->setObjectName("basic_hp0_ks");
  HSignalQPushButton *s12push = new HSignalQPushButton();
  scene->addWidget(s12push);
  QLabel *s12label = new QLabel();
  scene->addWidget(s12label);
  //speicher GUIs
  QGraphicsRectItem *sphin12 = new QGraphicsRectItem();
  sphin12->setRect(0,0,10,10);
  sphin12->setBrush(Qt::yellow);
  sphin12->setPos(QPointF(75,290));
  QGraphicsRectItem *spweg12 = new QGraphicsRectItem();
  spweg12->setRect(0,0,10,10);
  spweg12->setBrush(Qt::yellow);
  spweg12->setPos(QPointF(75,300));
  scene->addItem(spweg12);
  scene->addItem(sphin12);
  s12ptr->addHSignalitem(s12item1, s12label,s12push,spweg12,sphin12);
  s12ptr->moveButton(120,265);
  s12ptr->moveLabel(45,290);
  
  QGraphicsSvgItem *itemv81 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv81);
  itemv81->setPos(QPointF(-215,580));
  itemv81->setScale(0.36);
  itemv81->setRotation(90);
  itemv81->setElementId("basic_vr0");
  itemv81->setObjectName("basic_vr0");
  v8ptr->addVSignalitems(itemv81);
  
  QGraphicsSvgItem *s7item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s7item1);
  s7item1->setPos(QPointF(-175,585));
  s7item1->setScale(0.36);
  s7item1->setRotation(90);
  s7item1->setElementId("basic_hp0");
  s7item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s7push = new HSignalQPushButton();
  scene->addWidget(s7push);
  QLabel *s7label = new QLabel();
  scene->addWidget(s7label);
  //speicher GUIs
  QGraphicsRectItem *sphin7 = new QGraphicsRectItem();
  sphin7->setRect(0,0,10,10);
  sphin7->setBrush(Qt::yellow);
  sphin7->setPos(QPointF(-255,585));
  QGraphicsRectItem *spweg7 = new QGraphicsRectItem();
  spweg7->setRect(0,0,10,10);
  spweg7->setBrush(Qt::yellow);
  spweg7->setPos(QPointF(-255,595));
  scene->addItem(spweg7);
  scene->addItem(sphin7);
  s7ptr->addHSignalitem(s7item1, s7label,s7push,spweg7,sphin7);
  s7ptr->moveButton(-205,560);
  s7ptr->moveLabel(-290,590);

  QGraphicsSvgItem *itemv91 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv91);
  itemv91->setPos(QPointF(-185,645));
  itemv91->setScale(0.36);
  itemv91->setRotation(45);
  itemv91->setElementId("basic_vr0");
  itemv91->setObjectName("basic_vr0");
  v9ptr->addVSignalitems(itemv91);
  
  QGraphicsSvgItem *s6item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s6item1);
  s6item1->setPos(QPointF(-150,615));
  s6item1->setScale(0.36);
  s6item1->setRotation(45);
  s6item1->setElementId("basic_hp0");
  s6item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s6push = new HSignalQPushButton();
  scene->addWidget(s6push);
  QLabel *s6label = new QLabel();
  scene->addWidget(s6label);
  //speicher GUIs
  QGraphicsRectItem *sphin6 = new QGraphicsRectItem();
  sphin6->setRect(0,0,10,10);
  sphin6->setBrush(Qt::yellow);
  sphin6->setPos(QPointF(-215,680));
  QGraphicsRectItem *spweg6 = new QGraphicsRectItem();
  spweg6->setRect(0,0,10,10);
  spweg6->setBrush(Qt::yellow);
  spweg6->setPos(QPointF(-203,683));
  spweg6->setRotation(45);
  scene->addItem(spweg6);
  scene->addItem(sphin6);
  s6ptr->addHSignalitem(s6item1, s6label,s6push,spweg6,sphin6);
  s6ptr->moveButton(-180,605);
  s6ptr->moveLabel(-240,695);

  QGraphicsSvgItem *s8item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s8item1);
  s8item1->setPos(QPointF(-150,558));
  s8item1->setScale(0.36);
  s8item1->setRotation(-90);
  s8item1->setElementId("basic_hp0");
  s8item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s8push = new HSignalQPushButton();
  scene->addWidget(s8push);
  QLabel *s8label = new QLabel();
  scene->addWidget(s8label);
  //speicher GUIs
  QGraphicsRectItem *sphin8 = new QGraphicsRectItem();
  sphin8->setRect(0,0,10,10);
  sphin8->setBrush(Qt::yellow);
  sphin8->setPos(QPointF(-90,548));
  QGraphicsRectItem *spweg8 = new QGraphicsRectItem();
  spweg8->setRect(0,0,10,10);
  spweg8->setBrush(Qt::yellow);
  spweg8->setPos(QPointF(-90,538));
  scene->addItem(spweg8);
  scene->addItem(sphin8);
  s8ptr->addHSignalitem(s8item1, s8label,s8push,spweg8,sphin8);
  s8ptr->moveButton(-115,560);
  s8ptr->moveLabel(-70,545);

  WSignalQPushButton *ww42push = new WSignalQPushButton();
  scene->addWidget(ww42push);
  ww42ptr->addButtonAndLabel(ww42label,ww42push);
  ww42ptr->moveButton(145,115);
  ww42ptr->moveLabel(180,95);

  WSignalQPushButton *ww43push = new WSignalQPushButton();
  scene->addWidget(ww43push);
  ww43ptr->addButtonAndLabel(ww43label,ww43push);
  ww43ptr->moveButton(-1000,195);
  ww43ptr->moveLabel(-1045,220);

  //Schattenbahnhof
  QGraphicsRectItem *bzrect2 = new QGraphicsRectItem();
  bzrect2->setRect(0,0,10,140);
  bzrect2->setPos(QPointF(-1075,1800));
  bzrect2->setBrush(QColor(79,79,79));
  bzrect2->setRotation(-90);
  bzptr->addBlockitems(bzrect2);
  scene->addItem(bzrect2);
  QGraphicsRectItem *bzrect3 = new QGraphicsRectItem();
  bzrect3->setRect(0,0,10,729);
  bzrect3->setPos(QPointF(-1075,1061));
  bzrect3->setBrush(QColor(79,79,79));
  bzptr->addBlockitems(bzrect3);
  scene->addItem(bzrect3);
  

  QGraphicsRectItem *w40rectge = new QGraphicsRectItem();
  w40rectge->setRect(0,0,10,75);
  w40rectge->setPos(QPointF(-935,1800));
  w40rectge->setBrush(QColor(79,79,79));
  w40rectge->setRotation(-90);
  scene->addItem(w40rectge);
  QGraphicsRectItem *w40rectab = new QGraphicsRectItem();
  w40rectab->setRect(0,0,10,99);
  w40rectab->setPos(QPointF(-935,1800));
  w40rectab->setBrush(QColor(79,79,79));
  w40rectab->setRotation(-135);
  scene->addItem(w40rectab);
  QLabel *w40label = new QLabel();
  scene->addWidget(w40label);
  WeicheQPushButton *w40push = new WeicheQPushButton();
  scene->addWidget(w40push);
  w40ptr->addWeichenitem(w40rectab,w40rectge,w40label,w40push);
  w40ptr->moveButton(-945,1785);
  w40ptr->moveLabel(-920,1805);

  QGraphicsRectItem *w42rectge = new QGraphicsRectItem();
  w42rectge->setRect(0,0,10,99);
  w42rectge->setPos(QPointF(-865,1730));
  w42rectge->setBrush(QColor(79,79,79));
  w42rectge->setRotation(-135);
  scene->addItem(w42rectge);
  QGraphicsRectItem *w42rectab = new QGraphicsRectItem();
  w42rectab->setRect(0,0,10,50);
  w42rectab->setPos(QPointF(-865,1730));
  w42rectab->setBrush(QColor(79,79,79));
  w42rectab->setRotation(-90);
  scene->addItem(w42rectab);
  QLabel *w42label = new QLabel();
  scene->addWidget(w42label);
  WeicheQPushButton *w42push = new WeicheQPushButton();
  scene->addWidget(w42push);
  w42ptr->addWeichenitem(w42rectab,w42rectge,w42label,w42push);
  w42ptr->moveButton(-875,1715);
  w42ptr->moveLabel(-900,1705);

  QGraphicsRectItem *w44rectge = new QGraphicsRectItem();
  w44rectge->setRect(0,0,10,99);
  w44rectge->setPos(QPointF(-795,1660));
  w44rectge->setBrush(QColor(79,79,79));
  w44rectge->setRotation(-135);
  scene->addItem(w44rectge);
  QGraphicsRectItem *w44rectab = new QGraphicsRectItem();
  w44rectab->setRect(0,0,10,50);
  w44rectab->setPos(QPointF(-795,1660));
  w44rectab->setBrush(QColor(79,79,79));
  w44rectab->setRotation(-90);
  scene->addItem(w44rectab);
  QLabel *w44label = new QLabel();
  scene->addWidget(w44label);
  WeicheQPushButton *w44push = new WeicheQPushButton();
  scene->addWidget(w44push);
  w44ptr->addWeichenitem(w44rectab,w44rectge,w44label,w44push);
  w44ptr->moveButton(-805,1645);
  w44ptr->moveLabel(-830,1635);

  QGraphicsRectItem *w46rectge = new QGraphicsRectItem();
  w46rectge->setRect(0,0,10,99);
  w46rectge->setPos(QPointF(-725,1590));
  w46rectge->setBrush(QColor(79,79,79));
  w46rectge->setRotation(-135);
  scene->addItem(w46rectge);
  QGraphicsRectItem *w46rectab = new QGraphicsRectItem();
  w46rectab->setRect(0,0,10,50);
  w46rectab->setPos(QPointF(-725,1590));
  w46rectab->setBrush(QColor(79,79,79));
  w46rectab->setRotation(-90);
  scene->addItem(w46rectab);
  QLabel *w46label = new QLabel();
  scene->addWidget(w46label);
  WeicheQPushButton *w46push = new WeicheQPushButton();
  scene->addWidget(w46push);
  w46ptr->addWeichenitem(w46rectab,w46rectge,w46label,w46push);
  w46ptr->moveButton(-735,1575);
  w46ptr->moveLabel(-760,1565);

  QGraphicsRectItem *w48rectge = new QGraphicsRectItem();
  w48rectge->setRect(0,0,10,99);
  w48rectge->setPos(QPointF(-655,1520));
  w48rectge->setBrush(QColor(79,79,79));
  w48rectge->setRotation(-135);
  scene->addItem(w48rectge);
  QGraphicsRectItem *w48rectab = new QGraphicsRectItem();
  w48rectab->setRect(0,0,10,50);
  w48rectab->setPos(QPointF(-655,1520));
  w48rectab->setBrush(QColor(79,79,79));
  w48rectab->setRotation(-90);
  scene->addItem(w48rectab);
  QLabel *w48label = new QLabel();
  scene->addWidget(w48label);
  WeicheQPushButton *w48push = new WeicheQPushButton();
  scene->addWidget(w48push);
  w48ptr->addWeichenitem(w48rectab,w48rectge,w48label,w48push);
  w48ptr->moveButton(-665,1505);
  w48ptr->moveLabel(-690,1495);

  QGraphicsRectItem *w50rectge = new QGraphicsRectItem();
  w50rectge->setRect(0,0,10,99);
  w50rectge->setPos(QPointF(-585,1450));
  w50rectge->setBrush(QColor(79,79,79));
  w50rectge->setRotation(-135);
  scene->addItem(w50rectge);
  QGraphicsRectItem *w50rectab = new QGraphicsRectItem();
  w50rectab->setRect(0,0,10,50);
  w50rectab->setPos(QPointF(-585,1450));
  w50rectab->setBrush(QColor(79,79,79));
  w50rectab->setRotation(-90);
  scene->addItem(w50rectab);
  QLabel *w50label = new QLabel();
  scene->addWidget(w50label);
  WeicheQPushButton *w50push = new WeicheQPushButton();
  scene->addWidget(w50push);
  w50ptr->addWeichenitem(w50rectab,w50rectge,w50label,w50push);
  w50ptr->moveButton(-595,1435);
  w50ptr->moveLabel(-620,1425);

  QGraphicsRectItem *w52rectge = new QGraphicsRectItem();
  w52rectge->setRect(0,0,10,99);
  w52rectge->setPos(QPointF(-515,1380));
  w52rectge->setBrush(QColor(79,79,79));
  w52rectge->setRotation(-135);
  scene->addItem(w52rectge);
  QGraphicsRectItem *w52rectab = new QGraphicsRectItem();
  w52rectab->setRect(0,0,10,50);
  w52rectab->setPos(QPointF(-515,1380));
  w52rectab->setBrush(QColor(79,79,79));
  w52rectab->setRotation(-90);
  scene->addItem(w52rectab);
  QLabel *w52label = new QLabel();
  scene->addWidget(w52label);
  WeicheQPushButton *w52push = new WeicheQPushButton();
  scene->addWidget(w52push);
  w52ptr->addWeichenitem(w52rectab,w52rectge,w52label,w52push);
  w52ptr->moveButton(-525,1365);
  w52ptr->moveLabel(-550,1355);

  QGraphicsRectItem *bwrect = new QGraphicsRectItem();
  bwrect->setRect(0,0,10,750);
  bwrect->setPos(QPointF(-865,1800));
  bwrect->setBrush(QColor(79,79,79));
  bwrect->setRotation(-90);
  bwptr->addBlockitems(bwrect);
  scene->addItem(bwrect);

  QGraphicsRectItem *bvrect = new QGraphicsRectItem();
  bvrect->setRect(0,0,10,720);
  bvrect->setPos(QPointF(-815,1730));
  bvrect->setBrush(QColor(79,79,79));
  bvrect->setRotation(-90);
  bvptr->addBlockitems(bvrect);
  scene->addItem(bvrect);

  QGraphicsRectItem *burect = new QGraphicsRectItem();
  burect->setRect(0,0,10,720);
  burect->setPos(QPointF(-745,1660));
  burect->setBrush(QColor(79,79,79));
  burect->setRotation(-90);
  buptr->addBlockitems(burect);
  scene->addItem(burect);

  QGraphicsRectItem *btrect = new QGraphicsRectItem();
  btrect->setRect(0,0,10,720);
  btrect->setPos(QPointF(-675,1590));
  btrect->setBrush(QColor(79,79,79));
  btrect->setRotation(-90);
  btptr->addBlockitems(btrect);
  scene->addItem(btrect);

  QGraphicsRectItem *bsrect = new QGraphicsRectItem();
  bsrect->setRect(0,0,10,720);
  bsrect->setPos(QPointF(-605,1520));
  bsrect->setBrush(QColor(79,79,79));
  bsrect->setRotation(-90);
  bsptr->addBlockitems(bsrect);
  scene->addItem(bsrect);

  QGraphicsRectItem *brrect = new QGraphicsRectItem();
  brrect->setRect(0,0,10,720);
  brrect->setPos(QPointF(-535,1450));
  brrect->setBrush(QColor(79,79,79));
  brrect->setRotation(-90);
  brptr->addBlockitems(brrect);
  scene->addItem(brrect);

  QGraphicsRectItem *bqrect = new QGraphicsRectItem();
  bqrect->setRect(0,0,10,720);
  bqrect->setPos(QPointF(-465,1380));
  bqrect->setBrush(QColor(79,79,79));
  bqrect->setRotation(-90);
  bqptr->addBlockitems(bqrect);
  scene->addItem(bqrect);

  QGraphicsRectItem *bprect = new QGraphicsRectItem();
  bprect->setRect(0,0,10,755);
  bprect->setPos(QPointF(-445,1310));
  bprect->setBrush(QColor(79,79,79));
  bprect->setRotation(-90);
  bpptr->addBlockitems(bprect);
  scene->addItem(bprect);

  QGraphicsRectItem *bgrect = new QGraphicsRectItem();
  bgrect->setRect(0,0,10,200);
  bgrect->setPos(QPointF(1135,-220));
  bgrect->setBrush(QColor(79,79,79));
  bgrect->setRotation(90);
  bgptr->addBlockitems(bgrect);
  scene->addItem(bgrect);

  QGraphicsRectItem *w53rectge = new QGraphicsRectItem();
  w53rectge->setRect(0,0,10,75);
  w53rectge->setPos(QPointF(375,1300));
  w53rectge->setBrush(QColor(79,79,79));
  w53rectge->setRotation(90);
  scene->addItem(w53rectge);
  QGraphicsRectItem *w53rectab = new QGraphicsRectItem();
  w53rectab->setRect(0,0,10,99);
  w53rectab->setPos(QPointF(375,1300));
  w53rectab->setBrush(QColor(79,79,79));
  w53rectab->setRotation(45);
  scene->addItem(w53rectab);
  QLabel *w53label = new QLabel();
  scene->addWidget(w53label);
  WeicheQPushButton *w53push = new WeicheQPushButton();
  scene->addWidget(w53push);
  w53ptr->addWeichenitem(w53rectab,w53rectge,w53label,w53push);
  w53ptr->moveButton(370,1295);
  w53ptr->moveLabel(350,1285);

  QGraphicsRectItem *w51rectge = new QGraphicsRectItem();
  w51rectge->setRect(0,0,10,99);
  w51rectge->setPos(QPointF(305,1370));
  w51rectge->setBrush(QColor(79,79,79));
  w51rectge->setRotation(45);
  scene->addItem(w51rectge);
  QGraphicsRectItem *w51rectab = new QGraphicsRectItem();
  w51rectab->setRect(0,0,10,50);
  w51rectab->setPos(QPointF(305,1370));
  w51rectab->setBrush(QColor(79,79,79));
  w51rectab->setRotation(90);
  scene->addItem(w51rectab);
  QLabel *w51label = new QLabel();
  scene->addWidget(w51label);
  WeicheQPushButton *w51push = new WeicheQPushButton();
  scene->addWidget(w51push);
  w51ptr->addWeichenitem(w51rectab,w51rectge,w51label,w51push);
  w51ptr->moveButton(295,1365);
  w51ptr->moveLabel(320,1375);

  QGraphicsRectItem *w49rectge = new QGraphicsRectItem();
  w49rectge->setRect(0,0,10,99);
  w49rectge->setPos(QPointF(235,1440));
  w49rectge->setBrush(QColor(79,79,79));
  w49rectge->setRotation(45);
  scene->addItem(w49rectge);
  QGraphicsRectItem *w49rectab = new QGraphicsRectItem();
  w49rectab->setRect(0,0,10,50);
  w49rectab->setPos(QPointF(235,1440));
  w49rectab->setBrush(QColor(79,79,79));
  w49rectab->setRotation(90);
  scene->addItem(w49rectab);
  QLabel *w49label = new QLabel();
  scene->addWidget(w49label);
  WeicheQPushButton *w49push = new WeicheQPushButton();
  scene->addWidget(w49push);
  w49ptr->addWeichenitem(w49rectab,w49rectge,w49label,w49push);
  w49ptr->moveButton(225,1435);
  w49ptr->moveLabel(250,1445);

  QGraphicsRectItem *w47rectge = new QGraphicsRectItem();
  w47rectge->setRect(0,0,10,99);
  w47rectge->setPos(QPointF(165,1510));
  w47rectge->setBrush(QColor(79,79,79));
  w47rectge->setRotation(45);
  scene->addItem(w47rectge);
  QGraphicsRectItem *w47rectab = new QGraphicsRectItem();
  w47rectab->setRect(0,0,10,50);
  w47rectab->setPos(QPointF(165,1510));
  w47rectab->setBrush(QColor(79,79,79));
  w47rectab->setRotation(90);
  scene->addItem(w47rectab);
  QLabel *w47label = new QLabel();
  scene->addWidget(w47label);
  WeicheQPushButton *w47push = new WeicheQPushButton();
  scene->addWidget(w47push);
  w47ptr->addWeichenitem(w47rectab,w47rectge,w47label,w47push);
  w47ptr->moveButton(155,1505);
  w47ptr->moveLabel(180,1515);

  QGraphicsRectItem *w45rectge = new QGraphicsRectItem();
  w45rectge->setRect(0,0,10,99);
  w45rectge->setPos(QPointF(95,1580));
  w45rectge->setBrush(QColor(79,79,79));
  w45rectge->setRotation(45);
  scene->addItem(w45rectge);
  QGraphicsRectItem *w45rectab = new QGraphicsRectItem();
  w45rectab->setRect(0,0,10,50);
  w45rectab->setPos(QPointF(95,1580));
  w45rectab->setBrush(QColor(79,79,79));
  w45rectab->setRotation(90);
  scene->addItem(w45rectab);
  QLabel *w45label = new QLabel();
  scene->addWidget(w45label);
  WeicheQPushButton *w45push = new WeicheQPushButton();
  scene->addWidget(w45push);
  w45ptr->addWeichenitem(w45rectab,w45rectge,w45label,w45push);
  w45ptr->moveButton(85,1575);
  w45ptr->moveLabel(110,1585);

  QGraphicsRectItem *w43rectge = new QGraphicsRectItem();
  w43rectge->setRect(0,0,10,99);
  w43rectge->setPos(QPointF(25,1650));
  w43rectge->setBrush(QColor(79,79,79));
  w43rectge->setRotation(45);
  scene->addItem(w43rectge);
  QGraphicsRectItem *w43rectab = new QGraphicsRectItem();
  w43rectab->setRect(0,0,10,50);
  w43rectab->setPos(QPointF(25,1650));
  w43rectab->setBrush(QColor(79,79,79));
  w43rectab->setRotation(90);
  scene->addItem(w43rectab);
  QLabel *w43label = new QLabel();
  scene->addWidget(w43label);
  WeicheQPushButton *w43push = new WeicheQPushButton();
  scene->addWidget(w43push);
  w43ptr->addWeichenitem(w43rectab,w43rectge,w43label,w43push);
  w43ptr->moveButton(15,1645);
  w43ptr->moveLabel(40,1655);

  QGraphicsRectItem *w41rectge = new QGraphicsRectItem();
  w41rectge->setRect(0,0,10,99);
  w41rectge->setPos(QPointF(-45,1720));
  w41rectge->setBrush(QColor(79,79,79));
  w41rectge->setRotation(45);
  scene->addItem(w41rectge);
  QGraphicsRectItem *w41rectab = new QGraphicsRectItem();
  w41rectab->setRect(0,0,10,50);
  w41rectab->setPos(QPointF(-45,1720));
  w41rectab->setBrush(QColor(79,79,79));
  w41rectab->setRotation(90);
  scene->addItem(w41rectab);
  QLabel *w41label = new QLabel();
  scene->addWidget(w41label);
  WeicheQPushButton *w41push = new WeicheQPushButton();
  scene->addWidget(w41push);
  w41ptr->addWeichenitem(w41rectab,w41rectge,w41label,w41push);
  w41ptr->moveButton(-55,1715);
  w41ptr->moveLabel(-20,1725);

  QGraphicsRectItem *byrect3 = new QGraphicsRectItem();
  byrect3->setRect(0,0,10,140);
  byrect3->setPos(QPointF(474,1201));
  byrect3->setBrush(QColor(79,79,79));
  byrect3->setRotation(45);
  byptr->addBlockitems(byrect3);
  scene->addItem(byrect3);
  QGraphicsRectItem *byrect4 = new QGraphicsRectItem();
  byrect4->setRect(0,0,10,140);
  byrect4->setPos(QPointF(474,1061));
  byrect4->setBrush(QColor(79,79,79));
  byptr->addBlockitems(byrect4);
  scene->addItem(byrect4);

  WSignalQPushButton *ww24push = new WSignalQPushButton();
  scene->addWidget(ww24push);
  ww24ptr->addButtonAndLabel(ww24label,ww24push);
  ww24ptr->moveButton(-865,1785);
  ww24ptr->moveLabel(-835,1770);

  WSignalQPushButton *ww25push = new WSignalQPushButton();
  scene->addWidget(ww25push);
  ww25ptr->addButtonAndLabel(ww25label,ww25push);
  ww25ptr->moveButton(-815,1715);
  ww25ptr->moveLabel(-785,1700);

  WSignalQPushButton *ww26push = new WSignalQPushButton();
  scene->addWidget(ww26push);
  ww26ptr->addButtonAndLabel(ww26label,ww26push);
  ww26ptr->moveButton(-745,1645);
  ww26ptr->moveLabel(-715,1630);

  WSignalQPushButton *ww27push = new WSignalQPushButton();
  scene->addWidget(ww27push);
  ww27ptr->addButtonAndLabel(ww27label,ww27push);
  ww27ptr->moveButton(-675,1575);
  ww27ptr->moveLabel(-645,1560);

  WSignalQPushButton *ww28push = new WSignalQPushButton();
  scene->addWidget(ww28push);
  ww28ptr->addButtonAndLabel(ww28label,ww28push);
  ww28ptr->moveButton(-605,1505);
  ww28ptr->moveLabel(-575,1490);

  WSignalQPushButton *ww29push = new WSignalQPushButton();
  scene->addWidget(ww29push);
  ww29ptr->addButtonAndLabel(ww29label,ww29push);
  ww29ptr->moveButton(-535,1435);
  ww29ptr->moveLabel(-505,1420);

  WSignalQPushButton *ww30push = new WSignalQPushButton();
  scene->addWidget(ww30push);
  ww30ptr->addButtonAndLabel(ww30label,ww30push);
  ww30ptr->moveButton(-465,1365);
  ww30ptr->moveLabel(-435,1350);

  WSignalQPushButton *ww31push = new WSignalQPushButton();
  scene->addWidget(ww31push);
  ww31ptr->addButtonAndLabel(ww31label,ww31push);
  ww31ptr->moveButton(-445,1295);
  ww31ptr->moveLabel(-415,1280);

  WSignalQPushButton *ww39push = new WSignalQPushButton();
  scene->addWidget(ww39push);
  ww39ptr->addButtonAndLabel(ww39label,ww39push);
  ww39ptr->moveButton(-1005,1785);
  ww39ptr->moveLabel(-1045,1810);

  WSignalQPushButton *ww40push = new WSignalQPushButton();
  scene->addWidget(ww40push);
  ww40ptr->addButtonAndLabel(ww40label,ww40push);
  ww40ptr->moveButton(410,1250);
  ww40ptr->moveLabel(391,1219);

  WSignalQPushButton *ww41push = new WSignalQPushButton();
  scene->addWidget(ww41push);
  ww41ptr->addButtonAndLabel(ww41label,ww41push);
  ww41ptr->moveButton(-135,1785);
  ww41ptr->moveLabel(-180,1810);

  WSignalQPushButton *ww38push = new WSignalQPushButton();
  scene->addWidget(ww38push);
  ww38ptr->addButtonAndLabel(ww38label,ww38push);
  ww38ptr->moveButton(-115,1715);
  ww38ptr->moveLabel(-160,1740);

  WSignalQPushButton *ww37push = new WSignalQPushButton();
  scene->addWidget(ww37push);
  ww37ptr->addButtonAndLabel(ww37label,ww37push);
  ww37ptr->moveButton(-45,1645);
  ww37ptr->moveLabel(-90,1670);

  WSignalQPushButton *ww36push = new WSignalQPushButton();
  scene->addWidget(ww36push);
  ww36ptr->addButtonAndLabel(ww36label,ww36push);
  ww36ptr->moveButton(25,1575);
  ww36ptr->moveLabel(-20,1600);

  WSignalQPushButton *ww35push = new WSignalQPushButton();
  scene->addWidget(ww35push);
  ww35ptr->addButtonAndLabel(ww35label,ww35push);
  ww35ptr->moveButton(95,1505);
  ww35ptr->moveLabel(50,1530);

  WSignalQPushButton *ww34push = new WSignalQPushButton();
  scene->addWidget(ww34push);
  ww34ptr->addButtonAndLabel(ww34label,ww34push);
  ww34ptr->moveButton(165,1435);
  ww34ptr->moveLabel(120,1460);

  WSignalQPushButton *ww33push = new WSignalQPushButton();
  scene->addWidget(ww33push);
  ww33ptr->addButtonAndLabel(ww33label,ww33push);
  ww33ptr->moveButton(235,1365);
  ww33ptr->moveLabel(190,1390);

  WSignalQPushButton *ww32push = new WSignalQPushButton();
  scene->addWidget(ww32push);
  ww32ptr->addButtonAndLabel(ww32label,ww32push);
  ww32ptr->moveButton(290,1295);
  ww32ptr->moveLabel(245,1320);

  QGraphicsSvgItem *itemv44 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv44);
  itemv44->setPos(QPointF(-1000,1785));
  itemv44->setScale(0.36);
  itemv44->setRotation(-90);
  itemv44->setElementId("basic_vr0");
  itemv44->setObjectName("basic_vr0");
  v44ptr->addVSignalitems(itemv44);

  QGraphicsSvgItem *s52item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s52item1);
  s52item1->setPos(QPointF(-1040,1780));
  s52item1->setScale(0.36);
  s52item1->setRotation(-90);
  s52item1->setElementId("basic_hp0");
  s52item1->setObjectName("basic_hp0");
  HSignalQPushButton *s52push = new HSignalQPushButton();
  scene->addWidget(s52push);
  QLabel *s52label = new QLabel();
  scene->addWidget(s52label);
  //speicher GUIs
  QGraphicsRectItem *sphin52 = new QGraphicsRectItem();
  sphin52->setRect(0,0,10,10);
  sphin52->setBrush(Qt::yellow);
  sphin52->setPos(QPointF(-960,1770));
  QGraphicsRectItem *spweg52 = new QGraphicsRectItem();
  spweg52->setRect(0,0,10,10);
  spweg52->setBrush(Qt::yellow);
  spweg52->setPos(QPointF(-960,1760));
  scene->addItem(spweg52);
  scene->addItem(sphin52);
  s52ptr->addHSignalitem(s52item1,s52label,s52push,spweg52,sphin52);
  s52ptr->moveButton(-985,1785);
  s52ptr->moveLabel(-950,1765);

  QGraphicsSvgItem *itemv431 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv431);
  itemv431->setPos(QPointF(439,1275));
  itemv431->setScale(0.36);
  itemv431->setRotation(45);
  itemv431->setElementId("basic_vr0");
  itemv431->setObjectName("basic_vr0");
  v43ptr->addVSignalitems(itemv431);
  
  QGraphicsSvgItem *s51item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s51item1);
  s51item1->setPos(QPointF(474,1245));
  s51item1->setScale(0.36);
  s51item1->setRotation(45);
  s51item1->setElementId("basic_hp0");
  s51item1->setObjectName("basic_hp0");
  HSignalQPushButton *s51push = new HSignalQPushButton();
  scene->addWidget(s51push);
  QLabel *s51label = new QLabel();
  scene->addWidget(s51label);
  //speicher GUIs
  QGraphicsRectItem *sphin51 = new QGraphicsRectItem();
  sphin51->setRect(0,0,10,10);
  sphin51->setBrush(Qt::yellow);
  sphin51->setPos(QPointF(409,1310));
  QGraphicsRectItem *spweg51 = new QGraphicsRectItem();
  spweg51->setRect(0,0,10,10);
  spweg51->setBrush(Qt::yellow);
  spweg51->setPos(QPointF(421,1313));
  spweg51->setRotation(45);
  scene->addItem(spweg51);
  scene->addItem(sphin51);
  s51ptr->addHSignalitem(s51item1, s51label,s51push,spweg51,sphin51);
  s51ptr->moveButton(391,1270);
  s51ptr->moveLabel(384,1325);

  //----------------------------------------------
  QPair<Weiche*, bool> ww40toww31w53(w53ptr,true);
  QList<QPair<Weiche*, bool>> ww40toww31;
  ww40toww31.push_back(ww40toww31w53);
  ww40ptr->addWeichenstatus(ww31ptr,ww40toww31);
  QPair<Weiche*, bool> ww40toww30w53(w53ptr,false);
  QPair<Weiche*, bool> ww40toww30w51(w51ptr,false);
  QList<QPair<Weiche*, bool>> ww40toww30;
  ww40toww30.push_back(ww40toww30w53);
  ww40toww30.push_back(ww40toww30w51);
  ww40ptr->addWeichenstatus(ww30ptr,ww40toww30);
  QPair<Weiche*, bool> ww40toww29w53(w53ptr,false);
  QPair<Weiche*, bool> ww40toww29w51(w51ptr,true);
  QPair<Weiche*, bool> ww40toww29w49(w49ptr,false);
  QList<QPair<Weiche*, bool>> ww40toww29;
  ww40toww29.push_back(ww40toww29w53);
  ww40toww29.push_back(ww40toww29w51);
  ww40toww29.push_back(ww40toww29w49);
  ww40ptr->addWeichenstatus(ww29ptr,ww40toww29);
  QPair<Weiche*, bool> ww40toww28w53(w53ptr,false);
  QPair<Weiche*, bool> ww40toww28w51(w51ptr,true);
  QPair<Weiche*, bool> ww40toww28w49(w49ptr,true);
  QPair<Weiche*, bool> ww40toww28w47(w47ptr,false);
  QList<QPair<Weiche*, bool>> ww40toww28;
  ww40toww28.push_back(ww40toww28w53);
  ww40toww28.push_back(ww40toww28w51);
  ww40toww28.push_back(ww40toww28w49);
  ww40toww28.push_back(ww40toww28w47);
  ww40ptr->addWeichenstatus(ww28ptr,ww40toww28);
  QPair<Weiche*, bool> ww40toww27w53(w53ptr,false);
  QPair<Weiche*, bool> ww40toww27w51(w51ptr,true);
  QPair<Weiche*, bool> ww40toww27w49(w49ptr,true);
  QPair<Weiche*, bool> ww40toww27w47(w47ptr,true);
  QPair<Weiche*, bool> ww40toww27w45(w45ptr,false);
  QList<QPair<Weiche*, bool>> ww40toww27;
  ww40toww27.push_back(ww40toww27w53);
  ww40toww27.push_back(ww40toww27w51);
  ww40toww27.push_back(ww40toww27w49);
  ww40toww27.push_back(ww40toww27w47);
  ww40toww27.push_back(ww40toww27w45);
  ww40ptr->addWeichenstatus(ww27ptr,ww40toww27);
  QPair<Weiche*, bool> ww40toww26w53(w53ptr,false);
  QPair<Weiche*, bool> ww40toww26w51(w51ptr,true);
  QPair<Weiche*, bool> ww40toww26w49(w49ptr,true);
  QPair<Weiche*, bool> ww40toww26w47(w47ptr,true);
  QPair<Weiche*, bool> ww40toww26w45(w45ptr,true);
  QPair<Weiche*, bool> ww40toww26w43(w43ptr,false);
  QList<QPair<Weiche*, bool>> ww40toww26;
  ww40toww26.push_back(ww40toww26w53);
  ww40toww26.push_back(ww40toww26w51);
  ww40toww26.push_back(ww40toww26w49);
  ww40toww26.push_back(ww40toww26w47);
  ww40toww26.push_back(ww40toww26w45);
  ww40toww26.push_back(ww40toww26w43);
  ww40ptr->addWeichenstatus(ww26ptr,ww40toww26);
  QPair<Weiche*, bool> ww40toww25w53(w53ptr,false);
  QPair<Weiche*, bool> ww40toww25w51(w51ptr,true);
  QPair<Weiche*, bool> ww40toww25w49(w49ptr,true);
  QPair<Weiche*, bool> ww40toww25w47(w47ptr,true);
  QPair<Weiche*, bool> ww40toww25w45(w45ptr,true);
  QPair<Weiche*, bool> ww40toww25w43(w43ptr,true);
  QPair<Weiche*, bool> ww40toww25w41(w41ptr,false);
  QList<QPair<Weiche*, bool>> ww40toww25;
  ww40toww25.push_back(ww40toww25w53);
  ww40toww25.push_back(ww40toww25w51);
  ww40toww25.push_back(ww40toww25w49);
  ww40toww25.push_back(ww40toww25w47);
  ww40toww25.push_back(ww40toww25w45);
  ww40toww25.push_back(ww40toww25w43);
  ww40toww25.push_back(ww40toww25w41);
  ww40ptr->addWeichenstatus(ww25ptr,ww40toww25);
  QPair<Weiche*, bool> ww40toww24w53(w53ptr,false);
  QPair<Weiche*, bool> ww40toww24w51(w51ptr,true);
  QPair<Weiche*, bool> ww40toww24w49(w49ptr,true);
  QPair<Weiche*, bool> ww40toww24w47(w47ptr,true);
  QPair<Weiche*, bool> ww40toww24w45(w45ptr,true);
  QPair<Weiche*, bool> ww40toww24w43(w43ptr,true);
  QPair<Weiche*, bool> ww40toww24w41(w41ptr,true);
  QList<QPair<Weiche*, bool>> ww40toww24;
  ww40toww24.push_back(ww40toww24w53);
  ww40toww24.push_back(ww40toww24w51);
  ww40toww24.push_back(ww40toww24w49);
  ww40toww24.push_back(ww40toww24w47);
  ww40toww24.push_back(ww40toww24w45);
  ww40toww24.push_back(ww40toww24w43);
  ww40toww24.push_back(ww40toww24w41);
  ww40ptr->addWeichenstatus(ww24ptr,ww40toww24);

  QPair<Weiche*, bool> ww39toww41w40(w40ptr,true);
  QList<QPair<Weiche*, bool>> ww39toww41;
  ww39toww41.push_back(ww39toww41w40);
  ww39ptr->addWeichenstatus(ww41ptr,ww39toww41);
  QPair<Weiche*, bool> ww39toww38w40(w40ptr,false);
  QPair<Weiche*, bool> ww39toww38w42(w42ptr,false);
  QList<QPair<Weiche*, bool>> ww39toww38;
  ww39toww38.push_back(ww39toww38w40);
  ww39toww38.push_back(ww39toww38w42);
  ww39ptr->addWeichenstatus(ww38ptr,ww39toww38);
  QPair<Weiche*, bool> ww39toww37w40(w40ptr,false);
  QPair<Weiche*, bool> ww39toww37w42(w42ptr,true);
  QPair<Weiche*, bool> ww39toww37w44(w44ptr,false);
  QList<QPair<Weiche*, bool>> ww39toww37;
  ww39toww37.push_back(ww39toww37w40);
  ww39toww37.push_back(ww39toww37w42);
  ww39toww37.push_back(ww39toww37w44);
  ww39ptr->addWeichenstatus(ww37ptr,ww39toww37);
  QPair<Weiche*, bool> ww39toww36w40(w40ptr,false);
  QPair<Weiche*, bool> ww39toww36w42(w42ptr,true);
  QPair<Weiche*, bool> ww39toww36w44(w44ptr,true);
  QPair<Weiche*, bool> ww39toww36w46(w46ptr,false);
  QList<QPair<Weiche*, bool>> ww39toww36;
  ww39toww36.push_back(ww39toww36w40);
  ww39toww36.push_back(ww39toww36w42);
  ww39toww36.push_back(ww39toww36w44);
  ww39toww36.push_back(ww39toww36w46);
  ww39ptr->addWeichenstatus(ww36ptr,ww39toww36);
  QPair<Weiche*, bool> ww39toww35w40(w40ptr,false);
  QPair<Weiche*, bool> ww39toww35w42(w42ptr,true);
  QPair<Weiche*, bool> ww39toww35w44(w44ptr,true);
  QPair<Weiche*, bool> ww39toww35w46(w46ptr,true);
  QPair<Weiche*, bool> ww39toww35w48(w48ptr,false);
  QList<QPair<Weiche*, bool>> ww39toww35;
  ww39toww35.push_back(ww39toww35w40);
  ww39toww35.push_back(ww39toww35w42);
  ww39toww35.push_back(ww39toww35w44);
  ww39toww35.push_back(ww39toww35w46);
  ww39toww35.push_back(ww39toww35w48);
  ww39ptr->addWeichenstatus(ww35ptr,ww39toww35);
  QPair<Weiche*, bool> ww39toww34w40(w40ptr,false);
  QPair<Weiche*, bool> ww39toww34w42(w42ptr,true);
  QPair<Weiche*, bool> ww39toww34w44(w44ptr,true);
  QPair<Weiche*, bool> ww39toww34w46(w46ptr,true);
  QPair<Weiche*, bool> ww39toww34w48(w48ptr,true);
  QPair<Weiche*, bool> ww39toww34w50(w50ptr,false);
  QList<QPair<Weiche*, bool>> ww39toww34;
  ww39toww34.push_back(ww39toww34w40);
  ww39toww34.push_back(ww39toww34w42);
  ww39toww34.push_back(ww39toww34w44);
  ww39toww34.push_back(ww39toww34w46);
  ww39toww34.push_back(ww39toww34w48);
  ww39toww34.push_back(ww39toww34w50);
  ww39ptr->addWeichenstatus(ww34ptr,ww39toww34);
  QPair<Weiche*, bool> ww39toww33w40(w40ptr,false);
  QPair<Weiche*, bool> ww39toww33w42(w42ptr,true);
  QPair<Weiche*, bool> ww39toww33w44(w44ptr,true);
  QPair<Weiche*, bool> ww39toww33w46(w46ptr,true);
  QPair<Weiche*, bool> ww39toww33w48(w48ptr,true);
  QPair<Weiche*, bool> ww39toww33w50(w50ptr,true);
  QPair<Weiche*, bool> ww39toww33w52(w52ptr,false);
  QList<QPair<Weiche*, bool>> ww39toww33;
  ww39toww33.push_back(ww39toww33w40);
  ww39toww33.push_back(ww39toww33w42);
  ww39toww33.push_back(ww39toww33w44);
  ww39toww33.push_back(ww39toww33w46);
  ww39toww33.push_back(ww39toww33w48);
  ww39toww33.push_back(ww39toww33w50);
  ww39toww33.push_back(ww39toww33w52);
  ww39ptr->addWeichenstatus(ww33ptr,ww39toww33);
  QPair<Weiche*, bool> ww39toww32w40(w40ptr,false);
  QPair<Weiche*, bool> ww39toww32w42(w42ptr,true);
  QPair<Weiche*, bool> ww39toww32w44(w44ptr,true);
  QPair<Weiche*, bool> ww39toww32w46(w46ptr,true);
  QPair<Weiche*, bool> ww39toww32w48(w48ptr,true);
  QPair<Weiche*, bool> ww39toww32w50(w50ptr,true);
  QPair<Weiche*, bool> ww39toww32w52(w52ptr,true);
  QList<QPair<Weiche*, bool>> ww39toww32;
  ww39toww32.push_back(ww39toww32w40);
  ww39toww32.push_back(ww39toww32w42);
  ww39toww32.push_back(ww39toww32w44);
  ww39toww32.push_back(ww39toww32w46);
  ww39toww32.push_back(ww39toww32w48);
  ww39toww32.push_back(ww39toww32w50);
  ww39toww32.push_back(ww39toww32w52);
  ww39ptr->addWeichenstatus(ww32ptr,ww39toww32);

  QPair<Weiche*, bool> ww24tos52w40(w40ptr,true);
  QList<QPair<Weiche*, bool>> ww24tos52;
  ww24tos52.push_back(ww24tos52w40);
  ww24ptr->addWeichenstatusZuH(s52ptr->getS_id(),ww24tos52);
  
  QPair<Weiche*, bool> ww25tos52w40(w40ptr,false);
  QPair<Weiche*, bool> ww25tos52w42(w42ptr,false);
  QList<QPair<Weiche*, bool>> ww25tos52;
  ww25tos52.push_back(ww25tos52w40);
  ww25tos52.push_back(ww25tos52w42);
  ww25ptr->addWeichenstatusZuH(s52ptr->getS_id(),ww25tos52);
  
  QPair<Weiche*, bool> ww26tos52w40(w40ptr,false);
  QPair<Weiche*, bool> ww26tos52w42(w42ptr,true);
  QPair<Weiche*, bool> ww26tos52w44(w44ptr,false);
  QList<QPair<Weiche*, bool>> ww26tos52;
  ww26tos52.push_back(ww26tos52w40);
  ww26tos52.push_back(ww26tos52w42);
  ww26tos52.push_back(ww26tos52w44);
  ww26ptr->addWeichenstatusZuH(s52ptr->getS_id(),ww26tos52);
  
  QPair<Weiche*, bool> ww27tos52w40(w40ptr,false);
  QPair<Weiche*, bool> ww27tos52w42(w42ptr,true);
  QPair<Weiche*, bool> ww27tos52w44(w44ptr,true);
  QPair<Weiche*, bool> ww27tos52w46(w46ptr,false);
  QList<QPair<Weiche*, bool>> ww27tos52;
  ww27tos52.push_back(ww27tos52w40);
  ww27tos52.push_back(ww27tos52w42);
  ww27tos52.push_back(ww27tos52w44);
  ww27tos52.push_back(ww27tos52w46);
  ww27ptr->addWeichenstatusZuH(s52ptr->getS_id(),ww27tos52);
  
  QPair<Weiche*, bool> ww28tos52w40(w40ptr,false);
  QPair<Weiche*, bool> ww28tos52w42(w42ptr,true);
  QPair<Weiche*, bool> ww28tos52w44(w44ptr,true);
  QPair<Weiche*, bool> ww28tos52w46(w46ptr,true);
  QPair<Weiche*, bool> ww28tos52w48(w48ptr,false);
  QList<QPair<Weiche*, bool>> ww28tos52;
  ww28tos52.push_back(ww28tos52w40);
  ww28tos52.push_back(ww28tos52w42);
  ww28tos52.push_back(ww28tos52w44);
  ww28tos52.push_back(ww28tos52w46);
  ww28tos52.push_back(ww28tos52w48);
  ww28ptr->addWeichenstatusZuH(s52ptr->getS_id(),ww28tos52);
  
  QPair<Weiche*, bool> ww29tos52w40(w40ptr,false);
  QPair<Weiche*, bool> ww29tos52w42(w42ptr,true);
  QPair<Weiche*, bool> ww29tos52w44(w44ptr,true);
  QPair<Weiche*, bool> ww29tos52w46(w46ptr,true);
  QPair<Weiche*, bool> ww29tos52w48(w48ptr,true);
  QPair<Weiche*, bool> ww29tos52w50(w50ptr,false);
  QList<QPair<Weiche*, bool>> ww29tos52;
  ww29tos52.push_back(ww29tos52w40);
  ww29tos52.push_back(ww29tos52w42);
  ww29tos52.push_back(ww29tos52w44);
  ww29tos52.push_back(ww29tos52w46);
  ww29tos52.push_back(ww29tos52w48);
  ww29tos52.push_back(ww29tos52w50);
  ww29ptr->addWeichenstatusZuH(s52ptr->getS_id(),ww29tos52);
  
  QPair<Weiche*, bool> ww30tos52w40(w40ptr,false);
  QPair<Weiche*, bool> ww30tos52w42(w42ptr,true);
  QPair<Weiche*, bool> ww30tos52w44(w44ptr,true);
  QPair<Weiche*, bool> ww30tos52w46(w46ptr,true);
  QPair<Weiche*, bool> ww30tos52w48(w48ptr,true);
  QPair<Weiche*, bool> ww30tos52w50(w50ptr,true);
  QPair<Weiche*, bool> ww30tos52w52(w52ptr,false);
  QList<QPair<Weiche*, bool>> ww30tos52;
  ww30tos52.push_back(ww30tos52w40);
  ww30tos52.push_back(ww30tos52w42);
  ww30tos52.push_back(ww30tos52w44);
  ww30tos52.push_back(ww30tos52w46);
  ww30tos52.push_back(ww30tos52w48);
  ww30tos52.push_back(ww30tos52w50);
  ww30tos52.push_back(ww30tos52w52);
  ww30ptr->addWeichenstatusZuH(s52ptr->getS_id(),ww30tos52);
  
  QPair<Weiche*, bool> ww31tos52w40(w40ptr,false);
  QPair<Weiche*, bool> ww31tos52w42(w42ptr,true);
  QPair<Weiche*, bool> ww31tos52w44(w44ptr,true);
  QPair<Weiche*, bool> ww31tos52w46(w46ptr,true);
  QPair<Weiche*, bool> ww31tos52w48(w48ptr,true);
  QPair<Weiche*, bool> ww31tos52w50(w50ptr,true);
  QPair<Weiche*, bool> ww31tos52w52(w52ptr,true);
  QList<QPair<Weiche*, bool>> ww31tos52;
  ww31tos52.push_back(ww31tos52w40);
  ww31tos52.push_back(ww31tos52w42);
  ww31tos52.push_back(ww31tos52w44);
  ww31tos52.push_back(ww31tos52w46);
  ww31tos52.push_back(ww31tos52w48);
  ww31tos52.push_back(ww31tos52w50);
  ww31tos52.push_back(ww31tos52w52);
  ww31ptr->addWeichenstatusZuH(s52ptr->getS_id(),ww31tos52);

  QPair<Weiche*, bool> ww32tos51w53(w53ptr,true);
  QList<QPair<Weiche*, bool>> ww32tos51;
  ww32tos51.push_back(ww32tos51w53);
  ww32ptr->addWeichenstatusZuH(s51ptr->getS_id(),ww32tos51);
  
  QPair<Weiche*, bool> ww33tos51w53(w53ptr,false);
  QPair<Weiche*, bool> ww33tos51w51(w51ptr,false);
  QList<QPair<Weiche*, bool>> ww33tos51;
  ww33tos51.push_back(ww33tos51w53);
  ww33tos51.push_back(ww33tos51w51);
  ww33ptr->addWeichenstatusZuH(s51ptr->getS_id(),ww33tos51);
  
  QPair<Weiche*, bool> ww34tos51w53(w53ptr,false);
  QPair<Weiche*, bool> ww34tos51w51(w51ptr,true);
  QPair<Weiche*, bool> ww34tos51w49(w49ptr,false);
  QList<QPair<Weiche*, bool>> ww34tos51;
  ww34tos51.push_back(ww34tos51w53);
  ww34tos51.push_back(ww34tos51w51);
  ww34tos51.push_back(ww34tos51w49);
  ww34ptr->addWeichenstatusZuH(s51ptr->getS_id(),ww34tos51);
  
  QPair<Weiche*, bool> ww35tos51w53(w53ptr,false);
  QPair<Weiche*, bool> ww35tos51w51(w51ptr,true);
  QPair<Weiche*, bool> ww35tos51w49(w49ptr,true);
  QPair<Weiche*, bool> ww35tos51w47(w47ptr,false);
  QList<QPair<Weiche*, bool>> ww35tos51;
  ww35tos51.push_back(ww35tos51w53);
  ww35tos51.push_back(ww35tos51w51);
  ww35tos51.push_back(ww35tos51w49);
  ww35tos51.push_back(ww35tos51w47);
  ww35ptr->addWeichenstatusZuH(s51ptr->getS_id(),ww35tos51);
  
  QPair<Weiche*, bool> ww36tos51w53(w53ptr,false);
  QPair<Weiche*, bool> ww36tos51w51(w51ptr,true);
  QPair<Weiche*, bool> ww36tos51w49(w49ptr,true);
  QPair<Weiche*, bool> ww36tos51w47(w47ptr,true);
  QPair<Weiche*, bool> ww36tos51w45(w45ptr,false);
  QList<QPair<Weiche*, bool>> ww36tos51;
  ww36tos51.push_back(ww36tos51w53);
  ww36tos51.push_back(ww36tos51w51);
  ww36tos51.push_back(ww36tos51w49);
  ww36tos51.push_back(ww36tos51w47);
  ww36tos51.push_back(ww36tos51w45);
  ww36ptr->addWeichenstatusZuH(s51ptr->getS_id(),ww36tos51);
  
  QPair<Weiche*, bool> ww37tos51w53(w53ptr,false);
  QPair<Weiche*, bool> ww37tos51w51(w51ptr,true);
  QPair<Weiche*, bool> ww37tos51w49(w49ptr,true);
  QPair<Weiche*, bool> ww37tos51w47(w47ptr,true);
  QPair<Weiche*, bool> ww37tos51w45(w45ptr,true);
  QPair<Weiche*, bool> ww37tos51w43(w43ptr,false);
  QList<QPair<Weiche*, bool>> ww37tos51;
  ww37tos51.push_back(ww37tos51w53);
  ww37tos51.push_back(ww37tos51w51);
  ww37tos51.push_back(ww37tos51w49);
  ww37tos51.push_back(ww37tos51w47);
  ww37tos51.push_back(ww37tos51w45);
  ww37tos51.push_back(ww37tos51w43);
  ww37ptr->addWeichenstatusZuH(s51ptr->getS_id(),ww37tos51);
  
  QPair<Weiche*, bool> ww38tos51w53(w53ptr,false);
  QPair<Weiche*, bool> ww38tos51w51(w51ptr,true);
  QPair<Weiche*, bool> ww38tos51w49(w49ptr,true);
  QPair<Weiche*, bool> ww38tos51w47(w47ptr,true);
  QPair<Weiche*, bool> ww38tos51w45(w45ptr,true);
  QPair<Weiche*, bool> ww38tos51w43(w43ptr,true);
  QPair<Weiche*, bool> ww38tos51w41(w41ptr,false);
  QList<QPair<Weiche*, bool>> ww38tos51;
  ww38tos51.push_back(ww38tos51w53);
  ww38tos51.push_back(ww38tos51w51);
  ww38tos51.push_back(ww38tos51w49);
  ww38tos51.push_back(ww38tos51w47);
  ww38tos51.push_back(ww38tos51w45);
  ww38tos51.push_back(ww38tos51w43);
  ww38tos51.push_back(ww38tos51w41);
  ww38ptr->addWeichenstatusZuH(s51ptr->getS_id(),ww38tos51);
  
  QPair<Weiche*, bool> ww41tos51w53(w53ptr,false);
  QPair<Weiche*, bool> ww41tos51w51(w51ptr,true);
  QPair<Weiche*, bool> ww41tos51w49(w49ptr,true);
  QPair<Weiche*, bool> ww41tos51w47(w47ptr,true);
  QPair<Weiche*, bool> ww41tos51w45(w45ptr,true);
  QPair<Weiche*, bool> ww41tos51w43(w43ptr,true);
  QPair<Weiche*, bool> ww41tos51w41(w41ptr,true);
  QList<QPair<Weiche*, bool>> ww41tos51;
  ww41tos51.push_back(ww41tos51w53);
  ww41tos51.push_back(ww41tos51w51);
  ww41tos51.push_back(ww41tos51w49);
  ww41tos51.push_back(ww41tos51w47);
  ww41tos51.push_back(ww41tos51w45);
  ww41tos51.push_back(ww41tos51w43);
  ww41tos51.push_back(ww41tos51w41);
  ww41ptr->addWeichenstatusZuH(s51ptr->getS_id(),ww41tos51);

  QList<Block*> ww39toww41b;
  ww39toww41b.push_back(bwptr);
  ww39ptr->addBlock(ww41ptr,ww39toww41b);

  QList<Block*> ww39toww38b;
  ww39toww38b.push_back(bvptr);
  ww39ptr->addBlock(ww38ptr,ww39toww38b);

  QList<Block*> ww39toww37b;
  ww39toww37b.push_back(buptr);
  ww39ptr->addBlock(ww37ptr,ww39toww37b);

  QList<Block*> ww39toww36b;
  ww39toww36b.push_back(btptr);
  ww39ptr->addBlock(ww36ptr,ww39toww36b);

  QList<Block*> ww39toww35b;
  ww39toww35b.push_back(bsptr);
  ww39ptr->addBlock(ww35ptr,ww39toww35b);

  QList<Block*> ww39toww34b;
  ww39toww34b.push_back(brptr);
  ww39ptr->addBlock(ww34ptr,ww39toww34b);

  QList<Block*> ww39toww33b;
  ww39toww33b.push_back(bqptr);
  ww39ptr->addBlock(ww33ptr,ww39toww33b);

  QList<Block*> ww39toww32b;
  ww39toww32b.push_back(bpptr);
  ww39ptr->addBlock(ww32ptr,ww39toww32b);

  QList<Block*> ww40toww24b;
  ww40toww24b.push_back(bwptr);
  ww40ptr->addBlock(ww24ptr,ww40toww24b);

  QList<Block*> ww40toww25b;
  ww40toww25b.push_back(bvptr);
  ww40ptr->addBlock(ww25ptr,ww40toww25b);

  QList<Block*> ww40toww26b;
  ww40toww26b.push_back(buptr);
  ww40ptr->addBlock(ww26ptr,ww40toww26b);

  QList<Block*> ww40toww27b;
  ww40toww27b.push_back(btptr);
  ww40ptr->addBlock(ww27ptr,ww40toww27b);

  QList<Block*> ww40toww28b;
  ww40toww28b.push_back(bsptr);
  ww40ptr->addBlock(ww28ptr,ww40toww28b);

  QList<Block*> ww40toww29b;
  ww40toww29b.push_back(brptr);
  ww40ptr->addBlock(ww29ptr,ww40toww29b);

  QList<Block*> ww40toww30b;
  ww40toww30b.push_back(bqptr);
  ww40ptr->addBlock(ww30ptr,ww40toww30b);

  QList<Block*> ww40toww31b;
  ww40toww31b.push_back(bpptr);
  ww40ptr->addBlock(ww31ptr,ww40toww31b);

  QList<Block*> s51tos49b;
  s51tos49b.push_back(byptr);
  s51tos49b.push_back(bxptr);
  s51ptr->addBlock(s49ptr,s51tos49b);
  QList<Block*> s51toww42b;
  s51toww42b.push_back(byptr);
  s51ptr->addBlockZuRangier(ww42ptr,s51toww42b);

  QList<Block*> s52tos10b;
  s52tos10b.push_back(bzptr);
  s52tos10b.push_back(ahptr);
  s52ptr->addBlock(s10ptr,s52tos10b);
  QList<Block*> s52toww43b;
  s52toww43b.push_back(bzptr);
  s52ptr->addBlockZuRangier(ww43ptr,s52toww43b);

  QList<Block*> s9toww39b;
  s9toww39b.push_back(bzptr);
  s9ptr->addBlockZuRangier(ww39ptr,s9toww39b);

  QList<Block*> s50toww40b;
  s50toww40b.push_back(byptr);
  s50ptr->addBlockZuRangier(ww40ptr,s50toww40b);
  

  
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
  scene->addItem(w12rectge);
  QGraphicsRectItem *w12rectab = new QGraphicsRectItem();
  w12rectab->setRect(0,0,10,50);
  w12rectab->setPos(QPointF(415,-110));
  w12rectab->setBrush(QColor(79,79,79));
  w12rectab->setRotation(45);
  scene->addItem(w12rectab);
  QLabel *w12label = new QLabel();
  scene->addWidget(w12label);

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

  WeicheQPushButton *w12push = new WeicheQPushButton();
  scene->addWidget(w12push);
  w12ptr->addWeichenitem(w12rectab,w12rectge,w12label,w12push);
  w12ptr->moveButton(410,-130);
  w12ptr->moveLabel(425,-110);
  
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
  WeicheQPushButton *w11push = new WeicheQPushButton();
  scene->addWidget(w11push);
  w11ptr->addWeichenitem(w11rectab,w11rectge,w11label,w11push);
  w11ptr->moveButton(280,-615);
  w11ptr->moveLabel(280,-625);

  QGraphicsRectItem *acrect4 = new QGraphicsRectItem();
  acrect4->setRect(0,0,10,500);
  acrect4->setPos(QPointF(220,-610));
  acrect4->setBrush(QColor(79,79,79));
  acrect4->setRotation(90);
  acptr->addBlockitems(acrect4);
  scene->addItem(acrect4);

  QGraphicsRectItem *aerect3 = new QGraphicsRectItem();
  aerect3->setRect(0,0,10,510);
  aerect3->setPos(QPointF(230,-540));
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
  WeicheQPushButton *w13push = new WeicheQPushButton();
  scene->addWidget(w13push);
  w13ptr->addWeichenitem(w13rectab,w13rectge,w13label,w13push);
  w13ptr->moveButton(310,-80);
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
  WeicheQPushButton *w14push = new WeicheQPushButton();
  w14ptr->addWeichenitem(w14rectab,w14rectge,w14label,w14push);
  w14ptr->moveButton(230,-165);
  w14ptr->moveLabel(250,-150);
  w15ptr->addWeichenitem(w15rectab,w15rectge,w15label,w14push);
  w15ptr->moveLabel(240,-175);

  scene->addWidget(w14push);
  
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
  WeicheQPushButton *w19push = new WeicheQPushButton();
  scene->addWidget(w19push);
  w19ptr->addWeichenitem(w19rectab,w19rectge,w19label,w19push);
  w19ptr->moveButton(-595,-5);
  w19ptr->moveLabel(-605,10);

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
  WeicheQPushButton *w17push = new WeicheQPushButton();
  scene->addWidget(w17push);
  w18ptr->addWeichenitem(w18rectab,w18rectge,w18label,w17push);
  w18ptr->moveButton(-670,-80);
  w18ptr->moveLabel(-620,-65);
  QLabel *w17label = new QLabel();
  scene->addWidget(w17label);
  w17ptr->addWeichenitem(w17rectab,w17rectge,w17label,w17push);
  w17ptr->moveLabel(-700,-90);

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
  arrect4->setRect(0,0,10,210);
  arrect4->setPos(QPointF(-870,-300));
  arrect4->setBrush(QColor(79,79,79));
  arrect4->setRotation(90);
  arptr->addBlockitems(arrect4);
  scene->addItem(arrect4);

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
  WeicheQPushButton *w16push = new WeicheQPushButton();
  scene->addWidget(w16push);
  w16ptr->addWeichenitem(w16rectab,w16rectge,w16label,w16push);
  w16ptr->moveButton(-750,-165);
  w16ptr->moveLabel(-765,-150);

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

  QGraphicsSvgItem *s14item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s14item1);
  s14item1->setPos(QPointF(230,-590));
  s14item1->setScale(0.36);
  s14item1->setRotation(90);
  s14item1->setElementId("basic_hp0");
  s14item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s14push = new HSignalQPushButton();
  scene->addWidget(s14push);
  QLabel *s14label = new QLabel();
  scene->addWidget(s14label);
  //speicher GUIs
  QGraphicsRectItem *sphin14 = new QGraphicsRectItem();
  sphin14->setRect(0,0,10,10);
  sphin14->setBrush(Qt::yellow);
  sphin14->setPos(QPointF(155,-590));
  QGraphicsRectItem *spweg14 = new QGraphicsRectItem();
  spweg14->setRect(0,0,10,10);
  spweg14->setBrush(Qt::yellow);
  spweg14->setPos(QPointF(155,-580));
  scene->addItem(spweg14);
  scene->addItem(sphin14);
  s14ptr->addHSignalitem(s14item1, s14label,s14push,spweg14,sphin14);
  s14ptr->moveButton(200,-615);
  s14ptr->moveLabel(120,-590);

  QGraphicsSvgItem *s15item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s15item1);
  s15item1->setPos(QPointF(230,-520));
  s15item1->setScale(0.36);
  s15item1->setRotation(90);
  s15item1->setElementId("basic_hp0");
  s15item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s15push = new HSignalQPushButton();
  scene->addWidget(s15push);
  QLabel *s15label = new QLabel();
  scene->addWidget(s15label);
  //speicher GUIs
  QGraphicsRectItem *sphin15 = new QGraphicsRectItem();
  sphin15->setRect(0,0,10,10);
  sphin15->setBrush(Qt::yellow);
  sphin15->setPos(QPointF(155,-520));
  QGraphicsRectItem *spweg15 = new QGraphicsRectItem();
  spweg15->setRect(0,0,10,10);
  spweg15->setBrush(Qt::yellow);
  spweg15->setPos(QPointF(155,-510));
  scene->addItem(spweg15);
  scene->addItem(sphin15);
  s15ptr->addHSignalitem(s15item1, s15label,s15push,spweg15,sphin15);
  s15ptr->moveButton(200,-545);
  s15ptr->moveLabel(120,-520);

  QGraphicsSvgItem *itemv111 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv111);
  itemv111->setPos(QPointF(405,-350));
  itemv111->setScale(0.36);
  itemv111->setRotation(180);
  itemv111->setElementId("basic_vr0");
  itemv111->setObjectName("basic_vr0");
  v11ptr->addVSignalitems(itemv111);

  QGraphicsSvgItem *s18item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s18item1);
  s18item1->setPos(QPointF(220,-140));
  s18item1->setScale(0.36);
  s18item1->setRotation(90);
  s18item1->setElementId("basic_hp0");
  s18item1->setObjectName("basic_hp0");
  HSignalQPushButton *s18push = new HSignalQPushButton();
  scene->addWidget(s18push);
  QLabel *s18label = new QLabel();
  scene->addWidget(s18label);
  //speicher GUIs
  QGraphicsRectItem *sphin18 = new QGraphicsRectItem();
  sphin18->setRect(0,0,10,10);
  sphin18->setBrush(Qt::yellow);
  sphin18->setPos(QPointF(145,-140));
  QGraphicsRectItem *spweg18 = new QGraphicsRectItem();
  spweg18->setRect(0,0,10,10);
  spweg18->setBrush(Qt::yellow);
  spweg18->setPos(QPointF(145,-130));
  scene->addItem(spweg18);
  scene->addItem(sphin18);
  s18ptr->addHSignalitem(s18item1, s18label,s18push,spweg18,sphin18);
  s18ptr->moveButton(170,-165);
  s18ptr->moveLabel(110,-140);

  QGraphicsSvgItem *s17item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s17item1);
  s17item1->setPos(QPointF(270,-55));
  s17item1->setScale(0.36);
  s17item1->setRotation(90);
  s17item1->setElementId("basic_hp0");
  s17item1->setObjectName("basic_hp0");
  HSignalQPushButton *s17push = new HSignalQPushButton();
  scene->addWidget(s17push);
  QLabel *s17label = new QLabel();
  scene->addWidget(s17label);
  //speicher GUIs
  QGraphicsRectItem *sphin17 = new QGraphicsRectItem();
  sphin17->setRect(0,0,10,10);
  sphin17->setBrush(Qt::yellow);
  sphin17->setPos(QPointF(185,-55));
  QGraphicsRectItem *spweg17 = new QGraphicsRectItem();
  spweg17->setRect(0,0,10,10);
  spweg17->setBrush(Qt::yellow);
  spweg17->setPos(QPointF(185,-45));
  scene->addItem(spweg17);
  scene->addItem(sphin17);
  s17ptr->addHSignalitem(s17item1, s17label,s17push,spweg17,sphin17);
  s17ptr->moveButton(235,-80);
  s17ptr->moveLabel(155,-55);

  QGraphicsSvgItem *s16item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s16item1);
  s16item1->setPos(QPointF(420,25));
  s16item1->setScale(0.36);
  s16item1->setRotation(90);
  s16item1->setElementId("basic_hp0");
  s16item1->setObjectName("basic_hp0");
  HSignalQPushButton *s16push = new HSignalQPushButton();
  scene->addWidget(s16push);
  QLabel *s16label = new QLabel();
  scene->addWidget(s16label);
  //speicher GUIs
  QGraphicsRectItem *sphin16 = new QGraphicsRectItem();
  sphin16->setRect(0,0,10,10);
  sphin16->setBrush(Qt::yellow);
  sphin16->setPos(QPointF(340,25));
  QGraphicsRectItem *spweg16 = new QGraphicsRectItem();
  spweg16->setRect(0,0,10,10);
  spweg16->setBrush(Qt::yellow);
  spweg16->setPos(QPointF(340,35));
  scene->addItem(spweg16);
  scene->addItem(sphin16);
  s16ptr->addHSignalitem(s16item1, s16label,s16push,spweg16,sphin16);
  s16ptr->moveButton(390,-5);
  s16ptr->moveLabel(310,30);

  QGraphicsSvgItem *item141 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(item141);
  item141->setPos(QPointF(-660,20));
  item141->setScale(0.36);
  item141->setRotation(90);
  item141->setElementId("basic_vr0");
  item141->setObjectName("basic_vr0");
  v14ptr->addVSignalitems(item141);
  
  QGraphicsSvgItem *s22item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s22item1);
  s22item1->setPos(QPointF(-620,25));
  s22item1->setScale(0.36);
  s22item1->setRotation(90);
  s22item1->setElementId("basic_hp0");
  s22item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s22push = new HSignalQPushButton();
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
  s22ptr->addHSignalitem(s22item1, s22label,s22push,spweg22,sphin22);
  s22ptr->moveButton(-650,-5);
  s22ptr->moveLabel(-730,30);

  QGraphicsSvgItem *itemv151 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv151);
  itemv151->setPos(QPointF(-545,-5));
  itemv151->setScale(0.36);
  itemv151->setRotation(-90);
  itemv151->setElementId("basic_vr0");
  itemv151->setObjectName("basic_vr0");
  v15ptr->addVSignalitems(itemv151);
  
  QGraphicsSvgItem *s21item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s21item1);
  s21item1->setPos(QPointF(-585,-10));
  s21item1->setScale(0.36);
  s21item1->setRotation(-90);
  s21item1->setElementId("basic_hp0");
  s21item1->setObjectName("basic_hp0");
  HSignalQPushButton *s21push = new HSignalQPushButton();
  scene->addWidget(s21push);
  QLabel *s21label = new QLabel();
  scene->addWidget(s21label);
  //speicher GUIs
  QGraphicsRectItem *sphin21 = new QGraphicsRectItem();
  sphin21->setRect(0,0,10,10);
  sphin21->setBrush(Qt::yellow);
  sphin21->setPos(QPointF(-505,-20));
  QGraphicsRectItem *spweg21 = new QGraphicsRectItem();
  spweg21->setRect(0,0,10,10);
  spweg21->setBrush(Qt::yellow);
  spweg21->setPos(QPointF(-505,-30));
  scene->addItem(spweg21);
  scene->addItem(sphin21);
  s21ptr->addHSignalitem(s21item1, s21label,s21push,spweg21,sphin21);
  s21ptr->moveButton(-570,-5);
  s21ptr->moveLabel(-490,-25);

  QGraphicsSvgItem *itemv131 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv131);
  itemv131->setPos(QPointF(-585,-80));
  itemv131->setScale(0.36);
  itemv131->setRotation(-90);
  itemv131->setElementId("basic_vr0");
  itemv131->setObjectName("basic_vr0");
  v13ptr->addVSignalitems(itemv131);
  
  QGraphicsSvgItem *s20item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s20item1);
  s20item1->setPos(QPointF(-625,-85));
  s20item1->setScale(0.36);
  s20item1->setRotation(-90);
  s20item1->setElementId("basic_hp0");
  s20item1->setObjectName("basic_hp0");
  HSignalQPushButton *s20push = new HSignalQPushButton();
  scene->addWidget(s20push);
  QLabel *s20label = new QLabel();
  scene->addWidget(s20label);
  //speicher GUIs
  QGraphicsRectItem *sphin20 = new QGraphicsRectItem();
  sphin20->setRect(0,0,10,10);
  sphin20->setBrush(Qt::yellow);
  sphin20->setPos(QPointF(-545,-95));
  QGraphicsRectItem *spweg20 = new QGraphicsRectItem();
  spweg20->setRect(0,0,10,10);
  spweg20->setBrush(Qt::yellow);
  spweg20->setPos(QPointF(-545,-105));
  scene->addItem(spweg20);
  scene->addItem(sphin20);
  s20ptr->addHSignalitem(s20item1, s20label,s20push,spweg20,sphin20);
  s20ptr->moveButton(-600,-80);
  s20ptr->moveLabel(-530,-100);

  
  QGraphicsSvgItem *itemv121 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv121);
  itemv121->setPos(QPointF(-665,-165));
  itemv121->setScale(0.36);
  itemv121->setRotation(-90);
  itemv121->setElementId("basic_vr0");
  itemv121->setObjectName("basic_vr0");
  v12ptr->addVSignalitems(itemv121);
  
  QGraphicsSvgItem *s19item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s19item1);
  s19item1->setPos(QPointF(-705,-170));
  s19item1->setScale(0.36);
  s19item1->setRotation(-90);
  s19item1->setElementId("basic_hp0");
  s19item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s19push = new HSignalQPushButton();
  scene->addWidget(s19push);
  QLabel *s19label = new QLabel();
  scene->addWidget(s19label);
  //speicher GUIs
  QGraphicsRectItem *sphin19 = new QGraphicsRectItem();
  sphin19->setRect(0,0,10,10);
  sphin19->setBrush(Qt::yellow);
  sphin19->setPos(QPointF(-625,-180));
  QGraphicsRectItem *spweg19 = new QGraphicsRectItem();
  spweg19->setRect(0,0,10,10);
  spweg19->setBrush(Qt::yellow);
  spweg19->setPos(QPointF(-625,-190));
  scene->addItem(spweg19);
  scene->addItem(sphin19);
  s19ptr->addHSignalitem(s19item1, s19label,s19push,spweg19,sphin19);
  s19ptr->moveButton(-690,-165);
  s19ptr->moveLabel(-610,-185);

  QGraphicsSvgItem *itemv161 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv161);
  itemv161->setPos(QPointF(-800,-200));
  itemv161->setScale(0.36);
  itemv161->setRotation(135);
  itemv161->setElementId("basic_vr0");
  itemv161->setObjectName("basic_vr0");
  v16ptr->addVSignalitems(itemv161);
  
  QGraphicsSvgItem *s23item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s23item1);
  s23item1->setPos(QPointF(-775,-170));
  s23item1->setScale(0.36);
  s23item1->setRotation(135);
  s23item1->setElementId("basic_hp0");
  s23item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s23push = new HSignalQPushButton();
  scene->addWidget(s23push);
  QLabel *s23label = new QLabel();
  scene->addWidget(s23label);
  //speicher GUIs
  QGraphicsRectItem *sphin23 = new QGraphicsRectItem();
  sphin23->setRect(0,0,10,10);
  sphin23->setBrush(Qt::yellow);
  sphin23->setPos(QPointF(-825,-220));
  QGraphicsRectItem *spweg23 = new QGraphicsRectItem();
  spweg23->setRect(0,0,10,10);
  spweg23->setBrush(Qt::yellow);
  spweg23->setPos(QPointF(-835,-210));
  spweg23->setRotation(135);
  sphin23->setRotation(135);
  scene->addItem(spweg23);
  scene->addItem(sphin23);
  s23ptr->addHSignalitem(s23item1, s23label,s23push,spweg23,sphin23);
  s23ptr->moveButton(-770,-185);
  s23ptr->moveLabel(-865,-240);

  //WS
  WSignalQPushButton *ww4push = new WSignalQPushButton();
  scene->addWidget(ww4push);
  ww4ptr->addButtonAndLabel(ww4label,ww4push);
  ww4ptr->moveButton(320,-615);
  ww4ptr->moveLabel(350,-630);

  WSignalQPushButton *ww9push = new WSignalQPushButton();
  scene->addWidget(ww9push);
  ww9ptr->addButtonAndLabel(ww9label,ww9push);
  ww9ptr->moveButton(-30,-245);
  ww9ptr->moveLabel(0,-280);

  WSignalQPushButton *ww6push = new WSignalQPushButton();
  scene->addWidget(ww6push);
  ww6ptr->addButtonAndLabel(ww6label,ww6push);
  ww6ptr->moveButton(280,-165);
  ww6ptr->moveLabel(310,-200);

  WSignalQPushButton *ww11push = new WSignalQPushButton();
  scene->addWidget(ww11push);
  ww11ptr->addButtonAndLabel(ww11label,ww11push);
  ww11ptr->moveButton(-900,-165);
  ww11ptr->moveLabel(-850,-155);

  WSignalQPushButton *ww10push = new WSignalQPushButton();
  scene->addWidget(ww10push);
  ww10ptr->addButtonAndLabel(ww10label,ww10push);
  ww10ptr->moveButton(-710,-80);
  ww10ptr->moveLabel(-740,-60);

  WSignalQPushButton *ww7push = new WSignalQPushButton();
  scene->addWidget(ww7push);
  ww7ptr->addButtonAndLabel(ww7label,ww7push);
  ww7ptr->moveButton(380,-165);
  ww7ptr->moveLabel(335,-140);

  WSignalQPushButton *ww5push = new WSignalQPushButton();
  scene->addWidget(ww5push);
  ww5ptr->addButtonAndLabel(ww5label,ww5push);
  ww5ptr->moveButton(410,-150);
  ww5ptr->moveLabel(375,-210);

  WSignalQPushButton *ww8push = new WSignalQPushButton();
  scene->addWidget(ww8push);
  ww8ptr->addButtonAndLabel(ww8label,ww8push);
  ww8ptr->moveButton(190,-205);
  ww8ptr->moveLabel(135,-210);

  WSignalQPushButton *ww3push = new WSignalQPushButton();
  scene->addWidget(ww3push);
  ww3ptr->addButtonAndLabel(ww3label,ww3push);
  ww3ptr->moveButton(240,265);
  ww3ptr->moveLabel(270,250);

  
  //new part of the model railway--------------------------------------------
  QGraphicsRectItem *asrect2 = new QGraphicsRectItem();
  asrect2->setRect(0,0,10,190);
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
  aurect2->setRect(0,0,10,335);
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

  QGraphicsRectItem *axrect = new QGraphicsRectItem();
  axrect->setRect(0,0,10,1585);
  axrect->setPos(QPointF(2505,-540));
  axrect->setBrush(QColor(79,79,79));
  axrect->setRotation(90);
  axptr->addBlockitems(axrect);
  scene->addItem(axrect);

  QGraphicsRectItem *w20rectge = new QGraphicsRectItem();
  w20rectge->setRect(0,0,10,75);
  w20rectge->setPos(QPointF(925,-610));
  w20rectge->setBrush(QColor(79,79,79));
  w20rectge->setRotation(90);
  scene->addItem(w20rectge);
  QGraphicsRectItem *w20rectab = new QGraphicsRectItem();
  w20rectab->setRect(0,0,10,50);
  w20rectab->setPos(QPointF(850,-600));
  w20rectab->setBrush(QColor(79,79,79));
  w20rectab->setRotation(315);
  scene->addItem(w20rectab);
  QLabel *w20label = new QLabel();
  scene->addWidget(w20label);
  WeicheQPushButton *w20push = new WeicheQPushButton();
  scene->addWidget(w20push);
  w20ptr->addWeichenitem(w20rectab,w20rectge,w20label,w20push);
  w20ptr->moveButton(850,-615);
  w20ptr->moveLabel(850,-625);

  QGraphicsRectItem *w21rectge = new QGraphicsRectItem();
  w21rectge->setRect(0,0,10,75);
  w21rectge->setPos(QPointF(925,-540));
  w21rectge->setBrush(QColor(79,79,79));
  w21rectge->setRotation(90);
  scene->addItem(w21rectge);
  QGraphicsRectItem *w21rectab = new QGraphicsRectItem();
  w21rectab->setRect(0,0,10,50);
  w21rectab->setPos(QPointF(925,-540));
  w21rectab->setBrush(QColor(79,79,79));
  w21rectab->setRotation(135);
  scene->addItem(w21rectab);
  QLabel *w21label = new QLabel();
  scene->addWidget(w21label);
  WeicheQPushButton *w21push = new WeicheQPushButton();
  scene->addWidget(w21push);
  w21ptr->addWeichenitem(w21rectab,w21rectge,w21label,w21push);
  w21ptr->moveButton(910,-545);
  w21ptr->moveLabel(880,-530);
  
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

  QGraphicsRectItem *w23rectge = new QGraphicsRectItem();
  w23rectge->setRect(0,0,10,75);
  w23rectge->setPos(QPointF(2580,-540));
  w23rectge->setBrush(QColor(79,79,79));
  w23rectge->setRotation(90);
  scene->addItem(w23rectge);
  QGraphicsRectItem *w23rectab = new QGraphicsRectItem();
  w23rectab->setRect(0,0,10,75);
  w23rectab->setPos(QPointF(2575,-540));
  w23rectab->setBrush(QColor(79,79,79));
  w23rectab->setRotation(45);
  scene->addItem(w23rectab);
  QLabel *w23label = new QLabel();
  scene->addWidget(w23label);
  WeicheQPushButton *w23push = new WeicheQPushButton();
  scene->addWidget(w23push);
  w23ptr->addWeichenitem(w23rectab,w23rectge,w23label,w23push);
  w23ptr->moveButton(2560,-545);
  w23ptr->moveLabel(2520,-555);

  QGraphicsRectItem *azrect = new QGraphicsRectItem();
  azrect->setRect(0,0,10,80);
  azrect->setPos(QPointF(2660,-540));
  azrect->setBrush(QColor(79,79,79));
  azrect->setRotation(90);
  azptr->addBlockitems(azrect);
  scene->addItem(azrect);

  QGraphicsRectItem *azrect2 = new QGraphicsRectItem();
  azrect2->setRect(0,0,10,920);
  azrect2->setPos(QPointF(2660,-540));
  azrect2->setBrush(QColor(79,79,79));
  azptr->addBlockitems(azrect2);
  scene->addItem(azrect2);

  QGraphicsRectItem *ayrect2 = new QGraphicsRectItem();
  ayrect2->setRect(0,0,10,1060);
  ayrect2->setPos(QPointF(2730,-610));
  ayrect2->setBrush(QColor(79,79,79));
  ayptr->addBlockitems(ayrect2);
  scene->addItem(ayrect2);

  QGraphicsRectItem *borect = new QGraphicsRectItem();
  borect->setRect(0,0,10,40);
  borect->setPos(QPointF(790,-435));
  borect->setBrush(QColor(79,79,79));
  borect->setRotation(90);
  boptr->addBlockitems(borect);
  scene->addItem(borect);

  QGraphicsRectItem *bcrect = new QGraphicsRectItem();
  bcrect->setRect(0,0,10,1640);
  bcrect->setPos(QPointF(2505,-435));
  bcrect->setBrush(QColor(79,79,79));
  bcrect->setRotation(90);
  bcptr->addBlockitems(bcrect);
  scene->addItem(bcrect);

  QGraphicsRectItem *w22rectge = new QGraphicsRectItem();
  w22rectge->setRect(0,0,10,75);
  w22rectge->setPos(QPointF(865,-435));
  w22rectge->setBrush(QColor(79,79,79));
  w22rectge->setRotation(90);
  scene->addItem(w22rectge);
  QGraphicsRectItem *w22rectab = new QGraphicsRectItem();
  w22rectab->setRect(0,0,10,50);
  w22rectab->setPos(QPointF(865,-435));
  w22rectab->setBrush(QColor(79,79,79));
  w22rectab->setRotation(45);
  scene->addItem(w22rectab);
  QLabel *w22label = new QLabel();
  scene->addWidget(w22label);
  WeicheQPushButton *w22push = new WeicheQPushButton();
  scene->addWidget(w22push);
  w22ptr->addWeichenitem(w22rectab,w22rectge,w22label,w22push);
  w22ptr->moveButton(860,-440);
  w22ptr->moveLabel(800,-450);

  QGraphicsRectItem *w24rectge = new QGraphicsRectItem();
  w24rectge->setRect(0,0,10,75);
  w24rectge->setPos(QPointF(2545,-435));
  w24rectge->setBrush(QColor(79,79,79));
  w24rectge->setRotation(90);
  scene->addItem(w24rectge);
  QGraphicsRectItem *w24rectab = new QGraphicsRectItem();
  w24rectab->setRect(0,0,10,80);
  w24rectab->setPos(QPointF(2475,-425));
  w24rectab->setBrush(QColor(79,79,79));
  w24rectab->setRotation(-135);
  scene->addItem(w24rectab);
  QLabel *w24label = new QLabel();
  scene->addWidget(w24label);
  WeicheQPushButton *w24push = new WeicheQPushButton();
  scene->addWidget(w24push);
  w24ptr->addWeichenitem(w24rectab,w24rectge,w24label,w24push);
  w24ptr->moveButton(2465,-440);
  w24ptr->moveLabel(2475,-425);

  QGraphicsRectItem *blrect = new QGraphicsRectItem();
  blrect->setRect(0,0,10,745);
  blrect->setPos(QPointF(2545,-435));
  blrect->setBrush(QColor(79,79,79));
  blptr->addBlockitems(blrect);
  scene->addItem(blrect);

  QGraphicsRectItem *cbrect = new QGraphicsRectItem();
  cbrect->setRect(0,0,10,765);
  cbrect->setPos(QPointF(2555,300));
  cbrect->setBrush(QColor(79,79,79));
  cbrect->setRotation(90);
  cbptr->addBlockitems(cbrect);
  scene->addItem(cbrect);

  QGraphicsRectItem *bmrect3 = new QGraphicsRectItem();
  bmrect3->setRect(0,0,10,50);
  bmrect3->setPos(QPointF(1790,300));
  bmrect3->setBrush(QColor(79,79,79));
  bmptr->addBlockitems(bmrect3);
  scene->addItem(bmrect3);

  QGraphicsRectItem *bmrect4 = new QGraphicsRectItem();
  bmrect4->setRect(0,0,10,80);
  bmrect4->setPos(QPointF(1790,470));
  bmrect4->setBrush(QColor(79,79,79));
  bmptr->addBlockitems(bmrect4);
  scene->addItem(bmrect4);

  QGraphicsRectItem *bmrect2 = new QGraphicsRectItem();
  bmrect2->setRect(0,0,10,330);
  bmrect2->setPos(QPointF(1790,550));
  bmrect2->setBrush(QColor(79,79,79));
  bmptr->addBlockitems(bmrect2);
  scene->addItem(bmrect2);

  QGraphicsRectItem *ayrect3 = new QGraphicsRectItem();
  ayrect3->setRect(0,0,10,1370);
  ayrect3->setPos(QPointF(2740,450));
  ayrect3->setBrush(QColor(79,79,79));
  ayrect3->setRotation(90);
  ayptr->addBlockitems(ayrect3);
  scene->addItem(ayrect3);

  QGraphicsRectItem *azrect3 = new QGraphicsRectItem();
  azrect3->setRect(0,0,10,1300);
  azrect3->setPos(QPointF(2670,380));
  azrect3->setBrush(QColor(79,79,79));
  azrect3->setRotation(90);
  azptr->addBlockitems(azrect3);
  scene->addItem(azrect3);

  QGraphicsRectItem *bbrect = new QGraphicsRectItem();
  bbrect->setRect(0,0,10,530);
  bbrect->setPos(QPointF(1370,450));
  bbrect->setBrush(QColor(79,79,79));
  bbrect->setRotation(90);
  bbptr->addBlockitems(bbrect);
  scene->addItem(bbrect);

  QGraphicsRectItem *bbrect2 = new QGraphicsRectItem();
  bbrect2->setRect(0,0,10,860);
  bbrect2->setPos(QPointF(830,-400));
  bbrect2->setBrush(QColor(79,79,79));
  bbptr->addBlockitems(bbrect2);
  scene->addItem(bbrect2);


  QGraphicsRectItem *barect = new QGraphicsRectItem();
  barect->setRect(0,0,10,450);
  barect->setPos(QPointF(1370,380));
  barect->setBrush(QColor(79,79,79));
  barect->setRotation(90);
  baptr->addBlockitems(barect);
  scene->addItem(barect);

  QGraphicsRectItem *barect2 = new QGraphicsRectItem();
  barect2->setRect(0,0,10,755);
  barect2->setPos(QPointF(910,-365));
  barect2->setBrush(QColor(79,79,79));
  baptr->addBlockitems(barect2);
  scene->addItem(barect2);

  QGraphicsRectItem *barect3 = new QGraphicsRectItem();
  barect3->setRect(0,0,10,150);
  barect3->setPos(QPointF(1060,-365));
  barect3->setBrush(QColor(79,79,79));
  barect3->setRotation(90);
  baptr->addBlockitems(barect3);
  scene->addItem(barect3);

  QGraphicsRectItem *bfrect = new QGraphicsRectItem();
  bfrect->setRect(0,0,10,800);
  bfrect->setPos(QPointF(2010,-220));
  bfrect->setBrush(QColor(79,79,79));
  bfrect->setRotation(90);
  bfptr->addBlockitems(bfrect);
  scene->addItem(bfrect);

  QGraphicsRectItem *w30rectge = new QGraphicsRectItem();
  w30rectge->setRect(0,0,10,75);
  w30rectge->setPos(QPointF(1135,-365));
  w30rectge->setBrush(QColor(79,79,79));
  w30rectge->setRotation(90);
  scene->addItem(w30rectge);
  QGraphicsRectItem *w30rectab = new QGraphicsRectItem();
  w30rectab->setRect(0,0,10,102);
  w30rectab->setPos(QPointF(1060,-355));
  w30rectab->setBrush(QColor(79,79,79));
  w30rectab->setRotation(315);
  scene->addItem(w30rectab);
  QLabel *w30label = new QLabel();
  scene->addWidget(w30label);
  WeicheQPushButton *w30push = new WeicheQPushButton();
  scene->addWidget(w30push);
  w30ptr->addWeichenitem(w30rectab,w30rectge,w30label,w30push);
  w30ptr->moveButton(1060,-370);
  w30ptr->moveLabel(1060,-385);

  QGraphicsRectItem *w29rectge = new QGraphicsRectItem();
  w29rectge->setRect(0,0,10,75);
  w29rectge->setPos(QPointF(1210,-295));
  w29rectge->setBrush(QColor(79,79,79));
  w29rectge->setRotation(90);
  scene->addItem(w29rectge);
  QGraphicsRectItem *w29rectab = new QGraphicsRectItem();
  w29rectab->setRect(0,0,10,50);
  w29rectab->setPos(QPointF(1135,-285));
  w29rectab->setBrush(QColor(79,79,79));
  w29rectab->setRotation(315);
  scene->addItem(w29rectab);
  QLabel *w29label = new QLabel();
  scene->addWidget(w29label);
  WeicheQPushButton *w29push = new WeicheQPushButton();
  scene->addWidget(w29push);
  w29ptr->addWeichenitem(w29rectab,w29rectge,w29label,w29push);
  w29ptr->moveButton(1120,-300);
  w29ptr->moveLabel(1150,-310);

  QGraphicsRectItem *w28rectge = new QGraphicsRectItem();
  w28rectge->setRect(0,0,10,75);
  w28rectge->setPos(QPointF(1210,-220));
  w28rectge->setBrush(QColor(79,79,79));
  w28rectge->setRotation(90);
  scene->addItem(w28rectge);
  QGraphicsRectItem *w28rectab = new QGraphicsRectItem();
  w28rectab->setRect(0,0,10,50);
  w28rectab->setPos(QPointF(1210,-220));
  w28rectab->setBrush(QColor(79,79,79));
  w28rectab->setRotation(135);
  scene->addItem(w28rectab);
  QLabel *w28label = new QLabel();
  scene->addWidget(w28label);
  WeicheQPushButton *w28push = new WeicheQPushButton();
  scene->addWidget(w28push);
  w28ptr->addWeichenitem(w28rectab,w28rectge,w28label,w28push);
  w28ptr->moveButton(1200,-225);
  w28ptr->moveLabel(1170,-210);

  QGraphicsRectItem *bdrect = new QGraphicsRectItem();
  bdrect->setRect(0,0,10,1000);
  bdrect->setPos(QPointF(2135,-365));
  bdrect->setBrush(QColor(79,79,79));
  bdrect->setRotation(90);
  bdptr->addBlockitems(bdrect);
  scene->addItem(bdrect);

  QGraphicsRectItem *berect = new QGraphicsRectItem();
  berect->setRect(0,0,10,925);
  berect->setPos(QPointF(2135,-295));
  berect->setBrush(QColor(79,79,79));
  berect->setRotation(90);
  beptr->addBlockitems(berect);
  scene->addItem(berect);

  QGraphicsRectItem *bkrect = new QGraphicsRectItem();
  bkrect->setRect(0,0,10,270);
  bkrect->setPos(QPointF(2480,-365));
  bkrect->setBrush(QColor(79,79,79));
  bkrect->setRotation(90);
  bkptr->addBlockitems(bkrect);
  scene->addItem(bkrect);

  QGraphicsRectItem *bkrect2 = new QGraphicsRectItem();
  bkrect2->setRect(0,0,10,595);
  bkrect2->setPos(QPointF(2470,-355));
  bkrect2->setBrush(QColor(79,79,79));
  bkptr->addBlockitems(bkrect2);
  scene->addItem(bkrect2);

  QGraphicsRectItem *w25rectge = new QGraphicsRectItem();
  w25rectge->setRect(0,0,10,75);
  w25rectge->setPos(QPointF(2210,-365));
  w25rectge->setBrush(QColor(79,79,79));
  w25rectge->setRotation(90);
  scene->addItem(w25rectge);
  QGraphicsRectItem *w25rectab = new QGraphicsRectItem();
  w25rectab->setRect(0,0,10,50);
  w25rectab->setPos(QPointF(2205,-360));
  w25rectab->setBrush(QColor(79,79,79));
  w25rectab->setRotation(45);
  scene->addItem(w25rectab);
  QLabel *w25label = new QLabel();
  scene->addWidget(w25label);
  WeicheQPushButton *w25push = new WeicheQPushButton();
  scene->addWidget(w25push);
  w25ptr->addWeichenitem(w25rectab,w25rectge,w25label,w25push);
  w25ptr->moveButton(2200,-370);
  w25ptr->moveLabel(2160,-385);

  QGraphicsRectItem *w26rectge = new QGraphicsRectItem();
  w26rectge->setRect(0,0,10,75);
  w26rectge->setPos(QPointF(2210,-295));
  w26rectge->setBrush(QColor(79,79,79));
  w26rectge->setRotation(90);
  scene->addItem(w26rectge);
  QGraphicsRectItem *w26rectab = new QGraphicsRectItem();
  w26rectab->setRect(0,0,10,50);
  w26rectab->setPos(QPointF(2150,-290));
  w26rectab->setBrush(QColor(79,79,79));
  w26rectab->setRotation(-135);
  scene->addItem(w26rectab);
  QLabel *w26label = new QLabel();
  scene->addWidget(w26label);
  WeicheQPushButton *w26push = new WeicheQPushButton();
  scene->addWidget(w26push);
  w26ptr->addWeichenitem(w26rectab,w26rectge,w26label,w26push);
  w26ptr->moveButton(2135,-300);
  w26ptr->moveLabel(2120,-310);

  QGraphicsRectItem *bjrect = new QGraphicsRectItem();
  bjrect->setRect(0,0,10,200);
  bjrect->setPos(QPointF(2410,-295));
  bjrect->setBrush(QColor(79,79,79));
  bjrect->setRotation(90);
  bjptr->addBlockitems(bjrect);
  scene->addItem(bjrect);

  QGraphicsRectItem *carect = new QGraphicsRectItem();
  carect->setRect(0,0,10,725);
  carect->setPos(QPointF(2470,230));
  carect->setBrush(QColor(79,79,79));
  carect->setRotation(90);
  captr->addBlockitems(carect);
  scene->addItem(carect);

  QGraphicsRectItem *bnrect2 = new QGraphicsRectItem();
  bnrect2->setRect(0,0,10,80);
  bnrect2->setPos(QPointF(1710,270));
  bnrect2->setBrush(QColor(79,79,79));
  bnptr->addBlockitems(bnrect2);
  scene->addItem(bnrect2);

  QGraphicsRectItem *w27rectge = new QGraphicsRectItem();
  w27rectge->setRect(0,0,10,75);
  w27rectge->setPos(QPointF(2085,-220));
  w27rectge->setBrush(QColor(79,79,79));
  w27rectge->setRotation(90);
  scene->addItem(w27rectge);
  QGraphicsRectItem *w27rectab = new QGraphicsRectItem();
  w27rectab->setRect(0,0,10,50);
  w27rectab->setPos(QPointF(2010,-210));
  w27rectab->setBrush(QColor(79,79,79));
  w27rectab->setRotation(315);
  scene->addItem(w27rectab);
  QLabel *w27label = new QLabel();
  scene->addWidget(w27label);
  WeicheQPushButton *w27push = new WeicheQPushButton();
  scene->addWidget(w27push);
  w27ptr->addWeichenitem(w27rectab,w27rectge,w27label,w27push);
  w27ptr->moveButton(2005,-225);
  w27ptr->moveLabel(1990,-235);

  QGraphicsRectItem *birect = new QGraphicsRectItem();
  birect->setRect(0,0,10,300);
  birect->setPos(QPointF(2385,-220));
  birect->setBrush(QColor(79,79,79));
  birect->setRotation(90);
  biptr->addBlockitems(birect);
  scene->addItem(birect);

  QGraphicsRectItem *bhrect = new QGraphicsRectItem();
  bhrect->setRect(0,0,10,300);
  bhrect->setPos(QPointF(2045,-175));
  bhrect->setBrush(QColor(79,79,79));
  bhrect->setRotation(315);
  bhptr->addBlockitems(bhrect);
  scene->addItem(bhrect);

  QGraphicsRectItem *w31rectge = new QGraphicsRectItem();
  w31rectge->setRect(0,0,10,75);
  w31rectge->setPos(QPointF(1745,230));
  w31rectge->setBrush(QColor(79,79,79));
  w31rectge->setRotation(90);
  scene->addItem(w31rectge);
  QGraphicsRectItem *w31rectab = new QGraphicsRectItem();
  w31rectab->setRect(0,0,10,50);
  w31rectab->setPos(QPointF(1745,230));
  w31rectab->setBrush(QColor(79,79,79));
  w31rectab->setRotation(45);
  scene->addItem(w31rectab);
  QLabel *w31label = new QLabel();
  scene->addWidget(w31label);
  WeicheQPushButton *w31push = new WeicheQPushButton();
  scene->addWidget(w31push);
  w31ptr->addWeichenitem(w31rectab,w31rectge,w31label,w31push);
  w31ptr->moveButton(1730,225);
  w31ptr->moveLabel(1720,215);

  QGraphicsRectItem *borect2 = new QGraphicsRectItem();
  borect2->setRect(0,0,10,730);
  borect2->setPos(QPointF(1670,230));
  borect2->setBrush(QColor(79,79,79));
  borect2->setRotation(90);
  boptr->addBlockitems(borect2);
  scene->addItem(borect2);

  QGraphicsRectItem *borect3 = new QGraphicsRectItem();
  borect3->setRect(0,0,10,60);
  borect3->setPos(QPointF(810,230));
  borect3->setBrush(QColor(79,79,79));
  borect3->setRotation(90);
  boptr->addBlockitems(borect3);
  scene->addItem(borect3);

  QGraphicsRectItem *borect4 = new QGraphicsRectItem();
  borect4->setRect(0,0,10,665);
  borect4->setPos(QPointF(750,-435));
  borect4->setBrush(QColor(79,79,79));
  boptr->addBlockitems(borect4);
  scene->addItem(borect4);


  QGraphicsRectItem *bnrect3 = new QGraphicsRectItem();
  bnrect3->setRect(0,0,10,340);
  bnrect3->setPos(QPointF(1710,470));
  bnrect3->setBrush(QColor(79,79,79));
  bnptr->addBlockitems(bnrect3);
  scene->addItem(bnrect3);

  WSignalQPushButton *ww12push = new WSignalQPushButton();
  scene->addWidget(ww12push);
  ww12ptr->addButtonAndLabel(ww12label,ww12push);
  ww12ptr->moveButton(595,220);
  ww12ptr->moveLabel(620,250);

  QGraphicsSvgItem *s24item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s24item1);
  s24item1->setPos(QPointF(690,240));
  s24item1->setScale(0.36);
  s24item1->setElementId("basic_hp0");
  s24item1->setObjectName("basic_hp0_Bks");
  HSignalQPushButton *s24push = new HSignalQPushButton();
  scene->addWidget(s24push);
  QLabel *s24label = new QLabel();
  scene->addWidget(s24label);
  //speicher GUIs
  QGraphicsRectItem *sphin24 = new QGraphicsRectItem();
  sphin24->setRect(0,0,10,10);
  sphin24->setBrush(Qt::yellow);
  sphin24->setPos(QPointF(690,300));
  QGraphicsRectItem *spweg24 = new QGraphicsRectItem();
  spweg24->setRect(0,0,10,10);
  spweg24->setBrush(Qt::yellow);
  spweg24->setPos(QPointF(700,300));
  scene->addItem(spweg24);
  scene->addItem(sphin24);
  s24ptr->addHSignalitem(s24item1, s24label,s24push,spweg24,sphin24);
  s24ptr->moveButton(665,240);
  s24ptr->moveLabel(690,320);


  QGraphicsSvgItem *itemv201 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv201);
  itemv201->setPos(QPointF(690,90));
  itemv201->setScale(0.36);
  itemv201->setElementId("basic_vr0");
  itemv201->setObjectName("basic_vr0");
  v20ptr->addVSignalitems(itemv201);

  QGraphicsSvgItem *itemv181 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv181);
  itemv181->setPos(QPointF(660,190));
  itemv181->setScale(0.36);
  itemv181->setRotation(180);
  itemv181->setElementId("basic_vr0");
  itemv181->setObjectName("basic_vr0");
  v18ptr->addVSignalitems(itemv181);

  QGraphicsSvgItem *itemv171 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv171);
  itemv171->setPos(QPointF(595,240));
  itemv171->setScale(0.36);
  itemv171->setRotation(180);
  itemv171->setElementId("basic_vr0");
  itemv171->setObjectName("basic_vr0_ks");
  v17ptr->addVSignalitems(itemv171);

  QGraphicsSvgItem *s25item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s25item1);
  s25item1->setPos(QPointF(590,120));
  s25item1->setScale(0.36);
  s25item1->setRotation(180);
  s25item1->setElementId("basic_hp0");
  s25item1->setObjectName("basic_hp0_woSh");
  HSignalQPushButton *s25push = new HSignalQPushButton();
  scene->addWidget(s25push);
  QLabel *s25label = new QLabel();
  scene->addWidget(s25label);
  //speicher GUIs
  QGraphicsRectItem *sphin25 = new QGraphicsRectItem();
  sphin25->setRect(0,0,10,10);
  sphin25->setBrush(Qt::yellow);
  sphin25->setPos(QPointF(580,40));
  QGraphicsRectItem *spweg25 = new QGraphicsRectItem();
  spweg25->setRect(0,0,10,10);
  spweg25->setBrush(Qt::yellow);
  spweg25->setPos(QPointF(570,40));
  scene->addItem(spweg25);
  scene->addItem(sphin25);
  s25ptr->addHSignalitem(s25item1, s25label,s25push,spweg25,sphin25);
  s25ptr->moveButton(595,90);
  s25ptr->moveLabel(560,20);

  QGraphicsSvgItem *itemv191 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv191);
  itemv191->setPos(QPointF(620,60));
  itemv191->setScale(0.36);
  itemv191->setElementId("basic_vr0");
  itemv191->setObjectName("basic_vr0_ks");
  v19ptr->addVSignalitems(itemv191);

  QGraphicsSvgItem *itemv211 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv211);
  itemv211->setPos(QPointF(595,-500));
  itemv211->setScale(0.36);
  itemv211->setRotation(180);
  itemv211->setElementId("basic_vr0");
  itemv211->setObjectName("basic_vr0_ks");
  v21ptr->addVSignalitems(itemv211);

  QGraphicsSvgItem *itemv221 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv221);
  itemv221->setPos(QPointF(820,-525));
  itemv221->setScale(0.36);
  itemv221->setRotation(90);
  itemv221->setElementId("basic_vr0");
  itemv221->setObjectName("basic_vr0");
  v22ptr->addVSignalitems(itemv221);

  QGraphicsSvgItem *s27item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s27item1);
  s27item1->setPos(QPointF(860,-520));
  s27item1->setScale(0.36);
  s27item1->setRotation(90);
  s27item1->setElementId("basic_hp0");
  s27item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s27push = new HSignalQPushButton();
  scene->addWidget(s27push);
  QLabel *s27label = new QLabel();
  scene->addWidget(s27label);
  //speicher GUIs
  QGraphicsRectItem *sphin27 = new QGraphicsRectItem();
  sphin27->setRect(0,0,10,10);
  sphin27->setBrush(Qt::yellow);
  sphin27->setPos(QPointF(781,-521));
  QGraphicsRectItem *spweg27 = new QGraphicsRectItem();
  spweg27->setRect(0,0,10,10);
  spweg27->setBrush(Qt::yellow);
  spweg27->setPos(QPointF(781,-511));
  scene->addItem(spweg27);
  scene->addItem(sphin27);
  s27ptr->addHSignalitem(s27item1, s27label,s27push,spweg27,sphin27);
  s27ptr->moveButton(830,-545);
  s27ptr->moveLabel(748,-520);

  QGraphicsSvgItem *itemv231 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv231);
  itemv231->setPos(QPointF(820,-595));
  itemv231->setScale(0.36);
  itemv231->setRotation(90);
  itemv231->setElementId("basic_vr0");
  itemv231->setObjectName("basic_vr0");
  v23ptr->addVSignalitems(itemv231);

  QGraphicsSvgItem *s28item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s28item1);
  s28item1->setPos(QPointF(860,-590));
  s28item1->setScale(0.36);
  s28item1->setRotation(90);
  s28item1->setElementId("basic_hp0");
  s28item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s28push = new HSignalQPushButton();
  scene->addWidget(s28push);
  QLabel *s28label = new QLabel();
  scene->addWidget(s28label);
  //speicher GUIs
  QGraphicsRectItem *sphin28 = new QGraphicsRectItem();
  sphin28->setRect(0,0,10,10);
  sphin28->setBrush(Qt::yellow);
  sphin28->setPos(QPointF(781,-591));
  QGraphicsRectItem *spweg28 = new QGraphicsRectItem();
  spweg28->setRect(0,0,10,10);
  spweg28->setBrush(Qt::yellow);
  spweg28->setPos(QPointF(781,-581));
  scene->addItem(spweg28);
  scene->addItem(sphin28);
  s28ptr->addHSignalitem(s28item1, s28label,s28push,spweg28,sphin28);
  s28ptr->moveButton(830,-615);
  s28ptr->moveLabel(748,-590);

  QGraphicsSvgItem *s29item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s29item1);
  s29item1->setPos(QPointF(895,-620));
  s29item1->setScale(0.36);
  s29item1->setRotation(-90);
  s29item1->setElementId("basic_hp0");
  s29item1->setObjectName("basic_hp0_woSh");
  HSignalQPushButton *s29push = new HSignalQPushButton();
  scene->addWidget(s29push);
  QLabel *s29label = new QLabel();
  scene->addWidget(s29label);
  //speicher GUIs
  QGraphicsRectItem *sphin29 = new QGraphicsRectItem();
  sphin29->setRect(0,0,10,10);
  sphin29->setBrush(Qt::yellow);
  sphin29->setPos(QPointF(956,-630));
  QGraphicsRectItem *spweg29 = new QGraphicsRectItem();
  spweg29->setRect(0,0,10,10);
  spweg29->setBrush(Qt::yellow);
  spweg29->setPos(QPointF(956,-640));
  scene->addItem(spweg29);
  scene->addItem(sphin29);
  s29ptr->addHSignalitem(s29item1, s29label,s29push,spweg29,sphin29);
  s29ptr->moveButton(920,-615);
  s29ptr->moveLabel(965,-630);

  QGraphicsSvgItem *s30item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s30item1);
  s30item1->setPos(QPointF(915,-550));
  s30item1->setScale(0.36);
  s30item1->setRotation(-90);
  s30item1->setElementId("basic_hp0");
  s30item1->setObjectName("basic_hp0_woSh");
  HSignalQPushButton *s30push = new HSignalQPushButton();
  scene->addWidget(s30push);
  QLabel *s30label = new QLabel();
  scene->addWidget(s30label);
  //speicher GUIs
  QGraphicsRectItem *sphin30 = new QGraphicsRectItem();
  sphin30->setRect(0,0,10,10);
  sphin30->setBrush(Qt::yellow);
  sphin30->setPos(QPointF(976,-560));
  QGraphicsRectItem *spweg30 = new QGraphicsRectItem();
  spweg30->setRect(0,0,10,10);
  spweg30->setBrush(Qt::yellow);
  spweg30->setPos(QPointF(976,-570));
  scene->addItem(spweg30);
  scene->addItem(sphin30);
  s30ptr->addHSignalitem(s30item1, s30label,s30push,spweg30,sphin30);
  s30ptr->moveButton(930,-545);
  s30ptr->moveLabel(985,-560);

  QGraphicsSvgItem *itemv311 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv311);
  itemv311->setPos(QPointF(2480,-525));
  itemv311->setScale(0.36);
  itemv311->setRotation(90);
  itemv311->setElementId("basic_vr0");
  itemv311->setObjectName("basic_vr0");
  v31ptr->addVSignalitems(itemv311);

  QGraphicsSvgItem *s39item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s39item1);
  s39item1->setPos(QPointF(2520,-520));
  s39item1->setScale(0.36);
  s39item1->setRotation(90);
  s39item1->setElementId("basic_hp0");
  s39item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s39push = new HSignalQPushButton();
  scene->addWidget(s39push);
  QLabel *s39label = new QLabel();
  scene->addWidget(s39label);
  //speicher GUIs
  QGraphicsRectItem *sphin39 = new QGraphicsRectItem();
  sphin39->setRect(0,0,10,10);
  sphin39->setBrush(Qt::yellow);
  sphin39->setPos(QPointF(2441,-521));
  QGraphicsRectItem *spweg39 = new QGraphicsRectItem();
  spweg39->setRect(0,0,10,10);
  spweg39->setBrush(Qt::yellow);
  spweg39->setPos(QPointF(2441,-511));
  scene->addItem(spweg39);
  scene->addItem(sphin39);
  s39ptr->addHSignalitem(s39item1, s39label,s39push,spweg39,sphin39);
  s39ptr->moveButton(2490,-545);
  s39ptr->moveLabel(2408,-520);

  QGraphicsSvgItem *itemv301 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv301);
  itemv301->setPos(QPointF(2595,-545));
  itemv301->setScale(0.36);
  itemv301->setRotation(-90);
  itemv301->setElementId("basic_vr0");
  itemv301->setObjectName("basic_vr0");
  v30ptr->addVSignalitems(itemv301);
  
  QGraphicsSvgItem *s41item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s41item1);
  s41item1->setPos(QPointF(2555,-550));
  s41item1->setScale(0.36);
  s41item1->setRotation(-90);
  s41item1->setElementId("basic_hp0");
  s41item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s41push = new HSignalQPushButton();
  scene->addWidget(s41push);
  QLabel *s41label = new QLabel();
  scene->addWidget(s41label);
  //speicher GUIs
  QGraphicsRectItem *sphin41 = new QGraphicsRectItem();
  sphin41->setRect(0,0,10,10);
  sphin41->setBrush(Qt::yellow);
  sphin41->setPos(QPointF(2631,-560));
  QGraphicsRectItem *spweg41 = new QGraphicsRectItem();
  spweg41->setRect(0,0,10,10);
  spweg41->setBrush(Qt::yellow);
  spweg41->setPos(QPointF(2631,-570));
  scene->addItem(spweg41);
  scene->addItem(sphin41);
  s41ptr->addHSignalitem(s41item1, s41label,s41push,spweg41,sphin41);
  s41ptr->moveButton(2580,-545);
  s41ptr->moveLabel(2645,-560);

  QGraphicsSvgItem *itemv291 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv291);
  itemv291->setPos(QPointF(2595,-615));
  itemv291->setScale(0.36);
  itemv291->setRotation(-90);
  itemv291->setElementId("basic_vr0");
  itemv291->setObjectName("basic_vr0");
  v29ptr->addVSignalitems(itemv291);
  
  QGraphicsSvgItem *s40item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s40item1);
  s40item1->setPos(QPointF(2555,-620));
  s40item1->setScale(0.36);
  s40item1->setRotation(-90);
  s40item1->setElementId("basic_hp0");
  s40item1->setObjectName("basic_hp0_Bks");
  HSignalQPushButton *s40push = new HSignalQPushButton();
  scene->addWidget(s40push);
  QLabel *s40label = new QLabel();
  scene->addWidget(s40label);
  //speicher GUIs
  QGraphicsRectItem *sphin40 = new QGraphicsRectItem();
  sphin40->setRect(0,0,10,10);
  sphin40->setBrush(Qt::yellow);
  sphin40->setPos(QPointF(2631,-630));
  QGraphicsRectItem *spweg40 = new QGraphicsRectItem();
  spweg40->setRect(0,0,10,10);
  spweg40->setBrush(Qt::yellow);
  spweg40->setPos(QPointF(2631,-640));
  scene->addItem(spweg40);
  scene->addItem(sphin40);
  s40ptr->addHSignalitem(s40item1,s40label,s40push,spweg40,sphin40);
  s40ptr->moveButton(2580,-615);
  s40ptr->moveLabel(2645,-630);

  QGraphicsSvgItem *itemv351 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv351);
  itemv351->setPos(QPointF(2720,-50));
  itemv351->setScale(0.36);
  itemv351->setRotation(180);
  itemv351->setElementId("basic_vr0");
  itemv351->setObjectName("basic_vr0_ks");
  v35ptr->addVSignalitems(itemv351);

  QGraphicsSvgItem *itemv381 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv381);
  itemv381->setPos(QPointF(1395,375));
  itemv381->setScale(0.36);
  itemv381->setRotation(-90);
  itemv381->setElementId("basic_vr0");
  itemv381->setObjectName("basic_vr0");
  v38ptr->addVSignalitems(itemv381);
  
  QGraphicsSvgItem *s48item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s48item1);
  s48item1->setPos(QPointF(1355,370));
  s48item1->setScale(0.36);
  s48item1->setRotation(-90);
  s48item1->setElementId("basic_hp0");
  s48item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s48push = new HSignalQPushButton();
  scene->addWidget(s48push);
  QLabel *s48label = new QLabel();
  scene->addWidget(s48label);
  //speicher GUIs
  QGraphicsRectItem *sphin48 = new QGraphicsRectItem();
  sphin48->setRect(0,0,10,10);
  sphin48->setBrush(Qt::yellow);
  sphin48->setPos(QPointF(1431,362));
  QGraphicsRectItem *spweg48 = new QGraphicsRectItem();
  spweg48->setRect(0,0,10,10);
  spweg48->setBrush(Qt::yellow);
  spweg48->setPos(QPointF(1431,352));
  scene->addItem(spweg48);
  scene->addItem(sphin48);
  s48ptr->addHSignalitem(s48item1, s48label,s48push,spweg48,sphin48);
  s48ptr->moveButton(1380,375);
  s48ptr->moveLabel(1445,360);

  QGraphicsSvgItem *itemv401 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv401);
  itemv401->setPos(QPointF(1405,445));
  itemv401->setScale(0.36);
  itemv401->setRotation(-90);
  itemv401->setElementId("basic_vr0");
  itemv401->setObjectName("basic_vr0");
  v40ptr->addVSignalitems(itemv401);
  
  QGraphicsSvgItem *s46item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s46item1);
  s46item1->setPos(QPointF(1355,440));
  s46item1->setScale(0.36);
  s46item1->setRotation(-90);
  s46item1->setElementId("basic_hp0");
  s46item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s46push = new HSignalQPushButton();
  scene->addWidget(s46push);
  QLabel *s46label = new QLabel();
  scene->addWidget(s46label);
  //speicher GUIs
  QGraphicsRectItem *sphin46 = new QGraphicsRectItem();
  sphin46->setRect(0,0,10,10);
  sphin46->setBrush(Qt::yellow);
  sphin46->setPos(QPointF(1431,432));
  QGraphicsRectItem *spweg46 = new QGraphicsRectItem();
  spweg46->setRect(0,0,10,10);
  spweg46->setBrush(Qt::yellow);
  spweg46->setPos(QPointF(1431,422));
  scene->addItem(spweg46);
  scene->addItem(sphin46);
  s46ptr->addHSignalitem(s46item1, s46label,s46push,spweg46,sphin46);
  s46ptr->moveButton(1380,445);
  s46ptr->moveLabel(1445,430);

  QGraphicsSvgItem *itemv391 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv391);
  itemv391->setPos(QPointF(1335,395));
  itemv391->setScale(0.36);
  itemv391->setRotation(90);
  itemv391->setElementId("basic_vr0");
  itemv391->setObjectName("basic_vr0");
  v39ptr->addVSignalitems(itemv391);
  
  QGraphicsSvgItem *s47item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s47item1);
  s47item1->setPos(QPointF(1375,400));
  s47item1->setScale(0.36);
  s47item1->setRotation(90);
  s47item1->setElementId("basic_hp0");
  s47item1->setObjectName("basic_hp0_Bks");
  HSignalQPushButton *s47push = new HSignalQPushButton();
  scene->addWidget(s47push);
  QLabel *s47label = new QLabel();
  scene->addWidget(s47label);
  //speicher GUIs
  QGraphicsRectItem *sphin47 = new QGraphicsRectItem();
  sphin47->setRect(0,0,10,10);
  sphin47->setBrush(Qt::yellow);
  sphin47->setPos(QPointF(1291,402));
  QGraphicsRectItem *spweg47 = new QGraphicsRectItem();
  spweg47->setRect(0,0,10,10);
  spweg47->setBrush(Qt::yellow);
  spweg47->setPos(QPointF(1291,412));
  scene->addItem(spweg47);
  scene->addItem(sphin47);
  s47ptr->addHSignalitem(s47item1, s47label,s47push,spweg47,sphin47);
  s47ptr->moveButton(1360,375);
  s47ptr->moveLabel(1260,400);

  QGraphicsSvgItem *itemv411 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv411);
  itemv411->setPos(QPointF(1335,465));
  itemv411->setScale(0.36);
  itemv411->setRotation(90);
  itemv411->setElementId("basic_vr0");
  itemv411->setObjectName("basic_vr0");
  v41ptr->addVSignalitems(itemv411);
  
  QGraphicsSvgItem *s45item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s45item1);
  s45item1->setPos(QPointF(1375,470));
  s45item1->setScale(0.36);
  s45item1->setRotation(90);
  s45item1->setElementId("basic_hp0");
  s45item1->setObjectName("basic_hp0_Bks");
  HSignalQPushButton *s45push = new HSignalQPushButton();
  scene->addWidget(s45push);
  QLabel *s45label = new QLabel();
  scene->addWidget(s45label);
  //speicher GUIs
  QGraphicsRectItem *sphin45 = new QGraphicsRectItem();
  sphin45->setRect(0,0,10,10);
  sphin45->setBrush(Qt::yellow);
  sphin45->setPos(QPointF(1291,472));
  QGraphicsRectItem *spweg45 = new QGraphicsRectItem();
  spweg45->setRect(0,0,10,10);
  spweg45->setBrush(Qt::yellow);
  spweg45->setPos(QPointF(1291,482));
  scene->addItem(spweg45);
  scene->addItem(sphin45);
  s45ptr->addHSignalitem(s45item1, s45label,s45push,spweg45,sphin45);
  s45ptr->moveButton(1360,445);
  s45ptr->moveLabel(1260,470);

  QGraphicsSvgItem *itemv331 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv331);
  itemv331->setPos(QPointF(2445,-420));
  itemv331->setScale(0.36);
  itemv331->setRotation(90);
  itemv331->setElementId("basic_vr0");
  itemv331->setObjectName("basic_vr0");
  v33ptr->addVSignalitems(itemv331);

  QGraphicsSvgItem *s38item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s38item1);
  s38item1->setPos(QPointF(2485,-415));
  s38item1->setScale(0.36);
  s38item1->setRotation(90);
  s38item1->setElementId("basic_hp0");
  s38item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s38push = new HSignalQPushButton();
  scene->addWidget(s38push);
  QLabel *s38label = new QLabel();
  scene->addWidget(s38label);
  //speicher GUIs
  QGraphicsRectItem *sphin38 = new QGraphicsRectItem();
  sphin38->setRect(0,0,10,10);
  sphin38->setBrush(Qt::yellow);
  sphin38->setPos(QPointF(2406,-416));
  QGraphicsRectItem *spweg38 = new QGraphicsRectItem();
  spweg38->setRect(0,0,10,10);
  spweg38->setBrush(Qt::yellow);
  spweg38->setPos(QPointF(2406,-406));
  scene->addItem(spweg38);
  scene->addItem(sphin38);
  s38ptr->addHSignalitem(s38item1, s38label,s38push,spweg38,sphin38);
  s38ptr->moveButton(2445,-440);
  s38ptr->moveLabel(2373,-415);

  QGraphicsSvgItem *itemv281 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv281);
  itemv281->setPos(QPointF(765,-375));
  itemv281->setScale(0.36);
  itemv281->setElementId("basic_vr0");
  itemv281->setObjectName("basic_vr0");
  v28ptr->addVSignalitems(itemv281);
  
  QGraphicsSvgItem *s26item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s26item1);
  s26item1->setPos(QPointF(770,-415));
  s26item1->setScale(0.36);
  s26item1->setElementId("basic_hp0");
  s26item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s26push = new HSignalQPushButton();
  scene->addWidget(s26push);
  QLabel *s26label = new QLabel();
  scene->addWidget(s26label);
  //speicher GUIs
  QGraphicsRectItem *sphin26 = new QGraphicsRectItem();
  sphin26->setRect(0,0,10,10);
  sphin26->setBrush(Qt::yellow);
  sphin26->setPos(QPointF(770,-335));
  QGraphicsRectItem *spweg26 = new QGraphicsRectItem();
  spweg26->setRect(0,0,10,10);
  spweg26->setBrush(Qt::yellow);
  spweg26->setPos(QPointF(780,-335));
  scene->addItem(spweg26);
  scene->addItem(sphin26);
  s26ptr->addHSignalitem(s26item1, s26label,s26push,spweg26,sphin26);
  s26ptr->moveButton(745,-405);
  s26ptr->moveLabel(770,-300);

  QGraphicsSvgItem *itemv321 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv321);
  itemv321->setPos(QPointF(2560,-440));
  itemv321->setScale(0.36);
  itemv321->setRotation(-90);
  itemv321->setElementId("basic_vr0");
  itemv321->setObjectName("basic_vr0");
  v32ptr->addVSignalitems(itemv321);

  QGraphicsSvgItem *s42item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s42item1);
  s42item1->setPos(QPointF(2520,-445));
  s42item1->setScale(0.36);
  s42item1->setRotation(-90);
  s42item1->setElementId("basic_hp0");
  s42item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s42push = new HSignalQPushButton();
  scene->addWidget(s42push);
  QLabel *s42label = new QLabel();
  scene->addWidget(s42label);
  //speicher GUIs
  QGraphicsRectItem *sphin42 = new QGraphicsRectItem();
  sphin42->setRect(0,0,10,10);
  sphin42->setBrush(Qt::yellow);
  sphin42->setPos(QPointF(2596,-455));
  QGraphicsRectItem *spweg42 = new QGraphicsRectItem();
  spweg42->setRect(0,0,10,10);
  spweg42->setBrush(Qt::yellow);
  spweg42->setPos(QPointF(2596,-465));
  scene->addItem(spweg42);
  scene->addItem(sphin42);
  s42ptr->addHSignalitem(s42item1, s42label,s42push,spweg42,sphin42);
  s42ptr->moveButton(2540,-440);
  s42ptr->moveLabel(2580,-440);

  QGraphicsSvgItem *s37item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s37item1);
  s37item1->setPos(QPointF(2145,-345));
  s37item1->setScale(0.36);
  s37item1->setRotation(90);
  s37item1->setElementId("basic_hp0");
  s37item1->setObjectName("basic_hp0_ks");
  HSignalQPushButton *s37push = new HSignalQPushButton();
  scene->addWidget(s37push);
  QLabel *s37label = new QLabel();
  scene->addWidget(s37label);
  //speicher GUIs
  QGraphicsRectItem *sphin37 = new QGraphicsRectItem();
  sphin37->setRect(0,0,10,10);
  sphin37->setBrush(Qt::yellow);
  sphin37->setPos(QPointF(2066,-346));
  QGraphicsRectItem *spweg37 = new QGraphicsRectItem();
  spweg37->setRect(0,0,10,10);
  spweg37->setBrush(Qt::yellow);
  spweg37->setPos(QPointF(2066,-336));
  scene->addItem(spweg37);
  scene->addItem(sphin37);
  s37ptr->addHSignalitem(s37item1, s37label,s37push,spweg37,sphin37);
  s37ptr->moveButton(2115,-370);
  s37ptr->moveLabel(2033,-345);

  QGraphicsSvgItem *s36item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s36item1);
  s36item1->setPos(QPointF(2145,-275));
  s36item1->setScale(0.36);
  s36item1->setRotation(90);
  s36item1->setElementId("basic_hp0");
  s36item1->setObjectName("basic_hp0_ks");
  HSignalQPushButton *s36push = new HSignalQPushButton();
  scene->addWidget(s36push);
  QLabel *s36label = new QLabel();
  scene->addWidget(s36label);
  //speicher GUIs
  QGraphicsRectItem *sphin36 = new QGraphicsRectItem();
  sphin36->setRect(0,0,10,10);
  sphin36->setBrush(Qt::yellow);
  sphin36->setPos(QPointF(2066,-276));
  QGraphicsRectItem *spweg36 = new QGraphicsRectItem();
  spweg36->setRect(0,0,10,10);
  spweg36->setBrush(Qt::yellow);
  spweg36->setPos(QPointF(2066,-266));
  scene->addItem(spweg36);
  scene->addItem(sphin36);
  s36ptr->addHSignalitem(s36item1, s36label,s36push,spweg36,sphin36);
  s36ptr->moveButton(2115,-300);
  s36ptr->moveLabel(2033,-275);

  QGraphicsSvgItem *s35item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s35item1);
  s35item1->setPos(QPointF(2015,-205));
  s35item1->setScale(0.36);
  s35item1->setRotation(90);
  s35item1->setElementId("basic_hp0");
  s35item1->setObjectName("basic_hp0_ks");
  HSignalQPushButton *s35push = new HSignalQPushButton();
  scene->addWidget(s35push);
  QLabel *s35label = new QLabel();
  scene->addWidget(s35label);
  //speicher GUIs
  QGraphicsRectItem *sphin35 = new QGraphicsRectItem();
  sphin35->setRect(0,0,10,10);
  sphin35->setBrush(Qt::yellow);
  sphin35->setPos(QPointF(1936,-206));
  QGraphicsRectItem *spweg35 = new QGraphicsRectItem();
  spweg35->setRect(0,0,10,10);
  spweg35->setBrush(Qt::yellow);
  spweg35->setPos(QPointF(1936,-196));
  scene->addItem(spweg35);
  scene->addItem(sphin35);
  s35ptr->addHSignalitem(s35item1, s35label,s35push,spweg35,sphin35);
  s35ptr->moveButton(1985,-225);
  s35ptr->moveLabel(1903,-205);

  QGraphicsSvgItem *s43item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s43item1);
  s43item1->setPos(QPointF(1695,245));
  s43item1->setScale(0.36);
  s43item1->setRotation(90);
  s43item1->setElementId("basic_hp0");
  s43item1->setObjectName("basic_hp0_woSh");
  HSignalQPushButton *s43push = new HSignalQPushButton();
  scene->addWidget(s43push);
  QLabel *s43label = new QLabel();
  scene->addWidget(s43label);
  //speicher GUIs
  QGraphicsRectItem *sphin43 = new QGraphicsRectItem();
  sphin43->setRect(0,0,10,10);
  sphin43->setBrush(Qt::yellow);
  sphin43->setPos(QPointF(1616,244));
  QGraphicsRectItem *spweg43 = new QGraphicsRectItem();
  spweg43->setRect(0,0,10,10);
  spweg43->setBrush(Qt::yellow);
  spweg43->setPos(QPointF(1616,254));
  scene->addItem(spweg43);
  scene->addItem(sphin43);
  s43ptr->addHSignalitem(s43item1, s43label,s43push,spweg43,sphin43);
  s43ptr->moveButton(1655,225);
  s43ptr->moveLabel(1583,245);

  QGraphicsSvgItem *itemv271 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv271);
  itemv271->setPos(QPointF(1245,-225));
  itemv271->setScale(0.36);
  itemv271->setRotation(-90);
  itemv271->setElementId("basic_vr0");
  itemv271->setObjectName("basic_vr0");
  v27ptr->addVSignalitems(itemv271);
  
  QGraphicsSvgItem *s34item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s34item1);
  s34item1->setPos(QPointF(1205,-230));
  s34item1->setScale(0.36);
  s34item1->setRotation(-90);
  s34item1->setElementId("basic_hp0");
  s34item1->setObjectName("basic_hp0");
  HSignalQPushButton *s34push = new HSignalQPushButton();
  scene->addWidget(s34push);
  QLabel *s34label = new QLabel();
  scene->addWidget(s34label);
  //speicher GUIs
  QGraphicsRectItem *sphin34 = new QGraphicsRectItem();
  sphin34->setRect(0,0,10,10);
  sphin34->setBrush(Qt::yellow);
  sphin34->setPos(QPointF(1281,-238));
  QGraphicsRectItem *spweg34 = new QGraphicsRectItem();
  spweg34->setRect(0,0,10,10);
  spweg34->setBrush(Qt::yellow);
  spweg34->setPos(QPointF(1281,-248));
  scene->addItem(spweg34);
  scene->addItem(sphin34);
  s34ptr->addHSignalitem(s34item1, s34label,s34push,spweg34,sphin34);
  s34ptr->moveButton(1220,-225);
  s34ptr->moveLabel(1295,-240);

  QGraphicsSvgItem *itemv261 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv261);
  itemv261->setPos(QPointF(1245,-300));
  itemv261->setScale(0.36);
  itemv261->setRotation(-90);
  itemv261->setElementId("basic_vr0");
  itemv261->setObjectName("basic_vr0");
  v26ptr->addVSignalitems(itemv261);
  
  QGraphicsSvgItem *s33item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s33item1);
  s33item1->setPos(QPointF(1205,-305));
  s33item1->setScale(0.36);
  s33item1->setRotation(-90);
  s33item1->setElementId("basic_hp0");
  s33item1->setObjectName("basic_hp0");
  HSignalQPushButton *s33push = new HSignalQPushButton();
  scene->addWidget(s33push);
  QLabel *s33label = new QLabel();
  scene->addWidget(s33label);
  //speicher GUIs
  QGraphicsRectItem *sphin33 = new QGraphicsRectItem();
  sphin33->setRect(0,0,10,10);
  sphin33->setBrush(Qt::yellow);
  sphin33->setPos(QPointF(1281,-313));
  QGraphicsRectItem *spweg33 = new QGraphicsRectItem();
  spweg33->setRect(0,0,10,10);
  spweg33->setBrush(Qt::yellow);
  spweg33->setPos(QPointF(1281,-323));
  scene->addItem(spweg33);
  scene->addItem(sphin33);
  s33ptr->addHSignalitem(s33item1, s33label,s33push,spweg33,sphin33);
  s33ptr->moveButton(1210,-300);
  s33ptr->moveLabel(1295,-315);

  QGraphicsSvgItem *itemv241 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv241);
  itemv241->setPos(QPointF(910,-440));
  itemv241->setScale(0.36);
  itemv241->setRotation(-90);
  itemv241->setElementId("basic_vr0");
  itemv241->setObjectName("basic_vr0");
  v24ptr->addVSignalitems(itemv241);
  
  QGraphicsSvgItem *s31item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s31item1);
  s31item1->setPos(QPointF(870,-445));
  s31item1->setScale(0.36);
  s31item1->setRotation(-90);
  s31item1->setElementId("basic_hp0");
  s31item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s31push = new HSignalQPushButton();
  scene->addWidget(s31push);
  QLabel *s31label = new QLabel();
  scene->addWidget(s31label);
  //speicher GUIs
  QGraphicsRectItem *sphin31 = new QGraphicsRectItem();
  sphin31->setRect(0,0,10,10);
  sphin31->setBrush(Qt::yellow);
  sphin31->setPos(QPointF(946,-453));
  QGraphicsRectItem *spweg31 = new QGraphicsRectItem();
  spweg31->setRect(0,0,10,10);
  spweg31->setBrush(Qt::yellow);
  spweg31->setPos(QPointF(946,-463));
  scene->addItem(spweg31);
  scene->addItem(sphin31);
  s31ptr->addHSignalitem(s31item1, s31label,s31push,spweg31,sphin31);
  s31ptr->moveButton(880,-440);
  s31ptr->moveLabel(980,-495);
  
  QGraphicsSvgItem *itemv251 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv251);
  itemv251->setPos(QPointF(1170,-370));
  itemv251->setScale(0.36);
  itemv251->setRotation(-90);
  itemv251->setElementId("basic_vr0");
  itemv251->setObjectName("basic_vr0");
  v25ptr->addVSignalitems(itemv251);
  
  QGraphicsSvgItem *s32item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s32item1);
  s32item1->setPos(QPointF(1130,-375));
  s32item1->setScale(0.36);
  s32item1->setRotation(-90);
  s32item1->setElementId("basic_hp0");
  s32item1->setObjectName("basic_hp0");
  HSignalQPushButton *s32push = new HSignalQPushButton();
  scene->addWidget(s32push);
  QLabel *s32label = new QLabel();
  scene->addWidget(s32label);
  //speicher GUIs
  QGraphicsRectItem *sphin32 = new QGraphicsRectItem();
  sphin32->setRect(0,0,10,10);
  sphin32->setBrush(Qt::yellow);
  sphin32->setPos(QPointF(1206,-383));
  QGraphicsRectItem *spweg32 = new QGraphicsRectItem();
  spweg32->setRect(0,0,10,10);
  spweg32->setBrush(Qt::yellow);
  spweg32->setPos(QPointF(1206,-393));
  scene->addItem(spweg32);
  scene->addItem(sphin32);
  s32ptr->addHSignalitem(s32item1, s32label,s32push,spweg32,sphin32);
  s32ptr->moveButton(1115,-370);
  s32ptr->moveLabel(1220,-385);

  QGraphicsSvgItem *itemv361 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv361);
  itemv361->setPos(QPointF(1755,220));
  itemv361->setScale(0.36);
  itemv361->setRotation(-90);
  itemv361->setElementId("basic_vr0");
  itemv361->setObjectName("basic_vr0_ks");
  v36ptr->addVSignalitems(itemv361);

  QGraphicsSvgItem *itemv341 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv341);
  itemv341->setPos(QPointF(2490,-355));
  itemv341->setScale(0.36);
  itemv341->setElementId("basic_vr0");
  itemv341->setObjectName("basic_vr0_ks");
  v34ptr->addVSignalitems(itemv341);

  QGraphicsSvgItem *itemv371 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv371);
  itemv371->setPos(QPointF(1920,320));
  itemv371->setScale(0.36);
  itemv371->setRotation(90);
  itemv371->setElementId("basic_vr0");
  itemv371->setObjectName("basic_vr0_ks");
  v37ptr->addVSignalitems(itemv371);

  QGraphicsSvgItem *itemv421 = new QGraphicsSvgItem(":/resources/VS.svg");
  scene->addItem(itemv421);
  itemv421->setPos(QPointF(1785,525));
  itemv421->setScale(0.36);
  itemv421->setRotation(180);
  itemv421->setElementId("basic_vr0");
  itemv421->setObjectName("basic_vr0");
  v42ptr->addVSignalitems(itemv421);

  QGraphicsSvgItem *s44item1 = new QGraphicsSvgItem(":/resources/HS.svg");
  scene->addItem(s44item1);
  s44item1->setPos(QPointF(1780,570));
  s44item1->setScale(0.36);
  s44item1->setRotation(180);
  s44item1->setElementId("basic_hp0");
  s44item1->setObjectName("basic_hp0_Esig");
  HSignalQPushButton *s44push = new HSignalQPushButton();
  scene->addWidget(s44push);
  QLabel *s44label = new QLabel();
  scene->addWidget(s44label);
  //speicher GUIs
  QGraphicsRectItem *sphin44 = new QGraphicsRectItem();
  sphin44->setRect(0,0,10,10);
  sphin44->setBrush(Qt::yellow);
  sphin44->setPos(QPointF(1770,480));
  QGraphicsRectItem *spweg44 = new QGraphicsRectItem();
  spweg44->setRect(0,0,10,10);
  spweg44->setBrush(Qt::yellow);
  spweg44->setPos(QPointF(1760,480));
  scene->addItem(spweg44);
  scene->addItem(sphin44);
  s44ptr->addHSignalitem(s44item1, s44label,s44push,spweg44,sphin44);
  s44ptr->moveButton(1785,530);
  s44ptr->moveLabel(1750,465);

  WSignalQPushButton *ww13push = new WSignalQPushButton();
  scene->addWidget(ww13push);
  ww13ptr->addButtonAndLabel(ww13label,ww13push);
  ww13ptr->moveButton(1040,-370);
  ww13ptr->moveLabel(1005,-345);

  WSignalQPushButton *ww14push = new WSignalQPushButton();
  scene->addWidget(ww14push);
  ww14ptr->addButtonAndLabel(ww14label,ww14push);
  ww14ptr->moveButton(935,-225);
  ww14ptr->moveLabel(975,-245);

  WSignalQPushButton *ww15push = new WSignalQPushButton();
  scene->addWidget(ww15push);
  ww15ptr->addButtonAndLabel(ww15label,ww15push);
  ww15ptr->moveButton(1125,-225);
  ww15ptr->moveLabel(1085,-200);

  WSignalQPushButton *ww16push = new WSignalQPushButton();
  scene->addWidget(ww16push);
  ww16ptr->addButtonAndLabel(ww16label,ww16push);
  ww16ptr->moveButton(2085,-225);
  ww16ptr->moveLabel(2095,-240);

  WSignalQPushButton *ww17push = new WSignalQPushButton();
  scene->addWidget(ww17push);
  ww17ptr->addButtonAndLabel(ww17label,ww17push);
  ww17ptr->moveButton(2375,-225);
  ww17ptr->moveLabel(2335,-200);

  WSignalQPushButton *ww18push = new WSignalQPushButton();
  scene->addWidget(ww18push);
  ww18ptr->addButtonAndLabel(ww18label,ww18push);
  ww18ptr->moveButton(2045,-185);
  ww18ptr->moveLabel(2080,-175);

  WSignalQPushButton *ww19push = new WSignalQPushButton();
  scene->addWidget(ww19push);
  ww19ptr->addButtonAndLabel(ww19label,ww19push);
  ww19ptr->moveButton(2250,25);
  ww19ptr->moveLabel(2200,20);

  WSignalQPushButton *ww20push = new WSignalQPushButton();
  scene->addWidget(ww20push);
  ww20ptr->addButtonAndLabel(ww20label,ww20push);
  ww20ptr->moveButton(2410,-300);
  ww20ptr->moveLabel(2370,-275);

  WSignalQPushButton *ww21push = new WSignalQPushButton();
  scene->addWidget(ww21push);
  ww21ptr->addButtonAndLabel(ww21label,ww21push);
  ww21ptr->moveButton(2210,-300);
  ww21ptr->moveLabel(2230,-315);

  WSignalQPushButton *ww22push = new WSignalQPushButton();
  scene->addWidget(ww22push);
  ww22ptr->addButtonAndLabel(ww22label,ww22push);
  ww22ptr->moveButton(2220,-370);
  ww22ptr->moveLabel(2240,-390);

  WSignalQPushButton *ww23push = new WSignalQPushButton();
  scene->addWidget(ww23push);
  ww23ptr->addButtonAndLabel(ww23label,ww23push);
  ww23ptr->moveButton(1815,225);
  ww23ptr->moveLabel(1855,200); 

  // initalize clickmanager
  //************************************************************************** 
  
  clickmanager *c1 = new clickmanager;
  QObject::connect(s1ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) ); //QOverload because of the two different recieveFS methods with different arguments; C++11, C++14 qOverload<>
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
  QObject::connect(s24ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s25ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s26ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s27ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s28ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s29ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s30ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s31ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s32ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s33ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s34ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s35ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s36ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s37ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s38ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s39ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s40ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s41ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s42ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s43ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s44ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s45ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s46ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s47ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s48ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s49ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s50ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s51ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(s52ptr,&HSignal::listened,c1,QOverload<HSignal*>::of(&clickmanager::recieveFS) );
  //also for WSignal
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
  QObject::connect(ww12ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww13ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww14ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww15ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww16ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww17ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww18ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww19ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww20ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww21ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww22ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww23ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww24ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww25ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww26ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww27ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww28ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww29ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww30ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww31ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww32ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww33ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww34ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww35ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww36ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww37ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww38ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww39ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww40ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww41ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww42ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  QObject::connect(ww43ptr,&WSignal::listened,c1,QOverload<WSignal*>::of(&clickmanager::recieveFS) );
  //:::::::::


  // worker thread for segment/turnout updates = input
  //************************************************************************** 
  QThread* thread = new QThread;
  worker* wrkr = new worker(config);
  wrkr->moveToThread(thread);
  QObject::connect(thread, &QThread::started, wrkr, &worker::timing); //thread start connection
  QObject::connect(&a, &QApplication::aboutToQuit, []() {
    qDebug() << "__main__: Application is about to quit!";
  });
  qDebug()<<QObject::connect(&a, &QApplication::aboutToQuit, wrkr, &worker::quit, Qt::DirectConnection); // quit updateBelegt on aboutToQuit
  QObject::connect(wrkr, &worker::finished, thread, &QThread::quit);
  QObject::connect(thread, &QThread::finished, wrkr, &worker::deleteLater);
  QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
  //QObject::connect(a, &&QApplication::aboutToQuit, thread, &QThread::quit); // quit thread on aboutToQuit
  QObject::connect(wrkr, &worker::callGUIb, &w, &MainWindow::calledb); // connection worker<->GUI update Block
  QObject::connect(wrkr, &worker::callGUIw, &w, &MainWindow::calledw); // connection worker<->GUI update Weiche
  QObject::connect(wrkr, &worker::callGUIbu, &w, &MainWindow::calledbu); // connection worker<->GUI update BU
  //thread can be finished by calling wrkr.quit() -> updateBelegt() -> emit finished -> above slot

  // memory thread for route memory
  //**************************************************************************  
  QThread* thread2 = new QThread;
  Spmemory *mem = new Spmemory();
  mem->moveToThread(thread2);
  QObject::connect(thread2, &QThread::started, mem, &Spmemory::timing); //thread start connection 
  QObject::connect(&a, &QApplication::aboutToQuit, mem, &Spmemory::quit); // quit timing on aboutToQuit
  QObject::connect(mem, &Spmemory::finished, thread2, &QThread::quit);
  QObject::connect(thread2, &QThread::finished, mem, &Spmemory::deleteLater);
  QObject::connect(thread2, &QThread::finished, thread2, &QThread::deleteLater);
  //QObject::connect(a, &&QApplication::aboutToQuit, thread2, &QThread::quit); // quit thread2 on aboutToQuit

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
  QObject::connect(s24ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s25ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s26ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s27ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s28ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s29ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s30ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s31ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s32ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s33ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s34ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s35ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s36ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s37ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s38ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s39ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s40ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s41ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s42ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s43ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s44ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s45ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s46ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s47ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s48ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s49ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s50ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s51ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  QObject::connect(s52ptr, &HSignal::callspmemory,mem, &Spmemory::addFS);
  thread2->start();
  
  w.showFullScreen(); //FullScreen as default


  // setting up hadware connections = GPIO pins
  //************************************************************************** 
  //wiringPiSetupGpio(); cf beginning of program (also needed in mainwindow)

  if( config->getWithHardware() ){
    sr595Setup (100, 28, 9, 5, 11);                    //pin base, pin number = active, serial, clock, refresh
    sr595Setup (200, 30, 19, 6, 26); 
    sr595Setup (300, 28, 17, 4, 27);
  
    //do not rely on random state of shift register: set all to LOW=0
    for(int pipin = 100; pipin < 132; pipin++){
      digitalWrite(pipin,LOW);                                                                
      digitalWrite(pipin+100,LOW);
      digitalWrite(pipin+200,LOW);
    }
  }

  //initialize all turnouts with pins:
  //   it seems that it is relevant, if there are spare pins or not (some turnouts switch status altough no hardware problems or Stellwerk commands in switchWeiche are called). Releated to wiringPi and choosen pinBase together with shifting immediately the shift register status to output
  w1ptr->setGpio(124,125); 
  w2ptr->setGpio(127,126); 
  w20ptr->setGpio(107,106);
  w21ptr->setGpio(105,104);
  w22ptr->setGpio(103,102);
  w23ptr->setGpio(100,101);
  w24ptr->setGpio(115,114);
  w25ptr->setGpio(113,112);
  w26ptr->setGpio(111,110);
  w27ptr->setGpio(108,109);
  w28ptr->setGpio(123,122); 
  w29ptr->setGpio(120,121);
  w30ptr->setGpio(119,118);
  w31ptr->setGpio(116,117); //spare pins: 128,129,130,131 --> see pinBase=28

  w3ptr->setGpio(200,201);
  w4ptr->setGpio(202,203);
  w5ptr->setGpio(204,205);
  w6ptr->setGpio(206,207);
  w7ptr->setGpio(209,208);
  w8ptr->setGpio(210,211);
  w9ptr->setGpio(213,212);
  w10ptr->setGpio(215,214);
  w11ptr->setGpio(217,216);
  w12ptr->setGpio(219,218);
  w13ptr->setGpio(220,221);
  connector1->setGpio(223,222); //instead of w14ptr->setGpio(223,222); //w15 is spare, double turnout with connector1
  w16ptr->setGpio(225,224); 
  connector2->setGpio(227,226); //instead of w17ptr->setGpio(226,227); //w18 is spare, double turnout with connector2
  w19ptr->setGpio(228,229);
                            //230, 231 are spare pins --> see pinBase=30

  w40ptr->setGpio(307,306);
  w41ptr->setGpio(305,304);
  w42ptr->setGpio(303,302);
  w43ptr->setGpio(300,301);
  w44ptr->setGpio(314,315);
  w45ptr->setGpio(312,313);
  w46ptr->setGpio(310,311);
  w47ptr->setGpio(309,308);
  w48ptr->setGpio(323,322);
  w49ptr->setGpio(320,321);
  w50ptr->setGpio(319,318);
  w51ptr->setGpio(316,317);
  w52ptr->setGpio(327,326);
  w53ptr->setGpio(324,325);
                            //327, 326, 325, 324 are spare pins --> see pinBase=28
  
  // setting all segments with pins:
  if( config->getWithHardware() ){
    mcp23017Setup (400, 0x20);
    mcp23017Setup (500, 0x21);
    mcp23017Setup (600, 0x22);
    mcp23017Setup (700, 0x23);
    for(int mcp = 0; mcp < 16; mcp++){
      pinMode (400 + mcp, INPUT);
      pinMode (500 + mcp, INPUT);
      pinMode (600 + mcp, INPUT);
      pinMode (700 + mcp, INPUT);
      pullUpDnControl(400 + mcp, PUD_UP);
      pullUpDnControl(500 + mcp, PUD_UP);
      pullUpDnControl(600 + mcp, PUD_UP);
      pullUpDnControl(700 + mcp, PUD_UP);
    }
  }

  // 0x20
  byptr->setGpio(400);
  abptr->setGpio(401);
  ahptr->setGpio(402);
  bzptr->setGpio(403);
  bxptr->setGpio(404);
  bpptr->setGpio(415);
  bqptr->setGpio(414);
  brptr->setGpio(413);
  bsptr->setGpio(412);
  btptr->setGpio(411);
  buptr->setGpio(410);
  bvptr->setGpio(409);
  bwptr->setGpio(408);
  apptr->setGpio(405);
  aiptr->setGpio(406);
  acptr->setGpio(407);

  // 0x21
  aoptr->setGpio(500);
  ajptr->setGpio(501);
  akptr->setGpio(502);
  amptr->setGpio(503);
  afptr->setGpio(504);
  agptr->setGpio(505);
  adptr->setGpio(506);
  avptr->setGpio(507);
  //->setGpio(508);
  alptr->setGpio(509);
  auptr->setGpio(510);
  anptr->setGpio(511);
  arptr->setGpio(512);
  aqptr->setGpio(513);
  aaptr->setGpio(514);
  aeptr->setGpio(515);

  // 0x22
  bjptr->setGpio(600);
  biptr->setGpio(601);
  bhptr->setGpio(602);
  bgptr->setGpio(603);
  bfptr->setGpio(604);
  beptr->setGpio(605);
  bdptr->setGpio(606);
  bcptr->setGpio(607);
  asptr->setGpio(608);
  atptr->setGpio(609);
  awptr->setGpio(610);
  axptr->setGpio(611);
  ayptr->setGpio(612);
  azptr->setGpio(613);
  baptr->setGpio(614);
  bbptr->setGpio(615);

  // 0x23
  //->setGpio(700);
  //->setGpio(701);
  //->setGpio(702);
  //->setGpio(703);
  //->setGpio(704);
  //->setGpio(705);
  //->setGpio(706);
  //->setGpio(707);
  bkptr->setGpio(708);
  blptr->setGpio(709);
  bmptr->setGpio(710);
  bnptr->setGpio(711);
  boptr->setGpio(712);
  captr->setGpio(713);
  cbptr->setGpio(714);
  //->setGpio(715);
  
  if( config->getWithHardware() ){
    //give them to the worker
    // 0x20
    wrkr->addBlocks(abptr);
    wrkr->addBlocks(ahptr);
    wrkr->addBlocks(bxptr);
    wrkr->addBlocks(byptr);
    wrkr->addBlocks(bzptr);
    wrkr->addBlocks(bpptr);
    wrkr->addBlocks(bqptr);
    wrkr->addBlocks(brptr);
    wrkr->addBlocks(bsptr);
    wrkr->addBlocks(btptr);
    wrkr->addBlocks(buptr);
    wrkr->addBlocks(bvptr);
    wrkr->addBlocks(bwptr);
    wrkr->addBlocks(apptr);
    wrkr->addBlocks(aiptr);
    wrkr->addBlocks(acptr);

    // 0x21
    wrkr->addBlocks(aoptr);
    wrkr->addBlocks(ajptr);
    wrkr->addBlocks(akptr);
    wrkr->addBlocks(amptr);
    wrkr->addBlocks(afptr);
    wrkr->addBlocks(agptr);
    wrkr->addBlocks(adptr);
    wrkr->addBlocks(avptr);
    wrkr->addBlocks(alptr);
    //wrkr->addBlocks();
    wrkr->addBlocks(auptr);
    wrkr->addBlocks(anptr);
    wrkr->addBlocks(arptr);
    wrkr->addBlocks(aqptr);
    wrkr->addBlocks(aaptr);
    wrkr->addBlocks(aeptr);

    // 0x22
    wrkr->addBlocks(bjptr);
    wrkr->addBlocks(biptr);
    wrkr->addBlocks(bhptr);
    wrkr->addBlocks(bgptr);
    wrkr->addBlocks(bfptr);
    wrkr->addBlocks(beptr);
    wrkr->addBlocks(bdptr);
    wrkr->addBlocks(bcptr);
    wrkr->addBlocks(asptr);
    wrkr->addBlocks(atptr);
    wrkr->addBlocks(awptr);
    wrkr->addBlocks(axptr);
    wrkr->addBlocks(ayptr);
    wrkr->addBlocks(azptr);
    wrkr->addBlocks(baptr);
    wrkr->addBlocks(bbptr);

    // 0x23
    wrkr->addBlocks(bkptr);
    wrkr->addBlocks(blptr);
    wrkr->addBlocks(bmptr);
    wrkr->addBlocks(bnptr);
    wrkr->addBlocks(boptr);
    wrkr->addBlocks(captr);
    wrkr->addBlocks(cbptr);

    // BU
    wrkr->addBUs(bu1ptr);
  }
  
  thread->start();
  
  // end of program; release resources and write current turnout states
  //************************************************************************** 
  // all classes with parents are deleted through this chain, addItem, addWidget takes ownership, the rest is left for the programmer:
  int eofprogram = a.exec();
  qDebug() <<"__main__: end of event loop";

  // add turnouts to filemanager and write the states to the txt file
  fmngr.add(w1ptr->getW_status()); fmngr.add(w2ptr->getW_status()); fmngr.add(w3ptr->getW_status()); fmngr.add(w4ptr->getW_status()); fmngr.add(w5ptr->getW_status()); fmngr.add(w6ptr->getW_status()); fmngr.add(w7ptr->getW_status());  fmngr.add(w8ptr->getW_status()); fmngr.add(w9ptr->getW_status()); fmngr.add(w10ptr->getW_status()); fmngr.add(w11ptr->getW_status()); fmngr.add(w12ptr->getW_status()); fmngr.add(w13ptr->getW_status()); fmngr.add(w14ptr->getW_status());  fmngr.add(w15ptr->getW_status()); fmngr.add(w16ptr->getW_status()); fmngr.add(w17ptr->getW_status()); fmngr.add(w18ptr->getW_status()); fmngr.add(w19ptr->getW_status()); fmngr.add(w20ptr->getW_status()); fmngr.add(w21ptr->getW_status());  fmngr.add(w22ptr->getW_status()); fmngr.add(w23ptr->getW_status()); fmngr.add(w24ptr->getW_status()); fmngr.add(w25ptr->getW_status()); fmngr.add(w26ptr->getW_status()); fmngr.add(w27ptr->getW_status()); fmngr.add(w28ptr->getW_status());  fmngr.add(w29ptr->getW_status()); fmngr.add(w30ptr->getW_status()); fmngr.add(w31ptr->getW_status());
  //order is relevant: spare turnouts [32;39], fill with "false"
  fmngr.add(false); fmngr.add(false); fmngr.add(false); fmngr.add(false); fmngr.add(false); fmngr.add(false); fmngr.add(false); fmngr.add(false); 
  fmngr.add(w40ptr->getW_status()); fmngr.add(w41ptr->getW_status()); fmngr.add(w42ptr->getW_status()); fmngr.add(w43ptr->getW_status()); fmngr.add(w44ptr->getW_status()); fmngr.add(w45ptr->getW_status()); fmngr.add(w46ptr->getW_status()); fmngr.add(w47ptr->getW_status()); fmngr.add(w48ptr->getW_status()); fmngr.add(w49ptr->getW_status()); fmngr.add(w50ptr->getW_status()); fmngr.add(w51ptr->getW_status()); fmngr.add(w52ptr->getW_status()); fmngr.add(w53ptr->getW_status()); 
  fmngr.write();
  //
  //delete in reverse order (thread and thread2 with wrkr and mem are already deleted with signals and slots)
  delete c1;
   
  delete stellwerkstecptr;
  
  delete aaptr; delete abptr; delete acptr; delete adptr; delete aeptr; delete afptr; delete agptr; delete ahptr; delete aiptr; delete ajptr; delete akptr; delete alptr; delete amptr; delete anptr; delete aoptr; delete apptr; delete aqptr; delete arptr; delete asptr; delete atptr; delete avptr; delete awptr; delete axptr; delete ayptr; delete azptr; delete baptr; delete bbptr; delete bcptr; delete bdptr; delete beptr; delete bfptr; delete bgptr; delete bhptr; delete biptr; delete bjptr; delete bkptr; delete blptr; delete bmptr; delete bnptr; delete boptr; delete bpptr; delete bqptr; delete brptr; delete bsptr; delete btptr; delete buptr; delete bvptr; delete bwptr; delete bxptr; delete byptr; delete bzptr; delete captr; delete cbptr;
  
  delete ww1ptr; delete ww2ptr; delete ww3ptr; delete ww4ptr; delete ww5ptr; delete ww6ptr; delete ww7ptr; delete ww8ptr; delete ww9ptr; delete ww10ptr; delete ww11ptr; delete ww12ptr; delete ww13ptr; delete ww14ptr; delete ww15ptr; delete ww16ptr; delete ww17ptr; delete ww18ptr; delete ww19ptr; delete ww20ptr; delete ww21ptr; delete ww22ptr; delete ww23ptr; delete ww24ptr; delete ww25ptr; delete ww26ptr; delete ww27ptr; delete ww28ptr; delete ww29ptr; delete ww30ptr; delete ww31ptr; delete ww32ptr; delete ww33ptr; delete ww34ptr; delete ww35ptr; delete ww36ptr; delete ww37ptr; delete ww38ptr; delete ww39ptr; delete ww40ptr; delete ww41ptr; delete ww42ptr; delete ww43ptr; 
  
  delete v1ptr; delete v2ptr; delete v3ptr; delete v4ptr; delete v5ptr; delete v6ptr; delete v7ptr; delete v8ptr; delete v9ptr; delete v10ptr; delete v11ptr; delete v12ptr; delete v13ptr; delete v14ptr; delete v15ptr; delete v16ptr; delete v17ptr; delete  v18ptr; delete v19ptr; delete v20ptr; delete v21ptr; delete v22ptr; delete v23ptr; delete v24ptr; delete v25ptr; delete v26ptr; delete v27ptr; delete v28ptr; delete v29ptr; delete v30ptr; delete v31ptr; delete v32ptr; delete v33ptr; delete v34ptr; delete v35ptr; delete v36ptr; delete v37ptr; delete v38ptr; delete v39ptr; delete v40ptr; delete v41ptr; delete v42ptr; delete v43ptr; delete v44ptr;
  
  delete w1ptr; delete w2ptr; delete w3ptr; delete w4ptr; delete w5ptr; delete w6ptr; delete w7ptr; delete w8ptr; delete w9ptr; delete w10ptr; delete w11ptr; delete w12ptr; delete w13ptr; delete w14ptr; delete w15ptr; delete w16ptr; delete w17ptr; delete w18ptr; delete w19ptr; delete w20ptr; delete w21ptr; delete w22ptr; delete w23ptr; delete w24ptr; delete w25ptr; delete w26ptr; delete w27ptr; delete w28ptr; delete w29ptr; delete w30ptr; delete w31ptr; delete w40ptr; delete w41ptr; delete w42ptr; delete w43ptr; delete w44ptr; delete w45ptr; delete w46ptr; delete w47ptr; delete w48ptr; delete w49ptr; delete w50ptr; delete w51ptr; delete w52ptr; delete w53ptr;
  
  delete s1ptr; delete s2ptr; delete s3ptr; delete s4ptr; delete s5ptr; delete s6ptr; delete s7ptr; delete s8ptr; delete s9ptr; delete s10ptr; delete s11ptr; delete s12ptr; delete s13ptr; delete s14ptr; delete s15ptr; delete s16ptr; delete s17ptr; delete s18ptr; delete s19ptr; delete s20ptr; delete s21ptr; delete s22ptr; delete s23ptr; delete s24ptr; delete s25ptr; delete s26ptr; delete s27ptr; delete s28ptr; delete s29ptr; delete s30ptr; delete s31ptr; delete s32ptr; delete s33ptr; delete s34ptr; delete s35ptr; delete s36ptr; delete s37ptr; delete s38ptr; delete s39ptr; delete s40ptr; delete s41ptr; delete s42ptr; delete s43ptr; delete s44ptr; delete s45ptr; delete s46ptr; delete s47ptr; delete s48ptr; delete s49ptr; delete s50ptr; delete s51ptr; delete s52ptr;
  
  return eofprogram;

}
