## signalling control center
electronic signal box for a model railway implemented on a cross compiled raspberry pi 3B (C++, Qt, wiringPi) :bullettrain_side:

### description
This is a electronic signal box implemented in C++ and Qt for a cross compiled raspberry pi 3B using wiringPi library.
The central aspect is to set up the routes from main signal A to main signal B, where all tournouts are set properly and the presignal signals the signal of B.
The whole information system for the electronic control center is based on which segments of rail is engaged due to trains.

### program description
The program has different sub-classes giving the chance to simulate a signalling control center.  
- **Block** divides the railway into smaller segments and signals the occupation of such segments and is connected to railway crossings
- **BU** implements the railway crossings
- **Weiche** implements the turnouts
- **HSignal** implements the main signal object and does mainly the work for setting up the whole route including the segments and tournouts, shunting signals and presignals 
- **VSignal** implements the presignal class
- **WSignal** implements the class of shunting signals
- **Stellwerkstechnik** implements a map of the states of the different main signals
- **clickmanager** is used for the GUI control plan to manage user inputs for settung up routes
- **wiringPi** includes the library of drogon http://wiringpi.com/ to manage I/O of the RaspberryPi
- **mainwindow** and **.pro** files are related to Qt which are used for GUI implementations
### cross compiling
Crosscompiling was tested with Qt 5.8.0, Raspbian Jessie on RaspberryPi 3B and Debian Jessie on the host computer.
For cross compiling with **qmake** do:  
`~/rpi/qt5/bin/qmake -r`  
`make`  
`make install`  

Then deploy the new files to the RaspberryPi3B:  
`rsync -avz qt5pi pi@raspberrypi.local:/usr/local`  

### repository on host PC for RaspberryPi deplyoment
Find instructions at:
https://wiki.qt.io/RaspberryPi2EGLFS

### notes on threads
For a arbitrary class myClass with methods ::hello and ::act one can execute the methods in separate threads shown below:
`#include <iostream>
`#include <thread>`
`#include <vector>`
`#include "myclass.h"`
`#include <queue>`
`int main( int argc , char *argv[] ){`
`  myclass c;//initialize object`
`  std::thread t( &myclass::hello , &c);//create thread which executes the hello function`
`  std::thread t2( &myclass::act , &c);//create thread which executes the act function`
`  t.join();//add it, that it can be executed at the same time`
`  t2.join();`
`  //now the queue test:`
`  std::queue<std::string> q;`
`  q.push("command1");`
`  std::cout<<q.front()<<std::endl;`
`  return 0;`
`}`

### insider notes
1. std::strings in VSignal+WSignal: circle of \#includes HS needs VS and VS needs HS =(  -->should work with this workaround 
2. No locking for BÜs: would require more sofisticated implementation for deleteFS, at the same time BU::setFreigabe() has to be set manually
3. No locking for VS, because of VSignal:setV_status(): one can change this either when using this method manually
4. check signal-slot connections and create them properly --> maybe there is a chance to do this automatically
5. the emit function is called at the end of HS::setFahrt(WSignal*): that is for the connection to Stellwerkstechnik, but (=stellwerkstec) signals the main signal as permitted route instead of signalling as a shunting signal, maybe this is not important, but check it again
6. deleteFS [only HS part] deletes the route memory, memory items, but after a short time getZiel is called again although there is no existing memory anymore --> it is hopefully save
7. release Speicher and process Speicher: normally the programm follws the emit command and setting the route memory to the current route required in evaluateFreigabe in setB_status directly a boolean return value of (before; emit release Speicher in evaluateFreigabe counter == 2 --> status=new status part was not executed and the programm followed processSpeicher and setting the memory route as current route resulted in a not unlocked segment
