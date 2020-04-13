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
private:
  int powerturnouts = 13;
  void createActions();
  void createMenus();
  Ui::MainWindow *ui;
  QMenu *ccMenu;
  QMenu *piMenu;
  QMenu *viewMenu;
  QMenu *aboutMenu;
  QAction *shutdownAct;
  QAction *poweronTurnouts;
  QAction *poweroffTurnouts;
  QAction *resetAct;
  QAction *aboutAct;
  QAction *upscaleAct;
  QAction *downscaleAct;
  QMessageBox aboutBox;
  QAction *helpAct;
  QMessageBox helpBox;
};

#endif // MAINWINDOW_H
