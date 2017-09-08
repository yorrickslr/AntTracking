#pragma once
#include <opencv2/opencv.hpp>

class ImagePreprocessor
{
public:
	ImagePreprocessor();
	~ImagePreprocessor();
	void  processImage(cv::Mat& input);
	void setBackground(cv::Mat& image);
	void setMask(cv::Mat& image);

	cv::Mat background; // 32F float, color
	cv::Mat mask;		// 8bit Mono. only movements in regions with pixel color !=0 will be tracked
	cv::Mat diffOutput;		// 8bit Mono.
	cv::Mat threshedOutput;		// 8bit Mono.

	const int bgScale = 1024;
	int stepUp = 10;
	int stepDown = 1;
	int threshold=3;
};

