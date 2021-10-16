#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    serial = new COMPort(this);

    /*setting Serial port speed*/
    QStringList SpeedPorts = { "9600", "14400", "19200", "56000", "115200", "128000", "256000" };
    ui->comboBoxBaudRate->clear();
    ui->comboBoxBaudRate->addItems(SpeedPorts);

    /*setting Serial port names*/
    ui->comboBoxPorts->clear();
    const auto listOfPorts = QSerialPortInfo::availablePorts();
    for(const auto &item : listOfPorts){
        ui->comboBoxPorts->addItem(item.portName());
    }
    connect(serial, SIGNAL(readyRead()), this, SLOT(readSerial()));
}

MainWindow::~MainWindow()
{
    serial->close();
    delete ui;
}


void MainWindow::on_pushAditionalSetings_clicked()
{
    ComDialog mComDialog(serial);
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
    if(serial->isOpen()) serial->close();
    serial->open(QIODevice::ReadWrite);

    if (serial->isOpen()) {
        ui->statusBar->showMessage("Serial port was open", 2000);
    } else {
        ui->statusBar->showMessage("Unable to open serial port!", 2000);
    };

}

void MainWindow::on_pushButtonCloseCOM_clicked()
{
    serial->close();
    if (serial->isOpen()) {
        ui->statusBar->showMessage("Unable to close serial port", 2000);
    } else {
        ui->statusBar->showMessage("Serial port was closed", 2000);
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString stringToSend = ui->lineEdit_write_to_serial->text();
    if(ui->checkBox_LF->isChecked()){
        stringToSend += '\n';
    }
    serial->write(stringToSend.toUtf8());
    ui->textEdit_transmitted->insertPlainText(stringToSend);
}

void MainWindow::readSerial()
{
    QByteArray responseData = serial->readAll();
    const QString response = QString::fromUtf8(responseData);
    ui->textEdit_received->insertPlainText(response);
}



void MainWindow::on_pushButton_received_clear_clicked()
{
    ui->textEdit_received->clear();
}

void MainWindow::on_pushButton__transmitted_clear_clicked()
{
    ui->textEdit_transmitted->clear();
}

