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

		zmq::message_t message(sizeof(float));
		float value;

		subscriber.recv(&message);

		std::memcpy(&value, message.data(), sizeof(float));

		std::cout << value << std::endl;
	}
	return 0;
}
