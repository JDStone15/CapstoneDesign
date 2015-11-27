//
//  scanner.cpp
//  ScanningTest
//
//  Created by Jacob Stone on 4/21/15.
//  Copyright (c) 2015 Jacob Stone. All rights reserved.
//

#include "scanner.h"

bool myfunc(const Point3f & a,const Point3f & b ){
    return a.x > b.x;
}


void Scanner::drawMidpoint(Mat threshold, Mat &img, int i){
    
    Mat temp;
    Mat nonZeroCoordinates;
    img.copyTo(temp);
    bool checkForPoint = false;
    
    float a, x, y, z;
    float aAvg = 0;
    int tempY = 0;
    int tempZ = 0;
    
    fi = float(i) *.088;
    
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
        
        if(size > 0){
            checkForPoint = true;
            sort(allPoints[i].begin(), allPoints[i].end(), myfunc);
            if (size  % 2 == 0)
                aAvg = (float)((allPoints[i][size / 2 - 1].x + allPoints[i][size / 2].x) / 2);
        
            else
                aAvg = (float)allPoints[i][size / 2].x;
        }
        
        // For all pixels with the same y value
        // we want to create a single averaged pixel
        for(int j = 0; j < allPoints[i].size(); j++){
            
            //aAvg += (float)allPoints[i][j].x;
            //checkForPoint = true;
            
            // We must flip the image otherwise it will be upside down
            tempY = imageHeight - allPoints[i][j].y;
            tempZ = allPoints[i][j].z;
            
        }
        
        if(checkForPoint == true){
            y = tempY;
            //aAvg = aAvg / (float)allPoints[i].size();
            calculateCoordinates(aAvg, x, y, z);
            
            Point3f tempPoint(x, y, z);
            midPoints.push_back(tempPoint);
            
            checkForPoint = false;
        }
        aAvg = 0;
    }

}

void Scanner::calculateCoordinates(float a, float &x, float &y, float &z){
    
    float ro;
    a = a - camOpticalAxel;
    ro = a / sin(alpha);
    
    x = ro * sin(fi) * mmPerPixel;
    y = y* mmPerPixel;
    z = ro * cos(fi) * mmPerPixel;
    
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