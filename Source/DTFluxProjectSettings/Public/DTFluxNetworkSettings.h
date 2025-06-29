// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DTFluxNetworkSettings.generated.h"

struct FDTFluxHttpSettings;
struct FDTFluxWsSettings;

/**
 * 
 */
UCLASS(Config=Engine, DefaultConfig, meta=(DisplayName="DTFlux Network Settings"))
class DTFLUXPROJECTSETTINGS_API UDTFluxNetworkSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UDTFluxNetworkSettings();
	
	UPROPERTY(Category="CHRONO PROXY|Websocket", Config, EditAnywhere, BlueprintReadOnly, DisplayName="Websocket Port")
	int WebSocketPort = 3000;
	
	UPROPERTY(Category="CHRONO PROXY|Websocket", Config, EditAnywhere, BlueprintReadOnly, DisplayName="Websocket Address")
	FString WebSocketAddress = "127.0.0.1";

	UPROPERTY(Category="CHRONO PROXY|Websocket", Config, EditAnywhere, BlueprintReadOnly, DisplayName="Websocket Path")
	FString WebSocketPath =  "/api/v1";
	
	UPROPERTY(Category="CHRONO PROXY|Websocket", Config, EditAnywhere, BlueprintReadOnly, DisplayName="Autoconnect to WebSocket Server at startup")
	bool bWebSocketShouldConnectAtStartup = false;	

	UPROPERTY(Category="CHRONO PROXY|Websocket", Config, EditAnywhere, BlueprintReadOnly, DisplayName="Auto Reconnect On WebSocket Closed")
	bool bWebSocketShouldReconnectOnClosed = false;
	
	UPROPERTY(Category="CHRONO PROXY|Websocket", Config, EditAnywhere, BlueprintReadOnly, DisplayName="Auto Reconnect On WebSocket Error")
	bool bWebSocketShouldReconnectOnError = false;
	
	UPROPERTY(Category="CHRONO PROXY|HTTP", Config, EditAnywhere, BlueprintReadOnly, DisplayName="HTTP Address")
	FString HTTPAddress = "http://localhost";
	
	UPROPERTY(Category="CHRONO PROXY|HTTP", Config, EditAnywhere, BlueprintReadOnly, DisplayName="HTTP Path")
	FString HTTPPath = "/";
	
	UPROPERTY(Category="CHRONO PROXY|HTTP", Config, EditAnywhere, BlueprintReadOnly, DisplayName="HTTP Port")
	int HTTPPort = 8080;
	


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDTFluxHttpSettingsChanged, const FDTFluxHttpSettings&); 
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDTFluxWebSocketSettingsChanged, const FDTFluxWsSettings&); 
	FOnDTFluxHttpSettingsChanged OnDTFluxHttpSettingsChanged;
	FOnDTFluxWebSocketSettingsChanged OnDTFluxWebSocketSettingsChanged;

#endif
	static void GetHTTPSettings(const UDTFluxNetworkSettings* Settings, FDTFluxHttpSettings& OutHTTPSettings);
	static void GetWebSocketSettings(const UDTFluxNetworkSettings* Settings, FDTFluxWsSettings& OutWsSettings);
	
};
