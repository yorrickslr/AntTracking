#include "CoordinateTranslator.h"

CoordinateTranslator::CoordinateTranslator()
{

	//add initial corner positions, clockwise, beginning at upper left corner
	levelCorners.push_back(cv::Point2f(0, 0));
	levelCorners.push_back(cv::Point2f(10, 0));
	levelCorners.push_back(cv::Point2f(10, 10));
	levelCorners.push_back(cv::Point2f(0, 10));

	imageCorners.push_back(cv::Point2f(461, 146));
	imageCorners.push_back(cv::Point2f(829, 104));
	imageCorners.push_back(cv::Point2f(860, 414));
	imageCorners.push_back(cv::Point2f(491, 446));

	transformMatrix = cv::findHomography(imageCorners, levelCorners, 0); // initialize perspective transform
}

cv::Point2f CoordinateTranslator::getLevelCoordinates(cv::Point2f & imageCoordinates)
{
	std::vector<cv::Point2f> inputPoints;
	std::vector<cv::Point2f> outputPoints;
	inputPoints.push_back(imageCoordinates);
	cv::perspectiveTransform(inputPoints, outputPoints, transformMatrix);
	return outputPoints[0];
}

void CoordinateTranslator::updateTransform(int cornerNumber, cv::Point2f & imageCoordinates)
{
	imageCorners[cornerNumber] = imageCoordinates;
	transformMatrix = cv::findHomography(imageCorners, levelCorners, 0); // initialize with a non-transform
}

void CoordinateTranslator::mouseEvent(int event, int x, int y, int flags, void * userdata)
{
	// select corner by clicking close to it and prepare it for drag&drop
	if (event == cv::EVENT_LBUTTONDOWN) {
		//find closest corner
		float minDist = FLT_MAX;
		int selected = -1;
		for (int i = 0; i < 4; i++) {
			float dist = (imageCorners[i].x - (float)x)*(imageCorners[i].x - (float)x) + (imageCorners[i].y - (float)y)*(imageCorners[i].y - (float)y);
			if (dist < minDist) {
				selected = i;
				minDist = dist;
			}
		}
		// close enough?
		if (minDist < 200) {
			selectedCorner = selected;
		}
		else {
			selectedCorner= - 1;
		}
	}
	// allow to move corner
	if (event == cv::EVENT_MOUSEMOVE) {
		cv::Point2f mousePos = getLevelCoordinates(cv::Point2f(x, y));
		//std::cout << "mouse in Level: x:" << mousePos.x << " y:"<<mousePos.y << "\n";
		if (selectedCorner != -1) {
			imageCorners[selectedCorner].x = x;
			imageCorners[selectedCorner].y = y;
		}
	}

	// drop corner when lifting button
	if (event == cv::EVENT_LBUTTONUP) {
		if (selectedCorner != -1) {
			imageCorners[selectedCorner].x = x;
			imageCorners[selectedCorner].y = y;
			transformMatrix = cv::findHomography(imageCorners, levelCorners, 0); // initialize with a non-transform
			std::cout << "dropped Corner " << selectedCorner << " to x:" << x << " y:" << y << "\n";
			selectedCorner = -1;

		}
	}
}

void CoordinateTranslator::drawGui(cv::Mat & image)
{
	for (int i = 0; i < 4; i++) {
		cv::circle(image, imageCorners[i], 3, cv::Scalar(i==selectedCorner? 255:0, i == selectedCorner ? 0 : 255, 0), 2);
	}
}
