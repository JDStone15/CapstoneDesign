#include "scanwindow.h"
#include "ui_scanwindow.h"

ScanWindow::ScanWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScanWindow)
{
    ui->setupUi(this);

    camera.open(0);
    if(camera.isOpened() == false){
        return;
    }

    tmrTimer = new QTimer(this);
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGui()));
    tmrTimer->start(20);    // start timer, every 20 milliseconds
}

ScanWindow::~ScanWindow()
{
    delete ui;
}

void ScanWindow::processFrameAndUpdateGui(){

    camera.read(matOriginal);
    if(matOriginal.empty() == true)
        return;

    // Convert the cameraFeed into a HSV colorspace
    cvtColor(matOriginal, HSVFeed, COLOR_BGR2HSV);
    if(HSVFeed.empty() == true)
        return;
    // Filter the HSV image and convert into our threshold cameraFeed
    inRange(matOriginal, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
    //cvtColor(HSVFeed, threshold, CV_HSV2RGB);
    if(threshold.empty() == true)
        return;

    // QT works better in RGB not BGR
    cvtColor(matOriginal, matOriginal, CV_BGR2RGB);

    QImage qimgOriginal((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    QImage qimgThreshold((uchar*)threshold.data, threshold.cols, threshold.rows, threshold.step, QImage::Format_Grayscale8);
 //   QImage::Format_Grayscale8

    QImage small1 = qimgOriginal.scaled(512, 288, Qt::KeepAspectRatio);
    ui->lbloriginal->setPixmap(QPixmap::fromImage(small1));

    QImage small2 = qimgThreshold.scaled(512, 288, Qt::KeepAspectRatio);
    ui->lbloriginal_2->setPixmap(QPixmap::fromImage(small2));
}


void ScanWindow::on_on_btnPauseOrResume_clicked(){

    if(tmrTimer->isActive() == true){
        tmrTimer->stop();
        ui->on_btnPauseOrResume->setText("resume");
    }
    else{
        tmrTimer->start(20);    // restart timer
        ui->on_btnPauseOrResume->setText("pause");
    }
}

void ScanWindow::on_horizontalSlider_valueChanged(int value) { H_MIN = value; }

void ScanWindow::on_horizontalSlider_2_valueChanged(int value) { S_MIN = value; }

void ScanWindow::on_horizontalSlider_3_valueChanged(int value) { V_MIN = value; }

void ScanWindow::on_horizontalSlider_6_valueChanged(int value) { H_MAX = value; }

void ScanWindow::on_horizontalSlider_5_valueChanged(int value) { S_MAX = value; }

void ScanWindow::on_horizontalSlider_4_valueChanged(int value) { V_MAX = value; }
