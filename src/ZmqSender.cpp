#include "ZmqSender.h"



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

void ZmqSender::sendTrackedObjectData(std::vector<FlannBasedTracker::TrackedObject> &trackedObjects)
{
	int nAnts = trackedObjects.size();
	std::vector<Ant> toSend(nAnts);
	for (int i = 0; i < nAnts; i++) {
		toSend[i].x = trackedObjects[i].lastPosition.x;
		toSend[i].y = trackedObjects[i].lastPosition.y;
		toSend[i].id = trackedObjects[i].id;
	}
	
	size_t payloadSize = nAnts*sizeof(Ant)+1; // one byte as a message identifier (and in order to be able to send empty messages)

	zmq::message_t message(payloadSize+1); //one additional byte for a type identifier
	unsigned char* messageData = (unsigned char*)message.data();
	messageData[0]='a'; //identifier

	if(nAnts>0)memcpy(&messageData[1], &toSend[0], nAnts*sizeof(Ant));
	publisher.send(message);
}
