#ifndef WORKER_H
#define WORKERR_H
#include <Block.h>

class worker : public QObject{// Vorbild nach https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
  Q_OBJECT
 public:
  worker( std::vector<Block*> allBlocks, std::vector<int> allpins );
  ~worker();
  void addBlockList( Block* moreBlocks );
  void showBlocks();
  void addPinList( int morePins );
  void showPins();
  public slots:
    void updateBelegt(); // updates von allen Belegtstatus über eine schleife
    void quit();         // wenn Programm beendet, dann terminate worker
 private:
    bool update = true; //update gewünscht yes = true || no = false
    std::vector<Block*> blocklist; //alle Blöcke, die nun upgedated werden
    std::vector<int> pins; //alle input pins
    
};
#endif // WORKER_H
