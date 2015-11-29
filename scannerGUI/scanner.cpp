//
//  scanner.cpp
//  ScanningTest
//
//  Created by Jacob Stone on 4/21/15.
//  Copyright (c) 2015 Jacob Stone. All rights reserved.
//

#include "scanner.h"

void Scanner::drawMidpoint(Mat img, int position){

    Mat temp;
    Mat nonZeroCoordinates;
    img.copyTo(temp);
    bool checkForPoint = false;

    float a, x, y, z;
    float aAvg = 0;
    float tempY = 0;
    float tempZ = 0;

    // there are 4095 positions avaialable with servo motor
    // Also want to convert from degrees to radians
    fi = (float(position) / 4095) * 2 * PI;

    vector<Point3f> tempmidPoints;
    vector<vector<Point3f> > allPoints(1280);

    findNonZero(img, nonZeroCoordinates);

    for (int i = 0; i < nonZeroCoordinates.total(); i++ ) {

        // Store every non zero pixel into an vector of vectors
        a = nonZeroCoordinates.at<Point>(i).x;
        y = nonZeroCoordinates.at<Point>(i).y;
        z = i;                                  // Assigning z a random value (will be calculated later)

        // Ignore all points that are not on the right side of
        // of the camera's optical axel
        if(a > camOpticalAxel){
            // Create a temporary point and store into allPoints
            Point3f now = Point3f(a, y, z);
            allPoints[y].push_back(now);
        }

    }

    // for all vectors contained in allPoints
    for(int i = 0; i < allPoints.size(); i++){

        // For all pixels with the same y value
        // we want to take the median x value
        // which will eliminate random outliers
        size_t size = allPoints[i].size();

        if(size > 0)
            checkForPoint = true;

        // For all pixels with the same y value
        // we want to create a single averaged pixel
        for(int j = 0; j < allPoints[i].size(); j++){

            tempY = (float)allPoints[i][j].y;
            tempZ = (float)allPoints[i][j].z;
            aAvg =(float)allPoints[i][j].x;

        }

        if(checkForPoint == true){
            y = tempY;

            calculateCoordinates(aAvg, x, y, z);
            if(y > 0){
              Point3f tempPoint(x, y, z);
              midPoints.push_back(tempPoint);
            }
            checkForPoint = false;
        }
        aAvg = 0;
    }

}

void Scanner::calculateCoordinates(float a, float &x, float &y, float &z){

    float ro = (a + 1 - camOpticalAxel) / sin(alphaRadians);

    x = ro * sin(fi) * mmPerPixelx;
    z = -1 * ro * cos(fi) * mmPerPixelx;

    // Calculate for the height, need to take into consideration position of laser
    y = (platformCenterY - y);
    y = mmPerPixely * (y - ((a - platformCenterX) / (tan(-alphaRadians))));

}

// Write into .txt file in format x y z followed by newline
void Scanner::writeToTxtFile(vector<Point3f> outputPoints){
    ofstream out(txtFile.c_str());
    for(int i = 0; i < outputPoints.size(); i++){
        if((outputPoints[i].x != 0) && (outputPoints[i].y != 0))
            out << outputPoints[i].x << " " << outputPoints[i].y << " " << outputPoints[i].z << endl;

    }
    out.close();

}

