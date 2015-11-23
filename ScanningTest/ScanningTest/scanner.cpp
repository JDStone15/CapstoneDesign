//
//  scanner.cpp
//  ScanningTest
//
//  Created by Jacob Stone on 4/21/15.
//  Copyright (c) 2015 Jacob Stone. All rights reserved.
//

#include "scanner.h"

void Scanner::drawMidpoint(Mat threshold, Mat &img){
    
    Mat temp;
    Mat nonZeroCoordinates;
    img.copyTo(temp);
    bool checkForPoint = false;
    
    float a, x, y, z;
    float aAvg = 0;
    int tempY = 0;
    int tempZ = 0;
    
    // src1 used only for debugging purposes!
    // src1 will display midpoint line to make sure it is accurate
    IplImage* src1 = cvLoadImage(binaryImage.c_str());
    
    vector<Point3f> tempmidPoints;
    vector<vector<Point3f> > allPoints(780);
    
    findNonZero(img, nonZeroCoordinates);
    
    for (int i = 0; i < nonZeroCoordinates.total(); i++ ) {
        
        // Store every non zero pixel into an vector of vectors
        a = nonZeroCoordinates.at<Point>(i).x;
        y = nonZeroCoordinates.at<Point>(i).y;
        z = i;                                  // Assigning z a random value (will be calculated later)
        
        // Create a temporary point and store into allPoints
        Point3f now = Point3f(a, y, z);
        allPoints[y].push_back(now);
        
    }
    
    // for all vectors contained in allPoints
    for(int i = 0; i < allPoints.size(); i++){
        
        // For all pixels with the same y value
        // we want to create a single averaged pixel
        for(int j = 0; j < allPoints[i].size(); j++){
            
            aAvg += (float)allPoints[i][j].x;
            checkForPoint = true;
            
            // We must flip the image otherwise it will be upside down
            tempY = imageHeight - allPoints[i][j].y;
            tempZ = allPoints[i][j].z;
            
        }
        if(checkForPoint == true){
            
            aAvg = aAvg / (float)allPoints[i].size();
            calculateCoordinates(aAvg, x, z);
            
            Point3f tempPoint(x, tempY, z);
            midPoints.push_back(tempPoint);
            
            // CvPoint is used only for drawing onto image on screen
            // FOR TESTING!!!
            CvPoint drawing = cvPoint(aAvg, tempY);
            cvLine(src1, drawing, drawing, red);
            
            checkForPoint = false;
        }
        aAvg = 0;
    }
    
    //   writeToTxtFile(midPoints);
    
    cvShowImage("Midpoint line", src1);
    fi += 3;
    //   getchar();
}

void Scanner::calculateCoordinates(float a, float &x, float &z){
    
    float ro;
    a = a - camOpticalAxel;
    ro = a / sin(alpha);
    x = ro * sin(fi) * mmPerPixel;
    z = ro * cos(fi) * mmPerPixel;
    //   fi += 1.0;
    
    
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