#ifndef WORKER_H
#define WORKER_H
#include <Block.h>
#include <Weiche.h>
#include <QList>
#include <QPair>

class worker : public QObject{// Vorbild nach https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
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
  void updateBelegt(); // updates von allen Belegtstatus über eine schleife
  void quit();         // wenn Programm beendet, dann terminate worker
signals:
  void finished();
private:
  bool update = true; //update gewünscht yes = true || no = false
  QList< QPair<int,Block*>> blocklist;                //alle Blöcke, die nun upgedated werden
  QList< QPair<int,Weiche*>> weichenlist;             //alle Weichen, die nun upgedated werden (Belegung)
  
};
#endif // WORKER_H
