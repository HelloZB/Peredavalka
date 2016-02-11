#include "checkcurrport.h"

#if QT_VERSION >= 0x050000
#ifndef Q_OS_ANDROID
#include <QSerialPortInfo>
#endif
#else
#include <QtAddOnSerialPort/serialportinfo.h>
#endif

//#include <QTime>
//#include <QDebug>
//=========================================================================================================================
CheckCurrPort::CheckCurrPort(QObject *parent) :
    QThread(parent)
{
//    connect(&mTimer, SIGNAL(timeout()), this, SLOT(onTimerTimeOut()) );
//    connect(this, SIGNAL(finished()), &mTimer, SLOT(stop()) );
//    mTimer.setSingleShot(true);
}
//=========================================================================================================================
void CheckCurrPort::zapuskalka(QString currPort)
{
    #ifndef Q_OS_ANDROID

    this->currPort=currPort;   
    this->start();
    #endif
}
//=========================================================================================================================
void CheckCurrPort::run()
{
//    counter = 1;
//    mTimer.start(700);
//    this->exec();
//    mTimer.stop();
    #ifndef Q_OS_ANDROID
    quint8 counter=1;
    while(true){
        msleep(1500);
        bool found=0;
#if QT_VERSION >= 0x050000
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
#else
        foreach (const SerialPortInfo &info, SerialPortInfo::availablePorts()){
#endif
            if(info.portName()==currPort){

                found=1;

                if(!info.isBusy()){
                    emit portDisconnected();
                    counter++;
                    if(counter>3){
                        counter=2;
                        emit terminateNow();
                    }

                }
                break;
            }
       }
        if(!found){

             emit portDisconnected();
            counter++;
            if(counter>3){
                counter=2;
                emit terminateNow();
            }
        }
    }
#endif
}
//=========================================================================================================================
/*void CheckCurrPort::onTimerTimeOut()
{
    if(!this->isRunning())
        return;

    bool found = false;
#if QT_VERSION >= 0x050000
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
#else
    foreach (const SerialPortInfo &info, SerialPortInfo::availablePorts()){
#endif
        if(info.portName() == currPort){

            found = true;

            if(!info.isBusy()){
                emit portDisconnected();
                counter++;
                if(counter > 3){
                    counter = 2;
                    emit terminateNow();
                }
                mTimer.start(200);
                return;
            }
            break;
        }
   }
    if(!found){

         emit portDisconnected();
        counter++;
        if(counter > 3){
            counter = 2;
            emit terminateNow();
        }
        mTimer.start(200);
        return;
    }
    mTimer.start(700);
}*/
//=========================================================================================================================
