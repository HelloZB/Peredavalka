#ifndef SOCKETCLASS_H
#define SOCKETCLASS_H

#include <QObject>
#include <QTcpSocket>
#include <QtNetwork>
#include <QAbstractSocket>
#include <QObject>
#include <QTimer>

class SocketClass : public QObject
{
    Q_OBJECT

public:
    explicit SocketClass(QTcpSocket *tcpSocket = 0 , QObject *parent = 0, int timeOut = 100, bool terminalkoMode = false);
    ~SocketClass();

signals:
    void mReaddedData(QByteArray);

    void vyvidDataFromThreads(int , QString , int );

public slots:
    void mChangeTimeOut(int timeOut);

private slots:
    void mWriteToSockets(QByteArray dataArr);
    void stopSocket();
    void mReadyRead();
    void tellAboutConnection();
      void tellAboutDisConnection();

private:
    int timeOut;
    QTcpSocket *tcpSocket;
    QString loclalAddrs;
    QString loclalAddrsLong;

    bool terminalkoMode;
};

#endif // SOCKETCLASS_H
