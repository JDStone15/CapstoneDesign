#include "calibratewindow.h"
#include "ui_calibratewindow.h"

CalibrateWindow::CalibrateWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrateWindow)
{
    ui->setupUi(this);

    camera.open(1);
    if(camera.isOpened() == false){
        return;
    }
    camera.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, 720);

    tmrTimer = new QTimer(this);
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGui()));
    tmrTimer->start(20);    // start timer, every 20 milliseconds

}

CalibrateWindow::~CalibrateWindow()
{
    delete ui;
}

void CalibrateWindow::processFrameAndUpdateGui(){

    camera.read(cameraFeed);
    if(cameraFeed.empty() == true)
        return;

    // QT works better in RGB not BGR
    cvtColor(cameraFeed, cameraFeed, CV_BGR2RGB);

    // Drawing red lines used to algin camera
    for(int i = 0; i < cameraFeed.rows; i++){
        cameraFeed.at<Vec3b>(i, 640)[0] = 255;
        cameraFeed.at<Vec3b>(i, 640)[1] = 0;
        cameraFeed.at<Vec3b>(i, 640)[2] = 0;
    }
    for(int i = 0; i < cameraFeed.cols; i++){
        cameraFeed.at<Vec3b>(592, i)[0] = 255;
        cameraFeed.at<Vec3b>(592, i)[1] = 0;
        cameraFeed.at<Vec3b>(592, i)[2] = 0;
    }

    QImage qimgCameraFeed((uchar*)cameraFeed.data, cameraFeed.cols, cameraFeed.rows, cameraFeed.step, QImage::Format_RGB888);

    QImage small = qimgCameraFeed.scaled(640, 360, Qt::KeepAspectRatio);
    ui->lblcamera->setPixmap(QPixmap::fromImage(small));

}

void CalibrateWindow::on_okButton_clicked()
{
    this->hide();
}
