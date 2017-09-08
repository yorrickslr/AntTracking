#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
class FlannBasedTracker {
public:
	class TrackedObject {
	public:
		cv::Point2f lastPosition;
		cv::Point2f lastMove;
		unsigned long lastSeenInCycle;
	};
	std::vector<TrackedObject> trackedObjects;
	void updateWithContours(std::vector<std::vector<cv::Point> > &contours);
};