// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxDataStorage/DTFluxDataStorage.h"

// #include "AsyncTreeDifferences.h"
#include "DTFluxAPILog.h"
#include "DTFluxModel/DTFluxModel.h"

bool UDTFluxDataStorage::UpdateDataStorage(const FString JsonPayload)
{
	UE_LOG(LogDTFluxAPI, Log, TEXT("UDPATE DataStorage with data : %s"), *JsonPayload);
	// Single json value (any of supported json types e.g.
	// object with properties, array, bool) at top level of json
	TSharedPtr<FJsonValue> JsonValue;

	// Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonPayload);
	if (FJsonSerializer::Deserialize(Reader, JsonValue)) {
		// Get the value of the json object by field name
		TSharedPtr<FJsonObject> Json = JsonValue->AsObject();
		FString Type = Json->GetStringField(TEXT("type"));
		TArray<TSharedPtr<FJsonValue>> Datas = Json->GetArrayField(TEXT("datas"));
		// UE_LOG(LogDTFluxAPI, Log, TEXT("DTFlux-Response-Type : %s"), *Type);
		if(Type.Contains("race-datas"))
		{
			UE_LOG(LogDTFluxAPI, Log, TEXT("DTFlux-Response-Type : \"race-datas\""));
			// Contests is empty;
			if(Contests.Num() == 0)
			{
				FDTFluxContest Contest;
				for(const auto& Data : Datas)
				{
					TSharedPtr<FJsonObject> ContestData = Data->AsObject();
					Contest.Id = ContestData->GetIntegerField(TEXT("id"));
					Contest.Name = ContestData->GetStringField(TEXT("name"));
					Contest.SetDate(ContestData->GetStringField(TEXT("date")));
					TArray<FDTFluxSplit> Splits;
					TArray<TSharedPtr<FJsonValue>> SplitDatas = ContestData->GetArrayField(TEXT("splits"));
					for(const auto& SplitData : SplitDatas)
					{
						FDTFluxSplit Split;
						Split.Id = SplitData->AsObject()->GetIntegerField(TEXT("id"));
						Split.Name = SplitData->AsObject()->GetStringField(TEXT("name"));
						Splits.Add(Split);
					}
					TArray<TSharedPtr<FJsonValue>> StagesData = ContestData->GetArrayField(TEXT("stages"));
					Contest.AddStage(StagesData, Splits);
				}
			}
			
		}
		else if(Type.Contains("team-list"))
		{
			UE_LOG(LogDTFluxAPI, Log, TEXT("DTFlux-Response-Type : \"team-list\""));
			
		}
		else if(Type.Contains("contest-ranking"))
		{
			int ContestId = Json->GetIntegerField(TEXT("contestID")); 
			UE_LOG(LogDTFluxAPI, Log, TEXT("DTFlux-Response-Type : \"contest-ranking\""));
			
		}
		else if(Type.Contains("stage-ranking"))
		{
			int ContestId = Json->GetIntegerField(TEXT("contestID")); 
			int StageId = Json->GetIntegerField(TEXT("stageID"));
			int SplitID = -1;
			if(Json->HasField(TEXT("splitID")))
			{
				SplitID = Json->GetIntegerField(TEXT("splitID"));
				if(SplitID == -1)
				{
					// we have all splits gaps from the request
				}
			}
			UE_LOG(LogDTFluxAPI, Log, TEXT("DTFlux-Response-Type : \"stage-ranking\""));
			
		}else if(Type.Contains("split-sensor"))
		{
			UE_LOG(LogDTFluxAPI, Log, TEXT("split-sensor received"));
			// Request New Ranking
			// 
		}

	}
	return true;
}



TArray<FDTFluxStage> UDTFluxDataStorage::GetStages(const int ContestId)
{
	TArray<FDTFluxStage> Stages;
	for(const auto& Contest : Contests)
	{
		Stages.Append(Contest.Stages);
	}
	return Stages;
}

bool UDTFluxDataStorage::GetContest(FDTFluxContest& OutContest, const int& ContestId)
{
	// Current contest requested
	if(ContestId == -1)
	{
		FDateTime Now = FDateTime::Now();
		for(auto& Contest : Contests)
		{
			for( auto& Stage : Contest.Stages)
			{
				if(Stage.StartTime >= Now && Stage.EndTime <= Now)
				{
					//We have a winner
					OutContest = Contest;
					return true;
				}
			}
		}
	}
	else
	{
		for( auto& Contest : Contests)
		{
			if(Contest.Id == ContestId)
			{
				OutContest = Contest;
				return true;
			}
		}
	}
	return false;
}

bool UDTFluxDataStorage::GetStage(FDTFluxStage& CurrentStage, const int& StageId)
{
	// Current contest requested
	if(StageId == -1)
	{
		FDateTime Now = FDateTime::Now();
		for(auto& Contest : Contests)
		{
			for( auto& Stage : Contest.Stages)
			{
				if(StageId <= -1)
				{
					if(Stage.StartTime >= Now && Stage.EndTime <= Now)
					{
						//We have a winner for current stage
						CurrentStage = Stage;
						return true;
					}
				}else
				{
					if(Stage.Id == StageId)
					{
						//We have a winner for the search stage
						CurrentStage = Stage;
						return true;
					}
				}

			}
		}
	}

	return false;
}

TArray<FDTFluxParticipant> UDTFluxDataStorage::GetParticipants(const int ContestId)
{
	TArray<FDTFluxParticipant> Participants;
	for(const auto& Contest : Contests)
	{
		if (ContestId <= -1)
		{
			Participants.Append(Contest.Participants);
		}
		else if(ContestId == Contest.Id)
		{
			Participants.Append(Contest.Participants);
		}
		
	}
	return Participants;
}

FDTFluxParticipant UDTFluxDataStorage::GetParticipant(const int ContestID, const int ParticipantBib)
{
	return GetParticipants(ContestID)[ParticipantBib];
}

TArray<FDTFluxStageRanking> UDTFluxDataStorage::GetStageRanking(const int ContestId, const int StageId)
{
	if(Contests.Num() > (ContestId -1))
	{
		FDTFluxContest Contest = Contests[ContestId - 1];
		if(Contest.Stages.Num() > (StageId -1))
		{
			FDTFluxStage Stage = Contest.Stages[StageId - 1];
			return Stage.StageRanking;
		}
	}
	return TArray<FDTFluxStageRanking>();
}

void UDTFluxDataStorage::AddOrUpdateContest(const FDTFluxContestResponse& ContestResponse)
{
	FDTFluxContest Contest;
	bool NewContest = false;
	if(!Contests.IsEmpty() )
	{
		for(auto& OldContest: Contests)
		{
			if(OldContest.Id == ContestResponse.Id)
			{
				Contest = OldContest;
				NewContest = false;
				break;
			}else
			{
				NewContest = true;
			}
		}
	}else
	{
		NewContest = true;
	}
	Contest.Id = ContestResponse.Id;
	Contest.Name = ContestResponse.Name;
	TArray<FDTFluxSplit> Splits;
	for(auto Split: ContestResponse.Splits)
	{
		FDTFluxSplit S;
		S.Id = Split.Id;
		S.Name = Split.Name;
		Splits.Add(S);
	}
	for(auto StageResp : ContestResponse.Stages )
	{
		FDTFluxStage Stage;
		Stage.Id = StageResp.Id;
		Stage.Name = StageResp.Name;
		FDateTime::Parse(StageResp.StartTime, Stage.StartTime);
		FDateTime::Parse(StageResp.EndTime, Stage.EndTime);
		Stage.Splits = Splits;
		Contest.Stages.Add(Stage);
	}
	if(NewContest)
	{
		Contests.Add(Contest);
	}
	// UE_LOG(LogDTFluxAPI, Log, TEXT("Contest DUMP %s"));

}

void UDTFluxDataStorage::AddOrUpdateParticipant(const FDTFluxTeamListItemResponse& TeamListItemResponse)
{
	// UE_LOG(LogDTFluxAPI, Log, TEXT("About to process Participant %s BIB : %d"), *TeamListItemResponse.LastName, TeamListItemResponse.Bib);
	FDTFluxParticipant Participant;
	Participant.Bib = TeamListItemResponse.Bib;
	Participant.Category = TeamListItemResponse.Category;
	Participant.Club = TeamListItemResponse.Club;
	Participant.Elite = TeamListItemResponse.Elite;
	Participant.Person1.Gender = TeamListItemResponse.Gender;
	Participant.Person1.FirstName = TeamListItemResponse.FirstName;
	Participant.Person1.LastName = TeamListItemResponse.LastName;
	// TODO ???
	// Participant.Person2.Gender = TeamListItemResponse.Gender2;
	Participant.Person2.FirstName = TeamListItemResponse.FirstName2;
	Participant.Person2.LastName = TeamListItemResponse.LastName2;
	Participant.Status = TeamListItemResponse.Status;
	for(auto& Contest: Contests)
	{
		if(Contest.Id == TeamListItemResponse.ContestId)
		{
			Contest.AddParticipant(Participant);
			return;
		}
	}
	// UE_LOG(LogDTFluxAPI, Error, TEXT(" Participant %s with BIB : %d Has no valid Contest associated. Got %d. This participant wil not be registered."),
	// 	*Participant.Person1.LastName, Participant.Bib, TeamListItemResponse.ContestId);

}


void UDTFluxDataStorage::AddSplitSensorResult(FDTFluxSplitSensorItemResponse Response)
{
	// Send SplitSensor Result to BP
	FDTFluxStage CurrentStage;
	if(GetStage(CurrentStage, Response.StageID))
	{
		// this is an empty stage
		if(CurrentStage.Id == -1 )
		{
		
		}
	}

}

void UDTFluxDataStorage::GoToNextStage()
{
	// If Number of stages is less or equal to the current stageID
	if(IsInitialized())
	{
		if(Contests[CurrentContestId].Stages.Num() -1 <= CurrentStageId)
		{
			CurrentStageId += 1;
		}else
		{
			ResetStageId();
			ChangeCurrentContest();
		}
	}

}

void UDTFluxDataStorage::ChangeCurrentContest()
{
	// Contest Are initialized
	if(IsInitialized())
	{
		if(CurrentContestId < Contests.Num() -1)
		{
			// last Contest
			CurrentContestId = 0;
		}

	}
}
