/*
 * Custom QPushButton for StellwerkQt [- WSIGNALQPUSHBUTTON.H -]
 *******************************************************************************
 * extends QPushButton in order to make custom context menus possible          *
 *******************************************************************************
 */
#ifndef WSIGNALQPUSHBUTTON_H
#define WSIGNALQPUSHBUTTON_H
#include <QObject>
#include "StellwerkQPushButton.h"
class WSignalQPushButton : public StellwerkQPushButton {
  Q_OBJECT
public:
  WSignalQPushButton(QWidget *parent = 0);  
};
#endif
