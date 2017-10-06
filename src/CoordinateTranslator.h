#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

// translates between image and level coordinates
class CoordinateTranslator {
public:
	CoordinateTranslator(); //sets up the transformation
	cv::Mat transformMatrix; 
	cv::Point2f getLevelCoordinates(cv::Point2f &imageCoordinates);

	void updateTransform(int cornerNumber, cv::Point2f &imageCoordinates);
	std::vector<cv::Point2f> levelCorners;
	std::vector<cv::Point2f> imageCorners;

	void mouseEvent(int event, int x, int y, int flags, void* userdata); // used to set coordinates in Image
	int selectedCorner=-1; // for mouse drag&drop

	void drawGui(cv::Mat& image);
};