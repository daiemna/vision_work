// debuging.cpp
#include "debuging.hpp"

int cv_debuging::viewImage(Mat frame,string name){
	namedWindow( name, WINDOW_NORMAL );
	imshow(name,frame);
	return 0;
}

int cv_debuging::printMinMax(Mat image){
	double minVal=0,maxVal=0;
    // binary.convertTo(image, CV_32SC1);
    minMaxLoc(image,&minVal,&maxVal);
    DEBUG_LOG("[min,max] : [%lf,%lf]\n",minVal,maxVal);
}