#include <zmq.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <iostream>

#include "../ant.hpp"

int main() {

	//  Prepare our context and publisher
	zmq::context_t context(1);
	zmq::socket_t publisher(context, ZMQ_PUB);
	publisher.bind("tcp://*:5556");

	float c = 100;

	while (true) {
		Sleep(10);
		std::cout << "sending " << c << std::endl;
		zmq::message_t message(sizeof(float));

		std::memcpy(message.data(), &c, sizeof(float));

		publisher.send(message);

		c += 0.1f;
		if (c > 200) c = 1;
	}
	return 0;
}
