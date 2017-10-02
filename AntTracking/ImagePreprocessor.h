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

	const int bgScale = 1024; // Background is scaled by this value, so we can make small adjustments to it all the time
	void createGui();


	int stepUp = 10;	// if background Pixel is darker than  in current image, it will be increased by this
	int stepDown = 1;   // if background Pixel is brighter than  in current image, it will be decreased by this
	int threshold=30;    // if background Pixel and current image differ by more than this, an object is detected
};

