// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DTFluxAPILog.h"
#include "DTFluxProjectSettings.generated.h"

UENUM()
enum EDTFluxAPIRoute: uint8
{
	Root UMETA(DisplayName="API Root"),
	Results UMETA(DisplayName="Results Route"),
	Starters UMETA(DisplayName="Starters List Route"),
	FinalClassification UMETA(DisplayName="Final Classification Route"),

};


UENUM()
enum EDTFluxProxyRoute : uint8
{
	ProxyRaceData UMETA(DisplayName="Proxy Race Data"),
	ProxyRankingContest UMETA(DisplayName="Proxy Ranking Contest Data"),
	ProxyRankingStage UMETA(DisplayName="Proxy Ranking Stage Data"),
	ProxyTeams UMETA(DisplayName="Proxy Teams Data"),
};

USTRUCT(BlueprintType)
struct FSearchFilters
{
	GENERATED_BODY()

	UPROPERTY(Blueprintable, Category="DTFlux|Config",  EditAnywhere)
	int ContestId = 0;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Config",  EditAnywhere)
	int StageId = 0;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Config",  EditAnywhere)
	int GenderId = 0;
	
	FString GetFilter() const
	{
		if(ContestId == 0 && StageId == 0 && GenderId == 0)
		{
			UE_LOG(LogDTFluxAPI, Type::Log, TEXT("Gender Test -> %i"), GenderId);
			return FString();
		}
		TArray<FString> Filters;
		Filters.Add(FString("?"));
		if (ContestId > 0)
		{
			Filters.Add(FString::Printf(TEXT("Contest=%i"), ContestId) );
		}
		if (StageId > 0)
		{
			Filters.Add(FString::Printf(TEXT("SelectorResult=%i"), StageId) );
		}
		if (GenderId > 0)
		{
			switch(GenderId)
			{
			case (2):
				// default is male
				UE_LOG(LogDTFluxAPI, Type::Log, TEXT("Gender Female -> %i"), GenderId);

				Filters.Add(FString(TEXT("filter=[Gender]=\"f\"") ) );
				break;
			default:
				// default is male
				UE_LOG(LogDTFluxAPI, Type::Log, TEXT("Gender male -> %i"), GenderId);
				Filters.Add(FString(TEXT("filter=[Gender]=\"m\"") ) );
				break;
			}
		}
		for(const auto& Element : Filters)
		{
			UE_LOG(LogDTFluxAPI, Type::Log, TEXT("Element : %s"), *Element);

		}
		FString FinalFilters = 	FString::Join(Filters, TEXT("&"));
		FinalFilters.RemoveAt(1);
		return FinalFilters;
	};
};

/**
 * 
 */
UCLASS(Blueprintable, Config=Engine, DefaultConfig, meta=(DisplayName="DTFlux Project Settings"))
class DTFLUXAPI_API UDTFluxProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:


	UPROPERTY(Category="DTFlux|Server Config", Config, EditAnywhere, BlueprintReadOnly)
	int InPort = 8080;
	
	UPROPERTY(Category="DTFlux|Config|Race Result API", Config, EditAnywhere, BlueprintReadOnly)
	int RaceResultPort = 80;
	
	UPROPERTY(Category="DTFlux|Server Config", Config, EditAnywhere, BlueprintReadOnly)
	TArray<FString> Endpoints;
	
	UPROPERTY(Category="DTFlux|Config|Race Result API", Config, EditAnywhere, BlueprintReadOnly)
	FString ProxyUrl = "http://localhost";
	
	UPROPERTY(Category="DTFlux|Config|Race Result API", Config, EditAnywhere, BlueprintReadOnly)
	FString APIToken;
	
	UPROPERTY(Category="DTFlux|Config|Race Result API", Config, EditAnywhere, BlueprintReadOnly)
	bool bAccessIsLocal = true;
			
	UPROPERTY(Category="DTFlux|Config|Race Result API", Config, EditAnywhere, BlueprintReadOnly)
	FString StartListAccessToken;
			
	UPROPERTY(Category="DTFlux|Config|Race Result API", Config, EditAnywhere, BlueprintReadOnly)
	FString GeneralClassificationAccessToken;
				
	UPROPERTY(Category="DTFlux|Config|Race Result API", Config, EditAnywhere, BlueprintReadOnly)
	FString LiveStageResultsAccessToken ;

	UPROPERTY(Category="DTFlux|Config|Chrono Proxy", Config, EditAnywhere, BlueprintReadOnly)
	FString ProxyAddress = "http://localhost";
	
	UPROPERTY(Category="DTFlux|Config|Chrono Proxy", Config, EditAnywhere, BlueprintReadOnly)
	FString ProxyRootPath = "/endpoints";
	
	UPROPERTY(Category="DTFlux|Chrono Proxy", Config, EditAnywhere, BlueprintReadOnly)
	int ProxyPort = 8000;

	UFUNCTION(BlueprintCallable, Category="DTFlux|Config")
	FString GetAPIPath(const TEnumAsByte<EDTFluxAPIRoute> RouteType, const FString& Filters = TEXT("") ) const;

	UFUNCTION(BlueprintCallable, Category="DTFlux|Config")
	FString GetAPIPathFiltered(const TEnumAsByte<EDTFluxAPIRoute> RouteType, const FSearchFilters& Filters ) const;

	UFUNCTION(BlueprintCallable, Category="DTFlux|Config")
	FString GetProxyPath(const TEnumAsByte<EDTFluxProxyRoute> RouteType, const int& InContest = -1, const int& InStage = -1) const;
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Config")
	static const UDTFluxProjectSettings* GetDTFluxAPIProjectSettings();

protected:

	
private:
	UDTFluxProjectSettings();
};
 