/*
 * filemanager of railroad [- FILEMANGER.H -]
 *******************************************************************************
 * Turnout states are saved throught different sessions and shutdowns in a     *
 * simple text file. The order is 001, 002, ... with the following content:    *
 *    state 1                                                                  *
 *    state 2                                                                  *
 *    ...                                                                      *
 *******************************************************************************
 */
#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QString>
#include "Weiche.h"
class filemanager{
public:
  filemanager( QString dir);
  //
  void read();                                           //read all previous turnout states from txt file
  void write();                                          //write all current turnout states to txt file
  bool passstate(int position);                          //extracts the state from previous session  at position
  void add( bool newstate );                             //adds the turnouts for the write process
  ~filemanager(); 
 private:
  QFile file;                                            //txt file with all status information turnouts
  QList<bool> status;                                    //list of the tunrout states (start program)
  QList<bool> weichen;                                   //list of all turnout states (end program)
  
};
#endif
