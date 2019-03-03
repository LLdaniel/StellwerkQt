#include <ctime>
int msleep(unsigned long milisec)
{
    struct timespec req={0};
    time_t sec=(int)(milisec/1000);
    milisec=milisec-(sec*1000);
    req.tv_sec=sec;
    req.tv_nsec=milisec*1000000L;
    while(nanosleep(&req,&req)==-1)
        continue;
    return 1;
}

void actrandomly(){
      bool array[1000] = {0};
      msleep(7000);//sleep for 7sec
      for(int i = 0; i < 1000 ; i++){
        int ran = rand()%1000;//generates a random number from 0-10000
        //std::cout<<"                                                   step"<<i<<" ran="<<ran<<std::endl;//Debug Line
        if(array[ran] == false){
          array[ran] = true;
        }
        else{ array[ran] = false; }
      }
      for(int k = 0 ; k<1000 ; k++){
        std::cout<<array[k]<<std::endl;
      }
    }

    void user(){
      int input;
      int array[10] = { 0 };
      for(int i = 0; i < 10 ; i++){
        std::cin>>input;
        array[i] = input;
      }
      std::cout<<"***************"<<std::endl;
      for(int j = 0 ; j<10 ; j++){
        std::cout<<array[j]<<std::endl;
      }
      std::cout<<"***************"<<std::endl;

    }
