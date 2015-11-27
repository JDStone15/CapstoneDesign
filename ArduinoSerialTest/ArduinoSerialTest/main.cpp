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
    
    int fd;
    int recieve = 0;
    string input;
    
    if(openFileDescriptor(fd) == -1)
        return -1;
    
    init_port(&fd);
    sleep(1);           // give the port time to initialize
    handshake(fd, 0);
    sleep(1);           // give the port time to initialize
    // just for testing purposes
    for(int i = 0; i < 1365; ++i){
        //cout << "Enter number between 0 and 4095 to move servo: ";
        //cin >> input;
        //int check = atoi(input.c_str());
        int check = i*3;
        //input = to_string(i);
        cout << "before write ";
        write(fd, to_string(check).c_str(), sizeof(to_string(check).c_str()));
        
        usleep(100);
       cout << "write" << endl;
        // write(fd, input.c_str(), sizeof(input));
        cout << "before receve ";
        while(recieve == 0){
            read(fd, &recieve, sizeof(recieve));
            usleep(100);
        }
        cout << "read" << endl;
        if(recieve == 255)
            recieve = 0;
        else if(recieve > 128)
            recieve = recieve - 256;
        
       // check += recieve;
        cout << "Goal Position = " << check;
        check += recieve;
        cout << "position error = " << check << endl;
        if(check > (i*3))
            i = check / 3;
        recieve = 0;
        
    }
    int check = -100;
    
    write(fd, to_string(check).c_str(), sizeof(to_string(check).c_str()));
    usleep(10000);
    cout << "returning motor to position 0" << endl;
    
    close(fd);
    sleep(1);
    return 1;
}
