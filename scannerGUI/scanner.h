#ifndef SCANNER_H
#define SCANNER_H


#include "common.h"


// Measured angle between cam and laser at motor
//const float alpha = 65;
const float alpha = 78;

const float imageHeight = 720;
const float camOpticalAxel = 640;   // 1280 / 2

//scanner parameters
const float PI = 3.14159265359;
const float alphaRadians = 1.36136;        // 78 degrees in radians
const float platformCenterX = 640;      // X pixel value for center of platform
const float platformCenterY = 592;      // Y pixel value for center of platform

// Calculated using matlab
const float mmPerPixelx = .3884365;
const float mmPerPixely = .236686;


class Scanner{
private:

    string txtFile = "Output.txt";
    float fi;
    vector<Point3f> midPoints;


public:
    Scanner(): fi(0) {}
    //    ~Scanner(void);

    void drawMidpoint(Mat img, int i);
    void calculateCoordinates(float a, float &x, float &y, float &z);
    void writeToTxtFile(vector<Point3f> outputPoints);
    vector<Point3f> getmidPoints(){return midPoints; }

};


#endif // SCANNER_H
