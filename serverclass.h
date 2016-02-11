#ifndef SERVERCLASS_H
#define SERVERCLASS_H

#include <QTcpServer>

#include <QTcpSocket>
#include <QtNetwork>
#include <QAbstractSocket>
#include <QObject>
#ifndef Q_OS_ANDROID
#include "myserialport.h"
#endif


class ServerClass : public QObject
{
    Q_OBJECT
public:
    explicit ServerClass(bool terminalkoMode = false, QObject *parent = 0);
    
signals:
    void serverStartedNow(bool);
    void mReaddedData(QByteArray);
    void mWriteToSockets(QByteArray);
    void vyvidDataFromThreads(int , QString , int );
    void mChangedTimeOut(int);


    //to myserialport
    void openSerial(QString, QString, QString, int timeOut, int, int, int, int, QStringList listSett);
    void closeSerialPort();
    void mPortStateIsOpened(bool);
    void mChangeTimeOutSerial(int timeOut);

private slots:
    void startOnIpPort(QString hAddr, quint16 port, int timeOut);
    void mNewConnection();
    void mChangeTimeOut(int timeOut);
    void stopServer();

public slots:
    
private:
    QTcpServer *mTCPServer;
#ifndef Q_OS_ANDROID
    MySerialPort *mSerial;
#endif
    int timeOut;
    bool terminalkoMode;
    int connectCounter;
};

#endif // SERVERCLASS_H
