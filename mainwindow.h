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
private:
  void createActions();
  void createMenus();
  Ui::MainWindow *ui;
  QMenu *ccMenu;
  QMenu *piMenu;
  QMenu *aboutMenu;
  QAction *shutdownAct;
  QAction *resetAct;
  QAction *aboutAct;
  QMessageBox aboutBox;
  QAction *helpAct;
  QMessageBox helpBox;
};

#endif // MAINWINDOW_H
