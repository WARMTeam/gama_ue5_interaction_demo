// Fill out your copyright notice in the Description page of Project Settings.


#include "GamaActions.h"
#include "GamaClient.h"
#include "ExpParameter.h"
#include "ObjectHandlerr.h"
#include "Math/Vector.h"
#include "GamaActionsMessageHandler.h"
#include "Common/TcpSocketBuilder.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"


// Sets default values
AGamaActions::AGamaActions()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGamaActions::BeginPlay()
{
	Super::BeginPlay();

	// we move the initializations here because the actors are constructed multiple times
	message_handler = new GamaActionsMessageHandler();
	client = new GamaClient(GAMA_URL, GAMA_SERVER_PORT, message_handler);
	client -> connect();
	
	//ObjHandler = new ObjectHandler();

	// Spawn an instance of ObjectHandlerr in the map in a place far from the objects
	UWorld* CurrentWorld = GetWorld();
	const FVector* Loc = new FVector(-1000, -1000, -1000);
	ObjHandlerr = (AObjectHandlerr*)CurrentWorld->SpawnActor(AObjectHandlerr::StaticClass(), Loc);
}

// Called every frame
void AGamaActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// read the model
	if (first && message_handler -> GetSocketId() > 0)
	{
		FString current_path = FPaths::ProjectDir();
		FString url = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead( *(current_path + "../MobilityModel/models/Grid Model.gaml"));
		FString model = "grid_model";
		client -> load(message_handler -> GetSocketId(), url, model);
		first = false;
	}

	// play command
	if(!played && message_handler -> GetExpId() > 0)
	{
		client -> play(message_handler -> GetSocketId(), message_handler -> GetExpId(), true);
		played = true;
	}

	// Connects to TCP server
	if (!tcp_connected && message_handler -> IsPlaying())
	{
		FIPv4Address IPAddress;
		FIPv4Address::Parse(GAMA_IP, IPAddress);
		FIPv4Endpoint Endpoint(IPAddress, TCP_PORT);

		TcpSocket = FTcpSocketBuilder(TEXT("TcpSocket")).AsReusable().Build();
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		if (TcpSocket -> Connect(*SocketSubsystem->CreateInternetAddr(Endpoint.Address.Value, Endpoint.Port)))
		{
			// Sends a message to gama to confirm it's connected
			FString connection_str = FString("connected\n");
			int32 BytesSent;
			if (TcpSocket -> Send((const uint8*)  TCHAR_TO_ANSI(*connection_str), connection_str.Len(), BytesSent))
			{
				tcp_connected = true;
			}
		}
	}

	

	if (tcp_connected)
	{
		uint32 BufferSize;
		if (TcpSocket -> HasPendingData(BufferSize))
		{
			uint8* DataChunk = new uint8[BufferSize+1];
			int32 BytesRead;
			if (TcpSocket -> Recv(DataChunk, BufferSize, BytesRead) && BytesRead > 0)
			{
				DataChunk[BufferSize] = '\0'; // Hack
				FString message = UTF8_TO_TCHAR(DataChunk);

				UE_LOG(LogTemp, Display, TEXT("%s"), *message);

				TSharedPtr<FJsonObject> MyJson;

				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*message);

				if (FJsonSerializer::Deserialize(Reader, MyJson))
				{
					// The deserialization failed, handle this case
					ObjHandlerr->HandleObject(MyJson, GetWorld());
				}
				// Cleaning the message from everything after the final \n
				//message.RemoveFromEnd("\n");
			}
			delete[] DataChunk;
		}	
	}
}

void AGamaActions::SendChange(FString type, int32 ID)
{
	FString change_msg = FString("{ \"type\": \"") + type + FString("\", \"id\": ") + FString::FromInt(ID) + FString("}\n");
	int32 BytesSent;
	TcpSocket -> Send((const uint8*)  TCHAR_TO_ANSI(*change_msg), change_msg.Len(), BytesSent);
}

