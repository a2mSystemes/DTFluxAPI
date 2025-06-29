// Fill out your copyright notice in the Description page of Project Settings.


#include "Types/Objects/DTFluxContestStorage.h"

void UDTFluxContestStorage::Initialize()
{
}

// bool UDTFluxContestStorage::InitializeRaceData(const FDTFluxRaceDataDefinition& InRaceDataResponse)
// {
// 	return false;
// }
//
// bool UDTFluxContestStorage::InitializeTeamList(const FDTFluxTeamListDefinition& InTeamListDefinition)
// {
// 	return false;
// }
//
// bool UDTFluxContestStorage::UpdateParticipantStatus(const FDTFluxTeamListDefinition& InTeamListDefinition)
// {
// 	return false;
// }

#pragma region ContestRegion

const FDTFluxContest& UDTFluxContestStorage::GetCurrentContest() const
{
	return CurrentContest;
}

const FDTFluxStage& UDTFluxContestStorage::GetCurrentStage(const int InContest, const int InStage)
{
	return CurrentStage;
}

void UDTFluxContestStorage::AddContest(const FDTFluxContest InContest)
{
}
//
// void UDTFluxContestStorage::AddContestResponse(const FDTFluxContestResponse& InContestResponse)
// {
// }

#pragma endregion

#pragma region ContestRankingRegion
void UDTFluxContestStorage::GetContestRankings(const TArray<FDTFluxContestRanking> OutContestRanking, const int InContestId, const bool bShouldSortByRank,
	const bool bShouldSkipEmpty)
{
}

void UDTFluxContestStorage::GetContestRankingsSorted(const TArray<FDTFluxContestRanking> OutSortedContestRankings, const int InContestId, const EDTFluxSortingFilter InFilter,
	const bool bShouldSkipEmpty)
{
}

#pragma endregion 

#pragma region StageRegion

void UDTFluxContestStorage::GetStage(const int InContest, const int InStage, const FDTFluxStage& OutStage)
{
}

void UDTFluxContestStorage::GetStages(const int InContest, TArray<FDTFluxStage> OutStages)
{
}

void UDTFluxContestStorage::GetStageCurrentStage(const FDTFluxStage& OutStage)
{
}

void UDTFluxContestStorage::AddStage(const int InContest, const FDTFluxStage& InStage)
{
}

// void UDTFluxContestStorage::AddStageResponse(const int InContestId, const FDTFluxStageResponse& StageResponse)
// {
// }

#pragma endregion

#pragma region StageRankingRegion

void UDTFluxContestStorage::AddStageRanking(const int InContest, const FDTFluxStageRanking& InStageRanking)
{
}

#pragma endregion

#pragma region SplitRegion

void UDTFluxContestStorage::AddSplit(const int InContest, const FDTFluxSplit& InSplit)
{
}

#pragma endregion 

#pragma region SplitRankingRegion

void UDTFluxContestStorage::AddSplitRanking(const int InContest, const FDTFluxSplitRanking& InSplitRanking)
{
}

void UDTFluxContestStorage::GetSplitRankingByParticipant(const FDTFluxParticipant& InParticipant, const int InContestId,
	const int InStageId, const FDTFluxSplitRanking& OutSplitRankingForBib)
{
}

void UDTFluxContestStorage::GetSplitRankingByBib(const int InBib, const int InContestId, const int InStageId,
	const FDTFluxSplitRanking& OutSplitRankingForBib)
{
}

#pragma endregion 

#pragma region ParticipantSection


void UDTFluxContestStorage::GetParticipantsForContest(const int InContestId,
	const TArray<FDTFluxParticipant> OutParticipants)
{
}

void UDTFluxContestStorage::GetPoursuiteStartupLine(const int InContestId,
	const TArray<FDTFluxParticipant> OutParticipants)
{
}

FDateTime UDTFluxContestStorage::GetMassStart(const int InContestId)
{
	return FDateTime();
}
#pragma endregion

void UDTFluxContestStorage::SetCurrentContest()
{
}
