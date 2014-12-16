// main.cpp


#include <opencv2/opencv.hpp>
#include <stdio.h>

#include "arrow_processing.hpp"

using namespace arrow_detection;
using namespace cv;
#define DEBUG_LOG printf
int main(int argc,char* argv[]){
	Mat image;
	DEBUG_LOG("arrow_detection main!\n");
	preProcessing(image);
}