#include "ZmqSender.h"
#include <opencv2/opencv.hpp>


ZmqSender::ZmqSender():
	context(1),
	publisher(context, ZMQ_PUB)
{
	//  Prepare our context and publisher
	publisher.bind("tcp://*:5556");
}


ZmqSender::~ZmqSender()
{
}

void ZmqSender::sendTrackedObjectData(std::vector<FlannBasedTracker::TrackedObject> &trackedObjects, CoordinateTranslator &transformer)
{
	int nAnts = trackedObjects.size();
	std::vector<Ant> toSend(nAnts);
	for (int i = 0; i < nAnts; i++) {
		//transform to level coordinates:
		cv::Point2f transformedPoint = transformer.getLevelCoordinates(trackedObjects[i].lastPosition);

		toSend[i].x = transformedPoint.x;
		toSend[i].y = transformedPoint.y;
		toSend[i].id = trackedObjects[i].id;
	}
	
	zmq::message_t message(nAnts*sizeof(Ant) + 1); //one additional byte for a type identifier
	unsigned char* messageData = (unsigned char*)message.data();
	messageData[0]='a'; //identifier

	if(nAnts>0)memcpy(&messageData[1], &toSend[0], nAnts*sizeof(Ant));
	//std::cout << message.size() << "--" << sizeof(Ant) << "\n";
	publisher.send(message);
}
