#-------------------------------------------------
#
# Project created by QtCreator 2017-10-25T23:58:34
#
#-------------------------------------------------

QT       += core gui
QT       +=svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StellwerkQt
TEMPLATE = app
CONFIG += console #output goes to linux terminal

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    Vsignal.cpp \
    Weiche.cpp \
    WSignal.cpp \
    HSignal.cpp \
    BU.cpp \
    Block.cpp \
    Stellwerkstechnik.cpp \
    clickmanager.cpp \
    util.cpp \
    myqueue.cpp \
    worker.cpp

HEADERS  += mainwindow.h \
    VSignal.h \
    WSignal.h \
    Weiche.h \
    Stellwerkstechnik.h \
    HSignal.h \
    BU.h \
    Block.h \
    clickmanager.h \
    myqueue.h \
    worker.h

    FORMS    += mainwindow.ui
    RESOURCES+=resources.qrc
# wiringPi
LIBS+=-L/home/daniel/Documents/Stellwerk/StellwerkQt/wiringPi/ -lwiringPi
# pigpio
#LIBS+=-L/home/daniel/Documents/Stellwerk/StellwerkQt/PIGPIO/ -lpigpio

# wiringPi
INCLUDEPATH+=/home/daniel/Documents/Stellwerk/StellwerkQt/wiringPi/
DEPENDPATH+=/home/daniel/Documents/Stellwerk/StellwerkQt/wiringPi/
# pigpio
#INCLUDEPATH+=/home/daniel/Documents/Stellwerk/StellwerkQt/PIGPIO/
#DEPENDPATH+=/home/daniel/Documents/Stellwerk/StellwerkQt/PIGPIO/
