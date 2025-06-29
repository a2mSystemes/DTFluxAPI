// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IWebSocket.h"
#include "UObject/Object.h"
#include "Utils/HttpStatusCode.h"
#include "Utils/WebSocketStatusCode.h"
#include "DTFluxWebSocketClient.generated.h"


USTRUCT()
struct FDTFluxWsClientError
{
	GENERATED_BODY()
	FDTFluxWsClientError()
		:ClientId(FName("-1")),Reason(FString()), When(FDateTime::Now())
			{};
	FDTFluxWsClientError(const FName InName, EDTFluxProtocolError InCode = EDTFluxProtocolError::UnknownError ,
			FString InReason = FString(""))
		:ClientId(InName), Code(InCode), Reason(InReason), When(FDateTime::Now()){};
	
	
public:
	UPROPERTY(BlueprintInternalUseOnly)
	FName ClientId;

	UPROPERTY()
	EDTFluxProtocolError Code = EDTFluxProtocolError::NoErrors;

	UPROPERTY()
	FString Reason;
	
	UPROPERTY()
	FDateTime When;

	static  FDTFluxWsClientError CreateNoError(FName InClientId)
	{
		return FDTFluxWsClientError(InClientId);
		
	};
	static  FDTFluxWsClientError CreateUnknownError(FName InClientId)
	{
		return FDTFluxWsClientError(InClientId, EDTFluxProtocolError::UnknownError);
		
	};
};

class DTFLUXNETWORK_API FDTFluxWebSocketClient : public TSharedFromThis<FDTFluxWebSocketClient>
{
public:

	FDTFluxWebSocketClient();
	
static TSharedPtr<FDTFluxWebSocketClient> GetClient(const TArray<TSharedPtr<FDTFluxWebSocketClient>> InClients, const FName InName)
{
	for(auto Client: InClients)
	{
		if(Client->ClientId == InName)
		{
			return Client;
		}
	}
	return nullptr;
}
	
	void Connect();
	void Reconnect();
	void Disconnect();
	FDTFluxWsClientError GetError();
	void ClearErrors();
	void AddError(const FDTFluxWsClientError Error);
	void Send(const FString& Message) const;
	void SetAddress(FString NewAddress)
	{
		WsAddress = NewAddress;
	}
	FString GetAddress()
	{
		return WsAddress;
	}


	DECLARE_EVENT(FDTFluxWebSocketClient, FWebSocketConnectedEvent);
	FWebSocketConnectedEvent DTFluxWsClientConnectedEvent;
	FWebSocketConnectedEvent& RegisterConnectedEvent()
	{
		return DTFluxWsClientConnectedEvent;
	}
	FWebSocketConnectedEvent& UnregisterConnectedEvent()
	{
		return DTFluxWsClientConnectedEvent;
	}
	
	DECLARE_EVENT_OneParam(FDTFluxWebSocketClient, FDTFluxWsClientConnectionErrorEvent, const FString& /* Error */);
	FDTFluxWsClientConnectionErrorEvent DTFluxWsClientConnectionErrorEvent;
	FDTFluxWsClientConnectionErrorEvent& RegisterConnectionError()
	{
		return DTFluxWsClientConnectionErrorEvent;
	};
	FDTFluxWsClientConnectionErrorEvent& UnregisterConnectionError()
	{
		return DTFluxWsClientConnectionErrorEvent;
	};
	
	DECLARE_EVENT_ThreeParams(FDTFluxWebSocketClient, FDTFluxWsClientClosedEvent, int32 /* StatusCode */, const FString& /* Reason */, bool /* bWasClean */);
	FDTFluxWsClientClosedEvent DTFluxWsClientClosedEvent;
	FDTFluxWsClientClosedEvent& RegisterClosedEvent()
	{
		return DTFluxWsClientClosedEvent;
	}
	FDTFluxWsClientClosedEvent& UnregisterClosedEvent()
	{
		return DTFluxWsClientClosedEvent;
	}
	
	DECLARE_EVENT_OneParam(FDTFluxWebSocketClient, FDTFluxWsClientMessageEvent, const FString& /* MessageString */);
	FDTFluxWsClientMessageEvent DTFluxWsClientMessageEvent;
	FDTFluxWsClientMessageEvent& RegisterMessageEvent()
	{
		return DTFluxWsClientMessageEvent;
	};
	FDTFluxWsClientMessageEvent& UnregisterMessageEvent()
	{
		return DTFluxWsClientMessageEvent;
	};

	DECLARE_EVENT_ThreeParams(FDTFluxWebSocketClient, FDTFluxWsClientBinaryMessageEvent, const void* /* Data */, SIZE_T /* Size */, bool /* bIsLastFragment */);
	FDTFluxWsClientBinaryMessageEvent DTFluxWsClientBinaryMessageEvent;
	FDTFluxWsClientBinaryMessageEvent& RegisterBinaryMessageEvent()
	{
		return DTFluxWsClientBinaryMessageEvent;
	};
	FDTFluxWsClientBinaryMessageEvent& UnregisterBinaryMessageEvent()
	{
		return DTFluxWsClientBinaryMessageEvent;
	};

	DECLARE_EVENT_ThreeParams(FDTFluxWebSocketClient, FDTFluxWsClientRawMessageEvent, const void* /* Data */, SIZE_T /* Size */, SIZE_T /* BytesRemaining */);
	FDTFluxWsClientRawMessageEvent DTFluxWsClientRawMessageEvent;
	FDTFluxWsClientRawMessageEvent& RegisterRawMessageEvent()
	{
		return DTFluxWsClientRawMessageEvent;
	};
	FDTFluxWsClientRawMessageEvent& UnregisterRawMessageEvent()
	{
		return DTFluxWsClientRawMessageEvent;
	};

	DECLARE_EVENT_OneParam(FDTFluxWebSocketClient, FDTFluxWsClientMessageSentEvent, const FString& /* MessageString */);
	FDTFluxWsClientMessageSentEvent DTFluxWsClientMessageSentEvent;
	FDTFluxWsClientMessageSentEvent& RegisterMessageSentEvent()
	{
		return DTFluxWsClientMessageSentEvent;
	};
	FDTFluxWsClientMessageSentEvent& UnregisterMessageSentEvent()
	{
		return DTFluxWsClientMessageSentEvent;
	};
	
	const FName GetClientId()
	{
		return ClientId;
	};
	
	
private:
	void Bind_Internal();
	void OnConnected_Internal();
	void OnClientClosed_Internal(int32 StatusCode, const FString& Reason, bool bWasClea);
	void OnClientError_Internal(const FString& ErrorMessage);
	void OnClientReceiveMessage_Internal(const FString& Message);
	void OnClientReceiveBinaryMessage_Internal(const void* Data, SIZE_T Size, bool bIsLastFragment);
	void OnClientReceiveRawMessage_Internal(const void* Data, SIZE_T Size, SIZE_T BytesRemaining);
	void OnMessageSent_Internal(const FString& MessageString);
	void SendMessage_internal(const FString& Message);
	// void SendBin_internal(void* Data);

	TSharedPtr<IWebSocket> Ws;
	TArray<FDTFluxWsClientError> Errors;
	static int32 LastId;
	FName ClientId;	
	FString WsAddress;
	static FName DefaultId;
};


