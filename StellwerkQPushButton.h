/*
 * Custom QPushButton for StellwerkQt [- STELLWERKQPUSHBUTTON.H -]
 *******************************************************************************
 * extends QPushButton in order to make custom context menus possible          *
 *******************************************************************************
 */
#ifndef STELLWERKQPUSHBUTTON_H
#define STELLWERKQPUSHBUTTON_H
#include <QObject>
#include <QPushButton>
#include <QMouseEvent>
class StellwerkQPushButton : public QPushButton {
  Q_OBJECT
public:
  StellwerkQPushButton(QWidget *parent = 0);
  QMenu* getMenu(){return menu;}
protected slots:
  void mousePressEvent(QMouseEvent *mouseEvent) override;
signals:
  void rightClicked();
protected:
  QMenu *menu;
  
};
#endif
