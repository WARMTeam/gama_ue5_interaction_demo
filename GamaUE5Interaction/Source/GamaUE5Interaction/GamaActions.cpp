// Fill out your copyright notice in the Description page of Project Settings.


#include "GamaActions.h"
#include "GamaClient.h"
#include <iostream>
#include "Json.h"
#include "MessageHandler.h"
#include "ExpParameter.h"


// Sets default values
AGamaActions::AGamaActions()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	message_handler = new GamaActionsMessageHandler();
	client = new GamaClient("localhost", 6868, message_handler);

}

// Called when the game starts or when spawned
void AGamaActions::BeginPlay()
{
	Super::BeginPlay();
	client -> connect();
}

// Called every frame
void AGamaActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (first && message_handler -> GetSocketId() > 0)
	{
		FString url = "/home/across/Documents/Gama_Workspace/UnrealTest/models/UnrealTest.gaml";
		FString model = "prey_predator";
		ExpParameter* parameter1 = new ExpParameter("int", "10", "Initial number of preys: ");
		TArray<ExpParameter*> params;
		params.Add(parameter1);
		client -> load(message_handler -> GetSocketId(), url, model, true, false, false, params, "length(prey) < 10");
		first = false;
	}

	// play command
	if(!played && message_handler -> GetExpId() > 0)
	{
		client -> play(message_handler -> GetSocketId(), message_handler -> GetExpId(), false);
		played = true;

	}

	// step/stepBack command
	// expression command
	// if (message_handler -> GetExpId() > 0)
	// {
	// 	client -> step(message_handler -> GetSocketId(), message_handler -> GetExpId(), 2, false);
	// 	client -> expression(message_handler -> GetSocketId(), message_handler -> GetExpId(), "length(prey)");
	// 	// client -> stepBack(message_handler -> GetSocketId(), message_handler -> GetExpId(), 1, false);
	// 	// client -> expression(message_handler -> GetSocketId(), message_handler -> GetExpId(), "length(predator)");
	// }
}
