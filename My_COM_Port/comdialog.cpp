#include "comdialog.h"
#include "ui_comdialog.h"

ComDialog::ComDialog(COMPort *serial, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComDialog)
{
    ui->setupUi(this);
    mserial = serial;
    update_COM();
}


ComDialog::~ComDialog()
{
    delete ui;
}

void ComDialog::on_buttonBox_accepted()
{
    if(ui->radioButtonDataBits5->isChecked())       mserial->setDataBits(QSerialPort::Data5);
    else if(ui->radioButtonDataBits6->isChecked())  mserial->setDataBits(QSerialPort::Data6);
    else if(ui->radioButtonDataBits7->isChecked())  mserial->setDataBits(QSerialPort::Data7);
    else if(ui->radioButtonDataBits8->isChecked())  mserial->setDataBits(QSerialPort::Data8);

    if(ui->radioButtonParityNone->isChecked())      mserial->setParity(QSerialPort::NoParity);
    else if(ui->radioButtonParityOdd->isChecked())  mserial->setParity(QSerialPort::OddParity);
    else if(ui->radioButtonParityEven->isChecked()) mserial->setParity(QSerialPort::EvenParity);
    else if(ui->radioButtonParityMark->isChecked()) mserial->setParity(QSerialPort::MarkParity);
    else if(ui->radioButtonParitySpace->isChecked()) mserial->setParity(QSerialPort::SpaceParity);

    if(ui->radioButtonStopBits1->isChecked())                   mserial->setStopBits(QSerialPort::OneStop);
    else if(ui->radioButtonStopBits1_5->isChecked())            mserial->setStopBits(QSerialPort::OneAndHalfStop);
    else if(ui->radioButtonStopBits2->isChecked())              mserial->setStopBits(QSerialPort::TwoStop);

    if(ui->radioButtonHandschakingNone->isChecked())            mserial->setFlowControl(QSerialPort::NoFlowControl);
    else if(ui->radioButtonHandschakingRTSCTS->isChecked())     mserial->setFlowControl(QSerialPort::HardwareControl);
    else if(ui->radioButtonHandschakingXONXOFF->isChecked())    mserial->setFlowControl(QSerialPort::SoftwareControl);

}

void ComDialog::update_COM(){

    if(mserial->dataBits() == QSerialPort::Data5) ui->radioButtonDataBits5->setChecked(true);
    else if(mserial->dataBits() == QSerialPort::Data6) ui->radioButtonDataBits6->setChecked(true);
    else if(mserial->dataBits() == QSerialPort::Data7) ui->radioButtonDataBits7->setChecked(true);
    else if(mserial->dataBits() == QSerialPort::Data8) ui->radioButtonDataBits8->setChecked(true);
    else ui->radioButtonDataBits8->setChecked(true);

    if(mserial->parity() == QSerialPort::NoParity) ui->radioButtonParityNone->setChecked(true);
    else if(mserial->parity() == QSerialPort::OddParity) ui->radioButtonParityOdd->setChecked(true);
    else if(mserial->parity() == QSerialPort::EvenParity) ui->radioButtonParityEven->setChecked(true);
    else if(mserial->parity() == QSerialPort::MarkParity) ui->radioButtonParityMark->setChecked(true);
    else if(mserial->parity() == QSerialPort::SpaceParity) ui->radioButtonParitySpace->setChecked(true);
    else ui->radioButtonParityNone->setChecked(true);

    if(mserial->stopBits() == QSerialPort::OneStop) ui->radioButtonStopBits1->setChecked(true);
    else if(mserial->stopBits() == QSerialPort::OneAndHalfStop) ui->radioButtonStopBits1_5->setChecked(true);
    else if(mserial->stopBits() == QSerialPort::TwoStop) ui->radioButtonStopBits2->setChecked(true);
    else ui->radioButtonStopBits1->setChecked(true);

    if(mserial->flowControl() == QSerialPort::NoFlowControl) ui->radioButtonHandschakingNone->setChecked(true);
    else if(mserial->flowControl() == QSerialPort::HardwareControl) ui->radioButtonHandschakingRTSCTS->setChecked(true);
    else if(mserial->flowControl() == QSerialPort::SoftwareControl) ui->radioButtonHandschakingXONXOFF->setChecked(true);
    else ui->radioButtonParityNone->setChecked(true);
}

