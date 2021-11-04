#include "powersuply.h"

PowerSuply::PowerSuply(COMPort *serial, QObject *parent) : QObject(parent){


  serialPS = serial;
  pw = 0;
  mTimerPowerSupply = new QTimer(this);
  mTimerPowerSupply->setInterval(200);
  connect(mTimerPowerSupply, SIGNAL(timeout()), this, SLOT(updatePowerSupplyStatus()));
}

void PowerSuply::reinit(COMPort *serial){


}
void PowerSuply::setVoltage(double Value, ChannelNumber channalNumber){


    QString stringToSend = "INSTrument:NSELect " + QVariant(channalNumber).toString() + "\r\n";
    serialPS->write(stringToSend.toUtf8());
    Sleep(5);
    stringToSend = "VOLTage " + QVariant(Value).toString() + "\r\n";
    serialPS->write(stringToSend.toUtf8());



}
void PowerSuply::setCurrent(double Value, ChannelNumber channalNumber){

    QString stringToSend = "INSTrument:NSELect " + QVariant(channalNumber).toString() + "\r\n";
    serialPS->write(stringToSend.toUtf8());
    Sleep(5);
    stringToSend = "CURRent " + QVariant(Value).toString() + "\r\n";
    serialPS->write(stringToSend.toUtf8());

}
void PowerSuply::channel_Off(ChannelNumber channalNumber){

    mTimerPowerSupply->stop();
    QString stringToSend = "INSTrument:NSELect " + QVariant(channalNumber).toString() + "\r\n";
    serialPS->write(stringToSend.toUtf8());
    Sleep(5);
    stringToSend = "OUTPut 0\r\n";
    serialPS->write(stringToSend.toUtf8());
}
void PowerSuply::channel_On(ChannelNumber channalNumber){

     mTimerPowerSupply->start(); 
     QString stringToSend = "INSTrument:NSELect " + QVariant(channalNumber).toString() + "\r\n";
     serialPS->write(stringToSend.toUtf8());
     Sleep(5);
     stringToSend = "OUTPut 1\r\n";
     serialPS->write(stringToSend.toUtf8());
}



double PowerSuply::readVoltage(ChannelNumber channalNumber){
    return 1;
}

double PowerSuply::readCurrent(ChannelNumber channalNumber){
   return 1;
}

double PowerSuply::readPower(ChannelNumber channalNumber){

    pw +=0.001;
    return pw;
}

bool PowerSuply::isActive(ChannelNumber channalNumber){
    return true;
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
    Sleep(5);
    readStatus(CH2);
    Sleep(5);
    readStatus(CH3);
    Sleep(5);
    readStatus(CH4);
}







