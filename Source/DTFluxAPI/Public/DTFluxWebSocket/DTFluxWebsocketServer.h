// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "UObject/Object.h"
// #include "IWebSocketServer.h"
// #include "DTFluxWebsocketServer.generated.h"
//
//
//
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionConnected, const FString&, Channel);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConnectionClosed, const FString&, Channel, const FString&, Reason);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConnectionError, const FString&, Channel, const FString&, Error);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReceivedMessage, const FString&, Channel, const FString&, Message);
// class INetworkingWebSocket;
//
// UCLASS(Blueprintable, Category="DTFlux|Websocket")
// class DTFLUXAPI_API UDTFluxWebsocketServer : public UObject
// {
// 	GENERATED_BODY()
//
// public:
//
// 	UDTFluxWebsocketServer();
//
//
// 	
// 	void Initialize(const FString& Channel)
// 	{
// 		ChannelName = Channel;
// 	};
// 	
// 	virtual ~UDTFluxWebsocketServer() override;
// 	
// 	UFUNCTION(BlueprintCallable, Category="DTFlux|Websocket")
// 	bool ConnectToChannel(const FString URL, const int Port );
//
// 	UFUNCTION(BlueprintCallable, Category="DTFlux|Websocket")
// 	void LeaveChannel();
//
// 	UFUNCTION(BlueprintCallable, Category="DTFlux|Websocket")
// 	bool SendMessageToAll(const FString Message);
//
// 	UPROPERTY(BlueprintAssignable, Category="DTFlux|Websocket")
// 	FOnConnectionConnected OnConnectionConnected;
// 	
// 	UPROPERTY(BlueprintAssignable, Category="DTFlux|Websocket")
// 	FOnConnectionClosed OnConnectionClosed;
// 	
// 	UPROPERTY(BlueprintAssignable, Category="DTFlux|Websocket")
// 	FOnConnectionError OnConnectionError;
// 	
// 	UPROPERTY(BlueprintAssignable, Category="DTFlux|Websocket")
// 	FOnReceivedMessage OnReceivedMessage;
//
// 	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Websocket")
// 	FString ChannelName;
//
// 	void Close();
//
//
// protected:
//
//
// 	
// private:
//
// 	bool bIsConnected = false;
// 	void OnConnectionConnectedInternal(INetworkingWebSocket* ClientWebSocket);
// 	void OnConnectionErrorInternal();
// 	// void OnConnectionClosedInternal(const FString& Reason);
// 	void OnReceivedMessageInternal(void* Data, int32 Count);
//
// 	static TUniquePtr<IWebSocketServer> GetServer();
//
// 	TArray<INetworkingWebSocket*> Clients;
// 	// TSharedPtr<IWebSocket> Ws;
// 	TUniquePtr<IWebSocketServer> Ws;
// };

#pragma once

#include "IWebSocketServer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DTFluxWebsocketServer.generated.h"



UCLASS(Blueprintable, Category="DTFlux|Websocket")
class DTFLUXAPI_API UDTFluxServerWebSocket : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnectionConnected);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionClosed, const FString&, Reason);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionError, const FString&, Error);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceivedMessage, const FString&, Message);


	UDTFluxServerWebSocket();
	virtual ~UDTFluxServerWebSocket() override;

	void Init(const int& Port, const FString& Url = TEXT("ws://localhost"));
	virtual void BeginDestroy() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJsonRecieved, const FString&, Payload);

	UPROPERTY(BlueprintAssignable)
	FOnJsonRecieved OnJsonRecieved;
	void Close()
	{
		ServerWebSocket = nullptr;

	};

protected:

	void OnWebSocketClientConnected(INetworkingWebSocket* ClientWebSocket); // to the server.

	virtual void ReceivedRawPacket(void* Data, int32 Count);


private:

	TUniquePtr<class IWebSocketServer> ServerWebSocket;
	TMap<FGuid,INetworkingWebSocket*> ConnectedClients; 
	/** Delegate for callbacks to GameThreadTick */
	FTSTicker::FDelegateHandle TickHandle;
};
