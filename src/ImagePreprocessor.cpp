#include "ImagePreprocessor.h"


ImagePreprocessor::ImagePreprocessor()
{
	threshold = 30;
}


ImagePreprocessor::~ImagePreprocessor()
{
}

void ImagePreprocessor::processImage(cv::Mat &input)
{
	int inWidth = input.cols;
	int inHeight = input.rows;

	if (input.rows != background.rows || input.cols != background.cols) {
		background= cv::Mat::zeros(cv::Size(input.cols, input.rows), CV_32SC3);
		diffOutput = cv::Mat::zeros(cv::Size(input.cols, input.rows), CV_8UC1);
		threshedOutput== cv::Mat::zeros(cv::Size(input.cols, input.rows), CV_8UC1);
	}

	// uses a median based BG adjust algo
	unsigned char* inData = input.data;
	int32_t* bgData = (int32_t*)background.data;
	unsigned char* diffOutData = diffOutput.data;

	for (int i = 0; i < inWidth*inHeight;i++) {
		// this will make the background converge againt a quantile of the input values
		int32_t diffSum=0;
		for (int c = 0; c < 3; c++) {
			int32_t scaledInput = (*inData) *bgScale;
			diffSum += (int32_t)*bgData - (int32_t)scaledInput;
			if (*bgData < scaledInput) {
				*bgData += stepUp;
			}
			else {
				*bgData -= stepDown;
			}
			bgData++; // proceed to next Value 
			inData++;
		}
		if(diffSum>0)*diffOutData = diffSum /(3 * bgScale);
		diffOutData++;// proceed to next Value
	}
	cv::threshold(diffOutput, threshedOutput, threshold,255,CV_THRESH_BINARY);
	cv::bitwise_and(threshedOutput, mask, threshedOutput);
}

void ImagePreprocessor::setBackground(cv::Mat & input)
{
	if (input.rows != background.rows || input.cols != background.cols) {
		background = cv::Mat::zeros(cv::Size(input.cols, input.rows), CV_32SC3);
		diffOutput = cv::Mat::zeros(cv::Size(input.cols, input.rows), CV_8UC1);
		threshedOutput == cv::Mat::zeros(cv::Size(input.cols, input.rows), CV_8UC1);
	}
	int inWidth = input.cols;
	int inHeight = input.rows;
	unsigned char* inData = input.data;
	int32_t* bgData = (int32_t*)background.data;

	for (int i = 0; i < inWidth*inHeight * 3; i++) {
		*bgData = (*inData)*bgScale;
		bgData++;
		inData++;
	}
}

void ImagePreprocessor::setMask(cv::Mat & image)
{
	mask = image;
}

void ImagePreprocessor::createGui()
{
	cv::createTrackbar("threshold", "control", &threshold, 50);
}
