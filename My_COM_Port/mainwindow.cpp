#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "comdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    serial = new QSerialPort(this);

    QStringList SpeedPorts = { "9600", "14400", "19200", "56000", "115200", "128000", "256000" };
    ui->comboBoxBaudRate->clear();
    ui->comboBoxBaudRate->addItems(SpeedPorts);

    ui->comboBoxPorts->clear();
    const auto listOfPorts = QSerialPortInfo::availablePorts();
    for(const auto &item : listOfPorts){
        ui->comboBoxPorts->addItem(item.portName());
    }

}

MainWindow::~MainWindow()
{
    serial->close();
    delete ui;
}


void MainWindow::on_pushAditionalSetings_clicked()
{
    ComDialog mComDialog;
    mComDialog.setWindowTitle("Aditional COM settings");
    mComDialog.setModal(true);
    mComDialog.exec();
}

void MainWindow::on_pushButtonRefreshCOM_clicked()
{
    ui->comboBoxPorts->clear();
    const auto listOfPorts = QSerialPortInfo::availablePorts();
    for(const auto &item : listOfPorts){
        ui->comboBoxPorts->addItem(item.portName());
    }

}

void MainWindow::on_pushButtonOpenCOM_clicked()
{

    serial->setPortName(ui->comboBoxPorts->currentText());
    serial->setBaudRate(ui->comboBoxBaudRate->currentText().toInt(), QSerialPort::AllDirections);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    serial->open(QIODevice::ReadWrite);
    if (serial->isOpen()) {
        QMessageBox::information(this,"Информация", "Порт успешно открыт.");
    } else {
        QMessageBox::information(this,"Информация", "Порт не удалось открыть.");
    };

}

void MainWindow::on_pushButtonCloseCOM_clicked()
{
    serial->close();
    if (serial->isOpen()) {
        QMessageBox::information(this,"Информация", "Порт не удалось закрыть.");
    } else {
        QMessageBox::information(this,"Информация", "Порт успешно закрыт.");
    }
}
