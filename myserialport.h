#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QTime>
#include <QStringList>
#ifndef Q_OS_ANDROID
#include "checkcurrport.h"

#if QT_VERSION >= 0x050000
#include <QSerialPort>
#else
#include <QTextCodec> //qt4
#include <QtAddOnSerialPort/serialportinfo.h>
#include <QtAddOnSerialPort/serialport-global.h>
#include <QtAddOnSerialPort/serialport.h>
#endif

#if QT_VERSION < 0x050000
QT_BEGIN_NAMESPACE_SERIALPORT
class SerialPort;
QT_END_NAMESPACE_SERIALPORT

QT_USE_NAMESPACE_SERIALPORT
#endif

#if QT_VERSION >= 0x050000
class MySerialPort : public QSerialPort
#else
class MySerialPort : public SerialPort
#endif
{
    Q_OBJECT
public:
    explicit MySerialPort(QObject *parent = 0, int forVyvid = 0);
    
signals:
    void mPortStateIsOpened(bool);
    void mReaddedData(QByteArray);

    void vyvidDataFromThreads(int , QString , int );
    void mChangedTimeOut(int);

    void terminateCheckPort();

public slots:

private slots:
    void writeToSerial(QByteArray dataArr);
    void mReadyRead();
    void openSerial(QString portName, QString baud, QString aboutPort, int timeOut, int dataB, int stopB, int flowC, int parity, QStringList listSett);
    void closeSerialPort();
    void changeTimeOut(int timeOut);

private:
    int timeOut;
    QString currPortName;
    CheckCurrPort mCheckPort;
    int forVyvid;
//    int currPortNameLen;
};
#endif

#endif // MYSERIALPORT_H
