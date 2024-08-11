/*
 * Custom QPushButton for StellwerkQt [- HSIGNALQPUSHBUTTON.CPP -]
 *******************************************************************************
 */
#include "HSignalQPushButton.h"
#include <QMenu>
HSignalQPushButton::HSignalQPushButton(QWidget *parent) : StellwerkQPushButton(parent) {
  menu->addAction(new QAction("Show turnout status", this));
  menu->addAction(new QAction("Show segements", this));
  menu->addAction(new QAction("Show all segments", this));
  menu->addAction(new QAction("Show pre-signal backwards", this));
  menu->addAction(new QAction("Show pre-signal forwards", this));
  menu->addAction(new QAction("Show shunt signal backwards", this));
  menu->addAction(new QAction("Show shunt signal forwards", this));
}
