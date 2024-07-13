/*
 * filemanager of railroad [- FILEMANGER.CPP -]
 **************************************************************************
 */
#include "filemanager.h"
#include <QDebug>
#include <QVariant>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
filemanager::filemanager( QString dir){
  file.setFileName(dir);
}

filemanager::~filemanager(){

}


void filemanager::read(){
  QFileInfo check(file.filesystemFileName());
  if( !check.exists() or !check.isFile() ){
    // file does not exist or is a dir
    qInfo()<<"__filemanager__: File turnouts.txt does not exist. It will be created when closing the program.";
  }
  else{
    // file exists, read it
    isExistent = true;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
      QTextStream stream(&file);
      while (!stream.atEnd()){
	status.append( QVariant(stream.readLine()).toBool() );
      }
    }
    file.close();
    qDebug() << "__filemanager__: Reading file done.";
  }
}

void filemanager::write(){
  if( !isExistent ){
    QString home = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QDir newdir;
    newdir.mkdir(home + "/.config/StellwerkQt");
    QFile file(home + "/.config/StellwerkQt/turnouts.txt");
    file.open(QIODevice::WriteOnly);
    file.close();
    qInfo()<<"__filemanager__: Automatically created turnouts.txt in $HOME/.config/StellwerkQt.";
  }
  if(file.open( QIODevice::WriteOnly | QIODevice::Text) ){
    // We're going to streaming text to the file
    QTextStream stream(&file);
    for( int writer = 0; writer < weichen.size(); writer++){
      stream << weichen.at(writer) <<'\n';
    }
    file.close();
    qDebug() << "__filenamager__: Writing file done.";
  }
}

bool filemanager::passstate(int position){
  if( isExistent ){
    return status.at(position-1);
  }
  else{
    return false;
  }
}

void filemanager::add( bool newstate ){
  weichen.append(newstate);
}
