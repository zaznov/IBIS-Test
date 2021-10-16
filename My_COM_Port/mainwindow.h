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


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushAditionalSetings_clicked();

    void on_pushButtonRefreshCOM_clicked();

    void on_pushButtonOpenCOM_clicked();

    void on_pushButtonCloseCOM_clicked();

    void on_pushButton_clicked();

    void readSerial();

    void on_pushButton_received_clear_clicked();

    void on_pushButton__transmitted_clear_clicked();


private:
    Ui::MainWindow *ui;
    COMPort *serial;


};

#endif // MAINWINDOW_H
