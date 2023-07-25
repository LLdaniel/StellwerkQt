## Signalling Control Center
Electronic signalbox for a model railway implemented on a cross compiled raspberry pi 3B (C++, Qt, wiringPi) :bullettrain_side:

### Description
This is a electronic signal box implemented in C++ and Qt for a cross compiled raspberry pi 3B using wiringPi library.
The central aspect is to set up the routes from main signal S001 to main signal S002, where all tournouts are set properly and the presignal shows the state of S002.
The whole information system for the electronic control center is based on the segments of the railway: The occupation of segments is monitored with the GPIO interface of the Raspberry Pi.

![image](https://github.com/LLdaniel/StellwerkQt/assets/41345639/64aeac6d-5154-4f69-8fdf-a73d232a75f7)

### Program description
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
- **Spmemory** is the memory of routes not yet possible
- **Connector** connects two turnouts to one double-turnout (keeping the character of two single turnouts)
- **worker** manages the updates of the segments' status
- **filemanager** reads the last session's turnout state and writes the state for the next session

### Build Process
For the build process see the wikipages: https://github.com/LLdaniel/StellwerkQt/wiki.
There you also can find some notes on threads.