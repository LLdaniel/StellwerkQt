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
`~/raspi/qt5/binqmake`  
`make`  
`make install`  

Then deploy the new files to the RaspberryPi3B:  
`rsync -avz qt5pi pi@raspberrypi.local:/usr/local`  
