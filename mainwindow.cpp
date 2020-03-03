#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeySequence>
#include <QGraphicsView>

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
    //registriere Mouse Event, lokalisiere auf was geklickt wurde und wie, rufe Funktion auf, die nochmal einen Klick erwartet (auf Zielsignal), rufe dann setFahrt auf
    if(event->button() == Qt::LeftButton){
        qDebug()<<"Left mouse button was pressed";
        qDebug()<<"x= "<<event->x()<<" and y="<<event->y();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Escape){
        qDebug()<<"ESC was pressed";
    }
}


void MainWindow::createMenus(){
  ccMenu = menuBar()->addMenu(tr("&ControlCenter"));
  ccMenu->addAction(shutdownAct);

  piMenu = menuBar()->addMenu(tr("&RaspberryPi"));
  piMenu->addAction(resetAct);

  viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction(upscaleAct);
  viewMenu->addAction(downscaleAct);

  aboutMenu = menuBar()->addMenu(tr("&About"));
  aboutMenu->addAction(aboutAct);
  aboutBox.setText("<b>About the ControlCenter </b><br>This is an Electronic Signalling Control Center. It is modeled on <i>german 'ESTW'</i> control center of DB.<br><br>It is possible to signal the tracks as well as switching turnouts and processing feedback from monitored track segments.<br><br>For further information please see https://github.com/LLdaniel/StellwerkQt/wiki.");
  aboutMenu->addAction(helpAct);
  helpBox.setText("<b>How to control:</b><br>Guide the model trains through complete routes via PushButtons near the signals. Tournouts and distant signals will be automatically set.<br>It is possible to memorize a route from main signal to main signal. This will be indicated as yellow dots.<br><br>For further information please see https://github.com/LLdaniel/StellwerkQt/wiki</a>.");
}

void MainWindow::createActions(){
  shutdownAct = new QAction(tr("&Shutdown"), this);
  shutdownAct->setShortcuts(QKeySequence::Quit);
  shutdownAct->setStatusTip(tr("Initiate control center shutdown"));
  QObject::connect(shutdownAct, &QAction::triggered, this, &MainWindow::shutdown);

  resetAct = new QAction(tr("&Reset Pins"), this);
  resetAct->setShortcuts(QKeySequence::Refresh);
  resetAct->setStatusTip(tr("Sets pi pins as input"));
  QObject::connect(resetAct, &QAction::triggered, this, &MainWindow::reset);

  upscaleAct = new QAction(tr("&ZoomOut"), this);
  QKeySequence u(Qt::ControlModifier + Qt::Key_Minus);
  QList<QKeySequence> upSeq;
  upSeq.append(u);
  upscaleAct->setShortcuts(upSeq);
  upscaleAct->setStatusTip(tr("up-scales view"));
  QObject::connect(upscaleAct, &QAction::triggered, this, &MainWindow::upscale);
  downscaleAct = new QAction(tr("&ZoomIn"), this);
  QKeySequence d(Qt::ControlModifier + Qt::Key_Plus);
  QList<QKeySequence> downSeq;
  downSeq.append(d);
  downscaleAct->setShortcuts(downSeq);
  downscaleAct->setStatusTip(tr("down-scales view"));
  QObject::connect(downscaleAct, &QAction::triggered, this, &MainWindow::downscale);
  
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
  ((QGraphicsView*)this->centralWidget())->scale(0.5,0.5);
}

void MainWindow::downscale(){
  ((QGraphicsView*)this->centralWidget())->scale(1.5,1.5);
}
