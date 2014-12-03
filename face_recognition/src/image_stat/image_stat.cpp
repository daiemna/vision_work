
#include "image_stat.hpp"

#define DEBUG false

int image_stat::SSIM_window_wise(Mat image_1, Mat image_2,float *ssim){

	int win_dim = WINDOW_DIMENSION;
	Mat ssim_vec =Mat::zeros(int(ceilf(image_1.rows/win_dim))* int(ceilf(image_1.cols/win_dim)),1,image_1.type());
	cout << "ssim_vec size : "<< ssim_vec.total() << endl;
	return 0;
}

int image_stat::SSIM(Mat image_1, Mat image_2,float *ssim){
	Mat gray_1,gray_2,gray_1f,gray_2f;
	if(image_1.channels()==3)
		cvtColor(image_1, gray_1, CV_RGB2GRAY);
	else
		gray_1 = image_1;
	if(image_2.channels()==3)
		cvtColor(image_2, gray_2, CV_RGB2GRAY);
	else
		gray_2 = image_2;

	gray_1.convertTo(gray_1f,CV_32F);
	gray_2.convertTo(gray_2f,CV_32F);

	Mat mean1,std1,mean2,std2;
	Mat mean1f,std1f,mean2f,std2f;
	
	float u1=0, u2=0, sig1=0, sig2=0;
	meanStdDev(gray_1f,mean1,std1);
	meanStdDev(gray_2f,mean2,std2);

	mean1.convertTo(mean1f,CV_32F);
	std1.convertTo(std1f,CV_32F);

	mean2.convertTo(mean2f,CV_32F);
	std2.convertTo(std2f,CV_32F);

	u1 = mean1f.at<float>(0,0);
	sig1 = std1f.at<float>(0,0);
	u2 = mean2f.at<float>(0,0);
	sig2 = std2f.at<float>(0,0);
	float sig12;
	sig12 = sqrt(sig1*sig1 + sig2*sig2);
	// cross_correlation(image_1,image_2,&sig12);
	// sig12 = 1;

	*ssim = (4*u1*u2*sig12) / ((u1*u1 + u2*u2) * (sig1*sig1 + sig2*sig2));

	return 0;
}

int image_stat::mutual_information(Mat image_1,Mat image_2,float *mi){
	if(DEBUG)
		cout << "Using MI"<< endl;
	if( image_1.rows == 0  || image_1.cols == 0){ 
		return ERROR_IMAGE_DATA;
	}
	if(image_1.rows != image_2.rows || image_1.cols != image_2.cols){
		return ERROR_IMAGE_MISALIGNED;
	}
	Mat gray_1,gray_2;
	cvtColor(image_1, gray_1, CV_RGB2GRAY);
	cvtColor(image_2, gray_2, CV_RGB2GRAY);

	/// calcHis Params
	int histSize = 256;
	float range[] = { 0, 255 } ;
	const float* histRange = { range };
	bool uniform = true; 
	bool accumulate = false;

	Mat hist_1,hist_2;

	/// Compute the histograms:
	calcHist( &gray_1, 1, 0, Mat(), hist_1, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &gray_2, 1, 0, Mat(), hist_2, 1, &histSize, &histRange, uniform, accumulate );

	// converting to row vector
	hist_1.reshape(0,1);
	hist_2.reshape(0,1);

	Mat pdf_1(hist_1.rows,hist_1.cols,CV_32F),
	pdf_2(hist_1.rows,hist_1.cols,CV_32F);

	long size_1 = image_1.rows * image_1.cols,
	size_2 = image_2.rows * image_2.cols;

	// Probiblit Density Function(PDF)
	pdf_1 = hist_1/size_1;
	pdf_2 = hist_2/size_2;

	//Joint PDF
	Mat jpdf(hist_1.rows,hist_1.cols,CV_32F);
	jpdf = (hist_1+hist_2)/(size_1+size_2);

	// replace 0 with 1 in all 3 PDF
	// cause as x -> 0 ; log(x) -> -Inf
	Mat thr_pdf_1(hist_1.rows,hist_1.cols,CV_32F),
	thr_pdf_2(hist_1.rows,hist_1.cols,CV_32F),
	thr_jpdf(hist_1.rows,hist_1.cols,CV_32F);

	threshold(pdf_1,thr_pdf_1,0,1,THRESH_BINARY_INV);
	threshold(pdf_2,thr_pdf_2,0,1,THRESH_BINARY_INV);
	threshold(jpdf,thr_jpdf,0,1,THRESH_BINARY_INV);

	thr_pdf_1 = thr_pdf_1 + pdf_1;
	thr_pdf_2 = thr_pdf_2 + pdf_2;
	thr_jpdf = thr_jpdf + jpdf;


	// calculating Entropy
	float E1 = 0.0,E2 = 0.0,E12 = 0.0;
	Mat log_pdf_1,log_pdf_2,log_jpdf;
	Mat E1_mat(1,1,CV_32F),
	E2_mat(1,1,CV_32F),
	E12_mat(1,1,CV_32F);


	cv::log(thr_pdf_1,log_pdf_1);
	reduce(thr_pdf_1.mul(log_pdf_1),E1_mat,0,CV_REDUCE_SUM);
	E1 = -1 * E1_mat.at<float>(0);

	cv::log(thr_pdf_2,log_pdf_2);
	reduce(thr_pdf_2.mul(log_pdf_2),E2_mat,0,CV_REDUCE_SUM);
	E2 = -1 * E2_mat.at<float>(0,0);

	cv::log(thr_jpdf,log_jpdf);
	reduce(thr_jpdf.mul(log_jpdf),E12_mat,0,CV_REDUCE_SUM);
	E12 = -1 * E12_mat.at<float>(0,0);

	*mi =  E1 + E2 - E12;

	return 0;
}
int image_stat::cross_correlation(Mat image_1,Mat image_2,float *corr){
	if(DEBUG)
		cout << "Using cross_correlation"<< endl;
	if( image_1.rows == 0  || image_1.cols == 0){ 
		return ERROR_IMAGE_DATA;
	}
	if(image_1.rows != image_2.rows || image_1.cols != image_2.cols){
		return ERROR_IMAGE_MISALIGNED;
	}
	Mat op;
	matchTemplate(image_1,image_2,op,CV_TM_CCORR);

	*corr = op.at<float>(0,0);

	return 0;
}