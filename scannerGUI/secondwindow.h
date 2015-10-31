#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>

#include <scanwindow.h>

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = 0);
    ~SecondWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SecondWindow *ui;
};

#endif // SECONDWINDOW_H
