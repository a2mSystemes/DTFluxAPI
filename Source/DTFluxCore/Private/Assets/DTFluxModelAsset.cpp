// Fill out your copyright notice in the Description page of Project Settings.


#include "Assets/DTFluxModelAsset.h"
#include "DTFluxCoreModule.h"
#include "Types/Objects/DTFluxContestStorage.h"


// const FDateTime UDTFluxModelAsset::GetMassStart(const int& ContestId, const int& StageId)
// {
// }


// void UDTFluxModelAsset::InsertOrUpdateRaceData(const TSharedPtr<FJsonObject>& Response)
// {
// 	
// }
//
// void UDTFluxModelAsset::InsertOrUpdateContestRanking(const TSharedPtr<FJsonObject>& Response)
// {
// }
//
// void UDTFluxModelAsset::InsertOrUpdateStageRanking(const TSharedPtr<FJsonObject>& Response)
// {
// }
//
// void UDTFluxModelAsset::InsertOrUpdateSplitRanking(const TSharedPtr<FJsonObject>& Response)
// {
// }
//
// void UDTFluxModelAsset::InsertOrUpdateTeamList(const TSharedPtr<FJsonObject>& Response)
// {
// }
//
// void UDTFluxModelAsset::SplitSensorTriggered(const TSharedPtr<FJsonObject>& Response)
// {
// }


UDTFluxModelAsset::UDTFluxModelAsset(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDTFluxModelAsset::AddContest(const FDTFluxContest &Contest)
{
	Contests.Add(Contest.Name, Contest);
}

bool UDTFluxModelAsset::GetContestById(const int InContestId, FDTFluxContest& OutContest)
{
	for(auto& ContestItem : Contests)
	{
		if(ContestItem.Value.ContestId == InContestId)
		{
			OutContest = ContestItem.Value;
			return true;
		}

	}
	return false;
}

void UDTFluxModelAsset::AddPerson(const FDTFluxPerson& InPerson)
{
	Persons.Add(InPerson);
}

void UDTFluxModelAsset::AddParticipant(const FDTFluxParticipant& InParticipant, const int ContestId)
{
	FDTFluxContest TargetContest;
	if(GetContestById(ContestId, TargetContest))
	{
		if(!PersonExists(InParticipant.Person1))
		{
			AddPerson(InParticipant.Person1);
		}
		if(InParticipant.Person2 != 0)
		{
			if(!PersonExists(InParticipant.Person2))
			{
				AddPerson(InParticipant.Person2);
			}
		}
		Participants.Add(InParticipant.Bib, InParticipant);
		TargetContest.ParticipantsBib.Add(InParticipant.Bib);
	}
}

bool UDTFluxModelAsset::PersonExists(const FDTFluxPerson& InPerson) const
{
	return Persons.ContainsByPredicate([InPerson](const FDTFluxPerson& Person)
	{
		return Person == InPerson;
	});
}
