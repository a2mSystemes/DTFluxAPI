// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxDataStorage/DTFluxDataStorage.h"

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

bool UDTFluxDataStorage::GetContest(const int ContestId, FDTFluxContest& OutContest )
{
	// UE_LOG(LogDTFluxAPI, Warning, TEXT("RequestedContest %d"),
	// 	ContestId);
	for(auto& Contest : Contests)
	{
	// 	UE_LOG(LogDTFluxAPI, Warning, TEXT("Checking Contest %d"),
	// Contest.Id);
		if(Contest.Id == ContestId)
		{
			// UE_LOG(LogDTFluxAPI, Warning, TEXT("Found Contest %d"),
			// Contest.Id);
			OutContest = Contest;
			return true;
		}
	}
	// UE_LOG(LogDTFluxAPI, Error, TEXT("Contest %d not Found "),
	// ContestId);
	return false;
}

bool UDTFluxDataStorage::GetStage(const int ContestId, const int StageId, FDTFluxStage& OutStage)
{
	FDTFluxContest Contest;
	// UE_LOG(LogDTFluxAPI, Warning, TEXT("RequestedStage %d in Contest%02d ****"),
	// ContestId, StageId);
	if(GetContest(ContestId, Contest))
	{
		for(auto & Stage: Contest.Stages)
		{
// 			UE_LOG(LogDTFluxAPI, Warning, TEXT("Checking Stage %d "),
// Stage.Id);
			if(Stage.Id == StageId)
			{

				// UE_LOG(LogDTFluxAPI, Warning, TEXT("Found %s in Stage ***"),
		// *Stage.Name);
				OutStage = Stage;
				return true;
			}
		}
	}
	// UE_LOG(LogDTFluxAPI, Error, TEXT("Stage %d Not Found in Contest %d ****"),
	// StageId, ContestId);
	return false;
}

bool UDTFluxDataStorage::GetSplit(const int ContestId, const int StageId, const int SplitId, FDTFluxSplit& OutSplit)
{
	// DumpContest();
	FDTFluxStage Stage;
	// UE_LOG(LogDTFluxAPI, Warning, TEXT("RequestedSplit %d in Stage%02d of Contest%02d"),
	// 	ContestId, StageId, SplitId);
	if(GetStage(ContestId, StageId, Stage))
	{
		for(auto& Split: Stage.Splits)
		{
			if(Split.Id == SplitId)
			{
				// UE_LOG(LogDTFluxAPI, Warning, TEXT("Get Split %s in Stage%02d of Contest%02d"),
				// *Split.Name, StageId, SplitId);
				OutSplit = Split;
				return true;
			}
		}
	}
	return false;

}

FDTFluxSplitRanking UDTFluxDataStorage::AddSplitRanking(const FDTFluxSplitSensorItemResponse& SplitSensorItem)
{
	FDTFluxSplitRanking NewSplitRanking;
	NewSplitRanking.Bib = SplitSensorItem.Bib;
	NewSplitRanking.Gap = SplitSensorItem.Gap;
	NewSplitRanking.Rank = SplitSensorItem.Rank;
	NewSplitRanking.Time = SplitSensorItem.Time;
	FDTFluxSplit Split;
	if(GetSplit(SplitSensorItem.ContestID, SplitSensorItem.StageID,
		SplitSensorItem.SplitID, Split))
	{
		Split.SplitRankings.Add(NewSplitRanking);
		return NewSplitRanking;
	}
	UE_LOG(LogDTFluxAPI, Error,
		TEXT("Error, Cannot process split sensor."))
	UE_LOG(LogDTFluxAPI, Error, TEXT("Split %d from stage %d of Contest %d does not exist"),
		SplitSensorItem.SplitID, SplitSensorItem.StageID, SplitSensorItem.ContestID);
	return NewSplitRanking;
}

EDTFluxSplitType UDTFluxDataStorage::GetSplitStatus(int ContestID, int StageID, int SplitID)
{
	FDTFluxStage Stage;
	if(GetStage(ContestID, StageID, Stage))
	{
		int SplitCount = Stage.Splits.Num();
		FDTFluxSplit S;
		return Stage.GetSplitType(SplitID);
	}
	return EDTFluxSplitType::UnknownSplitType;
}

bool UDTFluxDataStorage::GetStageRankingForBib(int ContestID, int StageID, int Bib,
	FDTFluxStageRanking& OutStageRanking)
{
	FDTFluxStage Stage;
	GetStage(ContestID, StageID, Stage);
	for(auto& StageRanking : Stage.StageRanking)
	{
		if(StageRanking.Bib == Bib )
		{
			OutStageRanking = StageRanking;
			return true;
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
	TArray<FDTFluxStageRanking> StageRanking;
	FDTFluxStage Stage;
	if(GetStage(ContestId, StageId, Stage))
	{
		Stage.StageRanking.Sort([](FDTFluxStageRanking A, FDTFluxStageRanking B)
		{
			return A.Rank < B.Rank;
		});
		return Stage.StageRanking;
	}
	return StageRanking;
}

void UDTFluxDataStorage::AddOrUpdateContest(const FDTFluxContestResponse& ContestResponse)
{
	FDTFluxContest Contest;
	bool NewContest = false;
	int ContestIdx = 0;
	// UE_LOG(LogDTFluxAPI, Warning, TEXT("DateTime Json Contest \"%s\""), *ContestResponse.Date.ToString() );
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
	Contest.Date = ContestResponse.Date;
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
		// UE_LOG(LogDTFluxAPI, Warning, TEXT("ContestResponse.Stage StartTime = %s"), *StageResp.StartTime);
		// UE_LOG(LogDTFluxAPI, Warning, TEXT("ContestResponse.Stage EndTime = %s"), *StageResp.EndTime);
		// UE_LOG(LogDTFluxAPI, Warning, TEXT("ContestResponse.Stage CutOff = %s"), *StageResp.CutOff);		
		FTimespan StartTimeSpan;
		FTimespan::Parse(StageResp.StartTime, StartTimeSpan);
		FTimespan EndTimeSpan;
		FTimespan::Parse(StageResp.EndTime, EndTimeSpan);
		FTimespan CutOffTimeSpan;
		FTimespan::Parse(StageResp.CutOff, CutOffTimeSpan);
		Stage.StartTime = Contest.Date + StartTimeSpan;
		Stage.EndTime = Contest.Date + EndTimeSpan;
		Stage.CutOff = Stage.StartTime + CutOffTimeSpan;
		// UE_LOG(LogDTFluxAPI, Warning, TEXT("STAGE StartTime = %s"), *Stage.StartTime.ToString());
		// UE_LOG(LogDTFluxAPI, Warning, TEXT("STAGE EndTime = %s"), *Stage.EndTime.ToString());
		// UE_LOG(LogDTFluxAPI, Warning, TEXT("STAGE CutOff = %s"), *Stage.CutOff.ToString());

		Stage.Splits = Splits;
		Contest.Stages.Add(Stage);
	}
	if(NewContest)
	{
		// Contest.Dump();
		Contests.Add(Contest);
		return;
	}
	// Contest.Dump();
	Contests.RemoveAt(ContestIdx);
	Contests.Insert(Contest, ContestIdx);
	// handle updating contest
}

void UDTFluxDataStorage::AddOrUpdateParticipant(const FDTFluxTeamListItemResponse& TeamListItemResponse)
{
	// UE_LOG(LogDTFluxAPI, Log, TEXT("In DataStorage::AddOrUpdateParticipant"));
	// UE_LOG(LogDTFluxAPI, Log, TEXT("AboutToUpdateOrAdd Participant %d %s %s in Contest%02d "),
		// TeamListItemResponse.Bib, *TeamListItemResponse.FirstName, *TeamListItemResponse.LastName,
		// TeamListItemResponse.ContestId);
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
			// UE_LOG(LogDTFluxAPI, Log, TEXT("AboutToUpdateOrAdd Participant %d %s %s in Contest%02d "),
			// 	Participant.Bib, *Participant.Person1.FirstName, *Participant.Person1.LastName,
			// 	Contest.Id);

			Contest.AddParticipant(Participant);
			return;
		}
		// UE_LOG(LogDTFluxAPI, Log, TEXT("Contest%02d has now %04d Participants"), Contest.Id,
		// 	Contest.Participants.Num());

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
					for(auto& StageRankingResp: StageRankingResponse.Datas )
					{
						FDTFluxStageRanking NewStageRanking;
						NewStageRanking.TimeRun = StageRankingResp.TimeRun;
						FTimespan StartTimeSpan;
						FTimespan::Parse(StageRankingResp.TimeStart, StartTimeSpan);
						NewStageRanking.Time = StageRankingResp.Time;
						NewStageRanking.TimeStart = Contest.Date + StartTimeSpan;
						NewStageRanking.TimeTransition = StageRankingResp.TimeTransition;
						NewStageRanking.TimeSwim = StageRankingResp.TimeSwim;
						NewStageRanking.Bib = StageRankingResp.Bib;
						NewStageRanking.Gap = StageRankingResp.Gap;
						NewStageRanking.Rank = StageRankingResp.Rank;
						NewStageRanking.SpeedRunning = StageRankingResp.SpeedRunning;
						NewStageRanking.SpeedSwim = StageRankingResp.SpeedSwim;
						NewStageRanking.SpeedTotal = StageRankingResp.SpeedTotal;
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

// ReSharper disable once IdentifierTypo
TArray<FDTFluxStageRanking> UDTFluxDataStorage::GetPoursuitWithStageTime(const TArray<int> ContestIds, const int StageId, float DelaTimeSeconds)
{
	TArray<int> RemoveIdx;
	int Idx = 0;
	TArray<FDTFluxStageRanking> StagesRankingsTemp;
	TArray<FDTFluxStageRanking> ReturnStageRankings;
	FDateTime PresumedStartStage;
	for(const int ContestId : ContestIds)
	{
		FDTFluxStage StageTemp;
		if(GetStage(ContestId, StageId, StageTemp))
		{
			PresumedStartStage = StageTemp.StartTime;
			StagesRankingsTemp .Append(StageTemp.StageRanking);
			// ContestsRankings
		}
	}
	StagesRankingsTemp.Sort([](const FDTFluxStageRanking& A, const FDTFluxStageRanking& B)
	{
		return A.TimeStart < B.TimeStart;
	});
	FDateTime MassStartDate = PresumedStartStage + FTimespan::FromSeconds(DelaTimeSeconds) ;
	for( auto & StageRanking : StagesRankingsTemp)
	{
		if ( StageRanking.TimeStart < MassStartDate )
		{
			ReturnStageRankings.Add(StageRanking);
		}
	}
	return ReturnStageRankings;

}
// ReSharper disable once IdentifierTypo
TArray<FDTFluxStageRanking> UDTFluxDataStorage::GetPoursuitWithTimeStart(const TArray<int> ContestIds, const int StageId, float DelaTimeSeconds)
{
	TArray<int> RemoveIdx;
	int Idx = 0;
	TArray<FDTFluxStageRanking> StagesRankingsTemp;
	TArray<FDTFluxStageRanking> ReturnStageRankings;
	for(const int ContestId : ContestIds)
	{
		FDTFluxStage StageTemp;
		if(GetStage(ContestId, StageId, StageTemp))
		{
			StagesRankingsTemp .Append(StageTemp.StageRanking);
			// ContestsRankings
		}
	}
	StagesRankingsTemp.Sort([](const FDTFluxStageRanking& A, const FDTFluxStageRanking& B)
	{
		return A.TimeStart < B.TimeStart;
	});
	FDateTime MassStartDate;
	if (!StagesRankingsTemp.IsEmpty())
	{
		MassStartDate = StagesRankingsTemp[0].TimeStart + FTimespan::FromSeconds(DelaTimeSeconds) ;

	}
	for( auto & StageRanking : StagesRankingsTemp)
	{
		if ( StageRanking.TimeStart < MassStartDate )
		{
			ReturnStageRankings.Add(StageRanking);
		}
	}
	return ReturnStageRankings;

}

bool UDTFluxDataStorage::GetFirstStageOfContest(const int ContestId, FDTFluxStage& Stage)
{
	if(Contests.IsEmpty())
	{
		return false;
	}
	for (auto& Contest : Contests)
	{
		if(Contest.Id == ContestId)
		{
			Contest.Stages.Sort([](const FDTFluxStage& A, const FDTFluxStage& B)
			{
				return A.Id < B.Id;
			});
			if(Contest.Stages.IsValidIndex(0))
			{
				Stage =  Contest.Stages[0];
				return true;
			}
			return false;

		}
	}
	return false;
}

void UDTFluxDataStorage::DumpContest()
{
	for(const auto& Contest : Contests)
	{
		UE_LOG(LogDTFluxAPI, Warning, TEXT("Contest%02d with name %s : Date %s\n"),
			Contest.Id, *Contest.Name, *Contest.Date.ToString());
		// UE_LOG(LogDTFluxAPI, Warning, TEXT("Participants :\n"));
		// for(const auto& Participant : Contest.Participants)
		// {
		// 	Participant.Dump();
		// }
		UE_LOG(LogDTFluxAPI, Warning, TEXT("Stages :\n"));
		for(const auto& Stage : Contest.Stages)
		{
			Stage.Dump();
		}
		UE_LOG(LogDTFluxAPI, Warning, TEXT("ContestRanking :\n"));
		for(const auto& ContestRankingItem  : Contest.ContestRanking)
		{
			ContestRankingItem.Dump();
		}

	}
}


