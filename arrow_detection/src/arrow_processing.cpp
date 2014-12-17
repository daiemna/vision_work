// arrow_processing.cpp

#include "arrow_processing.hpp"


int arrow_detection::preProcessing(Mat frame,Mat& pro_image,double thresh,Size ksize){
	float th = 0;
	Mat blur_image;
	GaussianBlur( frame, blur_image, ksize, 0, 0 );
	if(!blur_image.data){
		ERROR_LOG("Error in Gaussian Bluring!\n");
		return -1;
	}
	if(thresh == -1){
		int error = otsuThresholding(blur_image,th);
		if(error < 0){
			ERROR_LOG("Error in Otsu's Thresholding!\n");
			return error;
		}
		thresh = th;
	}
	DEBUG_LOG("thresh hold was : %lf\n",(double)th);
	threshold( blur_image, pro_image, 1, (double)th,THRESH_BINARY);
	if(!pro_image.data){
		ERROR_LOG("Error: in preprocessing threshold stage!\n");
	}
	return 0;
}

int arrow_detection::segmentation(Mat frame){

}

int arrow_detection::featureExteraction(Mat arrow){

}

int arrow_detection::otsuThresholding(Mat frame,float& thresh){
	float range[] = {0, 255};
	const float* histRnage = {range};
	int histSize = 256;
	int total = frame.total();
	thresh = -1;
	Mat histogram;
	calcHist(&frame,1,0,Mat(),histogram,1,&histSize,&histRnage,true,false);
	DEBUG_LOG("size of histogram : %ld\n",histogram.total());
	DEBUG_LOG("type of histogram : %d\n", histogram.type());
	// DEBUG_STREAM << histogram << endl;
	if(!histogram.data){
		ERROR_LOG("No histogram calculated!\n");
		return -1;
	}
    int sum = 0;
    for (int i = 1; i < 256; ++i){
    	// DEBUG_LOG("histogram at %d = %f\n",i,histogram.at<float>(i));
        sum += i * histogram.at<float>(i);
    }
    DEBUG_LOG("sum is : %d\n",sum);
    float sumB = 0;
    float wB = 0;
    float wF = 0;
    float mB;
    float mF;
    float max = 0.0;
    float between = 0.0;
    float threshold1 = 0.0;
    float threshold2 = 0.0;

    for (int i = 0; i < 256; ++i) {
        wB += histogram.at<float>(i);
        if (wB == 0)
            continue;
        wF = total - wB;
        if (wF == 0)
            break;
        sumB += i * histogram.at<float>(i);
        mB = sumB / wB;
        mF = (sum - sumB) / wF;
        between = wB * wF * pow(mB - mF, 2);
        if ( between >= max ) {
            threshold1 = i;
            if ( between > max ) {
                threshold2 = i;
            }
            max = between;            
        }
    }

    thresh = ( threshold1 + threshold2 ) / 2.0;
    DEBUG_LOG("the thresh is : %f\n",thresh);
    DEBUG_LOG("the threshold1 : %f\n",threshold1);
    DEBUG_LOG("the threshold2 : %f\n",threshold2);
    return 0;
}