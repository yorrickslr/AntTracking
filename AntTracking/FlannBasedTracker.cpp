#include "stdafx.h"
#include "FlannBasedTracker.h"

void FlannBasedTracker::updateWithContours(std::vector<std::vector<cv::Point> > &contours)
{
	// get internal values from slider inputs
	double minSize = (double)minSizeInput / 10.0;
	double maxSize = (double)maxSizeInput / 10.0;
	double maxJump = (double)maxJumpInput / 10.0;


	std::vector<cv::Point2f> newContourCenters;
	//////////Step 1: filter Blobs by size and extract center

	//extract center of Mass
	/// Get the moments of the contours
	// first kick out everything that is too small or too big, then
	// calculate their center of mass from them
	for (int i = 0; i < contours.size(); i++)
	{
		// we do this using the "moments" of the contours
		cv::Moments mu = moments(contours[i], true);
		double size = mu.m00;
		if (size >= minSize&&size <= maxSize) {
			newContourCenters.push_back(cv::Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00));
		}
	}

	////////Step 2: try to find existing objects close to the detected blobs 
	
	std::vector<int> contourHasMatched; // flag: did the contour match to a known object?
	contourHasMatched.resize(newContourCenters.size());

	if (trackedObjects.size() > 0 && newContourCenters.size() > 0) {
		/////we use FLANN for the matching, so we have to convert the positions into "feature matrices" first
		// prepare matrices for old+new Positions. We will use them as "features" for the point matching
		cv::Mat oldPointFeatures(trackedObjects.size(), 3, CV_32F);
		for (int i = 0; i < trackedObjects.size(); i++) {
			oldPointFeatures.at<float>(i, 0) = trackedObjects[i].lastPosition.x;
			oldPointFeatures.at<float>(i, 1) = trackedObjects[i].lastPosition.y;
			// a little hack: established 
			oldPointFeatures.at<float>(i, 2) = trackedObjects[i].seenInCycleCount > cyclesTillObjectIsEstablished ? bonusForEstablishedObjects : 0;
		}

		cv::Mat newPointFeatures(newContourCenters.size(), 3, CV_32F);
		for (int i = 0; i < newContourCenters.size(); i++) {
			newPointFeatures.at<float>(i, 0) = newContourCenters[i].x;
			newPointFeatures.at<float>(i, 1) = newContourCenters[i].y;
			newPointFeatures.at<float>(i, 2) =  bonusForEstablishedObjects;
		}
		// Use FLANN to match the two sets of points
		cv::FlannBasedMatcher matcher;
		std::vector< cv::DMatch > matches;
		matcher.match(newPointFeatures, oldPointFeatures, matches); //new = "query", old= "train"

		// update all tracked objects that have been matched with a contour in the current image
		for (int matchIdx = 0; matchIdx < matches.size(); matchIdx++) {
			cv::DMatch &curMatch = matches[matchIdx];
			if (curMatch.distance < maxJump) {
				trackedObjects[curMatch.trainIdx].lastSeenInCycle = curCycle;
				trackedObjects[curMatch.trainIdx].seenInCycleCount++;
				contourHasMatched[curMatch.queryIdx] = 1;
				trackedObjects[curMatch.trainIdx].lastMove = (newContourCenters[curMatch.queryIdx] - trackedObjects[curMatch.trainIdx].lastPosition);
				trackedObjects[curMatch.trainIdx].lastPosition = newContourCenters[curMatch.queryIdx];
			}
		}
	}
	// add all countours that have not matched as new objects
	for (int i = 0; i < newContourCenters.size(); i++) {
		if (contourHasMatched[i] == 0) {
			TrackedObject newObject;
			newObject.lastSeenInCycle = curCycle;
			newObject.lastPosition = newContourCenters[i];
			newObject.id = nextObjectId;
			trackedObjects.push_back(newObject);
			nextObjectId++;
		}
	}
	// remove Objects that have not been seen for a while
	// this would be the fancy way
//	trackedObjects.erase(std::remove_if(trackedObjects.begin(), trackedObjects.end(), [](TrackedObject o) {(curCycle - o.lastSeenInCycle) > noSeeCyclesTillDiscard}))
	int nObjectsLeft = trackedObjects.size();
	for (int i = 0; i < nObjectsLeft;) {
		// if object hasn't been seen, replace it by the last in the list
		if ((curCycle - trackedObjects[i].lastSeenInCycle)>noSeeCyclesTillDiscard) {
			trackedObjects[i] = trackedObjects[nObjectsLeft - 1];
			nObjectsLeft--;
		}
		else {
			i++;
		}
	}
	trackedObjects.resize(nObjectsLeft);
	curCycle++;
}
void FlannBasedTracker::drawDebugOut(cv::Mat & image)
{
	for (int i = 0; i < trackedObjects.size(); i++) {
		std::string label =std::to_string(trackedObjects[i].id);
		
		cv::putText(image, label, trackedObjects[i].lastPosition, CV_FONT_HERSHEY_SIMPLEX, 1, cvScalar(0, 1, 0));
	}
}
;

void FlannBasedTracker::createGui()
{
	cv::createTrackbar("minSize", "control", &minSizeInput, 50, 0);
	cv::createTrackbar("maxSize", "control", &maxSizeInput, 500, 0);
	cv::createTrackbar("maxJump", "control", &maxJumpInput, 500, 0);

	cv::createTrackbar("cycles till old", "control", &cyclesTillObjectIsEstablished, 50);
	cv::createTrackbar("bonus for old", "control", &bonusForEstablishedObjects, 100);
	
}
