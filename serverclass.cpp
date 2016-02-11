#include "serverclass.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QTime>
#include "socketclass.h"
//================================================================================================================================
ServerClass::ServerClass(bool terminalkoMode, QObject *parent) :
    QObject(parent)
{
    mTCPServer = new QTcpServer(this);
    connect(mTCPServer, SIGNAL(newConnection()), SLOT(mNewConnection()) );
    this->terminalkoMode = terminalkoMode;
    #ifndef Q_OS_ANDROID
    if(!terminalkoMode){
        mSerial = new MySerialPort(this);
        connect(mSerial, SIGNAL(vyvidDataFromThreads(int,QString,int)), this, SIGNAL(vyvidDataFromThreads(int,QString,int)) );
        connect(mSerial, SIGNAL(mPortStateIsOpened(bool)), this, SIGNAL(mPortStateIsOpened(bool)) );
        connect(this, SIGNAL(closeSerialPort()), mSerial, SLOT(closeSerialPort()) );
        connect(this, SIGNAL(mChangeTimeOutSerial(int)), mSerial, SLOT(changeTimeOut(int)) );
        connect(this, SIGNAL(openSerial(QString,QString,QString,int,int,int,int,int,QStringList)),
                mSerial, SLOT(openSerial(QString,QString,QString,int,int,int,int,int,QStringList)) );
    }
    #endif

}
//================================================================================================================================
void ServerClass::startOnIpPort(QString hAddr, quint16 port, int timeOut)
{
    if(mTCPServer->isListening())
        mTCPServer->close();
    QHostAddress addr(hAddr);
    if(hAddr.isEmpty())
        addr = QHostAddress::Any;
    this->timeOut = timeOut;

    int indx = 1;
    if(this->terminalkoMode)
        indx = 31;

    if(mTCPServer->listen(addr, port)){
        emit vyvidDataFromThreads(indx, tr("<i>%1 Server started! IP:%2, Port:%3</i>").arg(QTime::currentTime().toString("hh.mm.ss"))
                                  .arg(mTCPServer->serverAddress().toString()).arg(mTCPServer->serverPort()), 0 );
    }else{
        emit vyvidDataFromThreads(indx, tr("<i>%1 Can't start server on IP:%2, Port:%3. Error code:%4</i>").arg(QTime::currentTime().toString("hh.mm.ss"))
                                  .arg(mTCPServer->serverAddress().toString()).arg(mTCPServer->serverPort()).arg(mTCPServer->errorString()), 0 );
    }

    emit serverStartedNow(mTCPServer->isListening());
}
//================================================================================================================================
void ServerClass::mNewConnection()
{
    SocketClass *mNewClient = new SocketClass(mTCPServer->nextPendingConnection(),  this, this->timeOut, this->terminalkoMode);

//    connect(this, SIGNAL(mWriteToSockets(QByteArray)), mNewClient, SLOT(mWriteToSockets(QByteArray)) );
//     connect(mNewClient, SIGNAL(mReaddedData(QByteArray)), this, SIGNAL(mReaddedData(QByteArray)) );
    if(this->terminalkoMode){
        connect(this, SIGNAL(mWriteToSockets(QByteArray)), mNewClient, SLOT(mWriteToSockets(QByteArray)) );
    }else{
        #ifndef Q_OS_ANDROID
        connect(mSerial, SIGNAL(mReaddedData(QByteArray)), mNewClient, SLOT(mWriteToSockets(QByteArray)) );
        connect(mNewClient, SIGNAL(mReaddedData(QByteArray)), mSerial, SLOT(writeToSerial(QByteArray)) );
#endif
    }
    connect(this, SIGNAL(serverStartedNow(bool)), mNewClient, SLOT(stopSocket()) );
    connect(mNewClient, SIGNAL(vyvidDataFromThreads(int,QString,int)), this, SIGNAL(vyvidDataFromThreads(int,QString,int)) );
    connect(this, SIGNAL(mChangedTimeOut(int)), mNewClient, SLOT(mChangeTimeOut(int)) );
}

//================================================================================================================================
void ServerClass::mChangeTimeOut(int timeOut)
{
    this->timeOut = timeOut;
    emit mChangedTimeOut(timeOut);
}
//================================================================================================================================

void ServerClass::stopServer()
{
    mTCPServer->close();
    emit serverStartedNow(mTCPServer->isListening());
    if(this->terminalkoMode)
        emit vyvidDataFromThreads(31, tr("<i>%1 Server stopped!</i>").arg(QTime::currentTime().toString("hh.mm.ss")), 0);
    else
        emit vyvidDataFromThreads(1, tr("<i>%1 Server stopped!</i>").arg(QTime::currentTime().toString("hh.mm.ss")), 0);
}
//================================================================================================================================

