#include "myremoteserverclass.h"
//=========================================================================================================================================
MyRemoteServerClass::MyRemoteServerClass(bool terminalkoMode, QObject *parent) :
    QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(mReadyRead()) );
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(tellAboutDisConnection()) );

    this->terminalkoMode = terminalkoMode;
    #ifndef Q_OS_ANDROID
    if(!terminalkoMode){
        mSerial = new MySerialPort(this, 20);
        connect(mSerial, SIGNAL(vyvidDataFromThreads(int,QString,int)), this, SIGNAL(vyvidDataFromThreads(int,QString,int)) );
        connect(mSerial, SIGNAL(mPortStateIsOpened(bool)), this, SIGNAL(mPortStateIsOpened(bool)) );

        connect(this, SIGNAL(closeSerialPort()), mSerial, SLOT(closeSerialPort()) );
        connect(this, SIGNAL(mChangeTimeOutSerial(int)), mSerial, SLOT(changeTimeOut(int)) );
        connect(this, SIGNAL(openSerial(QString,QString,QString,int,int,int,int,int,QStringList)),
                mSerial, SLOT(openSerial(QString,QString,QString,int,int,int,int,int,QStringList)) );

        connect(mSerial, SIGNAL(mReaddedData(QByteArray)), this, SLOT(mWriteToSockets(QByteArray)) );
        connect(this, SIGNAL(mReaddedData(QByteArray)), mSerial, SLOT(writeToSerial(QByteArray)) );
    }
    #endif
}
//=========================================================================================================================================
void MyRemoteServerClass::mChangeTimeOut(int timeOut)
{
    this->timeOut = timeOut;
}
//=========================================================================================================================================
void MyRemoteServerClass::mConnectToHost(QString hAddr, quint16 port, int timeOut)
{
    if(tcpSocket->isOpen())
        tcpSocket->close();

    this->timeOut = timeOut;

//    QHostAddress hAddres(hAddr);
     tcpSocket->connectToHost(hAddr, port);

    this->loclalAddrs = hAddr;

     loclalAddrsLong = loclalAddrs;

      while(loclalAddrsLong.length() < 15){
          loclalAddrsLong.prepend(" ");
          if(loclalAddrsLong.length() < 15)
              loclalAddrsLong.append(" ");
          else
              break;
      }
      int indx = 0;
      if(this->terminalkoMode)
          indx = 20;

      if(timeOut < 1000)
          timeOut = 1000;
      else{
          if(timeOut > 10000)
              timeOut = 10000;
      }
      if(tcpSocket->waitForConnected(timeOut)){
          emit vyvidDataFromThreads(21 + indx, tr("<i>%1 Connected to Remote Server! IP:%2, Port:%3</i>").arg(QTime::currentTime().toString("hh.mm.ss"))
                                    .arg(loclalAddrs).arg(port), 0 );
      }else{
          emit vyvidDataFromThreads(21 + indx, tr("<i>%1 Can't Connect To Remote Server on IP:%2, Port:%3. Error code:%4</i>").arg(QTime::currentTime().toString("hh.mm.ss"))
                                    .arg(loclalAddrs).arg(port).arg(tcpSocket->errorString()), 0 );
          tcpSocket->close();
      }

      emit mSocketStateIsConnected(tcpSocket->isOpen());

}
//=========================================================================================================================================
void MyRemoteServerClass::mWriteToSockets(QByteArray dataArr)
{
    if(tcpSocket->isOpen()){
        tcpSocket->write(dataArr);
        tcpSocket->waitForBytesWritten(this->timeOut);
    }
}
//=========================================================================================================================================
void MyRemoteServerClass::stopSocket()
{
    tcpSocket->close();
}
//=========================================================================================================================================
void MyRemoteServerClass::mReadyRead()
{
    QByteArray readArr = tcpSocket->readAll();
    while(tcpSocket->waitForReadyRead(this->timeOut))
        readArr.append(tcpSocket->readAll());

    if(this->terminalkoMode){
        QString strNorm = readArr;
        for(int ff = 0, ffMax = readArr.size(); ff < ffMax; ff += 16){
            QString hexStr = readArr.mid(ff,16).toHex().leftJustified(32, ' ').toUpper();

            for(int i = 30; i > 1; i -= 2)
                hexStr = hexStr.insert(i, " ");

            QString hexStrLog = hexStr;
            emit vyvidDataFromThreads(42 , tr("%3 > %4 > %1     %2").arg(hexStrLog).arg(strNorm.mid(ff,16).simplified())
                                      .arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(loclalAddrsLong), 0);
        }
    }else{


        QString strNorm = readArr;
        for(int ff = 0, ffMax = readArr.size(); ff < ffMax; ff += 16){
            QString hexStr = readArr.mid(ff,16).toHex().leftJustified(32, ' ').toUpper();

            for(int i = 30; i > 1; i -= 2)
                hexStr = hexStr.insert(i, " ");

            QString hexStrLog = hexStr;
            //if(ff == 0)
            //  hexStr.prepend(" read:"+QTime::currentTime().toString("hh:mm:ss.zzz")+" ");
            //else
            //  hexStr.prepend("                   ");
            //qDebug()<<hexStr<<"   "<<strNorm.mid(ff,16).simplified();
            emit vyvidDataFromThreads(22 , tr("%3 > %4 > %1     %2").arg(hexStrLog).arg(strNorm.mid(ff,16).simplified())
                                      .arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(loclalAddrsLong), 0);
        }
        emit mReaddedData(readArr);
    }
}
//=========================================================================================================================================
void MyRemoteServerClass::tellAboutDisConnection()
{
    tcpSocket->close();
    int indx = 0;
    if(this->terminalkoMode)
        indx = 20;

    emit vyvidDataFromThreads(21 + indx, tr("<i>%1 Disconnected From Remote Server IP:%2</i>").arg(QTime::currentTime().toString("hh.mm.ss")).arg(loclalAddrs), 0);
    emit mSocketStateIsConnected(tcpSocket->isOpen());
//    tcpSocket->deleteLater();
}
//=========================================================================================================================================
