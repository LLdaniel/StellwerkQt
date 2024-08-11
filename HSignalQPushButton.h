/*
 * Custom QPushButton for StellwerkQt [- HSIGNALQPUSHBUTTON.H -]
 *******************************************************************************
 * extends QPushButton in order to make custom context menus possible          *
 *******************************************************************************
 */
#ifndef HSIGNALQPUSHBUTTON_H
#define HSIGNALQPUSHBUTTON_H
#include <QObject>
#include "StellwerkQPushButton.h"
class HSignalQPushButton : public StellwerkQPushButton {
  Q_OBJECT
 public:
  HSignalQPushButton(QWidget *parent = 0);  
};
#endif
