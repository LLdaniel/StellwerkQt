/*
 * mainwindow and plan [- MAINWINDOW.CXX -]
 **************************************************************************
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeySequence>
#include <QGraphicsView>
#ifdef __cplusplus
extern "C"{
#endif
#include <wiringPi.h>
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
  qDebug()<<"Weichen off";
  //digitalWrite(powerturnouts,LOW);
}

void MainWindow::powerOnTurnouts(){
  qDebug()<<"Weichen on";
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
