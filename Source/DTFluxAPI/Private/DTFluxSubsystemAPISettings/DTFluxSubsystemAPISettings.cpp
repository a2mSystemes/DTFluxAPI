// Fill out your copyright notice in the Description page of Project Settings.

#include "DTFluxSubsystemAPISettings/DTFluxSubsystemAPISettings.h"
#include "DTFluxAPILog.h"

// DEPRECATED : Now in WS
FString FDTFluxSubsystemAPISettings::GetRaceDataEndpoint(const FDTFluxSubsystemAPISettings* Settings)
{
	if(Settings)
	{
		FString RaceDataEndpoint = 
			FString::Printf(TEXT("%s/%p"), *Settings->GetProxyBaseEndpoint(), Settings->ProxyEndpoints.FindKey("race-datas"));
		UE_LOG(LogDTFluxAPI, Log, TEXT("Proxy Race Data -> %s"), *RaceDataEndpoint);
		return RaceDataEndpoint;
	}
	return FString("");
}
// DEPRECATED : Now in WS
FString FDTFluxSubsystemAPISettings::GetContestRankingEndpoint(const FDTFluxSubsystemAPISettings* Settings, const int ContestId)
{
	if(Settings)
	{
		FString Ranking = *Settings->ProxyEndpoints.FindKey("ranking");
		const TCHAR* ContestIDTmpl = *FString("{:ContestID}");
		const TCHAR* ContestIDValue = *FString(TEXT("%i"),ContestId);
		FString ContestRanking = Ranking.Replace(ContestIDTmpl, ContestIDValue );
		FString ContestRankingEndpoint = Settings->GetProxyBaseEndpoint() + ContestRanking;
		UE_LOG(LogDTFluxAPI, Log, TEXT("Proxy Contest Ranking -> %s"), *ContestRankingEndpoint);
		return ContestRankingEndpoint;
	}
	return FString("");
}
// DEPRECATED : Now in WS
FString FDTFluxSubsystemAPISettings::GetStageRankingEndpoint(const FDTFluxSubsystemAPISettings* Settings, const int ContestId,
	const int StageId)
{
	if(Settings)
	{
		FString StageRanking = GetContestRankingEndpoint(Settings, ContestId);
		StageRanking = FString::Printf(TEXT("%s/stage/%i/"), *StageRanking, StageId);
		UE_LOG(LogDTFluxAPI, Log, TEXT("Proxy Stage Ranking -> %s"), *StageRanking);
		return StageRanking;
	}
	return FString("");
}
// DEPRECATED : Now in WS
FString FDTFluxSubsystemAPISettings::GetStageRankingFilteredEndpoint(const FDTFluxSubsystemAPISettings* Settings,
	const int ContestId, const int StageId, const FString SplitName)
{
	if (Settings){
		FString StageRanking = GetStageRankingEndpoint(Settings, ContestId, StageId);
		StageRanking = FString::Printf(TEXT("%s?splitname=%s"), *StageRanking, *SplitName);
		UE_LOG(LogDTFluxAPI, Log, TEXT("Proxy Stage Ranking with Splitname -> %s"), *StageRanking);
		return StageRanking;
	}
	return FString("");
}

// DEPRECATED : Now in WS
FString FDTFluxSubsystemAPISettings::GetTeamsEndpoint(const FDTFluxSubsystemAPISettings* Settings)
{
	if(Settings)
	{
		FString TeamsEndpoint = 
			FString::Printf(TEXT("%s/%p"), *Settings->GetProxyBaseEndpoint(), Settings->ProxyEndpoints.FindKey("teams"));
		UE_LOG(LogDTFluxAPI, Log, TEXT("Proxy Teams -> %s"), *TeamsEndpoint );
		return TeamsEndpoint;
	}
	return FString("");
}
