// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "zmq.hpp"
#include "GameFramework/Actor.h"
#include "ZMQclient.generated.h"


UCLASS()
class ZEROMQ_API AZMQclient : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZMQclient();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
		FString address;

private:
	zmq::context_t* zmq_context = nullptr;
	zmq::socket_t* zmq_socket = nullptr;
	bool is_connected = false;
	FVector pos;
};
