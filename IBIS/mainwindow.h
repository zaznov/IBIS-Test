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
/*-------------------Serial COM Ports signals-----------------------------*/
    void API_set_port_A(const QString &name, qint32 baudRate);
    void API_set_port_B(const QString &name, qint32 baudRate);
    void API_set_port_PS(const QString &name, qint32 baudRate);

    void API_set_all_port_A(const Settings &setStruct);
    void API_set_all_port_B(const Settings &setStruct);
    void API_set_all_port_PS(const Settings &setStruct);

    void API_connect_port_A();
    void API_connect_port_B();
    void API_connect_port_PS();

    void API_disconnect_port_A();
    void API_disconnect_port_B();
    void API_disconnect_port_PS();

    void API_write_to_port_A(const QString &str);
    void API_write_to_port_B(const QString &str);
    void API_write_to_port_PS(const QString &str);
/*-------------------*************************----------------------------*/


private slots:

/*-------------------Serial COM Ports slots-------------------------------*/
    void API_read_from_port_A(const QString &str);

    void API_read_from_port_B(const QString &str);


    void API_read_from_port_PS(const QString &str);

    void API_log(const QString &str);
/*-------------------*************************----------------------------*/

    void on_pushAditionalSetings_clicked();

    void on_pushButtonRefreshCOM_clicked();

    void on_pushButtonOpenCOM_clicked();

    void on_pushButtonCloseCOM_clicked();

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

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    COMPort *port_A;
    COMPort *port_B;
    COMPort *port_PS;
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

    QThread *mThread_port_A;
    QThread *mThread_port_B;
    QThread *mThread_port_PS;




};

#endif // MAINWINDOW_H
