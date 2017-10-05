#include <zmq.hpp>
#include <iostream>
#include <sstream>

#include "../ant.hpp"

int main(int argc, char *argv[])
{
	zmq::context_t context(1);

	zmq::socket_t subscriber(context, ZMQ_SUB);
	subscriber.connect("tcp://localhost:5556");
	subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

	std::cout << "started listening" << std::endl;


	while (true) {
		std::vector<Ant> ants;
		zmq::message_t message;
		subscriber.recv(&message);
		std::cout << message.size() <<"--"<< sizeof(Ant)<< "\n";
		// check message size and tag
		if (message.size() % sizeof(Ant) == 1) {
			unsigned char* messageData = (unsigned char*)message.data();
			if (messageData[0] == 'a') {
				int nAnts = (message.size() - 1) / sizeof(Ant);
				ants.resize(nAnts); //allocate space for the ants
				if (nAnts > 0) {
					std::memcpy(&ants[0], &messageData[1], nAnts*sizeof(Ant));
				}
			}
		}
		std::cout << ants.size() << std::endl;
	}
	return 0;
}
