//
//  main.cpp
//  ScanningTest
//
//  Created by Jacob Stone on 4/3/15.
//  Copyright (c) 2015 Jacob Stone. All rights reserved.
//

#include "scanner.h"


int H_MIN = 0;
int S_MIN = 22;
int V_MIN = 122;

int H_MAX = 255;
int S_MAX = 255;
int V_MAX = 255;



//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";


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
    // Matrix for storing HSV Image or BGR Image
    Mat HSVFeed;
    Mat BGRFeed;
    // Matrix for storing the threshold image
    Mat threshold;
    
    int fd;
    int recieve = 0;
    string input;
    
    if(openFileDescriptor(fd) == -1)
        return -1;
    
    init_port(&fd);
    sleep(1);           // give the port time to initialize
    handshake(fd, 0);
    
    // Video Capture object to acquire webcam feed
    VideoCapture capture;
    
    // Open Webcam (Default location is 0)
    capture.open(0);
    
    // Set height and width of capture frame
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);

    while(1){
        capture.read(cameraFeed);
        if(!cameraFeed.empty()){
            for(int i = 0; i < 1365; ++i){
                
                int check = i*3;

                write(fd, to_string(check).c_str(), sizeof(to_string(check).c_str()));
                usleep(100);

                while(recieve == 0){
                    read(fd, &recieve, sizeof(recieve));
                    usleep(100);
                }
                cout << "read" << endl;
                if(recieve == 255)
                    recieve = 0;
                else if(recieve > 128)
                    recieve = recieve - 256;
                
                //       check += recieve;
                cout << "Goal Position = " << check;
                check += recieve;
                cout << " Actual position = " << check << endl;
                
                if(check > (i*3))
                    i = check / 3;
                recieve = 0;
                
                capture.read(cameraFeed);
                
                // Convert the cameraFeed into a HSV colorspace
                cvtColor(cameraFeed, HSVFeed, COLOR_BGR2HSV);
                // Filter the HSV image and convert into our threshold cameraFeed
                inRange(cameraFeed, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
                
                //imshow("Camera Feed", cameraFeed);
                imshow("threshold", threshold);
                
                waitKey(30);
                // just for testing purposes
                scan.drawMidpoint(threshold, threshold, i);
            }
            
            break;
        }
        
    }
    
    
    close(fd);
    sleep(1);
    
    scan.writeToTxtFile(scan.getmidPoints());
    
    return 0;
}

