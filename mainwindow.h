#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMouseEvent>
#include <QMainWindow>
#include <QMenuBar>
#include <QKeyEvent>
#include <QAction>

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
private:
  void createActions();
  void createMenus();
  Ui::MainWindow *ui;
  QMenu *ccMenu;
  QMenu *piMenu;
  QAction *shutdownAct;
  QAction *resetAct;
};

#endif // MAINWINDOW_H
