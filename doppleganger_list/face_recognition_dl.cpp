// Face_recognition

#include "face_recognition_dl.hpp"

#define DEBUG false

using namespace face_recognition_dl;

int face_recognition_dl::similarityOrderedList(Mat image, 
vector<Mat>* imageList, 
int (*similarityFunction)(Mat,Mat,float*),
OutputArray sortedIndexList){

	long listLength = imageList->size();
	Mat unsortedList;
	float similarityValue;
	int err_code;
	for(Mat image_2 : *imageList){
		similarityValue = 0.0;
		if((err_code = similarityFunction(image,image_2,&(similarityValue) )) != 0){
			if(DEBUG)
				cerr << "similarityFunction returned: " << err_code << endl;
			return ERROR_SIMILRITY_LIST_FUNCTION;
		}
		unsortedList.push_back(similarityValue);
	}
	Mat sortIdxList;
	sortIdx(unsortedList, sortedIndexList, CV_SORT_EVERY_COLUMN + CV_SORT_DESCENDING);
	// sort(unsortedList, sortedList, CV_SORT_EVERY_COLUMN + CV_SORT_DESCENDING);
	return 0;
}

int face_recognition_dl::relevanceMeasure(Mat image1,
	Mat image2,
	vector<Mat>* imageList,
	int k,
	float* relevance,
	int (*similarityFunction)(Mat,Mat,float*)){

	Mat sortedIndexList1,sortedIndexList2;
	if(similarityOrderedList(image1,imageList,similarityFunction,sortedIndexList1) < 0){
		if(DEBUG)
			cerr << "error computing similarityOrderedList for image 1";
		return ERROR_SIMILRITY_LIST_COMPUTATION;
	}
	if(similarityOrderedList(image2,imageList,similarityFunction,sortedIndexList2) < 0){
		if(DEBUG)
			cerr << "error computing similarityOrderedList for image 2";
		return ERROR_SIMILRITY_LIST_COMPUTATION;
	}

	if(DEBUG)
			cout <<" first image sorted list:" << sortedIndexList1 << endl;
	if(DEBUG)
			cout <<" second image sorted list:" << sortedIndexList2 << endl;

	Mat taw1 = Mat::zeros(1,k,CV_32F);
	Mat taw2 = Mat::zeros(1,k,CV_32F);

	sortIdx(sortedIndexList1,taw1, CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING);
	sortIdx(sortedIndexList2,taw2, CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING);

	taw1 = -taw1+k+1;
	if(DEBUG)
			cout << "taw1" << taw1 << endl;
	taw2 = -taw2+k+1;
	if(DEBUG)
			cout << "taw2" << taw2 << endl;

	Mat R_val(0,0,CV_32F);
	R_val = taw1.mul(taw2);

	if(DEBUG)
			cout << "R_val type :" << R_val.type() << endl;
	if(DEBUG)
			cout << "R_val : " << R_val << endl;

	*relevance = 0;	
	for(int i = 0; i < R_val.rows * R_val.cols ; i++){
		unsigned int val = R_val.at<unsigned int>(i,0);
		if(DEBUG)
			cout << "val : " << val << endl;
		if(val > 0)
			*relevance = *relevance + val;
	}
	// *relevance = *relevance/(k*k);
	return 0;
}