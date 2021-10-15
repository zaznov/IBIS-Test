#ifndef COMPORT_H
#define COMPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>


class COMPort : public QSerialPort
{
    Q_OBJECT
public:
    COMPort(QWidget *parent);
    void DataRead();

};

#endif // COMPORT_H
