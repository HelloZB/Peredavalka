#ifndef MYREMOTESERVERCLASS_H
#define MYREMOTESERVERCLASS_H

#include <QObject>
#include <QTcpSocket>
#include <QtNetwork>
#include <QAbstractSocket>
#include <QObject>
#include <QTimer>
#ifndef Q_OS_ANDROID
#include "myserialport.h"
#endif
class MyRemoteServerClass : public QObject
{
    Q_OBJECT
public:
    explicit MyRemoteServerClass(bool terminalkoMode = false, QObject *parent = 0);
    
signals:
    void mReaddedData(QByteArray);
    void vyvidDataFromThreads(int , QString , int );
    void mSocketStateIsConnected(bool);

    //to myserialport
    void openSerial(QString, QString, QString, int timeOut, int, int, int, int, QStringList listSett);
    void closeSerialPort();
    void mPortStateIsOpened(bool);
    void mChangeTimeOutSerial(int timeOut);

public slots:
    void mChangeTimeOut(int timeOut);

private slots:
    void mConnectToHost(QString hAddr, quint16 port, int timeOut);
    void mWriteToSockets(QByteArray dataArr);
    void stopSocket();
    void mReadyRead();
//    void tellAboutConnection();
      void tellAboutDisConnection();
private:
    int timeOut;
    QTcpSocket *tcpSocket;
    QString loclalAddrs;
    QString loclalAddrsLong;
#ifndef Q_OS_ANDROID
    MySerialPort *mSerial;
#endif
    bool terminalkoMode;
};

#endif // MYREMOTESERVERCLASS_H
