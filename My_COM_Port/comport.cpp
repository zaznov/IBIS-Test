#include "comport.h"

COMPort::COMPort(QWidget *parent)
{
    //connect(this, SIGNAL(readyRead), this, SLOT(DataRead));
    setDataBits(QSerialPort::Data8);
    setParity(QSerialPort::NoParity);
    setStopBits(QSerialPort::OneStop);
    setFlowControl(QSerialPort::NoFlowControl);
}







void COMPort::DataRead()
{


   // emit sendRawData(&raw);
}
