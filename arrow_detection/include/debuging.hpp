// debuging.h

#ifndef DEBUGING_H
#define DEBUGING_H

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define DEBUG_LOG printf
#define DEBUG_STREAM cout
#define ERROR_LOG printf

namespace cv_debuging {
	int viewImage(Mat frame,string name);
	int printMinMax(Mat image);
} // debuging

#endif