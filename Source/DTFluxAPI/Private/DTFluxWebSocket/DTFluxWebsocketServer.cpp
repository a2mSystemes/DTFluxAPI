// // Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "DTFluxWebSocket/DTFluxWebsocketServer.h"
//
// #include <winsock2.h>
//
// #include "IWebSocket.h"
// #include "WebSocketsModule.h"
// #include "INetworkingWebSocket.h"
// #include "IWebSocketNetworkingModule.h"
// #include "WebSocketNetworkingDelegates.h"
// #include "DTFluxAPILog.h"
// #include "IWebSocketServer.h"
//
//
// UDTFluxWebsocketServer::UDTFluxWebsocketServer()
// {
// 	Ws = UDTFluxWebsocketServer::GetServer();
// }
//
// UDTFluxWebsocketServer::~UDTFluxWebsocketServer()
// {
// 	
// }
//
// bool UDTFluxWebsocketServer::ConnectToChannel(const FString URL, const int Port)
// {
// 	//try to get a WsServer
// 	if(!Ws)
// 		Ws = UDTFluxWebsocketServer::GetServer();
// 	if(Ws)
// 	{
// 		FWebSocketClientConnectedCallBack ClientConnectedCb;
// 		ClientConnectedCb.BindUObject(this, &UDTFluxWebsocketServer::OnConnectionConnectedInternal);
// 		if(Ws->Init((uint32) Port, ClientConnectedCb, FString("127.0.0.1")) )
// 		{
// 			UE_LOG(LogDTFluxAPI, Log, TEXT("Server listening on %s:%i"),*URL, Port);
// 			if (Ws.IsValid())
// 				UE_LOG(LogDTFluxAPI, Log, TEXT("Server pointer Is Valid %s"), *Ws->Info());
// 			
// 			return true;
// 		}
// 		else
// 		{
// 			UE_LOG(LogDTFluxAPI, Error, TEXT("Server Init() failed"));
// 			Ws.Reset();
// 			ClientConnectedCb.Unbind();
// 			return false;
// 		}
// 	}
// 	UE_LOG(LogDTFluxAPI, Error, TEXT("Unable to get a WsServer Object"));
//
// 	return false;
// }
//
// void UDTFluxWebsocketServer::LeaveChannel()
// {
// 	bIsConnected = false;
// 	Ws = nullptr;
// }
//
// bool UDTFluxWebsocketServer::SendMessageToAll(const FString Message)
// {
// 	if(!Ws)
// 	{
// 		UE_LOG(LogDTFluxAPI, Error, TEXT("WebSocketServer Not Up"));
// 		return false;
// 	}
// 	if(Clients.Num() <= 0)
// 	{
// 		UE_LOG(LogDTFluxAPI, Log, TEXT("No Clients Yet !!!"));
// 		return false;
// 	}
// 	int i = 0;
// 	for(const auto& Client : Clients)
// 	{
// 		const TCHAR* SerializedChar = Message.GetCharArray().GetData();
// 		int32 Size = FCString::Strlen(SerializedChar);
// 		int32 Sent = 0;
// 		uint8* Data = (uint8*)TCHAR_TO_UTF8(SerializedChar);
// 		if(Client->Send(Data, Size))
// 		{
// 			UE_LOG(LogDTFluxAPI, Log, TEXT("Message %s sent to client n°%i"), *Message, i);
// 			i++;
// 		}
// 	}
// 	if(i != 0)
// 	{
// 		UE_LOG(LogDTFluxAPI, Log, TEXT("%i Messages sent"),  i);
// 		return true;
// 	}
// 	UE_LOG(LogDTFluxAPI, Error, TEXT("Message has not been sent"));
// 	return false;
// }
//
// void UDTFluxWebsocketServer::OnConnectionConnectedInternal(INetworkingWebSocket* ClientWebSocket)
// {
// 	OnConnectionConnected.Broadcast(ChannelName);
// 	FWebSocketPacketReceivedCallBack PacketReceivedCallBack;
// 	PacketReceivedCallBack.BindUObject(this, &UDTFluxWebsocketServer::OnReceivedMessageInternal);
// 	ClientWebSocket->SetReceiveCallBack(PacketReceivedCallBack);
// 	FWebSocketInfoCallBack InfoCallBack;
// 	InfoCallBack.BindLambda([this, ClientWebSocket]()
// 	{
// 		int i = 0;
//
// 		for(const auto& Client : Clients)
// 		{
// 			int Removed = Clients.Remove(Client);
// 			if(Removed)
// 				UE_LOG(LogDTFluxAPI, Log, TEXT("Client Disconnected %i"), i);
// 			i++;
// 		}
// 	});
// 	ClientWebSocket->SetSocketClosedCallBack(InfoCallBack);
// 	FWebSocketInfoCallBack InfoCallBackError;
// 	InfoCallBackError.BindUObject(this, &UDTFluxWebsocketServer::OnConnectionErrorInternal);
// 	ClientWebSocket->SetErrorCallBack(InfoCallBackError);
// 	UE_LOG(LogDTFluxAPI, Log, TEXT("WebSocketServer', [Connected], Channel: %s"), *ChannelName);
// }
//
// void UDTFluxWebsocketServer::OnConnectionErrorInternal()
// {
// 	UE_LOG(LogDTFluxAPI, Error, TEXT("WebSocketServer'"));
// 	OnConnectionError.Broadcast(TEXT("Channel"), TEXT("UNKNOWN ERROR"));
//
// }
//
// void UDTFluxWebsocketServer::OnReceivedMessageInternal(void* Data, int32 Count)
// {
// 	FString Message;
// 	if (Count == 0)   // nothing to process
// 	{
// 		return;
// 	}
// 	const uint8* DataRef = reinterpret_cast<uint8*>(Data);
// 	const TArray<uint8> MessageData(DataRef, Count);
// 	const FString JSonData = UTF8_TO_TCHAR(MessageData.GetData());
// 	OnReceivedMessage.Broadcast(ChannelName, Message);
// 	UE_LOG(LogDTFluxAPI, Log, TEXT("WebSocketServer, [Message Reveived]\nMessage : %s"), *Message);
//
// }
//
// TUniquePtr<IWebSocketServer> UDTFluxWebsocketServer::GetServer()
// {
// 	return FModuleManager::Get().LoadModuleChecked<IWebSocketNetworkingModule>(TEXT("WebSocketNetworking")).
// 		CreateServer();
// }
//
// void UDTFluxWebsocketServer::Close()
// {
// 	UE_LOG(LogDTFluxAPI, Log, TEXT("WebSocketServer, [Closed()]"));
//
// }


#include "DTFluxWebSocket/DTFluxWebsocketServer.h"
//Fill out your copyright notice in the Description page of Project Settings.

#include "INetworkingWebSocket.h"
#include "IWebSocketNetworkingModule.h"
#include "WebSocketNetworkingDelegates.h"
#include "DTFluxAPILog.h"


UDTFluxServerWebSocket::UDTFluxServerWebSocket()
{
}

UDTFluxServerWebSocket::~UDTFluxServerWebSocket()
{
}

void UDTFluxServerWebSocket::Init(const int& Port, const FString& Url)
{
	ServerWebSocket = FModuleManager::Get().LoadModuleChecked<IWebSocketNetworkingModule>(TEXT("WebSocketNetworking")).CreateServer();

	FWebSocketClientConnectedCallBack CallBack;
	CallBack.BindLambda([this](INetworkingWebSocket* Client)
	{

		FGuid uuid = FGuid::NewGuid();
		ConnectedClients.Add(uuid, Client);	
	});

	if (!ServerWebSocket->Init(Port, CallBack, Url))
	{
		UE_LOG(LogDTFluxAPI, Error, TEXT("ServerWebSocket Init FAIL"));
		ServerWebSocket.Reset();
		CallBack.Unbind();
		return;
	}
	UE_LOG(LogDTFluxAPI, Log, TEXT("ServerWebSocket Connected to port %d"), Port);

	
	TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float Time)		
	{
		if (ServerWebSocket)
		{
			ServerWebSocket->Tick();
			return true;
		}
		else
		{
			return false;
		}
	}));
}

void UDTFluxServerWebSocket::BeginDestroy()
{
	Super::BeginDestroy();
	Close();

	if (TickHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
		TickHandle.Reset();
	}
}

void UDTFluxServerWebSocket::OnWebSocketClientConnected(INetworkingWebSocket* ClientWebSocket)
{
	FWebSocketPacketReceivedCallBack CallBack;
	CallBack.BindUObject(this, &UDTFluxServerWebSocket::ReceivedRawPacket);
	UE_LOG(LogDTFluxAPI, Log, TEXT("ServerWebSocket Connected"));

	ClientWebSocket->SetReceiveCallBack(CallBack);
}

void UDTFluxServerWebSocket::ReceivedRawPacket(void* Data, int32 Count)
{
	if (Count == 0)   // nothing to process
	{
		return;
	}

	const uint8* DataRef = reinterpret_cast<uint8*>(Data);

	const TArray<uint8> MessageData(DataRef, Count);

	const FString JSonData = UTF8_TO_TCHAR(MessageData.GetData());
	UE_LOG(LogDTFluxAPI, Log, TEXT("ServerWebSocket received %s"), *JSonData);
	OnJsonRecieved.Broadcast(JSonData);
}