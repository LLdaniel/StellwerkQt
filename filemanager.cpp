/*
 * filemanager of railroad [- FILEMANGER.CPP -]
 **************************************************************************
 */
#include "filemanager.h"
#include <QDebug>
#include <QVariant>
filemanager::filemanager( QString dir){
  file.rename(dir);
}

filemanager::~filemanager(){

}


void filemanager::read(){
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream stream(&file);
    while (!stream.atEnd()){
      status.append( QVariant(stream.readLine()).toBool() );
    }
  }
  file.close();
  qDebug() << "file reading finished";
}

void filemanager::write(){
  if(file.open( QIODevice::WriteOnly | QIODevice::Text) ){
    // We're going to streaming text to the file
    QTextStream stream(&file);
    for( int writer = 0; writer < weichen.size(); writer++){
      stream << weichen.at(writer) <<'\n';
    }
    file.close();
    qDebug() << "file writing finished";
  }
}

bool filemanager::passstate(int position){
  return status.at(position);
}

void filemanager::add( bool newstate ){
  weichen.append(newstate);
}
