//*************************************************************************
//Speicherliste des Stellwerks von Stellwerkstechnik [- STELLWERKSTECHNIK.CXX -]
//*************************************************************************
#include "Stellwerkstechnik.h"
#include <iostream>
//
void Stellwerkstechnik::add_Signal( std::string strid , bool status ){
    //Konvertierung von string zu int gemäß HSignal Benennung:
    int id = stoi( strid.substr(1,3) );
    //Unterscheidung von Signalen, die schon in der Liste sind, und welche, die neu hinzuzufügen sind:
    int pos = -1;//merkt sich die Position
        if(HSorWS(strid).compare("HS") == 0){//je nach dem, ob WS oder HS...
            //std::cout<<"STELLWERKSTECHNIK:: HS detektiert in add_Signal"<<std::endl;
            for( unsigned int i = 0 ; i < hsignale.size() ; i++){//suchen, ob Signal schon da ist?
                if( hsignale.at(i).first == id ){//es gibt schon einen Eintrag
                pos = i;
                hsignale.at(i).second = status;//Führe Statusänderung durch
                }
            }
            if(pos == -1){//es gibt keinen Eintrag, da nie == id zugetroffen hat und pos=-1 noch immer steht
                std::pair<int,bool> newpair(id,status);
                hsignale.push_back(newpair);
            }
        }
        pos = -1;//reset zur Vorsicht
        if(HSorWS(strid).compare("WS") == 0){
            //std::cout<<"STELLWERKSTECHNIK:: WS detektiert in add_Signal"<<std::endl;
            for( unsigned int i = 0 ; i < wsignale.size() ; i++){//suchen, ob Signal schon da ist?
                if( wsignale.at(i).first == id ){//es gibt schon einen Eintrag
                pos = i;
                wsignale.at(i).second = status;//Führe Statusänderung durch
                //std::cout<<"NIX neues"<<std::endl;
                }
            }
            if(pos == -1){//es gibt keinen Eintrag, da nie == id zugetroffen hat und pos=-1 noch immer steht
                std::pair<int,bool> newpair(id,status);
                wsignale.push_back(newpair);
            }
        }
}

void Stellwerkstechnik::delete_HSignal(std::string strtodelete ){
  int todelete = stoi( strtodelete.substr(1,3) );
  int remember = -1;
  if(HSorWS(strtodelete).compare("HS") == 0){
      for( unsigned int i = 0; i < hsignale.size() ; i++){
        if(hsignale.at(i).first == todelete ){//falls das zu löschende Signal gefunden ist
          remember = i;
          //std::cout<<"i_Stellwerkstechnik::deleteHsignal = "<<i<<std::endl;//DEBUG
          break;
        }
      }
  }
  if(HSorWS(strtodelete).compare("WS") == 0){
      for( unsigned int i = 0; i < wsignale.size() ; i++){
        if(wsignale.at(i).first == todelete ){//falls das zu löschende Signal gefunden ist
          remember = i;
          //std::cout<<"i_Stellwerkstechnik::deleteWsignal = "<<i<<std::endl;//DEBUG
          break;
        }
      }
}
  //deleting...
  hsignale.erase( hsignale.begin() + remember );
}

void Stellwerkstechnik::show_Signal(){
  std::cout<<"************************************************************"<<std::endl;
  std::cout<<"*** Dies sind alle HSignale in Stellwerkstechnik         ***"<<std::endl;
  for( unsigned int i = 0 ; i < hsignale.size() ; i++){
    std::cout<<"***   "<<hsignale.at(i).first<<"    =    "<<" "<<hsignale.at(i).second<<"                                       ***"<<std::endl;//alle IDs der HSignale
    }
  std::cout<<"*** Dies sind alle WSignale in Stellwerkstechnik         ***"<<std::endl;
  for( unsigned int j = 0 ; j < wsignale.size() ; j++){
    std::cout<<"***   "<<hsignale.at(j).first<<"    =    "<<" "<<wsignale.at(j).second<<"                                       ***"<<std::endl;//alle IDs der WSignale
    }
  std::cout<<"************************************************************"<<std::endl;
}

bool Stellwerkstechnik::getS_pass_status( std::string statusvonSignal ){
  //gebe den status des Signals zurück
  //zunächst den String in int gemäß HSignal/WSignal Benennung umwandeln
    int converted = stoi( statusvonSignal.substr(1,3) );
    std::cout<<"CONVERTED: "<<converted<<std::endl;
    bool feedback;
      if( HSorWS(statusvonSignal).compare("HS") == 0 ){
        for( unsigned int i = 0 ; i < hsignale.size() ; i++){//Suchen des betreffenden HSignals und Ausgabe des bool
            if( converted == hsignale.at(i).first ){
            feedback = hsignale.at(i).second;
	    std::cout<<"TEC_FEEDBACK_HSPfad"<<feedback<<std::endl;
            break;
            }
        }
      }
      if(HSorWS(statusvonSignal).compare("WS") == 0){
        for( unsigned int i = 0 ; i < wsignale.size() ; i++){//Suchen des betreffenden WSignals und Ausgabe des bool
            if( converted == wsignale.at(i).first ){
            feedback = wsignale.at(i).second;
	    std::cout<<"TEC_FEEDBACK_WSPfad"<<feedback<<std::endl;
	    break;
            }
        }
      }
      std::cout<<"TEC_FEEDBACK:"<<feedback<<std::endl;
  return feedback;
}

std::string Stellwerkstechnik::HSorWS(std::string zutesten){
    std::string decided;
    if(zutesten.at(0) == 'S'){ decided = "HS"; }
    if(zutesten.at(0) == 'W'){ decided = "WS"; }
    return decided;
}
