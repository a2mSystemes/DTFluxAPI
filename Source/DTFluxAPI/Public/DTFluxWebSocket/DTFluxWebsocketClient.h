// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IWebSocket.h"
#include "UObject/Object.h"
#include "DTFluxWebsocketClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnectionConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionClosed, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionError, const FString&, Error);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceivedMessage, const FString&, Message);


UCLASS(Blueprintable, Category="DTFlux|Websocket")
class DTFLUXAPI_API UDTFluxWebSocketClient : public UObject
{
	GENERATED_BODY()

public:

	// UDTFluxWebSocketClient() = default;
	void Initialize();
	// virtual void BeginDestroy() override;
	// virtual ~UDTFluxWebSocketClient() override;
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Websocket")
	bool Connect(const FString URL, const int Port );

	UFUNCTION(BlueprintCallable, Category="DTFlux|Websocket")
	void Disconnect();

	UFUNCTION(BlueprintCallable, Category="DTFlux|Websocket")
	bool SendMessage(const FString Message, const bool Broadcast = false);

	UPROPERTY(BlueprintAssignable, Category="DTFlux|Websocket")
	FOnConnectionConnected OnConnectionConnected;
	
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Websocket")
	FOnConnectionClosed OnConnectionClosed;
	
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Websocket")
	FOnConnectionError OnConnectionError;
	
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Websocket")
	FOnReceivedMessage OnReceivedMessage;

	bool Close() const;
	bool IsConnected();

protected:

	
private:
	void OnConnectionConnectedInternal() const;
	void OnConnectionErrorInternal(const FString& Error) const;
	void OnConnectionClosedInternal(const FString& Reason) const;
	void OnReceivedMessageInternal(const FString& Message) const;

	TSharedPtr<IWebSocket> Ws;
};
