/*
 * Custom QPushButton for StellwerkQt [- WEICHEQPUSHBUTTON.H -]
 *******************************************************************************
 * extends QPushButton in order to make custom context menus possible          *
 *******************************************************************************
 */
#ifndef WEICHEQPUSHBUTTON_H
#define WEICHEQPUSHBUTTON_H
#include <QObject>
#include "StellwerkQPushButton.h"
class WeicheQPushButton : public StellwerkQPushButton {
  Q_OBJECT
 public:
  WeicheQPushButton(QWidget *parent = 0);
};
#endif
