// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxNetworkSettings.h"
#include "Types/DTFluxNetworkSettingsTypes.h"

UDTFluxNetworkSettings::UDTFluxNetworkSettings()
{
	CategoryName = "DTFlux API";
}

#if WITH_EDITOR
void UDTFluxNetworkSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FString PropertyId = PropertyChangedEvent.GetPropertyName().ToString();
	if(PropertyId.Contains("WebSocket"))
	{
		FDTFluxWsSettings WsSettings;
		GetWebSocketSettings(this, WsSettings);
		OnDTFluxWebSocketSettingsChanged.Broadcast(WsSettings);
	}
	if(PropertyId.Contains("HTTP"))
	{
		FDTFluxHttpSettings HTTPSettings;
		GetHTTPSettings(this, HTTPSettings);
		OnDTFluxHttpSettingsChanged.Broadcast(HTTPSettings);
	}
}
#endif


void UDTFluxNetworkSettings::GetHTTPSettings(const UDTFluxNetworkSettings* Settings,
	FDTFluxHttpSettings& OutHTTPSettings)
{
	OutHTTPSettings.Address = Settings->HTTPAddress; 
	OutHTTPSettings.Port = Settings->HTTPPort; 
	OutHTTPSettings.Path = Settings->HTTPPath; 
}

void UDTFluxNetworkSettings::GetWebSocketSettings(const UDTFluxNetworkSettings* Settings,
	FDTFluxWsSettings& OutWsSettings)
{
	OutWsSettings.Address = Settings->WebSocketAddress;
	OutWsSettings.Path = Settings->WebSocketPath;
	OutWsSettings.Port = Settings->WebSocketPort;
	OutWsSettings.bShouldAutoReconnectOnClosed = Settings->bWebSocketShouldReconnectOnClosed;
	OutWsSettings.bShouldConnectAtStartup = Settings->bWebSocketShouldConnectAtStartup;
	OutWsSettings.bShouldAutoReconnectOnError = Settings->bWebSocketShouldReconnectOnError;
}


