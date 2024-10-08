/*
 * mainwindow and plan [- MAINWINDOW.CXX -]
 **************************************************************************
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeySequence>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#ifdef __cplusplus
extern "C"{
#endif
#include <WiringPi/wiringPi/wiringPi.h>
#ifdef __cplusplus
}
#endif
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    createActions();
    createMenus();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
      
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Escape){
      
    }
}


void MainWindow::createMenus(){
  ccMenu = menuBar()->addMenu(tr("&ControlCenter"));
  ccMenu->addAction(shutdownAct);
  ccMenu->addAction(poweronTurnouts);
  ccMenu->addAction(poweroffTurnouts);

  piMenu = menuBar()->addMenu(tr("&RaspberryPi"));
  piMenu->addAction(resetAct);

  viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction(upscaleAct);
  viewMenu->addAction(downscaleAct);
  viewMenu->addAction(fullscreenAct);

  subMenuSignals = viewMenu->addMenu(tr("&Display signals"));
  subMenuSignals->addAction(minimalisticAct);
  subMenuSignals->addAction(basicAct);
  subMenuSignals->addAction(detailedAct);

  aboutMenu = menuBar()->addMenu(tr("&About"));
  aboutMenu->addAction(aboutAct);
  aboutBox.setText("<b>About the ControlCenter </b><br>This is an Electronic Signalling Control Center. It is modeled on <i>german 'ESTW'</i> control center of DB.<br><br>It is possible to signal the tracks as well as switching turnouts and processing feedback from monitored track segments.<br><br>For further information please see https://github.com/LLdaniel/StellwerkQt/wiki.");
  aboutMenu->addAction(helpAct);
  helpBox.setText("<b>How to control:</b><br>Guide the model trains through complete routes via PushButtons near the signals. Tournouts and distant signals will be automatically set.<br>It is possible to memorize a route from main signal to main signal. This will be indicated as yellow dots.<br><br>For further information please see https://github.com/LLdaniel/StellwerkQt/wiki</a>.");
}

void MainWindow::createActions(){
  shutdownAct = new QAction(tr("&Shutdown"), this);
  QKeySequence s(Qt::ControlModifier | Qt::Key_Q);
  QList<QKeySequence> shtdwnSeq;
  shtdwnSeq.append(s);
  shutdownAct->setShortcuts(shtdwnSeq);
  shutdownAct->setStatusTip(tr("Initiate control center shutdown"));
  QObject::connect(shutdownAct, &QAction::triggered, this, &MainWindow::shutdown);

  poweronTurnouts = new QAction(tr("&Power on turnouts"), this);
  poweronTurnouts->setShortcuts(QKeySequence::Paste);
  poweronTurnouts->setStatusTip(tr("Power on turnouts"));
  QObject::connect(poweronTurnouts, &QAction::triggered, this, &MainWindow::powerOnTurnouts);

  poweroffTurnouts = new QAction(tr("&Power off turnouts"), this);
  poweroffTurnouts->setShortcuts(QKeySequence::Undo);
  poweroffTurnouts->setStatusTip(tr("Power off turnouts"));
  QObject::connect(poweroffTurnouts, &QAction::triggered, this, &MainWindow::powerOffTurnouts);

  resetAct = new QAction(tr("&Reset Pins"), this);
  resetAct->setShortcuts(QKeySequence::Refresh);
  resetAct->setStatusTip(tr("Sets pi pins as input"));
  QObject::connect(resetAct, &QAction::triggered, this, &MainWindow::reset);

  upscaleAct = new QAction(tr("&ZoomOut"), this);
  QKeySequence u(Qt::ControlModifier | Qt::Key_Minus);
  QList<QKeySequence> upSeq;
  upSeq.append(u);
  upscaleAct->setShortcuts(upSeq);
  upscaleAct->setStatusTip(tr("up-scales view"));
  QObject::connect(upscaleAct, &QAction::triggered, this, &MainWindow::upscale);
  downscaleAct = new QAction(tr("&ZoomIn"), this);
  QKeySequence d(Qt::ControlModifier | Qt::Key_Plus);
  QList<QKeySequence> downSeq;
  downSeq.append(d);
  downscaleAct->setShortcuts(downSeq);
  downscaleAct->setStatusTip(tr("down-scales view"));
  QObject::connect(downscaleAct, &QAction::triggered, this, &MainWindow::downscale);

  fullscreenAct = new QAction(tr("&Fullscreen"), this);
  QKeySequence sc(Qt::ControlModifier | Qt::Key_F);
  QList<QKeySequence> scSeq;
  scSeq.append(sc);
  fullscreenAct->setShortcuts(scSeq);
  fullscreenAct->setStatusTip(tr("Enable/Disable Fullscreen"));
  QObject::connect(fullscreenAct, &QAction::triggered, this, &MainWindow::screen);

  minimalisticAct = new QAction(tr("&minimalistic"), this);
  QKeySequence min(Qt::ControlModifier | Qt::Key_M);
  QList<QKeySequence> minSeq;
  minSeq.append(min);
  minimalisticAct->setShortcuts(minSeq);
  minimalisticAct->setStatusTip(tr("Enable minimalistic signals view"));
  QObject::connect(minimalisticAct, &QAction::triggered, this, &MainWindow::setMinimalistic);

  basicAct = new QAction(tr("&basic"), this);
  QKeySequence base(Qt::ControlModifier | Qt::Key_B);
  QList<QKeySequence> baseSeq;
  baseSeq.append(base);
  basicAct->setShortcuts(baseSeq);
  basicAct->setStatusTip(tr("Enable basic signals view"));
  QObject::connect(basicAct, &QAction::triggered, this, &MainWindow::setBasic);

  detailedAct = new QAction(tr("&detailed"), this);
  QKeySequence det(Qt::ControlModifier | Qt::Key_D);
  QList<QKeySequence> detSeq;
  detSeq.append(det);
  detailedAct->setShortcuts(detSeq);
  detailedAct->setStatusTip(tr("Enable detailed signals view"));
  QObject::connect(detailedAct, &QAction::triggered, this, &MainWindow::setDetailed);
  
  aboutAct = new QAction(tr("&About ControlCenter..."), this);
  aboutAct->setStatusTip(tr("More information about ControlCenter"));
  QObject::connect(aboutAct, &QAction::triggered, this, &MainWindow::showAboutDialog);

  helpAct = new QAction(tr("&Help..."), this);
  helpAct->setStatusTip(tr("Guide for the ControlCenter"));
  QObject::connect(helpAct, &QAction::triggered, this, &MainWindow::showHelpDialog);
}

void MainWindow::showAboutDialog(){
  aboutBox.exec();
}

void MainWindow::showHelpDialog(){
  helpBox.exec();
}

void MainWindow::upscale(){
  ((QGraphicsView*)this->centralWidget())->scale(0.90,0.90);
}

void MainWindow::downscale(){
  ((QGraphicsView*)this->centralWidget())->scale(1.111111,1.111111);
}

void MainWindow::powerOffTurnouts(){
  qInfo()<<"__mainwindow__: Turnouts off!";
  //digitalWrite(powerturnouts,LOW);
}

void MainWindow::powerOnTurnouts(){
  qDebug()<<"__mainwindow__: Turnouts on!";
  //digitalWrite(powerturnouts,HIGH);
}

void MainWindow::screen(){
  if(this->isFullScreen()){
    this->setWindowState(Qt::WindowMaximized);
  } else {
    this->setWindowState(Qt::WindowFullScreen);
  }
}

void MainWindow::calledw(Weiche *callw, bool statew){
  //implement turnout update -> coming from worker thread
}

void MainWindow::calledb(Block *callb, bool stateb){
  callb->setB_status(stateb);
}

void MainWindow::calledbu(BU *callbu, bool statebu){
  callbu->setBU_status(statebu);
}

void MainWindow::setSignalStyle(QString mode){
  // https://github.com/qt/qtsvg/blob/dev/src/svgwidgets/qgraphicssvgitem.h:47: QGraphicsSvgItem::Type == 13
  for(QGraphicsItem *o : this->getScene()->items()){
    if(o->type() == 13){
      QGraphicsSvgItem* s = (QGraphicsSvgItem*) o;
      if(mode == "minimalistic"){
	//WS
	if(s->objectName().startsWith("basic_sh")){
	  QStringList splitted = s->objectName().split("_");
	  s->setElementId(mode + "_" + splitted.at(1));
	  s->setObjectName(mode + "_" + splitted.at(1));
	}
	//VS
	if(s->objectName().contains("_vr") ){
	  QStringList splitted = s->objectName().split("_");
	  s->setElementId(mode + "_" + splitted.at(1));
	  if(splitted.size() == 3){
	    s->setObjectName(mode + "_" + splitted.at(1) + "_ks");
	  }
	  else s->setObjectName(mode + "_" + splitted.at(1));
	}
	//HS
	if(s->objectName().contains("_hp") ){
	  QStringList splitted = s->objectName().split("_");
	  s->setElementId(mode + "_" + splitted.at(1));
	  if(splitted.size() == 3){
	    s->setObjectName(mode + "_" + splitted.at(1) + "_" + splitted.at(2));
	  }
	  else{
	    s->setObjectName(mode + "_" + splitted.at(1));
	  }
	}
      }
      if(mode == "basic"){
	//WS
	if(s->objectName().startsWith("minimalistic_sh")){
	  QStringList splitted = s->objectName().split("_");
	  s->setElementId(mode + "_" + splitted.at(1));
	  s->setObjectName(mode + "_" + splitted.at(1));
	}
	//VS
	if(s->objectName().contains("_vr") ){
	  QStringList splitted = s->objectName().split("_");
	  s->setElementId(mode + "_" + splitted.at(1));
	  if(splitted.size() == 3){
	    s->setObjectName(mode + "_" + splitted.at(1) + "_ks");
	  }
	  else s->setObjectName(mode + "_" + splitted.at(1));
	}
	//HS
	if(s->objectName().contains("_hp") ){
	  QStringList splitted = s->objectName().split("_");
	  s->setElementId(mode + "_" + splitted.at(1));
	  if(splitted.size() == 3){
	    s->setObjectName(mode + "_" + splitted.at(1) + "_" + splitted.at(2));
	  }
	  else{
	    s->setObjectName(mode + "_" + splitted.at(1));
	  }
	}
      }
      if(mode == "realistic"){
	//WS
	if(s->objectName().startsWith("minimalistic_sh")){
	  QStringList splitted = s->objectName().split("_");
	  s->setElementId("basic_" + splitted.at(1));
	  s->setObjectName("basic_" + splitted.at(1));
	}
	//VS
	if(s->objectName().contains("_vr")){
	  QStringList splitted = s->objectName().split("_");
	  //s->setElementId(mode + "_" + splitted.at(1));
	  if(splitted.size() == 3){
	    s->setElementId(mode + "_" + splitted.at(1));
	    s->setObjectName(mode + "_" + splitted.at(1) + "_ks");
	  }
	  else{
	    s->setElementId("basic_" + splitted.at(1));
	    s->setObjectName("basic_" + splitted.at(1));
	  }
	}
	//HS
	if(s->objectName().contains("_hp") ){
	  QStringList splitted = s->objectName().split("_");
	  if(splitted.size() == 3){
	    s->setElementId(mode + "_" + splitted.at(1) + "_" + splitted.at(2));
	    s->setObjectName(mode + "_" + splitted.at(1) + "_" + splitted.at(2));
	  }
	  else{
	    s->setElementId("basic_" + splitted.at(1));
	    s->setObjectName("basic_" + splitted.at(1));
	  }
	}
      }
      qDebug(qUtf8Printable( "__mainwindow__: QGraphicsItem type: " + QString::number(o->type()) + " now using mode: " + mode + " elementId: " + s->elementId() + " objectName: " + s->objectName() ));
    }
  }
}

void MainWindow::setMinimalistic(){
  this->setSignalStyle("minimalistic");
}

void MainWindow::setBasic(){
  this->setSignalStyle("basic");
}

void MainWindow::setDetailed(){
  this->setSignalStyle("realistic");
}

QGraphicsScene* MainWindow::getScene(){
  QGraphicsScene *scene;
  for(QObject *o : this->children()){
    if( QLatin1String(o->metaObject()->className()) == "QGraphicsScene"){
      scene = (QGraphicsScene*) o;
      qDebug("__mainwindow__: Found child item: QGraphicsScene");
      break;
    }
  }
  return scene;
}
