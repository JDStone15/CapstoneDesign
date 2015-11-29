//
//  main.cpp
//  ScanningTest
//
//  Created by Jacob Stone on 4/3/15.
//  Copyright (c) 2015 Jacob Stone. All rights reserved.
//

#include "scanner.h"

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
    cout << "Serial port successfully opened\n";
    cout << "Initializing usb port...\n";
    return 0;
    
}

void handshake(int &fd, int initialization){
    // Wait to recieve the initialization from arduino
    cout << "Waiting for initialization signal from Arduino...\n";
    while(initialization != 1)
        read(fd, &initialization, sizeof(initialization));
    cout << "Initialization signal recieved from Arduino\n";
}

int main(int argc, const char * argv[]) {
    
    Scanner scan;
    
    // Matrix to store fram of Webcam feed
    Mat cameraFeed;
    // Matrix used to store greyscale image of camera feed
    Mat grey;
    
    int fd;
    int recieve = 0;
    int goalPos, actualPos;
    
    if(openFileDescriptor(fd) == -1)
        return -1;
    
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
    
    cout << "Please place item on platform and enter any character ";
    cout << "to begin your scan" << endl;
    getchar();
    
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
                
                write(fd, to_string(goalPos).c_str(), sizeof(to_string(goalPos).c_str()));
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
                
                cout << "Goal Position = " << goalPos;
                actualPos = goalPos + recieve;
                cout << " Actual position = " << actualPos << endl;
                
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
    
    return 0;
}

