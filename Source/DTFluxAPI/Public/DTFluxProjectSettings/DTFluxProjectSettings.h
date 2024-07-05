// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DTFluxAPILog.h"
#include "DTFluxProjectSettings.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(OnDTFluxProjectSettingsModified, FString, const UDTFluxProjectSettings* );


/**
 * DTFlux project settings
 */
UCLASS(Blueprintable, Config=Engine, DefaultConfig, meta=(DisplayName="DTFlux Project Settings"))
class DTFLUXAPI_API UDTFluxProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	OnDTFluxProjectSettingsModified OnProjectSettingsModified;
	
	UPROPERTY(Category="DTFlux|Config|Race Result API", Config, EditAnywhere, BlueprintReadOnly)
	int RaceResultPort = 80;
	
	UPROPERTY(Category="DTFlux|Config|Race Result API", Config, EditAnywhere, BlueprintReadOnly)
	FString RaceResultUrl = "http://localhost";
	
	UPROPERTY(Category="DTFlux|Config|Race Result API", Config, EditAnywhere, BlueprintReadOnly)
	FString RaceResultSessionID;
	
	UPROPERTY(Category="DTFlux|Config|Race Result API", Config, EditAnywhere, BlueprintReadOnly)
	bool bAccessIsLocal = true;
			
	UPROPERTY(Category="DTFlux|Config|Race Result API", Config, EditAnywhere, BlueprintReadOnly)
	FString StartListAccessToken;
			
	UPROPERTY(Category="DTFlux|Config|Race Result API", Config, EditAnywhere, BlueprintReadOnly)
	FString GeneralClassificationAccessToken;
				
	UPROPERTY(Category="DTFlux|Config|Race Result API", Config, EditAnywhere, BlueprintReadOnly)
	FString LiveStageResultsAccessToken ;
// Proxy
		
	UPROPERTY(Category="DTFlux|Config|Chrono Proxy", Config, EditAnywhere, BlueprintReadOnly)
	int WebsocketServerPort = 3000;
	
	UPROPERTY(Category="DTFlux|Config|Chrono Proxy", Config, EditAnywhere, BlueprintReadOnly)
	FString WebsocketServerAddress = "ws://127.0.0.1";
	
	UPROPERTY(Category="DTFlux|Config|Chrono Proxy", Config, EditAnywhere, BlueprintReadOnly)
	FString ProxyAddress = "http://localhost";

	UPROPERTY(Category="DTFlux|Chrono Proxy", Config, EditAnywhere, BlueprintReadOnly)
	int ProxyPort = 8000;
	
	UPROPERTY(Category="DTFlux|Config|Chrono Proxy", Config, EditAnywhere, BlueprintReadOnly)
	FString ProxyRootPath = "/endpoints";
	UPROPERTY(Category="DTFlux|Config|Chrono Proxy", Config, EditAnywhere, BlueprintReadOnly)
	FString ProxyRaceDataEndpoint;
	UPROPERTY(Category="DTFlux|Config|Chrono Proxy", Config, EditAnywhere, BlueprintReadOnly)
	FString ProxyRankingEndpoint;
	UPROPERTY(Category="DTFlux|Config|Chrono Proxy", Config, EditAnywhere, BlueprintReadOnly)
	FString ProxyTeamsEndpoint;

	
	//Server Config ****NOT USED****


	UPROPERTY(Category="DTFlux|Server Config", Config, EditAnywhere, BlueprintReadOnly)
	int InPort = 8080;
	
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Config")
	static const UDTFluxProjectSettings* GetDTFluxAPIProjectSettings();


	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	
protected:

	
private:
	UDTFluxProjectSettings();
};
 