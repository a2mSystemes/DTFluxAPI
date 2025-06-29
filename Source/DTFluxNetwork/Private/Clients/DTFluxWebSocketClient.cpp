// Fill out your copyright notice in the Description page of Project Settings.


#include "Clients/DTFluxWebSocketClient.h"

#include "DTFluxNetworkModule.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "Utils/WebSocketStatusCode.h"

int32 FDTFluxWebSocketClient::LastId = 0;
FName FDTFluxWebSocketClient::DefaultId = FName("DTFluxWsClient");

FDTFluxWebSocketClient::FDTFluxWebSocketClient()
{
	ClientId = FName("DTFluxWsClient_" + FDTFluxWebSocketClient::LastId);
	FDTFluxWebSocketClient::LastId++;
}

void FDTFluxWebSocketClient::Connect()
{
	Ws = FWebSocketsModule::Get().CreateWebSocket(WsAddress);
	if(Ws.IsValid())
	{
		Bind_Internal();
		UE_LOG(logDTFluxNetwork, Warning, TEXT("Connecting"))
		Ws->Connect();
	}
}

void FDTFluxWebSocketClient::Reconnect()
{
	if(Ws.IsValid())
	{
		if(Ws->IsConnected())
		{
			Ws->Close();
			Ws->Connect();
			return;
		}
		UE_LOG(logDTFluxNetwork, Warning, TEXT("Connecting"))
		Ws->Connect();
		return;
	}
	UE_LOG(logDTFluxNetwork, Error, TEXT("Undelying IWebSocket is Invalid, Cannot Connect !!!"))

}

void FDTFluxWebSocketClient::Disconnect()
{
	if(Ws.IsValid())
	{
		if(Ws->IsConnected())
		{
			EWebSocketCloseCode ReasonCode = EWebSocketCloseCode::Normal;
			Ws->Close(static_cast<int32>(ReasonCode), FString("Normal Disconnect, BYE !!!"));
		}
	}
}

FDTFluxWsClientError FDTFluxWebSocketClient::GetError()
{
	FDTFluxWsClientError Error;
	if(Errors.Num() > 0)
	{
		return Errors.Pop(EAllowShrinking::Yes);	
	}
	return FDTFluxWsClientError::CreateNoError(ClientId);
}

void FDTFluxWebSocketClient::ClearErrors()
{
	Errors.Empty();
}

void FDTFluxWebSocketClient::AddError(const FDTFluxWsClientError Error)
{
	Errors.Add(Error);
}

void FDTFluxWebSocketClient::Send(const FString& Message) const
{
	Ws->Send(Message);
}


void FDTFluxWebSocketClient::Bind_Internal()
{
	Ws->OnConnected().AddRaw(this, &FDTFluxWebSocketClient::OnConnected_Internal);
	Ws->OnClosed().AddRaw(this, &FDTFluxWebSocketClient::OnClientClosed_Internal);
	Ws->OnConnectionError().AddRaw(this, &FDTFluxWebSocketClient::OnClientError_Internal);
	Ws->OnMessage().AddRaw(this, &FDTFluxWebSocketClient::OnClientReceiveMessage_Internal);
	Ws->OnBinaryMessage().AddRaw(this, &FDTFluxWebSocketClient::OnClientReceiveBinaryMessage_Internal);
	Ws->OnMessageSent().AddRaw(this, &FDTFluxWebSocketClient::OnMessageSent_Internal);
	Ws->OnRawMessage().AddRaw(this, &FDTFluxWebSocketClient::OnClientReceiveRawMessage_Internal);
}

void FDTFluxWebSocketClient::OnConnected_Internal()
{
	DTFluxWsClientConnectedEvent.Broadcast();
}

void FDTFluxWebSocketClient::OnClientClosed_Internal(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	DTFluxWsClientClosedEvent.Broadcast(StatusCode, Reason, bWasClean);
}

void FDTFluxWebSocketClient::OnClientError_Internal(const FString& ErrorMessage)
{
	DTFluxWsClientConnectionErrorEvent.Broadcast(ErrorMessage);
}

void FDTFluxWebSocketClient::OnClientReceiveMessage_Internal(const FString& Message)
{
	DTFluxWsClientMessageEvent.Broadcast(Message);
}

void FDTFluxWebSocketClient::OnClientReceiveBinaryMessage_Internal(const void* Data, SIZE_T Size, bool bIsLastFragment)
{
	DTFluxWsClientBinaryMessageEvent.Broadcast(Data,Size, bIsLastFragment);
}

void FDTFluxWebSocketClient::OnClientReceiveRawMessage_Internal(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
	DTFluxWsClientRawMessageEvent.Broadcast(Data, Size, BytesRemaining);
}

void FDTFluxWebSocketClient::OnMessageSent_Internal(const FString& MessageString)
{
	DTFluxWsClientMessageSentEvent.Broadcast(MessageString);
}

void FDTFluxWebSocketClient::SendMessage_internal(const FString& Message)
{
	if(Ws.IsValid() && Ws->IsConnected())
	{
		Ws->Send(Message);
	}
}


