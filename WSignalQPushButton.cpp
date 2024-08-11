/*
 * Custom QPushButton for StellwerkQt [- WSIGNALQPUSHBUTTON.CPP -]
 *******************************************************************************
 */
#include "WSignalQPushButton.h"
#include <QMenu>
WSignalQPushButton::WSignalQPushButton(QWidget *parent) : StellwerkQPushButton(parent),
							  sato(new QAction("Show all turnout status", this)),
							  sas(new QAction("Show all segments", this)){
  menu->addAction(sato);
  menu->addAction(sas);
}
