#ifndef COMDIALOG_H
#define COMDIALOG_H

#include <QDialog>

namespace Ui {
class ComDialog;
}

class ComDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ComDialog(QWidget *parent = nullptr);
    ~ComDialog();

private:
    Ui::ComDialog *ui;
};

#endif // COMDIALOG_H
