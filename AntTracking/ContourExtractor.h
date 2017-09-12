#pragma once
#include <opencv2/opencv.hpp>

class ContourExtractor
{
public:
	ContourExtractor(int width, int height);
	~ContourExtractor();
	std::vector<cv::Vec4i> hierarchy;
	std::vector<std::vector<cv::Point> > contours;
	void extractContours(cv::Mat& image);
	void showContours(cv::Mat &drawingSurface);
};

