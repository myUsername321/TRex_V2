#include "serialcomm.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

SerialComm::SerialComm(QObject *parent)
{
    mSerialPort = nullptr;
    mTimer.setInterval(100);
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(fakeReadyRead()));
}

QStringList SerialComm::getSerialPorts()
{
    QStringList ports;
    QList<QSerialPortInfo> serialPortInfos=QSerialPortInfo::availablePorts();

    for(QSerialPortInfo &portInfo:serialPortInfos)
    {
       ports.append(portInfo.portName());
    }
    return ports;
}

bool SerialComm::connectToDevice(QString port)
{
    if(mSerialPort)
        return false;

    if(!port.startsWith(("\\\\.\\"))){
        port = "\\\\.\\" + port;
    }

    mSerialPort = new QSerialPort(port);
    mSerialPort->setBaudRate(19200);
    connect(mSerialPort, SIGNAL(readyRead()), this, SLOT(readyRead()));

    if(!mSerialPort->open(QIODevice::ReadWrite))
        return false;

    mTimer.start();
    mSerialPort->write("mem\r\n");
    return true;
}

void SerialComm::readyRead()
{
    qDebug()<<"readyRead";
}

void SerialComm::fakeReadyRead()
{
    mSerialPort->waitForReadyRead(0);
    if(mSerialPort->bytesAvailable()) {
        readyRead();
    }
}
