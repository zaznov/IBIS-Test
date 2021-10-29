#ifndef POWERSUPLY_H
#define POWERSUPLY_H


#include "comport.h"
#include <QObject>
#include <QtCore>
#include <QtGui>


enum ChannelNumber {
    CH1,
    CH2,
    CH3,
    CH4,
};


struct PS_Status
{
    double Current;
    double Voltage;
    double Power;
    bool isActive;
};

class PowerSuply : public QObject
{
    Q_OBJECT
public:
    explicit PowerSuply(COMPort *serial, QObject *parent = nullptr);



    void reinit(COMPort *serial);
    void setVoltage(double Value, ChannelNumber channalNumber);
    void setCurrent(double Value, ChannelNumber channalNumber);
    void channel_Off(ChannelNumber channalNumber);
    void channel_On(ChannelNumber channalNumber);
    void readStatus(ChannelNumber channalNumber);




private:
    bool isActive(ChannelNumber channalNumber);
    double readVoltage(ChannelNumber channalNumber);
    double readCurrent(ChannelNumber channalNumber);
    double readPower(ChannelNumber channalNumber);
    QTimer *mTimerPowerSupply;
    double pw;

private slots:
    void updatePowerSupplyStatus();


signals:
    void newData(PS_Status);

public slots:
};

#endif // POWERSUPLY_H
