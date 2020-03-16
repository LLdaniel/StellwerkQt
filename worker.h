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
#include <QPair>

class worker : public QObject{                        //after https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
  Q_OBJECT
public:
  worker( QList<QPair<int,Block*>> allBlocks, QList<QPair<int,Weiche*>> allWeichen );
  worker(){}
  ~worker();
  void addBlockList( int morePins, Block* moreBlocks );
  void showBlocks();
  void addWeichenList( int morePins, Weiche* moreWeichen );
  void showWeichen();
public slots:
  void updateBelegt();                                // updates of all segment status in loop
  void quit();                                        // if program is shutdown, quit thread
signals:
  void finished();
private:
  bool update = true;                                 //update wanted yes = true || no = false
  QList< QPair<int,Block*>> blocklist;                //all segments which must be updated
  QList< QPair<int,Weiche*>> weichenlist;             //alle turnout occupations which must be updated
  
};
#endif // WORKER_H
