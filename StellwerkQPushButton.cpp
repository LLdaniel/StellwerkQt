/*
 * Custom QPushButton for StellwerkQt [- STELLWEKQPUSHBUTTON.CPP -]
 *******************************************************************************
 */
#include "StellwerkQPushButton.h"
#include <QMenu>
StellwerkQPushButton::StellwerkQPushButton(QWidget *parent) : QPushButton(parent){
  menu = new QMenu(this);
  menu->setStyleSheet("QMenu { background-color: #8282fc;"
		      "        border: 2px solid #4F4F4F;"
		      "        border-radius: 4px;"
		      "      },"
		      "QMenu::item:selected { font-weight: bold;"
		      "                    background-color: yellow;"
		      "      }"
		      );
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
