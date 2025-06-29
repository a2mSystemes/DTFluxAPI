// Fill out your copyright notice in the Description page of Project Settings.

#include "Types/Struct/DTFluxRankingStructs.h"
#include "DTFluxCoreModule.h"

void FDTFluxContestRanking::Dump() const
{
	UE_LOG(logDTFluxCore, Log,
		TEXT("FDTFluxContestRanking ->> \n \"rank\" : %d, Participant with Bib %d \"Gap\" : %s, \"Time\" : %s "),
		Rank, Bib, *Gap, *Time );
};

void FDTFluxStageRanking::Dump() const
{	
	UE_LOG(logDTFluxCore, Log, TEXT("RANKING : %02d. Participant bib %d %s %s %s %s %s"),
		Rank, Bib, *Gap, *TimeSwim,
		*TimeTransition, *TimeRun, *StartTime.ToString());
}




void FDTFluxSplitRanking::Dump() const
{
	UE_LOG(logDTFluxCore, Log, TEXT("SplitGapItem"))
	// Participant.Dump();
	UE_LOG(logDTFluxCore, Log, TEXT("Bib %02d Rank %02d Gap %s Time %s"), Bib, Rank, *Gap, *Time);
}

