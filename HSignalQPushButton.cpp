/*
 * Custom QPushButton for StellwerkQt [- HSIGNALQPUSHBUTTON.CPP -]
 *******************************************************************************
 */
#include "HSignalQPushButton.h"
#include <QMenu>
HSignalQPushButton::HSignalQPushButton(QWidget *parent) : StellwerkQPushButton(parent),
							  sato(new QAction("Show all turnout status", this)),
							  sas(new QAction("Show all segment status", this)),
							  savr(new QAction("Show all VSignal status (backwards)", this)),
							  savv(new QAction("Show all VSignal status (forwards)", this)),
							  sawr(new QAction("Show all WSignal status (backwards)", this)),
							  sawv(new QAction("Show all WSignal status (forwards)", this)){
  menu->addAction(sato);
  menu->addAction(sas);
  menu->addAction(savr);
  menu->addAction(savv);
  menu->addAction(sawr);
  menu->addAction(sawv);
}
