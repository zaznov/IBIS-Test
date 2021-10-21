#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    serial_A = new COMPort(this);
    serial_B = new COMPort(this);
    mTimerTests = new QTimer(this);
    mTimerTests->setInterval(1000);
    makeplot();

    /*setting Serial port speed*/
    QStringList SpeedPorts = { "9600", "14400", "19200", "56000", "115200", "128000", "256000" };
    ui->comboBoxBaudRate->clear();
    ui->comboBoxBaudRate->addItems(SpeedPorts);
    ui->comboBoxBaudRate_2->clear();
    ui->comboBoxBaudRate_2->addItems(SpeedPorts);

    /*setting Serial port names*/
    ui->comboBoxPorts->clear();
    ui->comboBoxPorts_2->clear();
    const auto listOfPorts = QSerialPortInfo::availablePorts();
    for(const auto &item : listOfPorts){
        ui->comboBoxPorts->addItem(item.portName());
        ui->comboBoxPorts_2->addItem(item.portName());
    }
    connect(serial_A, SIGNAL(readyRead()), this, SLOT(readSerial_A()));
    connect(serial_B, SIGNAL(readyRead()), this, SLOT(readSerial_B()));
    connect(mTimerTests, SIGNAL(timeout()), this, SLOT(updateTimeTests()));
}

MainWindow::~MainWindow()
{
    serial_A->close();
    serial_B->close();
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
        if (serial_A->isOpen()) mTimerTests->start();
    } else {
        ui->statusBar->showMessage("Unable to open serial B port!", 2000);
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
    }
}

void MainWindow::readSerial_A()
{
    QByteArray responseData = serial_A->readAll();
    const QString response = QString::fromUtf8(responseData);
    ui->textEdit_received->insertPlainText(response);
}

void MainWindow::readSerial_B()
{
    QByteArray responseData = serial_B->readAll();
    const QString response = QString::fromUtf8(responseData);
    ui->textEdit_received_2->insertPlainText(response);
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





void MainWindow::on_pushButton_SendSerialA_clicked()
{
    QString stringToSend = ui->lineEdit_write_to_serial->text();
    if(ui->checkBox_LF->isChecked()){
        stringToSend += '\n';
    }
    serial_A->write(stringToSend.toUtf8());
    ui->textEdit_transmitted->insertPlainText(stringToSend);
}



void MainWindow::on_pushButton_SendSerialB_clicked()
{
    QString stringToSend = ui->lineEdit_write_to_serial_2->text();
    if(ui->checkBox_LF_2->isChecked()){
        stringToSend += '\n';
    }
    serial_B->write(stringToSend.toUtf8());
    ui->textEdit_transmitted_2->insertPlainText(stringToSend);
}


void MainWindow::updateTimeTests()
{
    ui->lcdNumberTimeTests->display(ui->lcdNumberTimeTests->value() + 1);
}


void MainWindow::makeplot()
{
    // add two new graphs and set their look:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    // generate some points of data (y0 for first, y1 for second graph):
    QVector<double> x(251), y0(251), y1(251);
    for (int i=0; i<251; ++i)
    {
      x[i] = i;
      y0[i] = qExp(-i/150.0)*qCos(i/10.0); // exponentially decaying cosine
      y1[i] = qExp(-i/150.0);              // exponential envelope
    }
    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setTickLabels(false);
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // pass data points to graphs:
    ui->customPlot->graph(0)->setData(x, y0);
    ui->customPlot->graph(1)->setData(x, y1);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    ui->customPlot->graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    ui->customPlot->graph(1)->rescaleAxes(true);
    // Note: we could have also just called ui->customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


}

void MainWindow::updatePlot(){






}
