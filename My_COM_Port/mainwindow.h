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

private slots:
    void on_pushAditionalSetings_clicked();

    void on_pushButtonRefreshCOM_clicked();

    void on_pushButtonOpenCOM_clicked();

    void on_pushButtonCloseCOM_clicked();

    void readSerial_A();

    void readSerial_B();

    void on_pushButton_received_clear_clicked();

    void on_pushButton__transmitted_clear_clicked();


    void on_pushAditionalSetings_2_clicked();

    void on_pushButtonOpenCOM_2_clicked();

    void on_pushButtonCloseCOM_2_clicked();

    void on_pushButton_SendSerialA_clicked();

    void updateTimeTests();

    void updateMeasurementsStatus();

    void on_pushButton_SendSerialB_clicked();

    void on_pushButton_received_clear_2_clicked();

    void on_pushButton__transmitted_clear_2_clicked();

    void on_pushButton_ApplyCyclesSettings_clicked();

    void on_pushButton_StartCycles_clicked();

    void on_pushButton_StopCycles_clicked();



private:
    Ui::MainWindow *ui;
    COMPort *serial_A;
    COMPort *serial_B;
    PowerSuply *mPowerSuply;
    QTimer *mTimerTests;
    QTimer *mTimerMeasurements;
    int timerTiks;
    QVector<double> xTeoryPlot, xRealPlot, teoryPlot, realPlot;

    int LengthXXX;
    int LengthTransition;
    int NumberCycles;
    int LengthMeasurements;




};

#endif // MAINWINDOW_H
