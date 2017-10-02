#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

/// FlannBasedTracker converts contours into tracked object positions. it tries to preserve identity

class FlannBasedTracker {
public:
	class TrackedObject {
	public:
		unsigned long id;
		cv::Point2f lastPosition;
		cv::Point2f lastMove;
		unsigned long lastSeenInCycle;  // when was it seen last time?
		unsigned long seenInCycleCount=0; // how many times has it been seen so far?
	};
	int minSizeInput = 20; // multiplied by ten for trackbar compatibility
	int maxSizeInput = 200;// multiplied by ten for trackbar compatibility
	int maxJumpInput = 30;// multiplied by ten for trackbar compatibility
	int noSeeCyclesTillDiscard = 50;
	
	int cyclesTillObjectIsEstablished = 5; // if an object is this old, it will be treated preferentially in matching
	int bonusForEstablishedObjects = 100;  // bonus in matching, should be in the range of pixel coordinates

	unsigned long curCycle = 0;
	unsigned long nextObjectId = 0; // for assigning unique ids to objects
	std::vector<TrackedObject> trackedObjects;

	void createGui();
	void updateWithContours(std::vector<std::vector<cv::Point> > &contours);
	void drawDebugOut(cv::Mat& image);
};