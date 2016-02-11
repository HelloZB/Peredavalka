#-------------------------------------------------
#
# Project created by QtCreator 2013-08-30T17:00:15
#
#-------------------------------------------------

!android{
    greaterThan(QT_MAJOR_VERSION, 4){
        QT += serialport
    }else{
        CONFIG += serialport
    }
}

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Peredavalko
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serverclass.cpp \
    socketclass.cpp \
    checkcurrport.cpp \
    myremoteserverclass.cpp

!android{
 SOURCES += myserialport.cpp
}


HEADERS  += mainwindow.h \
    serverclass.h \
    socketclass.h \
    checkcurrport.h \
    myremoteserverclass.h

!android{
 HEADERS += myserialport.H
}

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
win{
RC_FILE += file.rc
OTHER_FILES += \
    file.rc
}
