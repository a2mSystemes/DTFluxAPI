// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxDataStorage/DTFluxDataStorage.h"

// #include "AsyncTreeDifferences.h"
#include "DTFluxAPILog.h"
#include "DTFluxModel/DTFluxModel.h"


TArray<FDTFluxStage> UDTFluxDataStorage::GetStages(const int ContestId)
{
	TArray<FDTFluxStage> Stages;
	for(const auto& Contest : Contests)
	{
		Stages.Append(Contest.Stages);
	}
	return Stages;
}



void UDTFluxDataStorage::UpdateSplitRanking(const FDTFluxStageRankingResponse& SplitRankingResponse)
{
	for(auto& Contest : Contests)
	{
		if(Contest.Id == SplitRankingResponse.ContestID)
		{
			for(auto& Stage : Contest.Stages)
			{
				if(Stage.Id == SplitRankingResponse.StageID)
				{
					for(auto& Split : Stage.Splits)
					{
						if(Split.Id == SplitRankingResponse.SplitID)
						{
							for(auto& SplitRankingItemResp : SplitRankingResponse.Datas)
							{
								Split.InsertOrReplace(SplitRankingItemResp);
							}	
						}

					}
				}
			}
		}
	}
}

// // Bad implementation
// void UDTFluxDataStorage::UpdateParticipant(const FDTFluxTeamUpdateResponse& TeamUpdateResponse)
// {
// 	FDTFluxParticipant Participant;
//
// 	for(auto& TeamUpdateRespItem : TeamUpdateResponse.Datas)
// 	{
// 		Participant.Bib = TeamUpdateRespItem.Bib;
// 		Participant.Category = TeamUpdateRespItem.Category;
// 		Participant.Club = TeamUpdateRespItem.Club;
// 		Participant.Elite = TeamUpdateRespItem.Elite;
// 		Participant.Person1.Gender = TeamUpdateRespItem.Gender;
// 		Participant.Person1.FirstName = TeamUpdateRespItem.FirstName;
// 		Participant.Person1.LastName = TeamUpdateRespItem.LastName;
// 		// TODO ???
// 		// Participant.Person2.Gender = TeamListItemResponse.Gender2;
// 		Participant.Person2.FirstName = TeamUpdateRespItem.FirstName2;
// 		Participant.Person2.LastName = TeamUpdateRespItem.LastName2;
// 		Participant.Status = static_cast<TEnumAsByte<EDTFluxParticipantStatusType>>(TeamUpdateRespItem.Status);
// 		bool Update = false;
// 		for(auto& Contest : Contests)
// 		{
// 			int Idx = 0;
// 			for(auto& OldParticipant : Contest.Participants)
// 			{
// 				if(OldParticipant.Bib == Participant.Bib)
// 				{
// 					Update = true;
// 					UE_LOG(LogDTFluxAPI, Log, TEXT("Idx %d OLD : %s %s New : %s %s In Contest%02d"),
// 						Idx, *OldParticipant.Person1.FirstName, *OldParticipant.Person1.LastName,
// 						*Participant.Person1.FirstName, *Participant.Person1.LastName, Contest.Id);
// 					break;
// 				}
// 				Idx++;
// 			}
// 			if(Update)
// 			{
// 				if(Contest.Participants.IsValidIndex(Idx))
// 				{
// 					UE_LOG(LogDTFluxAPI, Log, TEXT("Idx %d, to be removed : %s %s in Contest%02d"),
// 						Idx, *Contest.Participants[Idx].Person1.FirstName, *Contest.Participants[Idx].Person1.LastName,
// 						Contest.Id);
// 					Contest.Participants.RemoveAt(Idx);
// 				}
// 			}
// 			Contest.Participants.Add(Participant);
// 			
// 		}
// 	}
// }

// TODO NOT IMPLEMENTED
void UDTFluxDataStorage::UpdateParticipantStatus(const FDTFluxStatusUpdateResponse& StatusUpdateResponse)
{
	EDTFluxParticipantStatusType NewStatus = static_cast<TEnumAsByte<EDTFluxParticipantStatusType>>(StatusUpdateResponse.Datas.Status);
	UE_LOG(LogDTFluxAPI, Log, TEXT("Status to be UPDATED for Bib %d to %s"), StatusUpdateResponse.Datas.Bib,
		*UEnum::GetValueAsString(NewStatus));
	bool Found = false;
		for(auto& Contest : Contests)
		{
			UE_LOG(LogDTFluxAPI, Log, TEXT("Checking Participant witg Bib %d in Contest %02d"),
				StatusUpdateResponse.Datas.Bib, Contest.Id);
			for(auto& Participant: Contest.Participants)
			{
				if(Participant.Bib == StatusUpdateResponse.Datas.Bib)
				{
					Found = true;
					EDTFluxParticipantStatusType OldStatus = Participant.Status;
					Participant.Status = static_cast<TEnumAsByte<EDTFluxParticipantStatusType>>(StatusUpdateResponse.Datas.Status);
					UE_LOG(LogDTFluxAPI, Log, TEXT("Status UPDATED from %s to %s"),
						*UEnum::GetValueAsString(OldStatus), *UEnum::GetValueAsString(Participant.Status));
					return;
				}
			}
		}
}

// TODO NOT IMPLEMENTED
bool UDTFluxDataStorage::IsFinisherSplit(const FDTFluxSplitSensorResponse& SplitSensorResponse)
{
	return true;
}

// TODO NOT IMPLEMENTED
FDTFluxFinisher UDTFluxDataStorage::GetFinisherStatus(const FDTFluxSplitSensorResponse& SplitSensorResponse)
{
	FDTFluxFinisher Finisher;
	Finisher.Participant.Bib = 15222;
	return Finisher;
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

void UDTFluxDataStorage::GetParticipant(const int ContestID, const int ParticipantBib, FDTFluxParticipant& OutParticipant)
{
	TArray<FDTFluxParticipant> Particpants = GetParticipants(ContestID);
	for(auto& Participant : Particpants)
	{
		if(Participant.Bib == ParticipantBib)
		{
			OutParticipant = Participant;
		}
	}
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
	int ContestIdx = 0;
	if(!Contests.IsEmpty() )
	{
		for(auto& OldContest: Contests)
		{
			NewContest = true;
			if(OldContest.Id == ContestResponse.Id)
			{
				// We have the contest that need to be updated
				Contest = OldContest;
				NewContest = false;
				break;
			}
			// updating ContestIndex
			ContestIdx++;
		}
	}else
	{
		// this is a new contest because we don't have one
		NewContest = true;
	}
	// Updating values
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
		Contest.Dump();
		Contests.Add(Contest);
		return;
	}
	Contest.Dump();
	Contests.RemoveAt(ContestIdx);
	Contests.Insert(Contest, ContestIdx);
	// handle updating contest
}

void UDTFluxDataStorage::AddOrUpdateParticipant(const FDTFluxTeamListItemResponse& TeamListItemResponse)
{
	UE_LOG(LogDTFluxAPI, Log, TEXT("In DataStorage::AddOrUpdateParticipant"));
	UE_LOG(LogDTFluxAPI, Log, TEXT("AboutToUpdateOrAdd Participant %d %s %s in Contest%02d "),
		TeamListItemResponse.Bib, *TeamListItemResponse.FirstName, *TeamListItemResponse.LastName,
		TeamListItemResponse.ContestId);
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
	Participant.Status = static_cast<TEnumAsByte<EDTFluxParticipantStatusType>>(TeamListItemResponse.Status);
	for(auto& Contest: Contests)
	{
		if(Contest.Id == TeamListItemResponse.ContestId)
		{
			UE_LOG(LogDTFluxAPI, Log, TEXT("AboutToUpdateOrAdd Participant %d %s %s in Contest%02d "),
				Participant.Bib, *Participant.Person1.FirstName, *Participant.Person1.LastName,
				Contest.Id);

			Contest.AddParticipant(Participant);
			return;
		}
		UE_LOG(LogDTFluxAPI, Log, TEXT("Contest%02d has now %04d Participants"), Contest.Id,
			Contest.Participants.Num());

	}
}

void UDTFluxDataStorage::UpdateContestRanking(const FDTFluxContestRankingResponse& InContestRanking)
{
	
	for(auto& Contest : Contests)
	{
		if(Contest.Id == InContestRanking.ContestID)
		{
			// Clean ContestRanking
			Contest.ContestRanking.Empty();
			for(const auto& TeamContestRankingResponse : InContestRanking.Datas)
			{
				FDTFluxContestRanking NewRankingEl;
				NewRankingEl.Bib = TeamContestRankingResponse.Bib;
				NewRankingEl.Rank = TeamContestRankingResponse.Rank;
				NewRankingEl.Gap = TeamContestRankingResponse.Gap;
				NewRankingEl.Time = TeamContestRankingResponse.Time;
				Contest.ContestRanking.Add(NewRankingEl);
				Contest.SortContestRanking();
			}
		}
	}
}

void UDTFluxDataStorage::UpdateStageRanking(const FDTFluxStageRankingResponse& StageRankingResponse)
{
	// UE_LOG(LogDTFluxAPI, Log, TEXT("In DataStorage::UdpateStageRanking"));
	// UE_LOG(LogDTFluxAPI, Log, TEXT("AboutToUpdate Contest%02d and Stage%02d"),
		// StageRankingResponse.ContestID, StageRankingResponse.StageID);
	for(auto& Contest: Contests)
	{
		if(Contest.Id == StageRankingResponse.ContestID)
		{
			// UE_LOG(LogDTFluxAPI, Log, TEXT("Found Contest::%02d "),Contest.Id);
			if(Contest.Stages.IsEmpty())
			{
				// UE_LOG(LogDTFluxAPI, Log, TEXT("WTF Contest::%02d HAS NO STAGES ???"),Contest.Id);
			}
			for(auto& Stage: Contest.Stages)
			{
				
				// UE_LOG(LogDTFluxAPI, Log, TEXT("Current Stage is Stage::%02d "),Stage.Id);
				if(Stage.Id == StageRankingResponse.StageID)
				{
					// UE_LOG(LogDTFluxAPI, Log, TEXT("Found Stage::%02d "),Stage.Id);
					// Cleaning StageRanking
					Stage.StageRanking.Empty();
					for(auto& StageRanking: StageRankingResponse.Datas )
					{
						FDTFluxStageRanking NewStageRanking;
						NewStageRanking.TimeRun = StageRanking.TimeRun;
						NewStageRanking.TimeStart = StageRanking.TimeStart;
						NewStageRanking.TimeTransition = StageRanking.TimeTransition;
						NewStageRanking.TimeSwim = StageRanking.TimeSwim;
						NewStageRanking.Bib = StageRanking.Bib;
						NewStageRanking.Gap = StageRanking.Gap;
						NewStageRanking.Rank = StageRanking.Rank;
						Stage.StageRanking.Add(NewStageRanking);
						Stage.SortStageRanking();
						// UE_LOG(LogDTFluxAPI, Log,
						// 	TEXT("Testing -> Stage %02d::%s in Contest %02d::%s, has now  %02d Rankings\n"),
						// 	Stage.Id, *Stage.Name, Contest.Id, *Contest.Name, Stage.StageRanking.Num());
					}					
				}
			}
		}
	}
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

const FString UDTFluxDataStorage::GetConcurrentFormatedName(int Bib, bool Truncate, int MaxSize)
{
	{
		for(const auto& Participant : GetParticipants())
		{
			if(Participant.Bib == Bib)
			{
				return Participant.GetParticipantFormatedName(Truncate, MaxSize);
			}
		}
		return "";
	};
}
