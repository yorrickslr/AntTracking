#include "stdafx.h"
#include "FlannBasedTracker.h"

void FlannBasedTracker::updateWithContours(std::vector<std::vector<cv::Point> > &contours)
{
	std::vector<cv::Point2f> newContourCenters(contours.size());

	//extract center of Mass
	/// Get the moments of the contours and calculate their center of mass from them
	for (int i = 0; i < contours.size(); i++)
	{
		// we do this using the "moments" of the contours
		cv::Moments mu = moments(contours[i], false);
		newContourCenters[i] = cv::Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
	}

	// prepare matrices for old+new Positions. We will use them as "features" for the point matching
	cv::Mat oldPointFeatures(trackedObjects.size(), 2, CV_32F);
	for (int i = 0; i < trackedObjects.size(); i++) {
		oldPointFeatures.at<float>(i, 0) = trackedObjects[i].lastPosition.x;
		oldPointFeatures.at<float>(i, 1) = trackedObjects[i].lastPosition.y;
	}

	cv::Mat newPointFeatures(newContourCenters.size(), 2, CV_32F);
	for (int i = 0; i < contours.size(); i++) {
		newPointFeatures.at<float>(i, 0) = newContourCenters[i].x;
		newPointFeatures.at<float>(i, 1) = newContourCenters[i].y;
	}
	// Use FLANN to match the two sets of points
	cv::FlannBasedMatcher matcher;
	std::vector< cv::DMatch > matches;
	matcher.match(oldPointFeatures, newPointFeatures, matches);

};
