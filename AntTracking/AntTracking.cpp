
#include "stdafx.h"
// Standard include files
#include <opencv2/opencv.hpp>
//#include <opencv2/tracking.hpp>
#include <opencv2/features2d.hpp>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <math.h>

#include "ImagePreprocessor.h"
#include "ContourExtractor.h"
#include "FlannBasedTracker.h"
//#define liveVideo

using namespace cv;
using namespace std;

// set gloabal tracking variables
int diffThreshold = 25;
int minSizeThreshold = 5; //min size of tracked objects (pixels)
int maxSizeThreshold = 25; //max size of tracked objects (pixels)


int camExposure = -6;
int usedBackground = 95;
VideoCapture setupCamera(int &exposure); //prototype
void createControlWindow(VideoCapture &cam);// prototype
int main(int argc, char **argv)
{
	unsigned long frameIdx=0;
	// set up all object needed
	VideoCapture cam = setupCamera(camExposure);
	int width = cam.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = cam.get(CV_CAP_PROP_FRAME_HEIGHT);

	createControlWindow(cam);
	ImagePreprocessor bgSubstractor;
	ContourExtractor  contourExtractor(width, height);
	bgSubstractor.setMask(imread("c:\\anttrack\\mask.png", CV_LOAD_IMAGE_GRAYSCALE));
	FlannBasedTracker tracker;
	tracker.createGui();
//	createControlWindow(cam);
	cv::Mat inputImage;
	cv::Mat debugOutImage;

	/////////initialize bg
	//take a few frames from the beginning of the file until it has stabelized
	cout << "dropping first frames...\n";
	for (int i = 0; i < 4000; i++) {
		cam.grab();
		frameIdx++;
	}

	cam >> inputImage;
	debugOutImage = inputImage.clone(); //allocate  debugout once
	bgSubstractor.setBackground(inputImage);


	while (true) {
		// update settings
		bgSubstractor.threshold = diffThreshold;
		// get image and process data
		cam>>inputImage;
		frameIdx++;
		inputImage.copyTo(debugOutImage); // we use this as a drawing surface for debug out
		bgSubstractor.processImage(inputImage);
		contourExtractor.extractContours(bgSubstractor.threshedOutput);
		tracker.updateWithContours(contourExtractor.contours);
	
		if(contourExtractor.contours.size())cout << "detected "<<contourExtractor.contours.size()<<"objects in frame"<< frameIdx<<"\n";
		/// Show results in a window
		contourExtractor.showContours( debugOutImage);
		namedWindow("debugOut", CV_WINDOW_AUTOSIZE);
		imshow("debugOut", debugOutImage);

		// we need to do this to give opencv time to display the results
		if (cv::waitKey(1) == 27)
			break;
	}
	//startTracking(cam);
}


VideoCapture setupCamera(int &exposure) {


#ifdef liveVideo
	cv::VideoCapture cam(0);

	cam.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
	cam.set(CV_CAP_PROP_FPS, 30);
	cam.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cam.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	cam.set(CV_CAP_PROP_AUTO_EXPOSURE, 0);
	cam.set(CV_CAP_PROP_EXPOSURE, exposure);
	cam.set(CV_CAP_PROP_GAIN, 1);
	cam.set(CV_CAP_PROP_FOCUS, 1);

	if (!cam.isOpened()) {
		std::cerr << "no camera detected" << std::endl;
		std::cin.get();
		return 0;
	}
#else
	cv::VideoCapture cam("c:\\anttrack\\antfeedMJPG2.avi");
#endif
	return cam;
}

//void createControlWindow(int &set1, int &set2, int &set3, VideoCapture &cam) {
void createControlWindow(VideoCapture &cam) {
	cvNamedWindow("control", 1);
	cv::createTrackbar("threshold", "control", &diffThreshold, 50, 0);

#ifdef liveVideo
	//exposure slider
	int a = cv::createTrackbar("exposure", "control", &camExposure, 5, onExposure, &cam);
#endif
	//background adaption slider
//	int b = cv::createTrackbar("background", "control", &usedBackground, 100, onBackground);
	//brightness slider
}

void measureFramerate(VideoCapture &cam) {
	// std::cout << "measuring fps... ";
	// time_t start, end;
	// time(&start);
	// for (int i = 0; i < 64; i++) {
	// 	cam >> frame;
	// }
	// time(&end);
	// std::cout << 64/difftime(end, start) << "fps" << std::endl;
	// std::cout << "press any key to continue..." << std::endl;
	// std::cin.get();
}


void onExposure(int slider_val, void* cam)
{
	((cv::VideoCapture*)cam)->set(CV_CAP_PROP_EXPOSURE, slider_val - 6);
}

void onBackground(int slider_val, void* cam) {}

void onBrightness(int slider_val, void* cam) {}

