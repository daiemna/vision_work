// Face_recognition

#ifndef FACE_RECOGNITION_HPP
#define FACE_RECOGNITION_HPP

#include <opencv2/opencv.hpp>
#include <iostream>

#include "image_stat/image_stat.hpp"

using namespace std;
namespace face_recognition {

	int similarityOrderedList(Mat image, 
	vector<Mat>* imageList,
	vector<int>* sortedIndexList, 
	int (*similarityFunction)(Mat,Mat,float*)){
		//need to improve the addressing of imageList
		long listLength = imageList->size();
		cout << "imageList Size: " << listLength << endl;
		cout << "sortedIndexList Size: " << sortedIndexList->size() << endl;



		return -1;
	}

} // face_recognition


#endif // FACE_RECOGNITION_HPP
