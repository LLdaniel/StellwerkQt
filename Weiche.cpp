//*************************************************************************
//Weiche des Stellwerks  [- WEICHE.CXX -]
//*************************************************************************
#include "Weiche.h"
#include <iostream>
#include <QFont>
//
Weiche::Weiche (int name){
  setW_id( name );
}

void Weiche::setBelegung( bool newbelegung ){
  if( counter == 2){ counter = 0; }//reset counter falls =2
  counter++;//ansonsten vermerk, dass ein Belegungszyklus gemacht wird-->Statusänderung
  evaluateVerriegelung();//evtl entriegeln
  belegung = newbelegung;//jetzt kann der Status gesetzt werden
  changeColor();
}

void Weiche::setW_id( int name ){
  std::string suffix = std::to_string( name );//name zu String konvertieren
  if( name > 0 && name <10 ){//einstellig
    w_id = "00" + suffix;
  }
  if( name > 10 && name <100 ){//zweistellig
    w_id = "0" + suffix;
  }
  if( name > 100 && name <1000 ){//dreistellig
    w_id = suffix;
  }
}

void Weiche::setW_status( bool status ){
  if( verriegelung || !belegung ){//wenn eine Verriegelung/Belegung besteht; keine Stellerlaubnis
    std::cout<<"Weiche::Diese Weiche ist verriegelt!"<<std::endl;
  }
  else{//ok, dann ist stellen erlaubt
    w_status = status;
    changeColor();
  }
}

void Weiche::addWeichenitem(QGraphicsRectItem *ab, QGraphicsRectItem *ge, QLabel *la)
{
    abknickend = ab;
    gerade = ge;
    beschriftung = la;
    QString qname = QString::fromStdString( w_id );//cast from std::string to QString
    beschriftung->setText(qname);
    QFont f( "Arial", 5, QFont::Bold);
    beschriftung->setFont(f);
    changeColor();
}

void Weiche::evaluateVerriegelung(){
  if( counter == 1 || counter == 0){//zum debuggen
    std::cout<<"Weiche::counter = 1|0"<<std::endl;
  }
  if( counter == 2 && !belegung){//hier wird dann die Weiche freigegeben-->ist der richtige Zyklus hier
    std::cout<<"Weiche::Der counter = 2"<<std::endl;
    verriegelung = false;//konkrete Entriegelung nach Zyklus true-false-true
    changeColor();
  }
}

void Weiche::changeColor(){
    if( w_status ){//immer die Stellung mit gelb anzeigen --> hier geradeaus
        gerade->setBrush(QColor(238,201,0));
        if(!belegung){
            abknickend->setBrush(Qt::red);
        }
        if(belegung && verriegelung){
            abknickend->setBrush(Qt::white);
        }
        if(belegung && !verriegelung){
            abknickend->setBrush(QColor(79,79,79));
        }
    }
    if( !w_status ){//immer die Stellung mit gelb anzeigen --> hier abknickend
        abknickend->setBrush(QColor(238,201,0));
        if(!belegung){
            gerade->setBrush(Qt::red);
        }
        if(belegung && verriegelung){
            gerade->setBrush(Qt::white);
        }
        if(belegung && !verriegelung){
            gerade->setBrush(QColor(79,79,79));
        }
    }
}



