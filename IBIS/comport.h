#ifndef COMPORT_H
#define COMPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

struct Settings {
    QString name;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
};


class COMPort : public QObject
{
    Q_OBJECT
public:
    explicit COMPort(QObject *parent = 0);
    //COMPort(QWidget *parent);
    ~COMPort();
    QSerialPort mSerialPort;

signals:
  void port_log(const QString &str);
  void port_readyRead(const QString &str);
  void port_finished();
public slots:

  void port_prosess_slot();
  void port_set_slot(const QString &name, qint32 baudRate);
  void port_set_all_slot(const Settings &setStruct);
  void port_connect_slot();
  void port_disconnect_slot();
  void port_write_slot(const QString &str);
  void port_error_slot(QSerialPort::SerialPortError);
  void port_readyRead_slot();

private:
  Settings PortSettings;
};

#endif // COMPORT_H
