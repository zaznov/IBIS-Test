#include "comport.h"

COMPort::COMPort(QWidget *parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(DataRead()));
    setDataBits(QSerialPort::Data8);
    setParity(QSerialPort::NoParity);
    setStopBits(QSerialPort::OneStop);
    setFlowControl(QSerialPort::NoFlowControl);
}







void COMPort::DataRead()
{
    QByteArray responseData = this->readAll();
    const QString response = QString::fromUtf8(responseData);
    emit newData(response);
}



void COMPort::writeDataToSerial(const QString &str)
{
    this->write(str.toUtf8());

}
