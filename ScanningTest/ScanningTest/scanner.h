//
//  scanner.h
//  3D Scanner
//
//  Created by Jacob Stone on 4/21/15.
//  Copyright (c) 2015 Jacob Stone. All rights reserved.
//

#ifndef _D_Scanner_scanner_h
#define _D_Scanner_scanner_h

#include "common.h"


// Measured angle between cam and laser at motor
const float alpha = 31.788;

const float imageHeight = 720;
// 1280/2
const float camOpticalAxel = 640;

// Amout of millimeters in a pixel
// According to www.translatorscafe.com
// Probably different at longer distances
// const float mmPerPixel = 3.779528;
const float mmPerPixel = 1;         // just temp

class Scanner{
private:
    string binaryImage = "/Users/jacobstone/Documents/ECEN-403-Project/3D Scanner/BinaryImage3.jpg";
    string unfilteredImage = "/Users/jacobstone/Documents/ECEN-403-Project/3D Scanner/DarkApple.jpg";
    //string binaryImage = "/Users/jacobstone/Documents/ECEN 403/3D Scanner/3D Scanner/BinaryImage2.jpg";
    string txtFile = "Output.txt";
    CvScalar red = CV_RGB(250,0,0);
    
    float fi;
    
    vector<Point3f> midPoints;
    
    
public:
    Scanner(): fi(0) {};
    //    ~Scanner(void);
    
    void drawMidpoint(Mat threshold, Mat &img);
    string getbinaryImage(){ return binaryImage; }
    void calculateCoordinates(float a, float &x, float &z);
    void writeToTxtFile(vector<Point3f> outputPoints);
    vector<Point3f> getmidPoints(){return midPoints; }
    void incrmentfi(float incr){
        fi += incr;
    }
};

#endif