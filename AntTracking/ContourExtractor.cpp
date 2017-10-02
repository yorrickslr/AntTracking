#include "stdafx.h"
#include "ContourExtractor.h"


ContourExtractor::ContourExtractor(int width, int height)
{
}


ContourExtractor::~ContourExtractor()
{
}


void  ContourExtractor::extractContours(cv::Mat & inputImage)
{
	//contours= std::vector<std::vector<cv::Point>>(); // reset contour storage
	cv::findContours(inputImage, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));

}

void ContourExtractor::showContours(cv::Mat &drawingSurface)
{
	if (debugOutLevel > 0) {
		/// Draw contours
		for (int i = 0; i < contours.size(); i++)
		{
			cv::Scalar color(0, 0, 255);
			drawContours(drawingSurface, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
		}
	}

}

void ContourExtractor::createGui()
{
	cv::createTrackbar("DBGContour", "control", &debugOutLevel, 3);
}
