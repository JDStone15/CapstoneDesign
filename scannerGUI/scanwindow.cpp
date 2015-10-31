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

    cvtColor(matOriginal, matOriginal, CV_BGR2RGB);

    QImage qimgOriginal((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);

    QImage small = qimgOriginal.scaled(512, 288, Qt::KeepAspectRatio);
    ui->lbloriginal->setPixmap(QPixmap::fromImage(small));
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
