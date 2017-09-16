#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
class FlannBasedTracker {
public:
	class TrackedObject {
	public:
		unsigned long id;
		cv::Point2f lastPosition;
		cv::Point2f lastMove;
		unsigned long lastSeenInCycle;
	};
	int minSizeInput = 20; // multiplied by ten for trackbar compatibility
	int maxSizeInput = 200;// multiplied by ten for trackbar compatibility
	int maxJumpInput = 30;// multiplied by ten for trackbar compatibility
	int noSeeCyclesTillDiscard = 5;
	unsigned long curCycle = 0;
	unsigned long nextObjectId = 0; // for assigning unique ids to objects
	std::vector<TrackedObject> trackedObjects;

	void createGui();
	void updateWithContours(std::vector<std::vector<cv::Point> > &contours);
	void drawDebugOut(cv::Mat& image);
};