#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QCompleter>
#include "serverclass.h"
#include "myremoteserverclass.h"
#include <QShortcut>
#include <QLabel>




#if QT_VERSION >= 0x050000

#ifndef Q_OS_ANDROID
#include <QSerialPortInfo>
#include <QSerialPort>
#endif

#else
#include <QTextCodec> //qt4
#include <QtAddOnSerialPort/serialportinfo.h>
#include <QtAddOnSerialPort/serialport-global.h>
#include <QtAddOnSerialPort/serialport.h>
#endif



namespace Ui {
class MainWindow;
}

#if QT_VERSION < 0x050000
QT_BEGIN_NAMESPACE_SERIALPORT
class SerialPort;
QT_END_NAMESPACE_SERIALPORT

QT_USE_NAMESPACE_SERIALPORT
#endif

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    //to server
    void startOnIpPort(QString , quint16 , int);
    void stopServer();
    //to server myserialport
    void openSerialServer(QString, QString, QString, int timeOut, int, int, int, int, QStringList listSett);
    void closeSerialPortServer();

    //to socket
    void connectToIPport(QString , quint16 ,  int);
    void closeSocket();
    //to socket myserialport
    void openSerialSocket(QString, QString, QString, int timeOut, int, int, int, int, QStringList listSett);
    void closeSerialPortSocket();

    //to server terminalko
    void mWriteToServer(QByteArray);
    void startOnIpPortTerminalko(QString, quint16, int);
    void stopServerTerminalko();
//    void changeViewMode(int);

    //to socket terminalko
    void mWriteToSocketTerminalko(QByteArray);
    void connectToIPportTerminalko(QString, quint16,  int);
    void closeSocketTerminalko();
//    void changeViewMode(int);


private slots:
    void startFunction();
    void andOnStart();
    void refreshAvSerialPorts();
    void requestRefreshAvSerialPorts(QString arg1);
    void vyvidDataFromThreads(int tag, QString str, int indx);
    void enableServer(bool disableNow);
    void enableSerialServer(bool disableNow);
    void enableSocket(bool disableNow);
    void enableSerialSocket(bool disableNow);
    void clear_leServerTerminalko();
    void mSecondKeyTerminalko_activated();
    void clear_leSocketTerminalko();
    void loadSettings();
    void saveSettings();
    void refreshServerIPlist();
    void addNewToSocketComplList(QString newIP);
    void enableServerTerminalko(bool disableNow);
    void enableSocketTerminalko(bool disableNow);

    void on_actionServer_To_Serial_Port_triggered();

    void on_actionSocket_To_Serial_Port_triggered();

    void on_cbSerialPortServer_activated(const QString &arg1);

    void on_cbSerialPortSocket_activated(const QString &arg1);

    void on_pbStartStopServer_clicked(bool checked);

    void on_pbOpenSerialServer_clicked(bool checked);

    void on_pbStartStopServer_toggled(bool checked);

    void on_pbOpenSerialServer_toggled(bool checked);

    void on_pbConnDisconSocket_clicked(bool checked);

    void on_pbOpenSerialSocket_clicked(bool checked);

    void on_pbConnDisconSocket_toggled(bool checked);

    void on_pbOpenSerialSocket_toggled(bool checked);

    void on_actionServer_Terminalko_triggered();

    void on_actionRemote_Server_Terminalko_triggered();

    void on_pbStartStopServerTerminalko_clicked(bool checked);

    void on_pushButton_clicked();

    void on_leServerTerminalko_returnPressed();

    void on_leServerTerminalko_textEdited(const QString &arg1);

    void on_viewServerTerminalko_currentIndexChanged(int index);

    void on_pbStartStopSocketTerminalko_clicked(bool checked);

    void on_leSocketTerminalko_returnPressed();

    void on_leSocketTerminalko_textEdited(const QString &arg1);

    void on_actionAbout_triggered();

    void on_pbStartStopServerTerminalko_toggled(bool checked);

    void on_actionHome_triggered();

    void on_pbStartStopSocketTerminalko_toggled(bool checked);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QTimer *enableRefreshSerialPortTimer;
    QString currentSerialPortName0, currentSerialPortName1;
    QShortcut *mSecondKeyTerminalko;

    QCompleter *leTerminalkoCompleter;
    QStringList leTerminalkoCompleterList;
    QCompleter *leSocketCompl;
    QStringList leSocketComplList;


    ServerClass *mServerok;
    QCompleter *serverIPcompleter;
    MyRemoteServerClass *mTcpSocket;
    ServerClass *mServerokTerminalko;
    MyRemoteServerClass *mTcpSocketTerminalko;

    QString aboutServerSerial;
    QString aboutSocketSerial;
    QString aboutServerTerminalko;
    QString aboutSocketTerminalko;
};


//class MyCompleter : public QCompleter
//{
//    Q_OBJECT

//public:
//    explicit MyCompleter(QStringList listCompl, QCompleter *parent = 0);
//signals:
//    void onMsecondKeyActivated();
//private:
//    QShortcut *mSecondKey;

//}
#endif // MAINWINDOW_H
