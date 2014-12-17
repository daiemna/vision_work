// debuging.h

#ifndef DEBUGING_H
#define DEBUGING_H

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

#define DEBUG_LOG printf
#define DEBUG_STREAM cout
#define ERROR_LOG printf

namespace cv_debuging {
	// int viewImage(cv::Mat frame,std::string name){
	// 	cv::namedWindow( name, cv::WINDOW_NORMAL );
	// 	cv::imshow(name,frame);
	// }
} // debuging


#endif