#ifndef COMDIALOG_H
#define COMDIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "comport.h"

namespace Ui {
class ComDialog;
}

class ComDialog : public QDialog
{
    Q_OBJECT

public:
    //ComDialog(COMPort *serial);
    ComDialog(COMPort *serial, QWidget *parent = nullptr);
    ~ComDialog();
    COMPort *mserial;


private slots:
    void on_buttonBox_accepted();

private:
    Ui::ComDialog *ui;
    void update_COM();

};

#endif // COMDIALOG_H


