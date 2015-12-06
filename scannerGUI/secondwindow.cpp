
#include "secondwindow.h"
#include "ui_secondwindow.h"

SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
}

SecondWindow::~SecondWindow()
{
    delete ui;
}

void SecondWindow::on_pushButton_clicked()
{
    this->hide();
    ScanWindow scanwindow;
    scanwindow.setModal(true);
    scanwindow.exec();
}



void SecondWindow::on_Calibration_button_clicked()
{
    CalibrateWindow calibratewindow;
    calibratewindow.setModal(true);
    calibratewindow.exec();
}

void SecondWindow::on_cameraSelectButton_clicked()
{
    ui->textBrowser->append("not implemented");
}
