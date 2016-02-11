#include "socketclass.h"
#include <QDebug>
//================================================================================================================================
SocketClass::SocketClass(QTcpSocket *tcpSocket, QObject *parent, int timeOut,  bool terminalkoMode) :
    QObject(parent)
{
    this->terminalkoMode = terminalkoMode;
    this->tcpSocket = tcpSocket;
    this->timeOut = timeOut;
    connect(this->tcpSocket, SIGNAL(readyRead()), this, SLOT(mReadyRead()) );
    connect(this->tcpSocket, SIGNAL(disconnected()), this, SLOT(tellAboutDisConnection()) ); //SLOT(deleteLater()) );
    loclalAddrs = this->tcpSocket->peerAddress().toString();
   loclalAddrsLong = loclalAddrs;

    while(loclalAddrsLong.length() < 15){
        loclalAddrsLong.prepend(" ");
        if(loclalAddrsLong.length() < 15)
            loclalAddrsLong.append(" ");
        else
            break;
    }

//    if(!terminalkoMode)
    QTimer::singleShot(1, this, SLOT(tellAboutConnection()) );
}
//================================================================================================================================
SocketClass::~SocketClass()
{
    this->deleteLater();
}

//================================================================================================================================
void SocketClass::tellAboutConnection()
{
    if(this->terminalkoMode)
        emit vyvidDataFromThreads(31, tr("<i>%1 New Connection IP:%2</i>").arg(QTime::currentTime().toString("hh.mm.ss")).arg(loclalAddrs), 0);
    else
        emit vyvidDataFromThreads(1, tr("<i>%1 New Connection IP:%2</i>").arg(QTime::currentTime().toString("hh.mm.ss")).arg(loclalAddrs), 0);

}
//================================================================================================================================
void SocketClass::tellAboutDisConnection()
{
    if(this->terminalkoMode)
        emit vyvidDataFromThreads(31, tr("<i>%1 Disconnected IP:%2</i>").arg(QTime::currentTime().toString("hh.mm.ss")).arg(loclalAddrs), 0);
    else
        emit vyvidDataFromThreads(1, tr("<i>%1 Disconnected IP:%2</i>").arg(QTime::currentTime().toString("hh.mm.ss")).arg(loclalAddrs), 0);
    this->deleteLater();
}
//================================================================================================================================
void SocketClass::mChangeTimeOut(int timeOut)
{
    this->timeOut = timeOut;
}
//================================================================================================================================

void SocketClass::mWriteToSockets(QByteArray dataArr)
{
    tcpSocket->write(dataArr);
    tcpSocket->waitForBytesWritten(this->timeOut);

    for(int ff = 0, ffMax = dataArr.size(); ff < ffMax; ff += 16){
        QString hexStr = dataArr.mid(ff,16).toHex().leftJustified(32, ' ').toUpper();

        for(int i = 30; i > 1; i -= 2)
            hexStr = hexStr.insert(i, " ");

        if(ff == 0)
          hexStr.prepend("write:"+QTime::currentTime().toString("hh:mm:ss.zzz")+" ");
        else
          hexStr.prepend("                   ");
        qDebug()<<hexStr<<"   "<< dataArr.mid(ff,16).simplified();


    }

}
//================================================================================================================================
void SocketClass::stopSocket()
{
    tcpSocket->disconnectFromHost();
}
//================================================================================================================================
void SocketClass::mReadyRead()
{
    QByteArray readArr = tcpSocket->readAll();

    while(tcpSocket->waitForReadyRead(this->timeOut)){
        readArr.append(tcpSocket->readAll());
    }

//    QFile file("log.txt");
//    file.open(QFile::ReadWrite);
//    file.write(file.readAll() + readArr);
//    file.close();

    if(this->terminalkoMode){
        QString strNorm = readArr;
        QString currentTime =  QTime::currentTime().toString("hh:mm:ss.zzz");
        for(int ff = 0, ffMax = readArr.size(); ff < ffMax; ff += 16){
            QString hexStr = readArr.mid(ff,16).toHex().leftJustified(32, ' ').toUpper();

            for(int i = 30; i > 1; i -= 2)
                hexStr = hexStr.insert(i, " ");

            QString hexStrLog = hexStr;

            if(ff == 0)
              hexStr.prepend(" read:"+QTime::currentTime().toString("hh:mm:ss.zzz")+" ");
            else
              hexStr.prepend("                   ");
            qDebug()<<hexStr<<"   "<< readArr.mid(ff,16).simplified();


            emit vyvidDataFromThreads(32 , tr("%3 > %4 > %1     %2").arg(hexStrLog).arg(strNorm.mid(ff,16).simplified())
                                      .arg(currentTime).arg(loclalAddrsLong), 0);
        }

    }else{
        QString strNorm = readArr;
        QString currentTime =  QTime::currentTime().toString("hh:mm:ss.zzz");
        for(int ff = 0, ffMax = readArr.size(); ff < ffMax; ff += 16){
            QString hexStr = readArr.mid(ff,16).toHex().leftJustified(32, ' ').toUpper();

            for(int i = 30; i > 1; i -= 2)
                hexStr = hexStr.insert(i, " ");

            QString hexStrLog = hexStr;
            if(ff == 0)
              hexStr.prepend(" read:"+QTime::currentTime().toString("hh:mm:ss.zzz")+" ");
            else
              hexStr.prepend("                   ");
            qDebug()<<hexStr<<"   "<< readArr.mid(ff,16).simplified();

            emit vyvidDataFromThreads(2 , tr("%3 > %4 > %1     %2").arg(hexStrLog).arg(strNorm.mid(ff,16).simplified())
                                      .arg(currentTime).arg(loclalAddrsLong), 0);
        }

        emit mReaddedData(readArr);
    }
}
//================================================================================================================================
