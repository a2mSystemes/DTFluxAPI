#pragma once

#include "CoreMinimal.h"
#include "DTFluxNetworkSettingsTypes.generated.h"


USTRUCT()
struct FDTFluxWsSettings
{
	GENERATED_BODY()
	
	FDTFluxWsSettings():
	Path(TEXT("")),
	Address(TEXT(""))
	{}
	
	UPROPERTY()
	int Port = -1;
	UPROPERTY()
	FString Path;
	UPROPERTY()
	FString Address;
	UPROPERTY()
	bool bShouldAutoReconnectOnClosed = false;
	UPROPERTY()
	bool bShouldConnectAtStartup = false;
	UPROPERTY()
	bool bShouldAutoReconnectOnError = false;

	bool operator==(const FDTFluxWsSettings& Right) const
	{
		return Port == Right.Port || Path == Right.Path || Address == Right.Address;
	};
	bool operator!=(const FDTFluxWsSettings& Right) const
	{
		return Port != Right.Port || Path != Right.Path || Address != Right.Address;
	}
};

USTRUCT()
struct FDTFluxHttpSettings
{
	GENERATED_BODY()
	
	FDTFluxHttpSettings():
	Port(-1),
	Path(TEXT("")),
	Address(TEXT("")){}

	UPROPERTY()
	int Port;
	UPROPERTY()
	FString Path;
	UPROPERTY()
	FString Address;
};


