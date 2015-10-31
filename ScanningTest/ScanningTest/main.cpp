//
//  main.cpp
//  ScanningTest
//
//  Created by Jacob Stone on 4/3/15.
//  Copyright (c) 2015 Jacob Stone. All rights reserved.
//

#include "scanner.h"

int H_MIN = 0;
int S_MIN = 0;
int V_MIN = 0;

int H_MAX = 256;
int S_MAX = 256;
int V_MAX = 256;

int B_MIN = 100;
int G_MIN = 0;
int R_MIN = 0;

int B_MAX = 256;
int G_MAX = 256;
int R_MAX = 256;


//default capture width and height
//const int FRAME_WIDTH = 640;
//const int FRAME_HEIGHT = 480;

//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";



void on_trackbar( int, void* ){
    //This function gets called whenever a
    // trackbar position is changed
}

void createTrackbars(){
    //create window for trackbars
    
    String trackbarWindowName = "TRACKBARS";
    namedWindow(trackbarWindowName,0);
    //create memory to store trackbar name on window
    char TrackbarName[50];
    sprintf( TrackbarName, "B_MIN", B_MIN);
    sprintf( TrackbarName, "B_MAX", B_MAX);
    sprintf( TrackbarName, "G_MIN", G_MIN);
    sprintf( TrackbarName, "G_MAX", G_MAX);
    sprintf( TrackbarName, "R_MIN", R_MIN);
    sprintf( TrackbarName, "R_MAX", R_MAX);
    
    //create trackbars
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
    createTrackbar( "I_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
    createTrackbar( "I_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );
    
    
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
    /*
     // Used just for testing!!!
     Mat img = imread(scan.getbinaryImage(), CV_LOAD_IMAGE_GRAYSCALE);
     if(! img.data )                              // Check for invalid input
     {
     cout <<  "Could not open or find the image" << endl ;
     waitKey(5000);
     return -1;
     }
     /*
     createTrackbars();
     while(1){
     inRange(img, Scalar(B_MIN, G_MIN, R_MIN), Scalar(B_MAX, G_MAX, R_MAX), threshold);
     imshow("img", img);
     imshow("threshold", threshold);
     waitKey(30);
     }
     
     for(int i = 0; i < 120; i++){
     inRange(img, Scalar(B_MIN, G_MIN, R_MIN), Scalar(B_MAX, G_MAX, R_MAX), threshold);
     scan.drawMidpoint(threshold, threshold);
     }
     scan.writeToTxtFile(scan.getmidPoints());
     imshow("Binary", img);
     getchar();
     */
    
    
    //    bool findEdges = true;
    
    int x = 0;
    int y = 0;
    
    // Video Capture object to acquire webcam feed
    VideoCapture capture;
    
    // Open Webcam (Default location is 0)
    capture.open(0);
    
    // Set height and width of capture frame
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
    
    //   capture.set(CV_CAP_PROP_FRAME_WIDTH, 720);
    //   capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    
    //create slider bars for HSV filtering
    createTrackbars();
    
    while(1){
        capture.read(cameraFeed);
        if(!cameraFeed.empty()){
            
            // Convert the cameraFeed into a HSV colorspace
            cvtColor(cameraFeed, HSVFeed, COLOR_BGR2HSV);
            // Filter the HSV image and convert into our threshold cameraFeed
            inRange(cameraFeed, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
            
            
            //            if(findEdges){
            //               findEdges(x, y, threshold, cameraFeed);
            //          }
            //          trackFilteredObject(x,y,threshold,cameraFeed);
            //          drawMidpoint(threshold, cameraFeed);
            imshow("Camera Feed", cameraFeed);
            imshow("threshold", threshold);
            //            CannyThreshold(0, threshold, 0);
            //            printf("B_MIN %d B_MAX %d\n", B_MIN, B_MAX);
            //            printf("G_MIN %d G_MAX %d\n", G_MIN, G_MAX);
            //            printf("R_MIN %d R_MAX %d\n", R_MIN, R_MAX);
            ///            cout << "threshold = \n" << threshold << endl;
        }
        // delay 30 ms so that screen can refresh.
        //  getchar();
        waitKey(30);
    }
    
    
    //    B_MIN 179 B_MAX 256
    //   G_MIN 150 G_MAX 256
    //   R_MIN 239 R_MAX 256
    return 0;
}