// arrow_processing.cpp

#include "arrow_processing.hpp"


int arrow_detection::preProcessing(Mat frame,Mat& pro_image,double thresh,Size filterSize){
	double th = 0;
	Mat blur_image = frame;
	// GaussianBlur( frame, blur_image, filterSize, 0, 0 );

	// DEBUG_LOG("Gaussian blur min, max : ");
	// printMinMax(blur_image);
	// blur_image = frame;
	// viewImage(blur_image,"blured image");
	if(!blur_image.data){
		ERROR_LOG("Error in Gaussian Bluring!\n");
		return -1;
	}
	if(thresh == -1){
		threshold( blur_image, pro_image, 0, 255,THRESH_BINARY_INV | THRESH_OTSU);
		th = 255;
	}else{
		// DEBUG_LOG("thresh hold was : %lf\n",(double)th);
		threshold( blur_image, pro_image, 1, thresh,THRESH_BINARY_INV);
        th = thresh;
	}
	
	// DEBUG_LOG("thresholded image min, max : ");
	// printMinMax(pro_image);

	pro_image = pro_image / th;
    // viewImage(pro_image*th,"Binary Image");
	if(!pro_image.data){
		ERROR_LOG("Error: in preprocessing threshold stage!\n");
	}
	return 0;
}

int arrow_detection::segmentation(Mat binary_image,vector<Rect> &blobs_roi, vector <vector<Point2i>> &blobs){
    int image_area = (float(3500)/float(1080*760)) * binary_image.total();
    // DEBUG_LOG("Minimum blob area: %d\n",image_area);
	findBlobs(binary_image, blobs,image_area);
	blobs_roi.clear();
	for(size_t i=0; i < blobs.size(); i++) {
		int min_x,min_y,max_x,max_y;
		max_x = max_y = 0;
		min_x = binary_image.cols;
		min_y = binary_image.rows;
        for(size_t j=0; j < blobs[i].size(); j++) {
            int x = blobs[i][j].x;
            int y = blobs[i][j].y;
            if(x < min_x){
            	min_x = x;
            }
            if (x > max_x){
            	max_x = x;
            }
            if(y < min_y){
            	min_y = y;
            }
            if(y > max_y){
            	max_y = y;
            }
        }
        blobs_roi.push_back(Rect(min_x,min_y,abs(min_x-max_x),abs(min_y - max_y)));
    }
    return 0;
}

int arrow_detection::featureExteraction(Mat arrow,vector<float> &features){
    features.clear();
    Mat binary;
    vector <std::vector<cv::Point2i >> blobs;
    vector<Rect> blobs_roi;

    preProcessing(arrow, binary);
    // viewImage(binary*255,"binary arrow");
    segmentation(binary,blobs_roi, blobs);
    // DEBUG_LOG("Blobs attained : %ld\n",blobs_roi.size());
    if(blobs_roi.size() > 1){
        // ERROR_LOG("More than one blobs detected possibly an error.");
        return -1;
    }
    float convex_area = blobs[0].size();
    features.push_back(convex_area);
    // DEBUG_LOG("convex_area : %f\n",convex_area);

    Rect arrow_rect(blobs_roi[0]);
    float a = 0,b = 0;
    if(arrow_rect.width > arrow_rect.height){
        a = arrow_rect.width/2;
        b = arrow_rect.height/2;
    }else{
        a = arrow_rect.height/2;
        b = arrow_rect.width/2;
    }
    
    // DEBUG_LOG("semi major axis: %f\n",a);
    // DEBUG_LOG("semi minor axis: %f\n",b);
    // DEBUG_LOG("their ratio: %f\n",b/a);
    // DEBUG_LOG("pow(b/a,2) : %f\n",pow(b/a,2));
    float ecentricity = sqrtf(1-pow(b/a,2));
    features.push_back(ecentricity);
    // DEBUG_LOG("ecentricity : %f\n",ecentricity);

    float extent = convex_area/float(arrow.total());
    features.push_back(extent);
    // DEBUG_LOG("extent : %f\n",extent);

    float solidity = float(arrow.total())/convex_area;
    features.push_back(extent);
    // DEBUG_LOG("solidity : %f\n",solidity);

    return 0;
}
int arrow_detection::findBlobs(const Mat &binary, vector <vector<Point2i>> &blobs,int min_pixel_count){
    blobs.clear();
    // DEBUG_LOG("Inside findBlobs!\n");
    // Fill the label_image with the blobs
    // 0  - background
    // 1  - unlabelled foreground
    // 2+ - labelled foreground
    // DEBUG_LOG(" min_pixel_count : %d\n",min_pixel_count);
    cv::Mat label_image;
    binary.convertTo(label_image, CV_32SC1);
    // DEBUG_LOG("CV_32SC1 image min,max :");
    // printMinMax(label_image);
    
    int label_count = 2; // starts at 2 because 0,1 are used already

    for(int y=0; y < label_image.rows; y++) {
        int *row = (int*)label_image.ptr(y);
        for(int x=0; x < label_image.cols; x++) {
            if(row[x] != 1) {
                continue;
            }

            cv::Rect rect;
            cv::floodFill(label_image, cv::Point(x,y), label_count, &rect, 0, 0, 4);

            std::vector <cv::Point2i> blob;

            for(int i=rect.y; i < (rect.y+rect.height); i++) {
                int *row2 = (int*)label_image.ptr(i);
                for(int j=rect.x; j < (rect.x+rect.width); j++) {
                    if(row2[j] != label_count) {
                        continue;
                    }

                    blob.push_back(cv::Point2i(j,i));
                }
            }
            if(blob.size() <= min_pixel_count){
            	continue;
            }
            blobs.push_back(blob);

            label_count++;
        }
    }
    return 0;
}