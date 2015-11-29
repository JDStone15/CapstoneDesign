#include "scanwindow.h"
#include "ui_scanwindow.h"
#include "scanner.h"

#include <QDebug>

ScanWindow::ScanWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScanWindow)
{
    ui->setupUi(this);
/*
    camera.open(0);
    if(camera.isOpened() == false){
        return;
    }
    camera.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
*/
//    tmrTimer = new QTimer(this);
//    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGui()));
//    tmrTimer->start(20);    // start timer, every 20 milliseconds
}

ScanWindow::~ScanWindow()
{
    delete ui;
}
/*

void ScanWindow::processFrameAndUpdateGui(){

    camera.read(cameraFeed);
    if(cameraFeed.empty() == true)
        return;


    // QT works better in RGB not BGR
    cvtColor(cameraFeed, cameraFeed, CV_BGR2RGB);

    QImage qimgCameraFeed((uchar*)cameraFeed.data, cameraFeed.cols, cameraFeed.rows, cameraFeed.step, QImage::Format_RGB888);

    QImage small = qimgCameraFeed.scaled(640, 360, Qt::KeepAspectRatio);
    ui->lblcamera->setPixmap(QPixmap::fromImage(small));
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

*/



// Found online
void init_port(int *fd){
    struct termios options;
    tcgetattr(*fd,&options);

    cfsetispeed(&options,B9600);    // set input baud rate
    cfsetospeed(&options,B9600);    // set output baud rate

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    tcsetattr(*fd,TCSANOW,&options);
}

int openFileDescriptor(int &fd){
    // List usbSerial devices using Terminal ls /dev/tty.*
    fd = open(USB_SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);

    // Check for port errors
    if(fd == -1) {
        perror("Unable to open serial port\n");
        return -1;
    }
    qDebug() << "Serial port successfully opened\n";
    qDebug() << "Initializing usb port...\n";
    return 0;

}

void handshake(int &fd, int initialization){
    // Wait to recieve the initialization from arduino
    qDebug() << "Waiting for initialization signal from Arduino...";
    while(initialization != 1)
        read(fd, &initialization, sizeof(initialization));
    qDebug() << "Initialization signal recieved from Arduino";
}

void performScan(){

    Scanner scan;

    // Matrix to store fram of Webcam feed
    Mat cameraFeed;
    // Matrix used to store greyscale image of camera feed
    Mat grey;

    int fd;
    int recieve = 0;
    int goalPos, actualPos;

    if(openFileDescriptor(fd) == -1)
        //return -1;
        exit(1);
    init_port(&fd);
    sleep(1);           // give the port time to initialize
    handshake(fd, 0);
    sleep(1);
    // Video Capture object to acquire webcam feed
    VideoCapture capture;

    // Open Webcam (Default location is 0)
    capture.open(0);

    // Set height and width of capture frame
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);

    qDebug() << "Please place item on platform and enter any character to begin your scan";
    //getchar();

    while(1){
       capture.read(cameraFeed);
        if(!cameraFeed.empty()){

            // Incrementing the servo 20 steps at a time
            for(int i = 0; i < 206; ++i){
                goalPos = i*20;

                // We do not want to try to move the servo outside of the range 0-4095
                if(goalPos > 4095)
                    goalPos = 4095;
                else if(goalPos < 0)
                    goalPos = 0;

                //write(fd, to_string(goalPos).c_str(), sizeof(to_string(goalPos).c_str()));
                //write(fd, QString::number(goalPos).toStdString().c_str(), sizeof(QString::number(goalPos).toStdString().c_str()));
                usleep(100);

                while(recieve == 0){
                    read(fd, &recieve, sizeof(recieve));
                    usleep(100);
                }

                // recieve = 0 while no data available so 255 = 0 from the arduino
                // also only arduino only sends one byte at a time so
                // anything greater than 128 is negative
                if(recieve == 255)
                    recieve = 0;
                else if(recieve > 128)
                    recieve = recieve - 256;

                actualPos = goalPos + recieve;
                qDebug() << "Goal Position = " << goalPos << " Actual position = " << actualPos;

                recieve = 0;

                // Convert the camera feed into gray scale
                cvtColor(cameraFeed, grey, CV_BGR2GRAY);
                uchar maxIntensity = 75;    // want at least a max intensity of 75
                int x = 0;
                int y = 0;
                bool found = false;

                // for every y pixel
                for(int j = 0; j < grey.rows;j++){
                    // for every x pixel
                    for (int i = 0; i < grey.cols; i++){
                        // look for the brightest x pixel for every row
                        if(maxIntensity < grey.at<uchar>(j,i)){
                            maxIntensity = grey.at<uchar>(j,i);
                            x = i;
                            y = j;
                            found = true;
                        }
                        // Set all pixel to black
                        grey.at<uchar>(j,i) = 0;
                    }
                    // if we found a brightest pixel change that pixel to white
                    if(found == true){
                        grey.at<uchar>(y,x) = 255;
                        found = false;
                    }
                    maxIntensity = 0;
                }

                // Function used to calculate coordinates for line laser
                scan.drawMidpoint(grey, actualPos);

                // Need to update the camera feed and make sure its not empty
                capture.read(cameraFeed);
                while(cameraFeed.empty()){
                    ;;
                }
            }

            break;
        }

    }


    close(fd);
    sleep(1);

    scan.writeToTxtFile(scan.getmidPoints());

    //return 0;
}

void ScanWindow::on_on_btnPauseOrResume_clicked(){
    performScan();
}
