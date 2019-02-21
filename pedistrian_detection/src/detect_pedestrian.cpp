#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;
/* Function Headers */
void detectAndDisplay( Mat frame );
/* Global variables */
// String face_cascade_name = "haarcascade_frontalface_alt.xml";
// String eyes_cascade_name = "../script/pedestrian_classifier/cascade.xml";
// String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
String window_name = "Capture - Face detection";
/* @function main */
int main(int argc,char **argv)
{
    VideoCapture capture;
    Mat frame;
    //-- 1. Load the cascades
    if( !face_cascade.load( argv[1] ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
    if( !eyes_cascade.load( argv[2] ) ){ printf("--(!)Error loading eyes cascade\n"); return -1; };
    //-- 2. Read the video stream
    capture.open( -1 );
    if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }
    while (  capture.read(frame) )
    {
        if( frame.empty() )
        {
            printf(" --(!) No captured frame -- Break!");
            break;
        }
        //-- 3. Apply the classifier to the frame
        detectAndDisplay( frame );
        int c = waitKey(10);
        if( (char)c == 27 ) { break; } // escape
    }
    return 0;
}
/* @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
    std::vector<Rect> faces;
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
    for( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
        Mat faceROI = frame_gray( faces[i] );
    }
    std::vector<Rect> eyes;
    //-- In each face, detect eyes
    eyes_cascade.detectMultiScale( frame_gray, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
    for( size_t j = 0; j < eyes.size(); j++ )
    {
        Point pt1(eyes[j].x, eyes[j].y);
        Point pt2(eyes[j].x + eyes[j].width, eyes[j].y + eyes[j].height);
        rectangle( frame, pt1, pt2, Scalar( 255, 0, 0 ), 4, 8, 0 );
    }
    //-- Show what you got
    imshow( window_name, frame );
}