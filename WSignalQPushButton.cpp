/*
 * Custom QPushButton for StellwerkQt [- WSIGNALQPUSHBUTTON.CPP -]
 *******************************************************************************
 */
#include "WSignalQPushButton.h"
#include <QMenu>
WSignalQPushButton::WSignalQPushButton(QWidget *parent) : StellwerkQPushButton(parent) {
  menu->addAction(new QAction("Show turnout status", this));
  menu->addAction(new QAction("Show all turnout status", this));
  menu->addAction(new QAction("Show segements", this));
  menu->addAction(new QAction("Show all segments", this));
}
