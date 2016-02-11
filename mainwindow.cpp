#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QSettings>

//============================================================================================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#if QT_VERSION < 0x050000
    QTextCodec *utfcodec = QTextCodec::codecForName("UTF-8"); //qt4
    QTextCodec::setCodecForTr(utfcodec);
    QTextCodec::setCodecForCStrings(utfcodec);
#endif

        #ifdef Q_OS_ANDROID
    QFont font = window()->font();
    font.setPixelSize(17);
    window()->setFont(font);
#endif
    startFunction();
}
//==================================================================================================================================
MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}
//============================================================================================================================
void MainWindow::on_actionServer_To_Serial_Port_triggered()
{
    ui->tabWidget->setCurrentIndex(1);
}
//============================================================================================================================
void MainWindow::on_actionSocket_To_Serial_Port_triggered()
{
    ui->tabWidget->setCurrentIndex(2);
}
//============================================================================================================================
void MainWindow::startFunction()
{
    ui->tabWidget->setCurrentIndex(0);
    enableRefreshSerialPortTimer = new QTimer(this);
    this->enableRefreshSerialPortTimer->setSingleShot(true);

    mSecondKeyTerminalko = new QShortcut(Qt::Key_F12, this);
    connect(mSecondKeyTerminalko, SIGNAL(activated()), this, SLOT(mSecondKeyTerminalko_activated()) );

    QFont font;
#ifdef Q_OS_WIN
    font.fromString("Consolas,9,-1,5,50,0,0,0,0,0"  );
#else
    font.fromString( "Ubuntu Mono,9,-1,5,50,0,0,0,0,0" );
#endif
    ui->pteServer->setFont(font);
    ui->pteServerTerminalko->setFont(font);
    ui->pteSocket->setFont(font);
    ui->pteSocketTerminalko->setFont(font);

    connect(ui->cbSerialPortServer, SIGNAL(highlighted(QString)) , this , SLOT(requestRefreshAvSerialPorts(QString)) );
    connect(ui->cbSerialPortSocket, SIGNAL(highlighted(QString)) , this , SLOT(requestRefreshAvSerialPorts(QString)) );
    connect(ui->tbRefreshSerialServer, SIGNAL(clicked()), this, SLOT(refreshAvSerialPorts()) );
    connect(ui->tbRefreshSerialSocket, SIGNAL(clicked()), this, SLOT(refreshAvSerialPorts()) );
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(refreshAvSerialPorts()) );

#if QT_VERSION >= 0x050000
    #ifndef Q_OS_ANDROID
    ui->cbDataBitsSerialServer->addItem(QLatin1String("8"), QSerialPort::Data8);
    ui->cbDataBitsSerialServer->addItem(QLatin1String("7"), QSerialPort::Data7);
    ui->cbDataBitsSerialServer->addItem(QLatin1String("6"), QSerialPort::Data6);
    ui->cbDataBitsSerialServer->addItem(QLatin1String("5"), QSerialPort::Data5);
    ui->cbDataBitsSerialServer->setCurrentIndex(0);

    // fill stop bits
    ui->cbStopBitsSerialSever->addItem(QLatin1String("2"), QSerialPort::TwoStop);
#ifdef Q_OS_WIN
    ui->cbStopBitsSerialSever->addItem(QLatin1String("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->cbStopBitsSerialSever->addItem(QLatin1String("1"), QSerialPort::OneStop);
    ui->cbStopBitsSerialSever->setCurrentIndex(ui->cbStopBitsSerialSever->count()-1);

    // fill parity
    ui->cbParitySerialServer->addItem(QLatin1String("None"), QSerialPort::NoParity);
    ui->cbParitySerialServer->addItem(QLatin1String("Even"), QSerialPort::EvenParity);
    ui->cbParitySerialServer->addItem(QLatin1String("Odd"), QSerialPort::OddParity);
    ui->cbParitySerialServer->addItem(QLatin1String("Mark"), QSerialPort::MarkParity);
    ui->cbParitySerialServer->addItem(QLatin1String("Space"), QSerialPort::SpaceParity);
    ui->cbParitySerialServer->setCurrentIndex(0);

    // fill flow control
    ui->cbFlowSerialServer->addItem(QLatin1String("None"), QSerialPort::NoFlowControl);
    ui->cbFlowSerialServer->addItem(QLatin1String("RTS/CTS"), QSerialPort::HardwareControl);
    ui->cbFlowSerialServer->addItem(QLatin1String("XON/XOFF"), QSerialPort::SoftwareControl);
    ui->cbFlowSerialServer->setCurrentIndex(0);

    ui->cbDataBitsSerialSocket->addItem(QLatin1String("8"), QSerialPort::Data8);
    ui->cbDataBitsSerialSocket->addItem(QLatin1String("7"), QSerialPort::Data7);
    ui->cbDataBitsSerialSocket->addItem(QLatin1String("6"), QSerialPort::Data6);
    ui->cbDataBitsSerialSocket->addItem(QLatin1String("5"), QSerialPort::Data5);
    ui->cbDataBitsSerialSocket->setCurrentIndex(0);

    // fill stop bits
    ui->cbStopBitsSerialSocket->addItem(QLatin1String("2"), QSerialPort::TwoStop);
#ifdef Q_OS_WIN
    ui->cbStopBitsSerialSocket->addItem(QLatin1String("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->cbStopBitsSerialSocket->addItem(QLatin1String("1"), QSerialPort::OneStop);
    ui->cbStopBitsSerialSocket->setCurrentIndex(ui->cbStopBitsSerialSocket->count()-1);

    // fill parity
    ui->cbParitySerialSocket->addItem(QLatin1String("None"), QSerialPort::NoParity);
    ui->cbParitySerialSocket->addItem(QLatin1String("Even"), QSerialPort::EvenParity);
    ui->cbParitySerialSocket->addItem(QLatin1String("Odd"), QSerialPort::OddParity);
    ui->cbParitySerialSocket->addItem(QLatin1String("Mark"), QSerialPort::MarkParity);
    ui->cbParitySerialSocket->addItem(QLatin1String("Space"), QSerialPort::SpaceParity);
    ui->cbParitySerialSocket->setCurrentIndex(0);

    // fill flow control
    ui->cbFlowSerialSocket->addItem(QLatin1String("None"), QSerialPort::NoFlowControl);
    ui->cbFlowSerialSocket->addItem(QLatin1String("RTS/CTS"), QSerialPort::HardwareControl);
    ui->cbFlowSerialSocket->addItem(QLatin1String("XON/XOFF"), QSerialPort::SoftwareControl);
    ui->cbFlowSerialSocket->setCurrentIndex(0);
#else
    QTimer::singleShot(1,this,SLOT(andOnStart()));
    #endif
#else
    ui->cbDataBitsSerialServer->addItem(QLatin1String("8"), SerialPort::Data8);
    ui->cbDataBitsSerialServer->addItem(QLatin1String("7"), SerialPort::Data7);
    ui->cbDataBitsSerialServer->addItem(QLatin1String("6"), SerialPort::Data6);
    ui->cbDataBitsSerialServer->addItem(QLatin1String("5"), SerialPort::Data5);
    ui->cbDataBitsSerialServer->setCurrentIndex(0);

    // fill stop bits
    ui->cbStopBitsSerialSever->addItem(QLatin1String("2"), SerialPort::TwoStop);
#ifdef Q_OS_WIN
    ui->cbStopBitsSerialSever->addItem(QLatin1String("1.5"), SerialPort::OneAndHalfStop);
#endif
    ui->cbStopBitsSerialSever->addItem(QLatin1String("1"), SerialPort::OneStop);
    ui->cbStopBitsSerialSever->setCurrentIndex(ui->cbStopBitsSerialSever->count()-1);

    // fill parity
    ui->cbParitySerialServer->addItem(QLatin1String("None"), SerialPort::NoParity);
    ui->cbParitySerialServer->addItem(QLatin1String("Even"), SerialPort::EvenParity);
    ui->cbParitySerialServer->addItem(QLatin1String("Odd"), SerialPort::OddParity);
    ui->cbParitySerialServer->addItem(QLatin1String("Mark"), SerialPort::MarkParity);
    ui->cbParitySerialServer->addItem(QLatin1String("Space"), SerialPort::SpaceParity);
    ui->cbParitySerialServer->setCurrentIndex(0);

    // fill flow control
    ui->cbFlowSerialServer->addItem(QLatin1String("None"), SerialPort::NoFlowControl);
    ui->cbFlowSerialServer->addItem(QLatin1String("RTS/CTS"), SerialPort::HardwareControl);
    ui->cbFlowSerialServer->addItem(QLatin1String("XON/XOFF"), SerialPort::SoftwareControl);
    ui->cbFlowSerialServer->setCurrentIndex(0);

    ui->cbDataBitsSerialSocket->addItem(QLatin1String("8"), SerialPort::Data8);
    ui->cbDataBitsSerialSocket->addItem(QLatin1String("7"), SerialPort::Data7);
    ui->cbDataBitsSerialSocket->addItem(QLatin1String("6"), SerialPort::Data6);
    ui->cbDataBitsSerialSocket->addItem(QLatin1String("5"), SerialPort::Data5);
    ui->cbDataBitsSerialSocket->setCurrentIndex(0);

    // fill stop bits
    ui->cbStopBitsSerialSocket->addItem(QLatin1String("2"), SerialPort::TwoStop);
#ifdef Q_OS_WIN
    ui->cbStopBitsSerialSocket->addItem(QLatin1String("1.5"), SerialPort::OneAndHalfStop);
#endif
    ui->cbStopBitsSerialSocket->addItem(QLatin1String("1"), SerialPort::OneStop);
    ui->cbStopBitsSerialSocket->setCurrentIndex(ui->cbStopBitsSerialSocket->count()-1);

    // fill parity
    ui->cbParitySerialSocket->addItem(QLatin1String("None"), SerialPort::NoParity);
    ui->cbParitySerialSocket->addItem(QLatin1String("Even"), SerialPort::EvenParity);
    ui->cbParitySerialSocket->addItem(QLatin1String("Odd"), SerialPort::OddParity);
    ui->cbParitySerialSocket->addItem(QLatin1String("Mark"), SerialPort::MarkParity);
    ui->cbParitySerialSocket->addItem(QLatin1String("Space"), SerialPort::SpaceParity);
    ui->cbParitySerialSocket->setCurrentIndex(0);

    // fill flow control
    ui->cbFlowSerialSocket->addItem(QLatin1String("None"), SerialPort::NoFlowControl);
    ui->cbFlowSerialSocket->addItem(QLatin1String("RTS/CTS"), SerialPort::HardwareControl);
    ui->cbFlowSerialSocket->addItem(QLatin1String("XON/XOFF"), SerialPort::SoftwareControl);

#endif
    QString tmpStr = "CR+LF;CR;LF;None";
    QStringList trmpList = tmpStr.split(';');
    ui->onEnterPressdServer->addItems(trmpList);
    ui->onF12PressdServerTerminalko->addItems(trmpList);
    ui->onEnterPresSocketTerminalko->addItems(trmpList);
    ui->onF12PressdSocketTerminalko->addItems(trmpList);

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0, iMax = ipAddressesList.size(); i < iMax; ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    QStringList ipList;
    while(!ipAddressesList.isEmpty())
        ipList.append(ipAddressesList.takeFirst().toString());

    ipList.append(QHostAddress(QHostAddress::Any).toString());


    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    serverIPcompleter = new QCompleter(ipList, this);
    serverIPcompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->leIPServer->setCompleter(serverIPcompleter);
    ui->leIPServer->setText(ipAddress);

    ui->leIPServerTerminalko->setCompleter(serverIPcompleter);
    ui->leIPServerTerminalko->setText(ipAddress);

    ui->leIPSocket->setCompleter(serverIPcompleter);
    ui->leIPSocketTerminalko->setCompleter(serverIPcompleter);
    loadSettings();

    while(!ipList.isEmpty()){
        QString str = ipList.takeFirst();
        int indx = this->leSocketComplList.indexOf(str);
        if(indx < 0)
            this->leSocketComplList.append(str);

    }

    leSocketCompl = new QCompleter(this->leSocketComplList, this);
    ui->leIPSocket->setCompleter(leSocketCompl);
    ui->leIPSocketTerminalko->setCompleter(leSocketCompl);

    if(leTerminalkoCompleterList.isEmpty())
        leTerminalkoCompleterList<<"ATID"<<"ATCH"<<"ATPL"<<"ATRE"<<"ATCN"<<"ATAC"<<"ATFR"<<"ATAD"<<"ATVR"<<"ATSH"<<"ATSL";
    leTerminalkoCompleter = new QCompleter(leTerminalkoCompleterList, this);
    leTerminalkoCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->leServerTerminalko->setCompleter(leTerminalkoCompleter);
    ui->leSocketTerminalko->setCompleter(leTerminalkoCompleter);


    refreshAvSerialPorts();
    // my server class
    mServerok = new ServerClass();
    connect(this, SIGNAL(startOnIpPort(QString,quint16,int)), mServerok, SLOT(startOnIpPort(QString,quint16,int)) );
    connect(this, SIGNAL(stopServer()), mServerok, SLOT(stopServer()) );
    connect(mServerok, SIGNAL(serverStartedNow(bool)), ui->pbStartStopServer, SLOT(setChecked(bool)) );
    connect(mServerok, SIGNAL(vyvidDataFromThreads(int,QString,int)), this, SLOT(vyvidDataFromThreads(int,QString,int)) );
    connect(ui->sbTimeOutServer, SIGNAL(valueChanged(int)), mServerok, SLOT(mChangeTimeOut(int)) );

    //to server myserialport
    connect(this, SIGNAL(openSerialServer(QString,QString,QString,int,int,int,int,int,QStringList)),
            mServerok, SIGNAL(openSerial(QString,QString,QString,int,int,int,int,int,QStringList)) );
    connect(this, SIGNAL(closeSerialPortServer()), mServerok, SIGNAL(closeSerialPort()) );
    connect(mServerok, SIGNAL(mPortStateIsOpened(bool)), ui->pbOpenSerialServer, SLOT(setChecked(bool)) );
    connect(ui->sbTimeOutSerialServer, SIGNAL(valueChanged(int)), mServerok, SIGNAL(mChangeTimeOutSerial(int)) );

    QThread *mServerThread = new QThread(this);
    mServerok->moveToThread(mServerThread);
    mServerThread->start();    


    // my tcpsocket class
    mTcpSocket = new MyRemoteServerClass();
    connect(this, SIGNAL(connectToIPport(QString,quint16,int)), mTcpSocket, SLOT(mConnectToHost(QString,quint16,int)) );
    connect(this, SIGNAL(closeSocket()), mTcpSocket, SLOT(stopSocket()) );
    connect(ui->sbTimeOutSocket, SIGNAL(valueChanged(int)), mTcpSocket, SLOT(mChangeTimeOut(int)) );
    connect(mTcpSocket, SIGNAL(mSocketStateIsConnected(bool)) , ui->pbConnDisconSocket, SLOT(setChecked(bool)) );
    connect(mTcpSocket, SIGNAL(vyvidDataFromThreads(int,QString,int)), this, SLOT(vyvidDataFromThreads(int,QString,int)) );

    //my tcpsocket serial
    connect(this, SIGNAL(openSerialSocket(QString,QString,QString,int,int,int,int,int,QStringList)),
            mTcpSocket, SIGNAL(openSerial(QString,QString,QString,int,int,int,int,int,QStringList)) );
    connect(this, SIGNAL(closeSerialPortSocket()), mTcpSocket, SIGNAL(closeSerialPort()) );
    connect(mTcpSocket, SIGNAL(mPortStateIsOpened(bool)), ui->pbOpenSerialSocket, SLOT(setChecked(bool)) );
    connect(ui->sbTimeOutSerialSocket, SIGNAL(valueChanged(int)), mTcpSocket, SIGNAL(mChangeTimeOutSerial(int)) );

    QThread *mTcpSocketThread = new QThread(this);
    mTcpSocket->moveToThread(mTcpSocketThread);
    mTcpSocketThread->start();


    mServerokTerminalko = new ServerClass(true, 0);
    connect(this, SIGNAL(startOnIpPortTerminalko(QString,quint16,int)), mServerokTerminalko, SLOT(startOnIpPort(QString,quint16,int)) );
    connect(this, SIGNAL(stopServerTerminalko()), mServerokTerminalko, SLOT(stopServer()) );
    connect(this, SIGNAL(mWriteToServer(QByteArray)), mServerokTerminalko, SIGNAL(mWriteToSockets(QByteArray)) );
    connect(mServerokTerminalko, SIGNAL(serverStartedNow(bool)), ui->pbStartStopServerTerminalko, SLOT(setChecked(bool)) );
    connect(mServerokTerminalko, SIGNAL(serverStartedNow(bool)), ui->leServerTerminalko, SLOT(setEnabled(bool)) );
    connect(mServerokTerminalko, SIGNAL(vyvidDataFromThreads(int,QString,int)), this, SLOT(vyvidDataFromThreads(int,QString,int)) );
    connect(ui->sbTimeOutServerTerminalko, SIGNAL(valueChanged(int)), mServerokTerminalko, SLOT(mChangeTimeOut(int)) );

    QThread *mServerokTerminalkoThread = new QThread(this);
    mServerokTerminalko->moveToThread(mServerokTerminalkoThread);
    mServerokTerminalkoThread->start();


    mTcpSocketTerminalko = new MyRemoteServerClass(true, 0);
    connect(this, SIGNAL(connectToIPportTerminalko(QString,quint16,int)), mTcpSocketTerminalko, SLOT(mConnectToHost(QString,quint16,int)) );
    connect(this, SIGNAL(closeSocketTerminalko()), mTcpSocketTerminalko, SLOT(stopSocket()) );
    connect(this, SIGNAL(mWriteToSocketTerminalko(QByteArray)), mTcpSocketTerminalko, SLOT(mWriteToSockets(QByteArray)) );
    connect(ui->sbTimeOutSocketTerminalko, SIGNAL(valueChanged(int)), mTcpSocketTerminalko, SLOT(mChangeTimeOut(int)) );
    connect(mTcpSocketTerminalko, SIGNAL(mSocketStateIsConnected(bool)), ui->pbStartStopSocketTerminalko, SLOT(setChecked(bool)) );
    connect(mTcpSocketTerminalko, SIGNAL(mSocketStateIsConnected(bool)), ui->leSocketTerminalko, SLOT(setEnabled(bool)) );
    connect(mTcpSocketTerminalko, SIGNAL(vyvidDataFromThreads(int,QString,int)), this, SLOT(vyvidDataFromThreads(int,QString,int)) );

    QThread *mSocketTerminalkoThread = new QThread(this);
    mTcpSocketTerminalko->moveToThread(mSocketTerminalkoThread);
    mSocketTerminalkoThread->start();
}
//============================================================================================================================

void MainWindow::andOnStart()
{
    this->showFullScreen();
}
//============================================================================================================================
void MainWindow::refreshAvSerialPorts()
{
    this->enableRefreshSerialPortTimer->stop();

    QString currPortName0;
    QString currPortName1;

    if(this->currentSerialPortName0.isEmpty())
        currPortName0 = ui->cbSerialPortServer->currentText();
    else{
        currPortName0 = this->currentSerialPortName0;
        this->currentSerialPortName0.clear();
    }

    if(this->currentSerialPortName1.isEmpty())
        currPortName1 = ui->cbSerialPortSocket->currentText();
    else{
        currPortName1 = this->currentSerialPortName1;
        this->currentSerialPortName1.clear();
    }

    QStringList oldList;
    QStringList oldListInfo;
    QStringList newList;
    QStringList newListInfo;

    for(int i = 1, iMax = ui->cbSerialPortServer->count(); i < iMax ; i++ ){
        oldList.append(ui->cbSerialPortServer->itemText(i));
        oldListInfo.append(ui->cbSerialPortServer->itemData(i, Qt::ToolTipRole).toString());
    }

    ui->cbSerialPortServer->clear();
    ui->cbSerialPortServer->addItem("REFRESH");

    ui->cbSerialPortSocket->clear();
    ui->cbSerialPortSocket->addItem("REFRESH");

#ifndef Q_OS_ANDROID
#if QT_VERSION >= 0x050000
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
    #ifdef Q_OS_LINUX
        if(info.portName().left(4)=="ttyU"){
            newList.append(info.portName());
            newListInfo.append(info.description()+",   "+ info.manufacturer() +",   "+ info.systemLocation());
        }
    #else
        newList.append(info.portName());
        newListInfo.append(info.description()+",   "+ info.manufacturer() +",   "+ info.systemLocation());
    #endif

    }
#else
    foreach (const SerialPortInfo &info, SerialPortInfo::availablePorts()){  //SerialPortInfo &info, SerialPortInfo::availablePorts())

        newList.append(info.portName());
        newListInfo.append(info.description()+",   "+ info.manufacturer() +",   "+ info.systemLocation());
}
#endif

#endif
    int currIndex0 = 0;
    int currIndex1 = 0;

   for(int i = 0, iMax = oldList.size(); i < iMax; i++) {
       int indx = newList.indexOf(oldList.at(i));

       if(indx >= 0){
           newList.removeAt(indx);
           newListInfo.removeAt(indx);
       }else{
           oldList.removeAt(i);
           oldListInfo.removeAt(i);
           iMax = oldList.size();
           i--;
       }
    }


    for(int i = 0, iMax = newList.size(); i < iMax; i++) {
        ui->cbSerialPortServer->addItem(newList.at(i));
        ui->cbSerialPortServer->setItemData( i + 1, newListInfo.at(i), Qt::ToolTipRole);

        ui->cbSerialPortSocket->addItem(newList.at(i));
        ui->cbSerialPortSocket->setItemData( i + 1, newListInfo.at(i), Qt::ToolTipRole);

        if(currPortName0 == newList.at(i))
            currIndex0 = i + 1;
        if(currPortName1 == newList.at(i))
            currIndex1 = i + 1;
    }

    for(int i = 0, j = ui->cbSerialPortServer->count(), iMax = oldList.size(); i < iMax ; i++){
        ui->cbSerialPortServer->addItem(oldList.at(i));
        ui->cbSerialPortServer->setItemData( i + j , oldListInfo.at(i), Qt::ToolTipRole);

        ui->cbSerialPortSocket->addItem(oldList.at(i));
        ui->cbSerialPortSocket->setItemData( i + j , oldListInfo.at(i), Qt::ToolTipRole);

        if(currPortName0 == oldList.at(i))
            currIndex0 = i + j;
        if(currPortName1 == oldList.at(i))
            currIndex1 = i + j;
    }

    if(currIndex0 > 0)
        ui->cbSerialPortServer->setCurrentIndex(currIndex0);
    else
        ui->cbSerialPortServer->setCurrentIndex(ui->cbSerialPortServer->count() - 1);

    if(currIndex1 > 0)
        ui->cbSerialPortSocket->setCurrentIndex(currIndex1);
    else
        ui->cbSerialPortSocket->setCurrentIndex(ui->cbSerialPortSocket->count() - 1);

    this->enableRefreshSerialPortTimer->start(1000);

}

//============================================================================================================================

void MainWindow::requestRefreshAvSerialPorts(QString arg1)
{
    if(!enableRefreshSerialPortTimer->isActive() && arg1 == "REFRESH")
        refreshAvSerialPorts();
}

//============================================================================================================================
void MainWindow::on_cbSerialPortServer_activated(const QString &arg1)
{
    if(arg1 == "REFRESH"){
        refreshAvSerialPorts();
    }else{
        currentSerialPortName0 = arg1;
    }
}
//============================================================================================================================
void MainWindow::on_cbSerialPortSocket_activated(const QString &arg1)
{
    if(arg1 == "REFRESH"){
        refreshAvSerialPorts();
    }else{
        currentSerialPortName1 = arg1;
    }
}
//============================================================================================================================

void MainWindow::vyvidDataFromThreads(int tag, QString str, int indx)
{
    Q_UNUSED(indx);
    switch(tag){
    case 1: { ui->pteServer->appendHtml(str); break;}
    case 2: { ui->pteServer->appendPlainText(str); break;} //from server
    case 3: { ui->pteServer->appendPlainText(str);  break;} //from server serial


    case 21: { ui->pteSocket->appendHtml(str); break;}
    case 22: { ui->pteSocket->appendPlainText(str); break;} //from socket
    case 23: { ui->pteSocket->appendPlainText(str);break;} //from socket serial

    case 31: { ui->pteServerTerminalko->appendHtml(str); break;}
    case 32: { ui->pteServerTerminalko->appendPlainText(str); break;} //from server terinalko
    case 33: { ui->pteServerTerminalko->appendPlainText(str); break;} //from server terinalko

    case 41: { ui->pteSocketTerminalko->appendHtml(str); break;}
    case 42: { ui->pteSocketTerminalko->appendPlainText(str); break;} //from socket terminalko
    case 43: { ui->pteSocketTerminalko->appendPlainText(str); break;} //from socket terminalko
    }
}
//============================================================================================================================

void MainWindow::on_pbStartStopServer_clicked(bool checked)
{
    if(!checked){
        emit stopServer();
    }else{
        if(ui->leIPServer->text().isEmpty()){
            ui->leIPServer->setText(QHostAddress(QHostAddress::Any).toString());
        }

        enableServer(true);
        emit startOnIpPort(ui->leIPServer->text(), (quint16)ui->sbPortServer->value(), ui->sbTimeOutServer->value());
    }
}
//============================================================================================================================
void MainWindow::on_pbOpenSerialServer_clicked(bool checked)
{
    if(!checked){
        emit closeSerialPortServer();
    }else{
        if(ui->cbSerialPortServer->currentText() == "REFRESH"){
            refreshAvSerialPorts();
            ui->pbOpenSerialServer->setChecked(false);
        }else{
            enableSerialServer(true);
#if QT_VERSION >= 0x050000
            #ifndef Q_OS_ANDROID
            QSerialPort::DataBits dataBits=static_cast<QSerialPort::DataBits>(ui->cbDataBitsSerialServer->itemData((ui->cbDataBitsSerialServer->currentIndex())).toInt()); //currentText().toInt());
            QSerialPort::Parity parity=static_cast<QSerialPort::Parity>(ui->cbParitySerialServer->itemData(ui->cbParitySerialServer->currentIndex()).toInt());
            QSerialPort::StopBits stopBits=static_cast<QSerialPort::StopBits>(ui->cbStopBitsSerialSever->itemData(ui->cbStopBitsSerialSever->currentIndex()).toInt());
            QSerialPort::FlowControl flowControl=static_cast<QSerialPort::FlowControl>(ui->cbFlowSerialServer->itemData(ui->cbFlowSerialServer->currentIndex()).toInt());
            #endif
#else
            SerialPort::DataBits dataBits=static_cast<SerialPort::DataBits>(ui->cbDataBitsSerialServer->itemData((ui->cbDataBitsSerialServer->currentIndex())).toInt()); //currentText().toInt());
            SerialPort::Parity parity=static_cast<SerialPort::Parity>(ui->cbParitySerialServer->itemData(ui->cbParitySerialServer->currentIndex()).toInt());
            SerialPort::StopBits stopBits=static_cast<SerialPort::StopBits>(ui->cbStopBitsSerialSever->itemData(ui->cbStopBitsSerialSever->currentIndex()).toInt());
            SerialPort::FlowControl flowControl=static_cast<SerialPort::FlowControl>(ui->cbFlowSerialServer->itemData(ui->cbFlowSerialServer->currentIndex()).toInt());

#endif
            QStringList list;
            QString aboutPort = ui->cbSerialPortServer->itemData(ui->cbSerialPortServer->currentIndex(), Qt::ToolTipRole).toString();
            list.append(ui->cbDataBitsSerialServer->currentText());
            list.append(ui->cbStopBitsSerialSever->currentText());
            list.append(ui->cbParitySerialServer->currentText());
            list.append(ui->cbFlowSerialServer->currentText());
            #ifndef Q_OS_ANDROID

            emit openSerialServer(ui->cbSerialPortServer->currentText(), ui->cbBaudSerialServer->currentText(), aboutPort,
                                  ui->sbTimeOutSerialServer->value(), dataBits, stopBits, flowControl,  parity, list);
#else
            emit openSerialServer(ui->cbSerialPortServer->currentText(), ui->cbBaudSerialServer->currentText(), aboutPort,
                                  ui->sbTimeOutSerialServer->value(), 0, 0, 0,  0, list);
             #endif
        }
    }

}
//============================================================================================================================

void MainWindow::on_pbStartStopServer_toggled(bool checked)
{
    if(checked)
        return;

   refreshServerIPlist();
    enableServer(false);
}
//============================================================================================================================
void MainWindow::enableServer(bool disableNow)
{
//    ui->leIPServer->setDisabled(disableNow);
    ui->leIPServer->setReadOnly(disableNow);
    ui->sbPortServer->setReadOnly(disableNow);
//    ui->sbPortServer->setDisabled(disableNow);
}

//============================================================================================================================
void MainWindow::enableSerialServer(bool disableNow)
{
    ui->cbSerialPortServer->setDisabled(disableNow);
    ui->cbBaudSerialServer->setDisabled(disableNow);
    ui->tbRefreshSerialServer->setDisabled(disableNow);
    ui->cbDataBitsSerialServer->setDisabled(disableNow);
    ui->cbStopBitsSerialSever->setDisabled(disableNow);
    ui->cbParitySerialServer->setDisabled(disableNow);
    ui->cbFlowSerialServer->setDisabled(disableNow);
}
//============================================================================================================================

void MainWindow::on_pbOpenSerialServer_toggled(bool checked)
{
    if(checked)
        return;
    refreshAvSerialPorts();
    enableSerialServer(false);
}
//============================================================================================================================

void MainWindow::on_pbConnDisconSocket_clicked(bool checked)
{
    if(!checked){
        emit closeSocket();
    }else{
        if(ui->leIPSocket->text().isEmpty()){
            ui->leIPSocket->setText(QHostAddress(QHostAddress::Any).toString());
        }

        enableSocket(true);
        emit connectToIPport(ui->leIPSocket->text(), (quint16)ui->sbPortSocket->value(), ui->sbTimeOutSocket->value());
        addNewToSocketComplList(ui->leIPSocket->text());
    }
}
//============================================================================================================================

void MainWindow::on_pbOpenSerialSocket_clicked(bool checked)
{
    if(!checked){
        emit closeSerialPortSocket();
    }else{
        if(ui->cbSerialPortSocket->currentText() == "REFRESH"){
            refreshAvSerialPorts();
            ui->pbOpenSerialSocket->setChecked(false);
        }else{
            enableSerialSocket(true);
#if QT_VERSION >= 0x050000
            #ifndef Q_OS_ANDROID
            QSerialPort::DataBits dataBits=static_cast<QSerialPort::DataBits>(ui->cbDataBitsSerialServer->itemData((ui->cbDataBitsSerialServer->currentIndex())).toInt()); //currentText().toInt());
            QSerialPort::Parity parity=static_cast<QSerialPort::Parity>(ui->cbParitySerialServer->itemData(ui->cbParitySerialServer->currentIndex()).toInt());
            QSerialPort::StopBits stopBits=static_cast<QSerialPort::StopBits>(ui->cbStopBitsSerialSever->itemData(ui->cbStopBitsSerialSever->currentIndex()).toInt());
            QSerialPort::FlowControl flowControl=static_cast<QSerialPort::FlowControl>(ui->cbFlowSerialServer->itemData(ui->cbFlowSerialServer->currentIndex()).toInt());
            #endif
#else
            SerialPort::DataBits dataBits=static_cast<SerialPort::DataBits>(ui->cbDataBitsSerialServer->itemData((ui->cbDataBitsSerialServer->currentIndex())).toInt()); //currentText().toInt());
            SerialPort::Parity parity=static_cast<SerialPort::Parity>(ui->cbParitySerialServer->itemData(ui->cbParitySerialServer->currentIndex()).toInt());
            SerialPort::StopBits stopBits=static_cast<SerialPort::StopBits>(ui->cbStopBitsSerialSever->itemData(ui->cbStopBitsSerialSever->currentIndex()).toInt());
            SerialPort::FlowControl flowControl=static_cast<SerialPort::FlowControl>(ui->cbFlowSerialServer->itemData(ui->cbFlowSerialServer->currentIndex()).toInt());

#endif
            QStringList list;
            QString aboutPort = ui->cbSerialPortServer->itemData(ui->cbSerialPortServer->currentIndex(), Qt::ToolTipRole).toString();
            list.append(ui->cbDataBitsSerialSocket->currentText());
            list.append(ui->cbStopBitsSerialSocket->currentText());
            list.append(ui->cbParitySerialSocket->currentText());
            list.append(ui->cbFlowSerialSocket->currentText());
            #ifndef Q_OS_ANDROID
            emit openSerialSocket(ui->cbSerialPortSocket->currentText(), ui->cbBaudSerialSocket->currentText(), aboutPort,
                                  ui->sbTimeOutSerialSocket->value(), dataBits, stopBits, flowControl,  parity, list);
#else
            emit openSerialSocket(ui->cbSerialPortSocket->currentText(), ui->cbBaudSerialSocket->currentText(), aboutPort,
                                  ui->sbTimeOutSerialSocket->value(), 0, 0, 0,  0, list);
#endif
        }
    }
}
//============================================================================================================================
void MainWindow::enableSocket(bool disableNow)
{
//    ui->leIPServer->setDisabled(disableNow);
    ui->leIPSocket->setReadOnly(disableNow);
    ui->sbPortSocket->setReadOnly(disableNow);
//    ui->sbPortServer->setDisabled(disableNow);
}

//============================================================================================================================
void MainWindow::enableSerialSocket(bool disableNow)
{
    ui->cbSerialPortSocket->setDisabled(disableNow);
    ui->cbBaudSerialSocket->setDisabled(disableNow);
    ui->tbRefreshSerialSocket->setDisabled(disableNow);
    ui->cbDataBitsSerialSocket->setDisabled(disableNow);
    ui->cbStopBitsSerialSocket->setDisabled(disableNow);
    ui->cbParitySerialSocket->setDisabled(disableNow);
    ui->cbFlowSerialSocket->setDisabled(disableNow);
}
//============================================================================================================================
void MainWindow::on_pbConnDisconSocket_toggled(bool checked)
{
    if(checked)
        return;
    enableSocket(false);
}
//============================================================================================================================
void MainWindow::on_pbOpenSerialSocket_toggled(bool checked)
{
    if(checked)
        return;
    refreshAvSerialPorts();
    enableSerialSocket(false);
}
//============================================================================================================================

void MainWindow::on_actionServer_Terminalko_triggered()
{
    ui->tabWidget->setCurrentIndex(3);
}
//============================================================================================================================
void MainWindow::on_actionRemote_Server_Terminalko_triggered()
{
    ui->tabWidget->setCurrentIndex(4);
}
//============================================================================================================================

void MainWindow::on_pbStartStopServerTerminalko_clicked(bool checked)
{
    if(!checked){
        emit stopServerTerminalko();
    }else{
        if(ui->leIPServerTerminalko->text().isEmpty()){
//            serverIPlen = 11;
            ui->leIPServerTerminalko->setText(QHostAddress(QHostAddress::Any).toString());
        }/*else
            serverIPlen = ui->leIPServer->text().length();*/

        enableServerTerminalko(true);
        emit startOnIpPortTerminalko(ui->leIPServerTerminalko->text(), (quint16)ui->sbPortServerTerminalko->value(), ui->sbTimeOutServerTerminalko->value());
    }
}
//============================================================================================================================

void MainWindow::on_pushButton_clicked()
{
    if(ui->rbServer->isChecked()){  //server
        if(ui->rbSerial->isChecked())
            ui->tabWidget->setCurrentIndex(1);
        else
            ui->tabWidget->setCurrentIndex(3);

    }else{
        if(ui->rbSerial->isChecked())
            ui->tabWidget->setCurrentIndex(2);
        else
            ui->tabWidget->setCurrentIndex(4);

    }
}
//============================================================================================================================

void MainWindow::on_leServerTerminalko_returnPressed()
{
    QByteArray readArr = ui->leServerTerminalko->text().toLocal8Bit();

    ui->leServerTerminalko->setStyleSheet("");


    if(ui->viewServerTerminalko->currentIndex() == 1){
        QString str = readArr;
        str.remove(" ");
        bool ok = true;
        for(int i = 0, iMax = str.size(); i < iMax; i++){
            str.mid(i,1).toInt(&ok, 16);
            if(!ok)
                break;
        }
        if(!ok){
            ui->leServerTerminalko->setStyleSheet("QLineEdit{background: qlineargradient(spread:reflect, x1:0.486513, y1:0.339, x2:1, y2:0.346, stop:0.572193 rgba(255, 0, 0, 193), stop:1 rgba(255, 255, 255, 255));}");
            return;
        }

        readArr=QByteArray::fromHex(str.remove(" ").toLocal8Bit());
    }

    switch(ui->onEnterPressdServer->currentIndex()){
    case 0: { readArr.append("\r\n"); break; }
    case 1: { readArr.append("\r") ; break; }
    case 2: { readArr.append("\n"); break; }
    }

    QString strNorm = readArr;
    QString currentTime =  QTime::currentTime().toString("hh:mm:ss.zzz");
    for(int ff = 0, ffMax = readArr.size(); ff < ffMax; ff += 16){
        QString hexStr = readArr.mid(ff,16).toHex().leftJustified(32, ' ').toUpper();

        for(int i = 30; i > 1; i -= 2)
            hexStr = hexStr.insert(i, " ");

        QString hexStrLog = hexStr;
        ui->pteServerTerminalko->appendPlainText(tr("%3 >   Terminalko    > %1     %2").arg(hexStrLog).arg(strNorm.mid(ff,16).simplified())
                                  .arg(currentTime));
    }
    emit mWriteToServer(readArr);
     QTimer::singleShot(10, this, SLOT(clear_leServerTerminalko()) );
}
//============================================================================================================================

void MainWindow::on_leServerTerminalko_textEdited(const QString &arg1)
{
    if(ui->viewServerTerminalko->currentIndex() == 2){
        QString str = arg1;
        str.remove(" ");
        bool ok = true;
        for(int i = 0, iMax = str.size(); i < iMax; i++){
            str.mid(i,1).toInt(&ok, 16);
            if(!ok)
                break;
        }
        if(!ok){
            ui->leServerTerminalko->setStyleSheet("QLineEdit{background: qlineargradient(spread:reflect, x1:0.486513, y1:0.339, x2:1, y2:0.346, stop:0.572193 rgba(255, 0, 0, 193), stop:1 rgba(255, 255, 255, 255));}");
            return;
        }else
            ui->leServerTerminalko->setStyleSheet(ui->leIPServerTerminalko->styleSheet());

    }
}
//============================================================================================================================
void MainWindow::on_viewServerTerminalko_currentIndexChanged(int index)
{
    if(index == 2){
        QString str = ui->leServerTerminalko->text();
        str.remove(" ");
        bool ok = true;
        for(int i = 0, iMax = str.size(); i < iMax; i++){
            str.mid(i,1).toInt(&ok, 16);
            if(!ok)
                break;
        }
        if(!ok){
            ui->leServerTerminalko->setStyleSheet("QLineEdit{background: qlineargradient(spread:reflect, x1:0.486513, y1:0.339, x2:1, y2:0.346, stop:0.572193 rgba(255, 0, 0, 193), stop:1 rgba(255, 255, 255, 255));}");
            return;
        }else
            ui->leServerTerminalko->setStyleSheet(ui->leIPServerTerminalko->styleSheet());
    }else
        ui->leServerTerminalko->setStyleSheet(ui->leIPServerTerminalko->styleSheet());
}
//============================================================================================================================

void MainWindow::clear_leServerTerminalko()
{
    if(ui->leServerTerminalko->isEnabled()){
        if(!ui->leServerTerminalko->text().isEmpty()){
            delete leTerminalkoCompleter;

            int indx = leTerminalkoCompleterList.indexOf(ui->leServerTerminalko->text());
            if(indx >= 0)
                leTerminalkoCompleterList.removeAt(indx);

            leTerminalkoCompleterList.insert(1, ui->leServerTerminalko->text());

            leTerminalkoCompleter=new QCompleter(leTerminalkoCompleterList, this);
            leTerminalkoCompleter->setCaseSensitivity(Qt::CaseInsensitive);
            ui->leServerTerminalko->setCompleter(leTerminalkoCompleter);
            ui->leSocketTerminalko->setCompleter(leTerminalkoCompleter);
        }
        ui->leServerTerminalko->clear();
        ui->leServerTerminalko->setFocus();
    }
}
//============================================================================================================================
void MainWindow::mSecondKeyTerminalko_activated()
{
    if(ui->tabWidget->currentIndex() == 3){ //server terminalko
        if(ui->leServerTerminalko->isEnabled()){
            QByteArray readArr = ui->leServerTerminalko->text().toLocal8Bit();

            if(ui->viewServerTerminalko->currentIndex() == 2){
                QString str = readArr;
                str.remove(" ");
                bool ok = true;
                for(int i = 0, iMax = str.size(); i < iMax; i++){
                    str.mid(i,1).toInt(&ok, 16);
                    if(!ok)
                        break;
                }
                if(!ok){
                    ui->leServerTerminalko->setStyleSheet("QLineEdit{background: qlineargradient(spread:reflect, x1:0.486513, y1:0.339, x2:1, y2:0.346, stop:0.572193 rgba(255, 0, 0, 193), stop:1 rgba(255, 255, 255, 255));}");
                    return;
                }else
                    ui->leServerTerminalko->setStyleSheet(ui->leIPServerTerminalko->styleSheet());

                readArr=QByteArray::fromHex(str.remove(" ").toLocal8Bit());
            }

            switch(ui->onF12PressdServerTerminalko->currentIndex()){
            case 0: { readArr.append("\r\n"); break; }
            case 1: { readArr.append("\r") ; break; }
            case 2: { readArr.append("\n"); break; }
            }

            QString strNorm = readArr;
            QString currentTime =  QTime::currentTime().toString("hh:mm:ss.zzz");
            for(int ff = 0, ffMax = readArr.size(); ff < ffMax; ff += 16){
                QString hexStr = readArr.mid(ff,16).toHex().leftJustified(32, ' ').toUpper();

                for(int i = 30; i > 1; i -= 2)
                    hexStr = hexStr.insert(i, " ");

                QString hexStrLog = hexStr;
                ui->pteServerTerminalko->appendPlainText(tr("%3 >   Terminalko    > %1     %2").arg(hexStrLog).arg(strNorm.mid(ff,16).simplified())
                                          .arg(currentTime));
            }
            emit mWriteToServer(readArr);
            QTimer::singleShot(10, this, SLOT(clear_leServerTerminalko()) );
        }
    }else{
        if(ui->tabWidget->currentIndex() == 4){
            if(ui->leSocketTerminalko->isEnabled()){
                QByteArray readArr = ui->leSocketTerminalko->text().toLocal8Bit();

                if(ui->viewSocketTerminalko->currentIndex() == 2){
                    QString str = readArr;
                    str.remove(" ");
                    bool ok = true;
                    for(int i = 0, iMax = str.size(); i < iMax; i++){
                        str.mid(i,1).toInt(&ok, 16);
                        if(!ok)
                            break;
                    }
                    if(!ok){
                        ui->leSocketTerminalko->setStyleSheet("QLineEdit{background: qlineargradient(spread:reflect, x1:0.486513, y1:0.339, x2:1, y2:0.346, stop:0.572193 rgba(255, 0, 0, 193), stop:1 rgba(255, 255, 255, 255));}");
                        return;
                    }else
                        ui->leSocketTerminalko->setStyleSheet(ui->leIPSocketTerminalko->styleSheet());

                    readArr=QByteArray::fromHex(str.remove(" ").toLocal8Bit());
                }

                switch(ui->onF12PressdSocketTerminalko->currentIndex()){
                case 0: { readArr.append("\r\n"); break; }
                case 1: { readArr.append("\r") ; break; }
                case 2: { readArr.append("\n"); break; }
                }

                QString strNorm = readArr;
                QString currentTime =  QTime::currentTime().toString("hh:mm:ss.zzz");
                for(int ff = 0, ffMax = readArr.size(); ff < ffMax; ff += 16){
                    QString hexStr = readArr.mid(ff,16).toHex().leftJustified(32, ' ').toUpper();

                    for(int i = 30; i > 1; i -= 2)
                        hexStr = hexStr.insert(i, " ");

                    QString hexStrLog = hexStr;
                    ui->pteSocketTerminalko->appendPlainText(tr("%3 >   Terminalko    > %1     %2").arg(hexStrLog).arg(strNorm.mid(ff,16).simplified())
                                              .arg(currentTime));
                }
                emit mWriteToSocketTerminalko(readArr);
                QTimer::singleShot(10, this, SLOT(clear_leSocketTerminalko()) );
            }
        }
    }
}
//============================================================================================================================

void MainWindow::on_pbStartStopSocketTerminalko_clicked(bool checked)
{
    if(!checked){
        emit closeSocketTerminalko();
    }else{
        if(ui->leIPSocketTerminalko->text().isEmpty()){
            ui->leIPSocketTerminalko->setText(QHostAddress(QHostAddress::Any).toString());
        }
        enableSocketTerminalko(true);
        emit connectToIPportTerminalko(ui->leIPSocketTerminalko->text(), (quint16)ui->sbPortSocketTerminalko->value(), ui->sbTimeOutSocketTerminalko->value());
        addNewToSocketComplList(ui->leIPSocketTerminalko->text());
    }
}
//============================================================================================================================

void MainWindow::on_leSocketTerminalko_returnPressed()
{
    QByteArray readArr = ui->leSocketTerminalko->text().toLocal8Bit();

    ui->leSocketTerminalko->setStyleSheet("");

    if(ui->viewSocketTerminalko->currentIndex() == 1){
        QString str = readArr;
        str.remove(" ");
        bool ok = true;
        for(int i = 0, iMax = str.size(); i < iMax; i++){
            str.mid(i,1).toInt(&ok, 16);
            if(!ok)
                break;
        }
        if(!ok){
            ui->leSocketTerminalko->setStyleSheet("QLineEdit{background: qlineargradient(spread:reflect, x1:0.486513, y1:0.339, x2:1, y2:0.346, stop:0.572193 rgba(255, 0, 0, 193), stop:1 rgba(255, 255, 255, 255));}");
            return;
        }


        readArr=QByteArray::fromHex(str.remove(" ").toLocal8Bit());
    }

    switch(ui->onEnterPresSocketTerminalko->currentIndex()){
    case 0: { readArr.append("\r\n"); break; }
    case 1: { readArr.append("\r") ; break; }
    case 2: { readArr.append("\n"); break; }
    }

    QString strNorm = readArr;
    QString currentTime =  QTime::currentTime().toString("hh:mm:ss.zzz");

    for(int ff = 0, ffMax = readArr.size(); ff < ffMax; ff += 16){
        QString hexStr = readArr.mid(ff,16).toHex().leftJustified(32, ' ').toUpper();

        for(int i = 30; i > 1; i -= 2)
            hexStr = hexStr.insert(i, " ");

        QString hexStrLog = hexStr;
        ui->pteSocketTerminalko->appendPlainText(tr("%3 >   Terminalko    > %1     %2").arg(hexStrLog).arg(strNorm.mid(ff,16).simplified())
                                  .arg(currentTime));
    }
    emit mWriteToSocketTerminalko(readArr);
     QTimer::singleShot(10, this, SLOT(clear_leSocketTerminalko()) );
}
//============================================================================================================================
void MainWindow::clear_leSocketTerminalko()
{
    if(ui->leSocketTerminalko->isEnabled()){
        if(!ui->leSocketTerminalko->text().isEmpty()){
            delete leTerminalkoCompleter;

            int indx = leTerminalkoCompleterList.indexOf(ui->leSocketTerminalko->text());
            if(indx >= 0)
                leTerminalkoCompleterList.removeAt(indx);

            leTerminalkoCompleterList.insert(1, ui->leSocketTerminalko->text());

            leTerminalkoCompleter=new QCompleter(leTerminalkoCompleterList, this);
            leTerminalkoCompleter->setCaseSensitivity(Qt::CaseInsensitive);
            ui->leSocketTerminalko->setCompleter(leTerminalkoCompleter);
            ui->leServerTerminalko->setCompleter(leTerminalkoCompleter);
            ui->leSocketTerminalko->setCompleter(leTerminalkoCompleter);
        }
        ui->leSocketTerminalko->clear();
        ui->leSocketTerminalko->setFocus();
    }
}
//============================================================================================================================

void MainWindow::on_leSocketTerminalko_textEdited(const QString &arg1)
{
    if(ui->viewSocketTerminalko->currentIndex() == 2){
        QString str = arg1;
        str.remove(" ");
        bool ok = true;
        for(int i = 0, iMax = str.size(); i < iMax; i++){
            str.mid(i,1).toInt(&ok, 16);
            if(!ok)
                break;
        }
        if(!ok){
            ui->leSocketTerminalko->setStyleSheet("QLineEdit{background: qlineargradient(spread:reflect, x1:0.486513, y1:0.339, x2:1, y2:0.346, stop:0.572193 rgba(255, 0, 0, 193), stop:1 rgba(255, 255, 255, 255));}");
            return;
        }else
            ui->leSocketTerminalko->setStyleSheet(ui->leIPSocketTerminalko->styleSheet());

    }
}
//============================================================================================================================

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("Peredavalko"), tr("Peredavalko<br>version beta 0.1<br>EMBEE 2013"));
}
//============================================================================================================================
//============================================================================================================================

void MainWindow::loadSettings()
{
    QSettings settings("EMBEE", "Peredavalko");
    settings.beginGroup("window");
    QRect rect = settings.value("position").toRect();
    int desktopW=qApp->desktop()->width();
    int desktopH=qApp->desktop()->height();

    if(rect.x()< 10)
        rect.setX(10);
    if(rect.y()<30)
        rect.setY(31);
    if(rect.x()>(desktopW)  || rect.y()>(desktopH)) {
        rect.setX(10);
        rect.setY(31);
        //         rect.setWidth(670);
        //         rect.setHeight(500);
    }
    this->setGeometry(rect);

    ui->rbTerminalko->setChecked(settings.value("rbTerminalko", false).toBool());
    ui->rbSerial->setChecked(settings.value("rbSerial", true).toBool());
    ui->rbClient->setChecked(settings.value("rbClient", false).toBool());
    ui->rbServer->setChecked(settings.value("rbServer", true).toBool());
    this->leTerminalkoCompleterList = settings.value("completer").toStringList();
    this->leSocketComplList = settings.value("completerIP").toStringList();

    ui->spinBox->setValue(settings.value("spinBox", 50000).toInt());
    settings.endGroup();

    settings.beginGroup("serverserial");
//    ui->leIPServer->setText(settings.value("serverIP", "0.0.0.0").toString());
    ui->sbPortServer->setValue(settings.value("serverPort", 7979).toInt());
    ui->sbTimeOutServer->setValue(settings.value("serverTimeOut", 500).toInt());
    this->currentSerialPortName0 = settings.value("currSerial", "").toString();
    ui->cbBaudSerialServer->setCurrentIndex(settings.value("baud", 1).toInt());
    ui->sbTimeOutSerialServer->setValue(settings.value("serialTimeOut", 100).toInt());
    ui->cbDataBitsSerialServer->setCurrentIndex(settings.value("dataB", 0).toInt());
    ui->cbStopBitsSerialSever->setCurrentIndex(settings.value("stopB", 1).toInt());
    ui->cbParitySerialServer->setCurrentIndex(settings.value("parity", 0).toInt());
    ui->cbFlowSerialServer->setCurrentIndex(settings.value("flow", 0).toInt());

    settings.endGroup();

    settings.beginGroup("socketserial");
    ui->leIPSocket->setText(settings.value("socketIP", "0.0.0.0").toString());
    ui->sbPortSocket->setValue(settings.value("socketPort", 7979).toInt());
    ui->sbTimeOutSocket->setValue(settings.value("socketTimeOut", 500).toInt());
    this->currentSerialPortName1 = settings.value("currSerial", "").toString();
    ui->cbBaudSerialSocket->setCurrentIndex(settings.value("baud", 1).toInt());
    ui->sbTimeOutSerialSocket->setValue(settings.value("serialTimeOut", 100).toInt());
    ui->cbDataBitsSerialSocket->setCurrentIndex(settings.value("dataB", 0).toInt());
    ui->cbStopBitsSerialSocket->setCurrentIndex(settings.value("stopB", 1).toInt());
    ui->cbParitySerialSocket->setCurrentIndex(settings.value("parity", 0).toInt());
    ui->cbFlowSerialSocket->setCurrentIndex(settings.value("flow", 0).toInt());
    settings.endGroup();

    settings.beginGroup("serverterminal");
//    ui->leIPServerTerminalko->setText(settings.value("serverIP", "0.0.0.0").toString());
    ui->sbPortServerTerminalko->setValue(settings.value("serverPort", 7979).toInt());
    ui->sbTimeOutServerTerminalko->setValue(settings.value("serverTimeOut", 500).toInt());
    ui->onEnterPressdServer->setCurrentIndex(settings.value("onenter", 0).toInt());
    ui->onF12PressdServerTerminalko->setCurrentIndex(settings.value("onf12", 3).toInt());
    ui->viewServerTerminalko->setCurrentIndex(settings.value("viewmode", 0).toInt());
    settings.endGroup();

    settings.beginGroup("socketterminal");
    ui->leIPSocketTerminalko->setText(settings.value("socketIP", "0.0.0.0").toString());
    ui->sbPortSocketTerminalko->setValue(settings.value("socketPort", 7979).toInt());
    ui->sbTimeOutSocketTerminalko->setValue(settings.value("socketTimeOut", 500).toInt());
    ui->onEnterPresSocketTerminalko->setCurrentIndex(settings.value("onenter", 0).toInt());
    ui->onF12PressdSocketTerminalko->setCurrentIndex(settings.value("onf12", 3).toInt());
    ui->viewSocketTerminalko->setCurrentIndex(settings.value("viewmode", 0).toInt());
    settings.endGroup();
}
//============================================================================================================================
void MainWindow::saveSettings()
{
    QSettings settings("EMBEE", "Peredavalko");
    settings.beginGroup("window");
    settings.setValue("position", this->geometry());
    settings.setValue("rbSerial", ui->rbSerial->isChecked());
    settings.setValue("rbServer", ui->rbServer->isChecked());
    settings.setValue("rbClient", ui->rbClient->isChecked());
    settings.setValue("rbTerminalko", ui->rbTerminalko->isChecked());
    settings.setValue("completer", this->leTerminalkoCompleterList);
    settings.setValue("completerIP", this->leSocketComplList);
    settings.setValue("spinBox", ui->spinBox->value());
    settings.endGroup();

    settings.beginGroup("serverserial");
//    settings.setValue("serverIP", ui->leIPServer->text());
    settings.setValue("serverPort", ui->sbPortServer->value());
    settings.setValue("serverTimeOut", ui->sbTimeOutServer->value());
    settings.setValue("currSerial", this->currentSerialPortName0);
    settings.setValue("baud", ui->cbBaudSerialServer->currentIndex());
    settings.setValue("serialTimeOut", ui->sbTimeOutSerialServer->value());
    settings.setValue("dataB", ui->cbDataBitsSerialServer->currentIndex());
    settings.setValue("stopB", ui->cbStopBitsSerialSever->currentIndex());
    settings.setValue("parity", ui->cbParitySerialServer->currentIndex());
    settings.setValue("flow", ui->cbFlowSerialServer->currentIndex());

    settings.endGroup();

    settings.beginGroup("socketserial");
    settings.setValue("socketIP", ui->leIPSocket->text());
    settings.setValue("socketPort", ui->sbPortSocket->value());
    settings.setValue("socketTimeOut", ui->sbTimeOutSocket->value());
    settings.setValue("currSerial", this->currentSerialPortName1);
    settings.setValue("baud", ui->cbBaudSerialSocket->currentIndex());
    settings.setValue("serialTimeOut", ui->sbTimeOutSerialSocket->value());
    settings.setValue("dataB", ui->cbDataBitsSerialSocket->currentIndex());
    settings.setValue("stopB", ui->cbStopBitsSerialSocket->currentIndex());
    settings.setValue("parity", ui->cbParitySerialSocket->currentIndex());
    settings.setValue("flow", ui->cbFlowSerialSocket->currentIndex());
    settings.endGroup();

    settings.beginGroup("serverterminal");
//    settings.setValue("serverIP", ui->leIPServerTerminalko->text());
    settings.setValue("serverPort", ui->sbPortServerTerminalko->value());
    settings.setValue("serverTimeOut", ui->sbTimeOutServerTerminalko->value());
    settings.setValue("onenter", ui->onEnterPressdServer->currentIndex());
    settings.setValue("onf12", ui->onF12PressdServerTerminalko->currentIndex());
    settings.setValue("viewmode", ui->viewServerTerminalko->currentIndex());
    settings.endGroup();

    settings.beginGroup("socketterminal");
    settings.setValue("socketIP", ui->leIPSocketTerminalko->text());
    settings.setValue("socketPort", ui->sbPortSocketTerminalko->value());
    settings.setValue("socketTimeOut", ui->sbTimeOutSocketTerminalko->value());
    settings.setValue("onenter", ui->onEnterPresSocketTerminalko->currentIndex());
    settings.setValue("onf12", ui->onF12PressdSocketTerminalko->currentIndex());
    settings.setValue("viewmode", ui->viewSocketTerminalko->currentIndex());
    settings.endGroup();

}
//============================================================================================================================
 void MainWindow::refreshServerIPlist()
 {
     delete serverIPcompleter;
     QString ipAddress;
     QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
     for (int i = 0, iMax = ipAddressesList.size(); i < iMax; ++i) {
         if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
             ipAddress = ipAddressesList.at(i).toString();
             break;
         }
     }
     QStringList ipList;
     while(!ipAddressesList.isEmpty())
         ipList.append(ipAddressesList.takeFirst().toString());

     ipList.append(QHostAddress(QHostAddress::Any).toString());

     serverIPcompleter = new QCompleter(ipList, this);
     serverIPcompleter->setCaseSensitivity(Qt::CaseInsensitive);
     ui->leIPServer->setCompleter(serverIPcompleter);
     ui->leIPServerTerminalko->setCompleter(serverIPcompleter);

     bool oneAdded = false;
     while(!ipList.isEmpty()){
         QString str = ipList.takeFirst();
         int indx = this->leSocketComplList.indexOf(str);
         if(indx < 0){
             this->leSocketComplList.append(str);
             oneAdded = true;
         }
     }
     if(oneAdded){
         delete leSocketCompl;

         while(leSocketComplList.size() > 200)
             leSocketComplList.removeLast();

         leSocketCompl = new QCompleter(this->leSocketComplList, this);
         ui->leIPSocket->setCompleter(leSocketCompl);
         ui->leIPSocketTerminalko->setCompleter(leSocketCompl);
     }
 }
//============================================================================================================================

void MainWindow::on_pbStartStopServerTerminalko_toggled(bool checked)
{
    if(checked)
        return;
    refreshServerIPlist();

    enableServerTerminalko(false);
}
//============================================================================================================================
 void MainWindow::addNewToSocketComplList(QString newIP)
 {
    refreshServerIPlist();

     int indx = leSocketComplList.indexOf(newIP);
     if(indx < 0){
         delete leSocketCompl;

         leSocketComplList.insert(1, newIP);

         while(leSocketComplList.size() > 200)
             leSocketComplList.removeLast();

         leSocketCompl = new QCompleter(this->leSocketComplList, this);
         ui->leIPSocket->setCompleter(leSocketCompl);
         ui->leIPSocketTerminalko->setCompleter(leSocketCompl);
     }
 }
//============================================================================================================================

void MainWindow::on_actionHome_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
}
//============================================================================================================================
void MainWindow::enableServerTerminalko(bool disableNow)
{
//    ui->leIPServer->setDisabled(disableNow);
    ui->leIPServerTerminalko->setReadOnly(disableNow);
    ui->sbPortServerTerminalko->setReadOnly(disableNow);
//    ui->sbPortServer->setDisabled(disableNow);
}
//============================================================================================================================
void MainWindow::enableSocketTerminalko(bool disableNow)
{
//    ui->leIPServer->setDisabled(disableNow);
    ui->leIPSocketTerminalko->setReadOnly(disableNow);
    ui->sbPortSocketTerminalko->setReadOnly(disableNow);
//    ui->sbPortServer->setDisabled(disableNow);
}
//============================================================================================================================

void MainWindow::on_pbStartStopSocketTerminalko_toggled(bool checked)
{
    if(checked)
        return;
    enableSocketTerminalko(false);
}
//============================================================================================================================

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    ui->pteServer->setMaximumBlockCount(arg1);
    ui->pteServerTerminalko->setMaximumBlockCount(arg1);
    ui->pteSocket->setMaximumBlockCount(arg1);
    ui->pteSocketTerminalko->setMaximumBlockCount(arg1);
}
//============================================================================================================================
