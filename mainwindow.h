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
private slots:
  void shutdown();
private:
  void createActions();
  void createMenus();
  Ui::MainWindow *ui;
  QMenu *ccMenu;
  QAction *shutdownAct;
};

#endif // MAINWINDOW_H
