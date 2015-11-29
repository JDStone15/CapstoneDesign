//
//  main.cpp
//  ArduinoSerialTest
//
//  Created by Jacob Stone on 10/30/15.
//  Copyright © 2015 Jacob Stone. All rights reserved.
//


#include <stdio.h>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <iostream>

#define USB_SERIAL_PORT "/dev/tty.usbserial-A9030OKI" // Macbook Pro

using namespace std;

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
    
    // Declare/initialize variables
    int fd;
    int recieve = 0;
    int goalPos, actualPos;
    
    if(openFileDescriptor(fd) == -1)
        return -1;
    
    init_port(&fd);
    sleep(1);           // give the port time to initialize
    handshake(fd, 0);
    sleep(1);           // give the arduino time to move servo to start position

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
        
        // Wait to recieve data from serial port
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
        actualPos = goalPos + recieve;   // calulate the actual position
        cout << "Actual Position = " << actualPos << endl;

        recieve = 0;
        
    }

    close(fd);
    sleep(1);
    return 1;
}
