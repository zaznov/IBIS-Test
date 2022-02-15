#include "comport.h"
#include <qdebug.h>

COMPort::COMPort(QObject *parent) :
    QObject(parent)
{
}




COMPort::~COMPort()
{
    qDebug("By in Thread!");
    emit port_finished();
}

void COMPort::port_prosess_slot(){
    qDebug("Hello World in Thread!");
    connect(&mSerialPort,SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error_slot(QSerialPort::SerialPortError)));
    connect(&mSerialPort, SIGNAL(readyRead()),this,SLOT(port_readyRead_slot()));

    PortSettings.dataBits = QSerialPort::Data8;
    PortSettings.parity = QSerialPort::NoParity;
    PortSettings.stopBits = QSerialPort::OneStop;
    PortSettings.flowControl = QSerialPort::NoFlowControl;
}

void COMPort::port_readyRead_slot()
{
    QByteArray responseData = mSerialPort.readAll();
    const QString response = QString::fromUtf8(responseData);
    emit port_readyRead(response);
}

void COMPort::port_write_slot(const QString &str)
{
    if(mSerialPort.isOpen()){
        mSerialPort.write(str.toUtf8());
    }
}

void COMPort::port_disconnect_slot()
{
    if(mSerialPort.isOpen()){
        mSerialPort.close();
        emit port_log((PortSettings.name + " port was closed!").toLocal8Bit());
    }
}

void COMPort::port_connect_slot()
{
    qDebug(PortSettings.name.toLocal8Bit());
    mSerialPort.setPortName(PortSettings.name);
    if (mSerialPort.open(QIODevice::ReadWrite)) {
        if (mSerialPort.setBaudRate(PortSettings.baudRate)
                && mSerialPort.setDataBits(PortSettings.dataBits)
                && mSerialPort.setParity(PortSettings.parity)
                && mSerialPort.setStopBits(PortSettings.stopBits)
                && mSerialPort.setFlowControl(PortSettings.flowControl))
        {
            if (mSerialPort.isOpen()){
                emit port_log((PortSettings.name + " port was open!").toLocal8Bit());
            }
        } else {
            mSerialPort.close();
            emit port_log(mSerialPort.errorString().toLocal8Bit());
          }
    } else {
        mSerialPort.close();
        emit port_log(mSerialPort.errorString().toLocal8Bit());
    }
}

void COMPort::port_set_slot(const QString &name, qint32 baudRate)
{
    PortSettings.name = name;
    PortSettings.baudRate = baudRate;
}

void COMPort::port_set_all_slot(const Settings &setStruct)
{
    PortSettings = setStruct;
}


void COMPort::port_error_slot(QSerialPort::SerialPortError error)
{
    if ( (mSerialPort.isOpen()) && (error == QSerialPort::ResourceError)) {
        emit port_log(mSerialPort.errorString().toLocal8Bit());
        emit port_disconnect_slot();
    }
}
