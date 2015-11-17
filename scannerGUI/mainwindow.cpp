#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // iMac image location
    //QPixmap logo(LOGO_IMAGE_IMAC);
    // Macbook image location
    QPixmap logo(LOGO_IMAGE_MACBOOK);

    ui->label_logo->setPixmap(logo);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked(){

    this->hide();
    SecondWindow secondwindow;
    secondwindow.setModal(true);
    secondwindow.exec();
}
