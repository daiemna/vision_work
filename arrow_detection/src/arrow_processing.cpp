// arrow_processing.cpp

#include "arrow_processing.hpp"


int arrow_detection::preProcessing(Mat frame,Mat pro_image,double thresh=255){
	threshold( frame, pro_image, 1, thresh,THRESH_BINARY);
	return 0;
}

int arrow_detection::segmentation(Mat frame){

}

int arrow_detection::featureExteraction(Mat arrow){

}