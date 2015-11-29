#include "calibratewindow.h"
#include "ui_calibratewindow.h"

CalibrateWindow::CalibrateWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrateWindow)
{
    ui->setupUi(this);

    camera.open(0);
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

void CalibrateWindow::drawLine(Mat img, Point start, Point end){
    int thickness = 3;
    int lineType = 8;
    line(img, start, end, Scalar(255, 0, 0), thickness, lineType);
}


void CalibrateWindow::processFrameAndUpdateGui(){

    camera.read(cameraFeed);
    if(cameraFeed.empty() == true)
        return;


    // QT works better in RGB not BGR
    cvtColor(cameraFeed, cameraFeed, CV_BGR2RGB);

    drawLine(cameraFeed, Point(640, 0), Point(640, 720));
    drawLine(cameraFeed, Point(0, 592), Point(1280, 592));

    QImage qimgCameraFeed((uchar*)cameraFeed.data, cameraFeed.cols, cameraFeed.rows, cameraFeed.step, QImage::Format_RGB888);

    QImage small = qimgCameraFeed.scaled(640, 360, Qt::KeepAspectRatio);
    ui->lblcamera->setPixmap(QPixmap::fromImage(small));

}

void CalibrateWindow::on_okButton_clicked()
{
    this->hide();
}
