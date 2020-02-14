#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QKeySequence>

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
        std::cout<<"Left mouse button was pressed"<<std::endl;
        std::cout<<"x= "<<event->x()<<" and y="<<event->y()<<std::endl;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        std::cout<<"ESC was pressed"<<std::endl;
    }
}


void MainWindow::createMenus(){
  ccMenu = menuBar()->addMenu(tr("&ControlCenter"));
  ccMenu->addAction(shutdownAct);

  piMenu = menuBar()->addMenu(tr("&RaspberryPi"));
  piMenu->addAction(resetAct);
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
}

void MainWindow::shutdown(){
  std::cout<<"shuting down"<<std::endl;
}

void MainWindow::reset(){
  std::cout<<"reset menu triggered"<<std::endl;
}
