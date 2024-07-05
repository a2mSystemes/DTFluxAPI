// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxWebSocket/DTFluxWebsocketClient.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxWebSocket/DTFluxWebsocketClient.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "DTFluxAPILog.h"



// UDTFluxWebSocketClient::~UDTFluxWebSocketClient()
// {
// 	
// }

void UDTFluxWebSocketClient::Initialize()
{
}

// void UDTFluxWebSocketClient::BeginDestroy()
// {
// 	// if(Ws->IsConnected())
// 	// 	Ws->Close();
// 	UObject::BeginDestroy();
// }

bool UDTFluxWebSocketClient::Connect(const FString URL, const int Port)
{
		FString ServerUrl = FString::Printf(TEXT("%s:%i/"), *URL, Port);
	
		UE_LOG(LogDTFluxAPI, Log, TEXT("[DTFluxWebsocketClient %s"), *ServerUrl);
		Ws = FWebSocketsModule::Get().CreateWebSocket(ServerUrl);
		Ws->OnConnected().AddLambda([this]()
		{
			if(IsValid(this) == false) return;
			OnConnectionConnectedInternal();
		});
	
		Ws->OnClosed().AddLambda([this](int32 StatusCode, const FString& Reason, bool bWasClean)
		{
			if(IsValid(this) == false) return;
			OnConnectionClosedInternal(Reason);
		});
	
		Ws->OnConnectionError().AddLambda([this](const FString& Error)
		{
			if(IsValid(this) == false) return;
			OnConnectionErrorInternal(Error);
		});
	
		Ws->OnMessage().AddLambda([this](const FString& MessageString)
		{
			if(IsValid(this) == false) return;
			OnReceivedMessageInternal(MessageString);
		});
		Ws->Connect();
	if(Ws->IsConnected())
	{
		UE_LOG(LogDTFluxAPI, Log, TEXT("Listening"));
		return true;
	}
		
	return false;
}

void UDTFluxWebSocketClient::Disconnect()
{
	Ws->Close();
}

bool UDTFluxWebSocketClient::SendMessage(const FString Message, const bool Broadcast)
{
	if(Ws->IsConnected() == false) return false;
	Ws->Send(Message);
	return true;
}

void UDTFluxWebSocketClient::OnConnectionConnectedInternal() const
{
	OnConnectionConnected.Broadcast();
	UE_LOG(LogDTFluxAPI, Log, TEXT("WebSocketsSubsystem, [Connected]"));
}

void UDTFluxWebSocketClient::OnConnectionErrorInternal(const FString& Error) const
{
	OnConnectionError.Broadcast( Error);
	UE_LOG(LogDTFluxAPI, Error, TEXT("WebSocketsSubsystem, [Error] : %s"), *Error);
}

void UDTFluxWebSocketClient::OnConnectionClosedInternal(const FString& Reason) const
{
	OnConnectionClosed.Broadcast(Reason);
	UE_LOG(LogDTFluxAPI, Log, TEXT("WebSocketsSubsystem, [Closed], Reason : %s"), *Reason);

}

void UDTFluxWebSocketClient::OnReceivedMessageInternal(const FString& Message) const
{
	OnReceivedMessage.Broadcast(Message);
	// UE_LOG(LogDTFluxAPI, Log, TEXT("WebSocketsSubsystem, [Message Reveived], Message : %s"), *Message);

}

bool UDTFluxWebSocketClient::Close() const
{
	if (Ws->IsConnected())
			Ws->Close();
	return Ws->IsConnected();
}

bool UDTFluxWebSocketClient::IsConnected()
{
	return Ws->IsConnected();
}


