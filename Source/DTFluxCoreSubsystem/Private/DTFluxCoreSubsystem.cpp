// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxCoreSubsystem.h"


#include "DTFluxCoreSubsystemModule.h"
#include "DTFluxGeneralSettings.h"
#include "Assets/DTFluxModelAsset.h"
#include "Subsystems/DTFluxNetworkSubsystem.h"

void UDTFluxCoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(logDTFluxCoreSubsystem, Log, TEXT("[UDTFluxCoreSubsystem] Initializing..."));
	if(!DataStorage)
	{
		const UDTFluxGeneralSettings* GeneralSettings = GetDefault<UDTFluxGeneralSettings>();
		TSoftObjectPtr<UDTFluxModelAsset> ModelAsset = GeneralSettings->ModelAsset;
		DataStorage = ModelAsset.LoadSynchronous();
		// UE_LOG(logDTFluxCore, Log, TEXT("GeneralSettings is nullptr -> %s"), GeneralSettings == nullptr ? TEXT("TRUE") : TEXT("FALSE"));
		// UE_LOG(logDTFluxCore, Log, TEXT("ModelAsset isNull() -> %s"), ModelAsset.IsNull() ? TEXT("TRUE") : TEXT("FALSE"));
		// UE_LOG(logDTFluxCore, Log, TEXT("ModelAsset IsValid() -> %s"), ModelAsset.IsValid() ? TEXT("TRUE") : TEXT("FALSE"));
		// UE_LOG(logDTFluxCore, Log, TEXT("DataStorage is nullptr -> %s"), DataStorage == nullptr ? TEXT("TRUE") : TEXT("FALSE"));
	}

	//TODO REMOVE This as it's only for testing purpose
	NetworkSubsystem = GEngine->GetEngineSubsystem<UFDTFluxNetworkSubsystem>();
	if(NetworkSubsystem->WsStatus != EDTFluxConnectionStatus::Connected)
	{
		RegisterDelegates();
		NetworkSubsystem->Connect();
	}

	
}

void UDTFluxCoreSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UDTFluxCoreSubsystem::RegisterDelegates()
{
	if(NetworkSubsystem)
	{
		NetworkSubsystem->OnReceivedRaceData().AddDynamic(this, &UDTFluxCoreSubsystem::ParseRaceData);
		NetworkSubsystem->OnReceivedTeamList().AddDynamic(this, &UDTFluxCoreSubsystem::ParseTeamList);
	}
}


void UDTFluxCoreSubsystem::ParseRaceData(const FDTFluxRaceData& RaceDataDefinition)
{

	if( RaceDataDefinition.Datas.Num() > 0 )
	{
		UE_LOG(logDTFluxCoreSubsystem, Warning, TEXT("Receiving RaceDataDefinition [%s]"), *RaceDataDefinition.Datas[0].Name);
		if(DataStorage != nullptr)
		{
			UE_LOG(logDTFluxCoreSubsystem, Error, TEXT("DataStorage Name %s"), *DataStorage->EventName);
			for(auto Contest : RaceDataDefinition.Datas)
			{
				DataStorage->AddContest(Contest);
			}
		}
		else
		{
			UE_LOG(logDTFluxCoreSubsystem, Error, TEXT("DataStorage is null"));
		}
		return;
	}
	UE_LOG(logDTFluxCoreSubsystem, Error, TEXT("RaceDataDefinition is empty !!!"));

}

void UDTFluxCoreSubsystem::ParseTeamList(const FDTFluxTeamListDefinition& TeamListDefinition)
{
	UE_LOG(logDTFluxCoreSubsystem, Warning, TEXT("Received TeamList with %i Items"), TeamListDefinition.Datas.Num());
	for(auto InParticipantDefinition : TeamListDefinition.Datas)
	{
		FDTFluxParticipant NewParticipant;
		NewParticipant.Person1.Gender = InParticipantDefinition.Gender;
		NewParticipant.Person1.FirstName = InParticipantDefinition.FirstName;
		NewParticipant.Person1.LastName = InParticipantDefinition.LastName;
		if(InParticipantDefinition.Team != "")
		{
			NewParticipant.Person2.Gender = InParticipantDefinition.Gender2;
			NewParticipant.Person2.FirstName = InParticipantDefinition.FirstName2;
			NewParticipant.Person2.LastName = InParticipantDefinition.LastName2;
		}
		NewParticipant.Bib = InParticipantDefinition.Bib;
		NewParticipant.Category = InParticipantDefinition.Category;
		NewParticipant.Club = InParticipantDefinition.Club;
		NewParticipant.Status = static_cast<EDTFluxParticipantStatusType>(InParticipantDefinition.Status);
		UE_LOG(logDTFluxCoreSubsystem, Warning, TEXT("Add Participant %s %s in %i ContestId"),
			*NewParticipant.Person1.FirstName, *NewParticipant.Person1.LastName, InParticipantDefinition.ContestId );
		DataStorage->AddParticipant(NewParticipant, InParticipantDefinition.ContestId);
	}
}

void UDTFluxCoreSubsystem::OnDataReceived()
{
}

void UDTFluxCoreSubsystem::SendRequest(const FString& Message)
{
	if(NetworkSubsystem)
	{
		NetworkSubsystem->SendMessage(Message);
	}
}


void UDTFluxCoreSubsystem::SendTeamListRequest()
{
	if (NetworkSubsystem)
	{
		NetworkSubsystem->SendRequest(EDTFluxRequestType::TeamList);
	}
}

void UDTFluxCoreSubsystem::SendRaceDataRequest()
{
	if (NetworkSubsystem)
	{
		NetworkSubsystem->SendRequest(EDTFluxRequestType::RaceData);
	}
}

void UDTFluxCoreSubsystem::SendContestRankingRequest(int InContestId)
{
	if (NetworkSubsystem)
	{
		NetworkSubsystem->SendRequest(EDTFluxRequestType::ContestRanking, InContestId);
	}
}

void UDTFluxCoreSubsystem::SendStageRankingRequest(int InContestId, int InStageId, bool bShouldIncludeSplitRanking)
{
	// TODO Implement this
}

void UDTFluxCoreSubsystem::RequestAllStageRankingOfContest(int InContestId, int InStageId,
	bool bShouldIncludeSplitRanking)
{
	// TODO Implement this
}

void UDTFluxCoreSubsystem::SendSplitRankingRequest(int InContestId, int InStageId, int InSplitId)
{
	// TODO Implement this
}

void UDTFluxCoreSubsystem::RequestAllSplitRankingOfContest(int InContestId, int InStageId)
{
	// TODO Implement this
}

void UDTFluxCoreSubsystem::RequestAllSplitRankingOfStage(int InContestId, int InStageId, int InSplitId)
{
	// TODO Implement this
}

void UDTFluxCoreSubsystem::RefreshStorage()
{
	// TODO Implement this
}
