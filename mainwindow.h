/*
 * mainwindow and plan [- MAINWINDOW.H -]
 *******************************************************************************
 * GUI related control center                                                  *
 *******************************************************************************
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMouseEvent>
#include <QMainWindow>
#include <QMenuBar>
#include <QKeyEvent>
#include <QAction>
#include <QMessageBox>
#include "Block.h"
#include "Weiche.h"
#include "BU.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void mousePressEvent(QMouseEvent *event);
  void keyPressEvent(QKeyEvent *event);
signals:
  void shutdown();
  void reset();
  
private slots:
  void showAboutDialog();
  void showHelpDialog();
  void upscale();
  void downscale();
  void powerOffTurnouts();
  void powerOnTurnouts();
  void screen();
  void setMinimalistic(){}
  void setBasic(){}
  void setDetailed(){}
public slots:
  void calledw(Weiche *callw, bool statew);
  void calledb(Block *callb, bool stateb);
  void calledbu(BU *callbu, bool statebu);
private:
  int powerturnouts = 13;
  void createActions();
  void createMenus();
  Ui::MainWindow *ui;
  QMenu *ccMenu;
  QMenu *piMenu;
  QMenu *viewMenu;
  QMenu *aboutMenu;
  QMenu *subMenuSignals;
  QAction *shutdownAct;
  QAction *poweronTurnouts;
  QAction *poweroffTurnouts;
  QAction *resetAct;
  QAction *aboutAct;
  QAction *upscaleAct;
  QAction *downscaleAct;
  QAction *fullscreenAct;
  QMessageBox aboutBox;
  QAction *helpAct;
  QMessageBox helpBox;
  QAction *minimalisticAct;
  QAction *basicAct;
  QAction *detailedAct;
};

#endif // MAINWINDOW_H
