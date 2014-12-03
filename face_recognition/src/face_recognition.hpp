// Face_recognition

#ifndef FACE_RECOGNITION_HPP
#define FACE_RECOGNITION_HPP

#include <opencv2/opencv.hpp>
#include <iostream>

#include "image_stat/image_stat.hpp"
// #include "debuging.h"

using namespace std;
using namespace cv;

namespace face_recognition {
	#define ERROR_SIMILRITY_LIST_FUNCTION -31;
	#define ERROR_SIMILRITY_LIST_COMPUTATION -33;

	int similarityOrderedList(Mat image, 
	vector<Mat>* imageList, 
	int (*similarityFunction)(Mat,Mat,float*),
	OutputArray sortedIndexList);

	int relevanceMeasure(Mat image1,
	Mat image2,
	vector<Mat>* imageList,
	int k,
	float* relevance,
	int (*similarityFunction)(Mat,Mat,float*));

} // face_recognition


#endif // FACE_RECOGNITION_HPP
