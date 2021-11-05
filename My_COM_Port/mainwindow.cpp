#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    timerTiks = 0;
    serial_A = new COMPort(this);
    serial_B = new COMPort(this);
    serial_PS = new COMPort(this);

    /*Threads*/
    mThreadSerialA= new QThread;
    mThreadSerialB= new QThread;
    mThreadSerialPS= new QThread;
    /*serial_A->moveToThread(mThreadSerialA);
    serial_B->moveToThread(mThreadSerialB);
    serial_PS->moveToThread(mThreadSerialPS);*/

    mPowerSuply = new PowerSuply(serial_PS, this);
    /*Timers*/
    mTimerTests = new QTimer(this);
    mTimerTests->setInterval(1000);
    mTimerMeasurements = new QTimer(this);
    mTimerMeasurements->setInterval(1000);

    makeplot();

    /*setting Serial port speed*/
    QStringList SpeedPorts = { "9600", "14400", "19200", "56000", "115200", "128000", "256000" };
    ui->comboBoxBaudRate->clear();
    ui->comboBoxBaudRate->addItems(SpeedPorts);
    ui->comboBoxBaudRate_2->clear();
    ui->comboBoxBaudRate_2->addItems(SpeedPorts);
    ui->comboBoxBaudRate_3->clear();
    ui->comboBoxBaudRate_3->addItems(SpeedPorts);

    /*setting Serial port names*/
    ui->comboBoxPorts->clear();
    ui->comboBoxPorts_2->clear();
    ui->comboBoxPorts_3->clear();
    const auto listOfPorts = QSerialPortInfo::availablePorts();
    for(const auto &item : listOfPorts){
        ui->comboBoxPorts->addItem(item.portName());
        ui->comboBoxPorts_2->addItem(item.portName());
        ui->comboBoxPorts_3->addItem(item.portName());
    }

    /*Buttons settings*/

    ui->pushButton_ONOFF_CH1->setStyleSheet("background-color: white");
    connect(ui->pushButton_ONOFF_CH1, SIGNAL(toggled(bool)), this, SLOT(Button_ONOFF_CH1_Toggled(bool)));

    /*Style settings*/
    /*ui->groupBox_2->setStyleSheet("background-image:url(\":/resource/img/r&h.jpg\"); "
                                  "background-repeat: no-repeat; "
                                  "background-position: center; ");*/






    /*Connections*/
    connect(serial_A, SIGNAL(newData(const QString &)), this, SLOT(readFromSerial_A(const QString &)));
    connect(serial_B, SIGNAL(newData(const QString &)), this, SLOT(readFromSerial_B(const QString &)));



    connect(this, SIGNAL(writeToSerial_A(const QString &)), serial_A, SLOT(writeDataToSerial(const QString &)));
    connect(this, SIGNAL(writeToSerial_B(const QString &)), serial_B, SLOT(writeDataToSerial(const QString &)));


    connect(mTimerTests, SIGNAL(timeout()), this, SLOT(updateTimeTests()));
    connect(mTimerMeasurements, SIGNAL(timeout()), this, SLOT(updateMeasurementsStatus()));
    connect(mPowerSuply, SIGNAL(newData(PS_Status)), this, SLOT(updatePowerSupplyStatus(PS_Status)));



}

MainWindow::~MainWindow()
{
    serial_A->close();
    serial_B->close();
    serial_PS->close();
    mThreadSerialA->exit();
    mThreadSerialB->exit();
    mThreadSerialPS->exit();
    delete ui;
}


void MainWindow::on_pushAditionalSetings_clicked()
{
    ComDialog mComDialog(serial_A);
    mComDialog.setWindowTitle("Aditional COM settings");
    mComDialog.setModal(true);
    mComDialog.exec();
}

void MainWindow::on_pushButtonRefreshCOM_clicked()
{
    ui->comboBoxPorts->clear();
    ui->comboBoxPorts_2->clear();
    const auto listOfPorts = QSerialPortInfo::availablePorts();
    for(const auto &item : listOfPorts){
        ui->comboBoxPorts->addItem(item.portName());
        ui->comboBoxPorts_2->addItem(item.portName());
    }

}

void MainWindow::on_pushButtonOpenCOM_clicked()
{
    serial_A->setPortName(ui->comboBoxPorts->currentText());
    serial_A->setBaudRate(ui->comboBoxBaudRate->currentText().toInt(), QSerialPort::AllDirections);
    if(serial_A->isOpen()) serial_A->close();
    serial_A->open(QIODevice::ReadWrite);

    if (serial_A->isOpen()) {
        ui->statusBar->showMessage("serial A port was open", 2000);
        serial_A->moveToThread(mThreadSerialA);
        mThreadSerialA->start();
        if (serial_B->isOpen()) mTimerTests->start();
    } else {
        ui->statusBar->showMessage("Unable to open serial A port!", 2000);
    };

}

void MainWindow::on_pushButtonOpenCOM_2_clicked()
{
    serial_B->setPortName(ui->comboBoxPorts_2->currentText());
    serial_B->setBaudRate(ui->comboBoxBaudRate_2->currentText().toInt(), QSerialPort::AllDirections);
    if(serial_B->isOpen()) serial_B->close();
    serial_B->open(QIODevice::ReadWrite);

    if (serial_B->isOpen()) {
        ui->statusBar->showMessage("serial B port was open", 2000);
        serial_B->moveToThread(mThreadSerialB);
        mThreadSerialB->start();
        if (serial_A->isOpen()) mTimerTests->start();
    } else {
        ui->statusBar->showMessage("Unable to open serial B port!", 2000);
    };
}

void MainWindow::on_pushButtonOpenCOM_3_clicked()
{
    serial_PS->setPortName(ui->comboBoxPorts_3->currentText());
    serial_PS->setBaudRate(ui->comboBoxBaudRate_3->currentText().toInt(), QSerialPort::AllDirections);
    if(serial_PS->isOpen()) serial_PS->close();
    serial_PS->open(QIODevice::ReadWrite);

    if (serial_PS->isOpen()) {
        ui->statusBar->showMessage("serial power supply port was open", 2000);
        serial_PS->moveToThread(mThreadSerialPS);
        mThreadSerialPS->start();
    } else {
        ui->statusBar->showMessage("Unable to open serial power supply port!", 2000);
    };
}

void MainWindow::on_pushButtonCloseCOM_clicked()
{
    serial_A->close();
    if (serial_A->isOpen()) {
        ui->statusBar->showMessage("Unable to close serial A port", 2000);
    } else {
        ui->statusBar->showMessage("serial A port was closed", 2000);
        mTimerTests->stop();
        mThreadSerialA->exit();
    }
}

void MainWindow::on_pushButtonCloseCOM_2_clicked()
{
    serial_B->close();
    if (serial_B->isOpen()) {
        ui->statusBar->showMessage("Unable to close serial B port", 2000);
    } else {
        ui->statusBar->showMessage("serial B port was closed", 2000);
        mTimerTests->stop();
        mThreadSerialB->exit();
    }
}

void MainWindow::on_pushButtonCloseCOM_3_clicked()
{
    serial_PS->close();
    if (serial_PS->isOpen()) {
        ui->statusBar->showMessage("Unable to close serial power supply port", 2000);
    } else {
        ui->statusBar->showMessage("serial power supply port was closed", 2000);
        mThreadSerialPS->exit();
    }
}

void MainWindow::readFromSerial_A(const QString &str)
{
    ui->textEdit_received->insertPlainText(str);
}
void MainWindow::readFromSerial_B(const QString &str)
{
    ui->textEdit_received_2->insertPlainText(str);
}
void MainWindow::readFromSerial_PS(const QString &str)
{

}


void MainWindow::on_pushButton_received_clear_clicked()
{
    ui->textEdit_received->clear();
}

void MainWindow::on_pushButton_received_clear_2_clicked()
{
    ui->textEdit_received_2->clear();
}

void MainWindow::on_pushButton__transmitted_clear_clicked()
{
    ui->textEdit_transmitted->clear();
}

void MainWindow::on_pushButton__transmitted_clear_2_clicked()
{
    ui->textEdit_transmitted_2->clear();
}


void MainWindow::on_pushAditionalSetings_2_clicked()
{
    ComDialog mComDialog(serial_B);
    mComDialog.setWindowTitle("Aditional COM settings");
    mComDialog.setModal(true);
    mComDialog.exec();
}

void MainWindow::on_pushAditionalSetings_3_clicked()
{
    ComDialog mComDialog(serial_PS);
    mComDialog.setWindowTitle("Aditional COM settings");
    mComDialog.setModal(true);
    mComDialog.exec();
}



void MainWindow::on_pushButton_SendSerialA_clicked()
{
    QString stringToSend = ui->lineEdit_write_to_serial->text();
    if(ui->checkBox_LF->isChecked()){
        stringToSend += '\n';
    }
    emit writeToSerial_A(stringToSend);
    //serial_A->write(stringToSend.toUtf8());
    ui->textEdit_transmitted->insertPlainText(stringToSend);
}



void MainWindow::on_pushButton_SendSerialB_clicked()
{
    QString stringToSend = ui->lineEdit_write_to_serial_2->text();
    if(ui->checkBox_LF_2->isChecked()){
        stringToSend += '\n';
    }
    emit writeToSerial_B(stringToSend);
    //serial_B->write(stringToSend.toUtf8());
    ui->textEdit_transmitted_2->insertPlainText(stringToSend);
}


void MainWindow::updateTimeTests()
{
    ui->lcdNumberTimeTests->display(ui->lcdNumberTimeTests->value() + 1);
}


void MainWindow::makeplot()
{
    // add two new graphs and set their look:
    ui->customPlot->xAxis->setLabel("T, Minutes");
    ui->customPlot->yAxis->setLabel("V, Volts");
    ui->customPlot->xAxis->setRange(0, 120);
    ui->customPlot->yAxis->setRange(22, 32);
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setTickLabels(false);
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

}

void MainWindow::updatePlot(){

        updateMeasurementsSettings();
        xRealPlot.resize(timerTiks+1);
        realPlot.resize(timerTiks+1);
        xRealPlot[timerTiks]  = xTeoryPlot[timerTiks];
        realPlot[timerTiks] = teoryPlot[timerTiks];


        if(timerTiks > 120){
            xRealPlot.pop_front();
            xTeoryPlot.pop_front();
            realPlot.pop_front();
            teoryPlot.pop_front();
        }

        ui->customPlot->graph(0)->setData(xRealPlot,realPlot);
        ui->customPlot->replot();

}

void MainWindow::on_pushButton_ApplyCyclesSettings_clicked()
{
    updateMeasurementsSettings();

    ui->customPlot->xAxis->setRange(0, LengthMeasurements);
    xTeoryPlot.resize(LengthMeasurements);
    teoryPlot.resize(LengthMeasurements);
    for (int i = 0; i < LengthMeasurements; i++) {
        xTeoryPlot[i] = i;
    }
    int counter = 0;
    for (int j = 0 ; j < NumberCycles; j++) {
        for (int i = 0; i < LengthXXX; i++) {
            teoryPlot[counter++] = 24;
        }
        for (int i = 0; i < LengthTransition; i++) {
            teoryPlot[counter++] = 27;
        }
        for (int i = 0; i < LengthXXX; i++) {
            teoryPlot[counter++] = 30;
        }
        for (int i = 0; i < LengthTransition; i++) {
            teoryPlot[counter++] = 27;
        }
    }





    ui->customPlot->graph(1)->setData(xTeoryPlot, teoryPlot);
    ui->customPlot->replot();


}



void MainWindow::on_pushButton_StartCycles_clicked()
{
    mTimerMeasurements->start();
    ui->statusBar->showMessage("Measurements started", 2000);
}

void MainWindow::on_pushButton_StopCycles_clicked()
{
    mTimerMeasurements->stop();
    ui->statusBar->showMessage("Measurements paused", 2000);
}




void MainWindow::updateMeasurementsStatus()
{

    updatePlot();
    if((++timerTiks) >= LengthMeasurements){
        mPowerSuply->channel_Off(CH1);
        mTimerMeasurements->stop();
        timerTiks = 0;
        ui->statusBar->showMessage("Measurements are finished", 2000);
    }

}

void MainWindow::updateMeasurementsSettings()
{
    LengthXXX = ui->lineEdit_LengthXXX->text().toInt();
    LengthTransition = ui->lineEdit_TransitionDuranion->text().toInt();
    NumberCycles = ui->lineEdit_NumberCycles->text().toInt();
    LengthMeasurements = (2*LengthXXX + 2*LengthTransition) * NumberCycles;


}

void MainWindow::updatePowerSupplyStatus(PS_Status status)
{
    displayValue(status.Power, ui->lcdNumber_PowerCH1);
}

void MainWindow::on_pushButton_SetVoltageCH1_clicked()
{
    mPowerSuply->setVoltage(ui->lineEdit_VoltageToSetCH1->text().toDouble(), CH1);
    displayValue(ui->lineEdit_VoltageToSetCH1->text().toDouble(), ui->lcdNumber_VoltageCH1);

}

void MainWindow:: displayValue(const double Value, QLCDNumber *lcd){
    int ValueInt = static_cast<int>(Value);
    QString part1 =  QString("%1").arg(ValueInt, 3, 10, QChar('0'));
    QString part2 =  QString("%1").arg(qRound((Value - ValueInt) * 100), 2, 10, QChar('0'));
    lcd->display(part1 + "." + part2);
}

void MainWindow::on_pushButton_SetCurrentCH1_clicked()
{
    mPowerSuply->setCurrent(ui->lineEdit_CurrentToSetCH1->text().toDouble(), CH1);
    displayValue(ui->lineEdit_CurrentToSetCH1->text().toDouble(), ui->lcdNumber_CurrentCH1);
}



void MainWindow::Button_ONOFF_CH1_Toggled(bool state){
    if(state){
        //Кнопка зажата
        mPowerSuply->channel_On(CH1);
        ui->pushButton_ONOFF_CH1->setStyleSheet("background-color: green");
    }else{
        //Кнопка отжата
        mPowerSuply->channel_Off(CH1);
        ui->pushButton_ONOFF_CH1->setStyleSheet("background-color: white");
    }
}








void MainWindow::on_pushButton_ONOFF_CH1_clicked()
{


}







