#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include "comport.h"
#include "comdialog.h"
#include "powersuply.h"
#include <QLCDNumber>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:

    void makeplot();
    void updatePlot();
    void updateMeasurementsSettings();
    void displayValue(const double value, QLCDNumber *lcd);

signals:
    void SetSerial_A(const QString &name, qint32 baudRate, QSerialPort::Directions directions);
    void SetSerial_B(const QString &name, qint32 baudRate, QSerialPort::Directions directions);
    void writeToSerial_A(const QString &str);
    void writeToSerial_B(const QString &str);
    void CloseSerial_A();
    void CloseSerial_B();
    void OpenPort_A();

private slots:

    void on_pushAditionalSetings_clicked();

    void on_pushButtonRefreshCOM_clicked();

    void on_pushButtonOpenCOM_clicked();

    void on_pushButtonCloseCOM_clicked();

    void readFromSerial_A(const QString &str);

    void readFromSerial_B(const QString &str);

    void readFromSerial_PS(const QString &str);

    void on_pushButton_received_clear_clicked();

    void on_pushButton__transmitted_clear_clicked();


    void on_pushAditionalSetings_2_clicked();

    void on_pushButtonOpenCOM_2_clicked();

    void on_pushButtonCloseCOM_2_clicked();

    void on_pushButton_SendSerialA_clicked();

    void updateTimeTests();

    void updateMeasurementsStatus();

    void updatePowerSupplyStatus(PS_Status);

    void on_pushButton_SendSerialB_clicked();

    void on_pushButton_received_clear_2_clicked();

    void on_pushButton__transmitted_clear_2_clicked();

    void on_pushButton_ApplyCyclesSettings_clicked();

    void on_pushButton_StartCycles_clicked();

    void on_pushButton_StopCycles_clicked();



    void on_pushButton_SetVoltageCH1_clicked();

    void on_pushButton_SetCurrentCH1_clicked();

    void on_pushButton_ONOFF_CH1_clicked();

    void Button_ONOFF_CH1_Toggled(bool);

    void on_pushAditionalSetings_3_clicked();

    void on_pushButtonOpenCOM_3_clicked();

    void on_pushButtonCloseCOM_3_clicked();

private:
    Ui::MainWindow *ui;
    COMPort *serial_A;
    COMPort *serial_B;
    COMPort *serial_PS;
    PowerSuply *mPowerSuply;
    QTimer *mTimerTests;
    QTimer *mTimerMeasurements;
    QTimer *mTimerPowerSupply;
    int timerTiks;
    QVector<double> xTeoryPlot, xRealPlot, teoryPlot, realPlot;

    int LengthXXX;
    int LengthTransition;
    int NumberCycles;
    int LengthMeasurements;

    QThread *mThreadSerialA;
    QThread *mThreadSerialB;
    QThread *mThreadSerialPS;




};

#endif // MAINWINDOW_H
