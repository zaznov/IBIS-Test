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


signals:
  void newData(const QString &str);

public slots:
  void DataRead(void);
  void writeDataToSerial(const QString &);
};

#endif // COMPORT_H
