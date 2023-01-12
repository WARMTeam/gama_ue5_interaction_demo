// Fill out your copyright notice in the Description page of Project Settings.


#include "GamaActionsMessageHandler.h"
#include "jsoncpp/json/value.h"
#include "jsoncpp/json/json.h"

GamaActionsMessageHandler::GamaActionsMessageHandler()
{
}

void GamaActionsMessageHandler::HandleConnectionSuccessful(TSharedPtr<FJsonObject> MyJson)
{
    if(MyJson -> TryGetNumberField("content", socket_id))
    {
        socket_id = MyJson -> GetIntegerField("content");
        // UE_LOG(LogTemp, Display, TEXT("socket_id extracted"));
        // UE_LOG(LogTemp, Display, TEXT("%s"), *FString(std::to_string(socket_id).c_str()));
    }
}

void GamaActionsMessageHandler::HandleCommandExecutedSuccessfully(TSharedPtr<FJsonObject> MyJson)
{
    const TSharedPtr<FJsonObject>* Content;
    
    if (MyJson -> TryGetObjectField("content", Content))
    {
        exp_id = (*Content) -> GetIntegerField("exp_id");
        // UE_LOG(LogTemp, Display, TEXT("exp_id extracted"));
        // UE_LOG(LogTemp, Display, TEXT("%s"), *FString(std::to_string(exp_id).c_str()));
    }
}

GamaActionsMessageHandler::~GamaActionsMessageHandler()
{
}