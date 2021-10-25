#ifndef POWERSUPLY_H
#define POWERSUPLY_H


#include "comport.h"
#include <QObject>



class PowerSuply : public QObject
{
    Q_OBJECT
public:
    explicit PowerSuply(COMPort *serial, QObject *parent = nullptr);

    void reinit(COMPort *serial);
    void setVoltage(double Value, int channalNumber);
    void setCurrent(double Value, int channalNumber);
    void Channel_Off(int channalNumber);
    void Channel_On(int channalNumber);

signals:

public slots:
};

#endif // POWERSUPLY_H
