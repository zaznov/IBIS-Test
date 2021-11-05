#include "powersuply.h"

PowerSuply::PowerSuply(COMPort *serial, QObject *parent) : QObject(parent){


  serialPS = serial;
  pw = 0;
  VoltageActual = 0;
  CurrentActual = 0;
  PowerActual = 0;
  StatusActual = false;
  mTimerPowerSupply = new QTimer(this);
  mTimerPowerSupply->setInterval(200);
  connect(mTimerPowerSupply, SIGNAL(timeout()), this, SLOT(updatePowerSupplyStatus()));
  connect(this, SIGNAL(writeToSerial_PS(const QString &)), serialPS, SLOT(writeDataToSerial(const QString &)));
  //connect(serialPS, SIGNAL(newData(const QString &)), this, SLOT(readFromSerial_PS(const QString &)));
}

void PowerSuply::reinit(COMPort *serial){
    serialPS = serial;
}
void PowerSuply::setVoltage(double Value, ChannelNumber channalNumber){


    QString stringToSend = "INSTrument:NSELect " + QVariant(channalNumber).toString() + "\r\n";
    this->writeToSerial_PS(stringToSend);
    //serialPS->write(stringToSend.toUtf8());
    Sleep(5);
    stringToSend = "VOLTage " + QVariant(Value).toString() + "\r\n";
    this->writeToSerial_PS(stringToSend);
    //serialPS->write(stringToSend.toUtf8());



}
void PowerSuply::setCurrent(double Value, ChannelNumber channalNumber){

    QString stringToSend = "INSTrument:NSELect " + QVariant(channalNumber).toString() + "\r\n";
    this->writeToSerial_PS(stringToSend);
    //serialPS->write(stringToSend.toUtf8());
    Sleep(5);
    stringToSend = "CURRent " + QVariant(Value).toString() + "\r\n";
    this->writeToSerial_PS(stringToSend);
    //serialPS->write(stringToSend.toUtf8());

}
void PowerSuply::channel_Off(ChannelNumber channalNumber){

    mTimerPowerSupply->stop();
    QString stringToSend = "INSTrument:NSELect " + QVariant(channalNumber).toString() + "\r\n";
    this->writeToSerial_PS(stringToSend);
    //serialPS->write(stringToSend.toUtf8());
    Sleep(5);
    stringToSend = "OUTPut 0\r\n";
    this->writeToSerial_PS(stringToSend);
    //serialPS->write(stringToSend.toUtf8());
}
void PowerSuply::channel_On(ChannelNumber channalNumber){

     mTimerPowerSupply->start(); 
     QString stringToSend = "INSTrument:NSELect " + QVariant(channalNumber).toString() + "\r\n";
     this->writeToSerial_PS(stringToSend);
    //serialPS->write(stringToSend.toUtf8());
     Sleep(5);
     stringToSend = "OUTPut 1\r\n";
    this->writeToSerial_PS(stringToSend);
    //serialPS->write(stringToSend.toUtf8());
}



double PowerSuply::readVoltage(ChannelNumber channalNumber){

    QString stringToSend = "INSTrument:NSELect " + QVariant(channalNumber).toString() + "\r\n";
    this->writeToSerial_PS(stringToSend);
    Sleep(5);
    stringToSend = "MEASure:VOLTage?\r\n";
    this->writeToSerial_PS(stringToSend);


    QByteArray VoltageBytes = serialPS->readLine();
    QDataStream stream(VoltageBytes);
    stream >> VoltageActual;
    return VoltageActual;
}

double PowerSuply::readCurrent(ChannelNumber channalNumber){
    QString stringToSend = "INSTrument:NSELect " + QVariant(channalNumber).toString() + "\r\n";
    this->writeToSerial_PS(stringToSend);
    Sleep(5);
    stringToSend = "MEASure:CURRent?\r\n";
    this->writeToSerial_PS(stringToSend);

    QByteArray VoltageBytes = serialPS->readLine();
    QDataStream stream(VoltageBytes);
    stream >> CurrentActual;
    return CurrentActual;
}

double PowerSuply::readPower(ChannelNumber channalNumber){
    PowerActual = CurrentActual * VoltageActual;
    return PowerActual;
}

bool PowerSuply::isActive(ChannelNumber channalNumber){
    QString stringToSend = "INSTrument:NSELect " + QVariant(channalNumber).toString() + "\r\n";
    this->writeToSerial_PS(stringToSend);
    Sleep(5);
    stringToSend = "OUTP?\r\n";
    this->writeToSerial_PS(stringToSend);

    QByteArray isOn = serialPS->readLine();
    QDataStream stream(isOn);
    stream >> StatusActual;
    return StatusActual;
}

void PowerSuply::readStatus(ChannelNumber channalNumber){
    PS_Status mStatus;
    mStatus.isActive = isActive(channalNumber);
    mStatus.Power = readPower(channalNumber);
    mStatus.Voltage = readVoltage(channalNumber);
    mStatus.Current = readCurrent(channalNumber);

    emit newData(mStatus);
}


void PowerSuply::updatePowerSupplyStatus(){

    readStatus(CH1);
    /*Sleep(5);
    readStatus(CH2);
    Sleep(5);
    readStatus(CH3);
    Sleep(5);
    readStatus(CH4);*/
}

/*

void PowerSuply::readFromSerial_PS(const QString &str){

//    switch (command) {
//        AD:...
//        FC...

//    }

}

*/



