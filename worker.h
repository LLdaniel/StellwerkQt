/*
 * worker responsible to collect all segments states [- WORKER.H -]
 *******************************************************************************
 * This is the great part of input for the control center                      *
 *    - segment status are updated continueously                               *
 *    - turnout occupation are updated continueously                           *
 *******************************************************************************
 */
#ifndef WORKER_H
#define WORKER_H
#include <Block.h>
#include <Weiche.h>
#include <QList>
#include <QTimer>

class worker : public QObject{                        //after https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
  Q_OBJECT
public:
  worker( QList<Block*> allBlocks, QList<Weiche*> allWeichen );
  worker(){t->callOnTimeout(this, &worker::updateBelegt);}
  ~worker();
  void showBlocks();
  void addBlocks(Block* bl);
  void showWeichen();
public slots:
  void timing();               // intermediate step to solve blocked thread: step towards processSpeicher
  void updateBelegt();                                // updates of all segment status in loop
  void quit();                                        // if program is shutdown, quit thread
private:
  bool update = true;                                 //update wanted yes = true || no = false
  QList<Block*> blocklist;                            //all segments which must be updated
  QList<Weiche*> weichenlist;                         //alle turnout occupations which must be updated
  QTimer *t = new QTimer(this);
  
};
#endif // WORKER_H
