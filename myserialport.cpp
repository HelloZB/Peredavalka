#include "myserialport.h"
#ifndef Q_OS_ANDROID
//==================================================================================================================================

MySerialPort::MySerialPort(QObject *parent, int forVyvid) :
    #if QT_VERSION >= 0x050000
    QSerialPort(parent),
    #else
    SerialPort(parent),
    #endif
    forVyvid(forVyvid)
{
    connect(this, SIGNAL(readyRead()), SLOT(mReadyRead()) );
    connect(this, SIGNAL(terminateCheckPort()), &mCheckPort, SLOT(terminate()) );
    connect(&mCheckPort, SIGNAL(portDisconnected()), this, SLOT(closeSerialPort()) );
}
//==================================================================================================================================
void MySerialPort::writeToSerial(QByteArray dataArr)
{
    if(this->isOpen()){
        this->write(dataArr);
        this->waitForBytesWritten(this->timeOut);
    }
}
//==================================================================================================================================
void MySerialPort::mReadyRead()
{
    QByteArray readArr = this->readAll();
    while(this->waitForReadyRead(this->timeOut))
        readArr.append(this->readAll());

    QString strNorm = readArr;
    QString currentTime =  QTime::currentTime().toString("hh:mm:ss.zzz");
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
        emit vyvidDataFromThreads(3 + this->forVyvid , tr("%3 > %4 > %1     %2").arg(hexStrLog).arg(strNorm.mid(ff,16).simplified())
                                  .arg(currentTime).arg(currPortName), 0);
    }

    emit mReaddedData(readArr);

}
//==================================================================================================================================
void MySerialPort::openSerial(QString portName, QString baud, QString aboutPort, int timeOut, int dataB, int stopB,
                              int flowC, int parity, QStringList listSett)
{
    if(this->isOpen()){
        emit terminateCheckPort();
        this->close();
    }

    this->currPortName = portName;
    while(currPortName.length() < 15){
        currPortName.prepend(" ");
        if(currPortName.length() < 15)
            currPortName.append(" ");
        else
            break;
    }
//    this->currPortNameLen = portName.length();
    this->timeOut = timeOut;

    while(listSett.size() < 4)
        listSett.append("-1");
#if QT_VERSION >= 0x050000
    this->setPortName(portName);
    if(this->open(QIODevice::ReadWrite)){

        QSerialPort::DataBits dataBits=static_cast<QSerialPort::DataBits>(dataB); //currentText().toInt());
        QSerialPort::Parity parityY=static_cast<QSerialPort::Parity>(parity);
        QSerialPort::StopBits stopBits=static_cast<QSerialPort::StopBits>(stopB);
        QSerialPort::FlowControl flowControl=static_cast<QSerialPort::FlowControl>(flowC);


        if(this->setBaudRate(baud.toInt()) && this->setDataBits(dataBits) && this->setStopBits(stopBits) && this->setFlowControl(flowControl) &&
                             this->setParity(parityY)){
            mCheckPort.zapuskalka(portName);
            this->clear(QSerialPort::AllDirections);
#else
    this->setPort(portName);
    if(this->open(QIODevice::ReadWrite)){

        SerialPort::DataBits dataBits=static_cast<SerialPort::DataBits>(dataB); //currentText().toInt());
        SerialPort::Parity parityY=static_cast<SerialPort::Parity>(parity);
        SerialPort::StopBits stopBits=static_cast<SerialPort::StopBits>(stopB);
        SerialPort::FlowControl flowControl=static_cast<SerialPort::FlowControl>(flowC);


        if(this->setRate(baud.toInt()) && this->setDataBits(dataBits) && this->setStopBits(stopBits) && this->setFlowControl(flowControl) &&
                             this->setParity(parityY)){
            mCheckPort.zapuskalka(portName);
            this->clear(SerialPort::AllDirections);
#endif

            emit mPortStateIsOpened(this->isOpen());
            emit vyvidDataFromThreads(1 + this->forVyvid, tr("<i>%1 Serial port:%2, %3 opened! Baud:%8, Data Bits:%4, Stop Bits:%5, Parity:%6, FlowControl:%7.</i>")
                                      .arg(QTime::currentTime().toString("hh.mm.ss")).arg(portName).arg(aboutPort).arg(listSett.at(0))
                                      .arg(listSett.at(1)).arg(listSett.at(2)).arg(listSett.at(3)).arg(baud), 0 );
            return;
        }else{
            emit vyvidDataFromThreads(1 + this->forVyvid, tr("<i>%1 Can't configure serial port:%2. Error code:%3</i>").arg(QTime::currentTime().toString("hh.mm.ss"))
                                      .arg(portName).arg(this->errorString()), 0 );
            this->close();
        }
    }else{
        emit vyvidDataFromThreads(1 + this->forVyvid, tr("<i>%1 Can't open serial port:%2. Error code:%3</i>").arg(QTime::currentTime().toString("hh.mm.ss"))
                                  .arg(portName).arg(this->errorString()), 0 );
    }
    emit mPortStateIsOpened(this->isOpen());
}
//==================================================================================================================================
void MySerialPort::closeSerialPort()
{
    this->close();
    emit terminateCheckPort();
    emit mPortStateIsOpened(this->isOpen());

    while(currPortName.left(1) == " ")
        currPortName.remove(0,1);
    while(currPortName.right(1) == " ")
        currPortName.remove(currPortName.length() -1 , 1);
    emit vyvidDataFromThreads(1 + this->forVyvid, tr("<i>%1 Closed serial port:%2.</i>").arg(QTime::currentTime().toString("hh.mm.ss")).arg(this->currPortName), 0 );
}
//==================================================================================================================================
void MySerialPort::changeTimeOut(int timeOut)
{
    this->timeOut = timeOut;
}
//==================================================================================================================================
#endif
