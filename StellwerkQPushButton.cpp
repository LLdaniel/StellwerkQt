/*
 * Custom QPushButton for StellwerkQt [- STELLWEKQPUSHBUTTON.CPP -]
 *******************************************************************************
 */
#include "StellwerkQPushButton.h"
#include <QMenu>
StellwerkQPushButton::StellwerkQPushButton(QWidget *parent) : QPushButton(parent){
  menu = new QMenu(this);
  menu->setStyleSheet("QMenu::item { background-color: #8282fc; }");
}

void StellwerkQPushButton::mousePressEvent(QMouseEvent *mouseEvent){
  /* from Qt6 QAbstractButton:
  void QAbstractButton::mousePressEvent(QMouseEvent *e)
  {
    Q_D(QAbstractButton);
    if (e->button() != Qt::LeftButton) {
      e->ignore();
      return;
    }
    if (hitButton(
		  e->position().toPoint())) {
      setDown(true);
      d->pressed = true;
      repaint();
      d->emitPressed();
      e->accept();
    } else {
      e->ignore();
    }
  }
  */
  if(mouseEvent->button() == Qt::RightButton){
    emit rightClicked();
  }
  else if(mouseEvent->button() == Qt::LeftButton){
    setDown(true);
    repaint();
    emit pressed();
    mouseEvent->accept();
  }
}
