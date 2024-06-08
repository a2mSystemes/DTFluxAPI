// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxProjectSettings/DTFluxProjectSettings.h"

FString UDTFluxProjectSettings::GetAPIPath(const TEnumAsByte<EDTFluxAPIRoute> RouteType, const FString& Filters) const
{
	FString ApiAccessRoute;
	UE_LOG(LogDTFluxAPI, Type::Log, TEXT("Filters in GetApiPath settings\nGot %s"), *Filters);

	if (bAccessIsLocal)
	{
		// http://localhost/_246158/api/8O8JMI2739JS58R8KRJGJEZUSQXF807O
		ApiAccessRoute += RaceResultPort != 80 ?  FString::Printf(TEXT("%s:%i/_%s/api/"), *ProxyUrl, RaceResultPort, *APIToken) :
		FString::Printf(TEXT("%s/_%s/api/"), *ProxyUrl, *APIToken);
	}
	else
	{
		// https://api.raceresult.com/246158/8O8JMI2739JS58R8KRJGJEZUSQXF807O
		ApiAccessRoute +=  FString::Printf(TEXT("https://api.raceresult.com/%s/"), *APIToken);
	}
	switch(RouteType)
	{
	case (EDTFluxAPIRoute::Results):
		return ApiAccessRoute + LiveStageResultsAccessToken + Filters;
		break;
	case (EDTFluxAPIRoute::FinalClassification):
		return ApiAccessRoute + GeneralClassificationAccessToken + Filters;
		break;

	default:
		return ApiAccessRoute + StartListAccessToken + Filters;
	}
}

FString UDTFluxProjectSettings::GetAPIPathFiltered(const TEnumAsByte<EDTFluxAPIRoute> RouteType,
	const FSearchFilters& Filters) const
{
	const FString Filter = Filters.GetFilter();
	UE_LOG(LogDTFluxAPI, Type::Log, TEXT("Filters in Project settings\nGot %s"), *Filter);
	return GetAPIPath(RouteType, Filter );
}

FString UDTFluxProjectSettings::GetProxyPath(const TEnumAsByte<EDTFluxProxyRoute> RouteType,
	const int& InContest, const int& InStage ) const
{
	switch(RouteType)
	{
	case (EDTFluxProxyRoute::ProxyRankingContest):
		return FString::Printf(TEXT("%s:%i%s/ranking/contest/%i/"), *ProxyAddress, ProxyPort, *ProxyRootPath, InContest);
		break;
	case (EDTFluxProxyRoute::ProxyRankingStage):
		return FString::Printf(TEXT("%s:%i%s/ranking/contest/%i/stage?id=%i"),
				*ProxyAddress, ProxyPort, *ProxyRootPath, InContest, InStage);
		break;
	case (EDTFluxProxyRoute::ProxyTeams):
		return FString::Printf(TEXT("%s:%i%s/teams/"), *ProxyAddress, ProxyPort, *ProxyRootPath);
		break;
	default :
		return FString::Printf(TEXT("%s:%i%s/race/datas/"), *ProxyAddress, ProxyPort, *ProxyRootPath);
		break;
	}
}

const UDTFluxProjectSettings* UDTFluxProjectSettings::GetDTFluxAPIProjectSettings()
{
	return GetDefault<UDTFluxProjectSettings>();
}

UDTFluxProjectSettings::UDTFluxProjectSettings()
{
	SectionName = "DTFlux Settings";
}
