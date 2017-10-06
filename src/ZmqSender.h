#pragma once
#include"zmq.hpp"
#include "ant.hpp"
#include "FlannBasedTracker.h"
#include "CoordinateTranslator.h"
class ZmqSender
{
public:
	//  Prepare our context and publisher
	zmq::context_t context;
	zmq::socket_t publisher;
	ZmqSender();
	~ZmqSender();
	void sendTrackedObjectData(std::vector<FlannBasedTracker::TrackedObject> &trackedObjects, CoordinateTranslator &transformer);
};

