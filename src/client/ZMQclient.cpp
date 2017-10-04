// Fill out your copyright notice in the Description page of Project Settings.

#include "ZeroMQ.h"
#include "ZMQclient.h"

// Sets default values
AZMQclient::AZMQclient()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AZMQclient::BeginPlay()
{
	Super::BeginPlay();
	
	// initialize ZeroMQ
	zmq_context = new zmq::context_t(1);
	zmq_socket = new zmq::socket_t(*zmq_context, ZMQ_SUB);

	try {
		//zmq_socket->connect(temp.c_str());
		uint32_t hwm = 1;
		zmq_socket->connect(TCHAR_TO_ANSI(*address));
		zmq_socket->setsockopt(ZMQ_SUBSCRIBE, "", 0);
		is_connected = true;
		UE_LOG(LogTemp, Log, TEXT("successfully bound socket"));
	}
	catch (std::exception e) {
		//throw std::logic_error("cannot bind server to address");
		UE_LOG(LogTemp, Error, TEXT("cannot bind socket to address '%s'!"), *address);
	}
	zmq::message_t message(sizeof(int));
}

// Called every frame
void AZMQclient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (is_connected) {
		float c = 0.0f;
		zmq::message_t message(sizeof(float));

		if (zmq_socket->recv(&message, ZMQ_NOBLOCK)) {
			std::memcpy(&c, message.data(), sizeof(float));
			UE_LOG(LogTemp, Log, TEXT("%f"), c);
			pos.Z = (float) c;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("setting pos to %s"), *pos.ToString());
	SetActorLocation(pos, false);
}

