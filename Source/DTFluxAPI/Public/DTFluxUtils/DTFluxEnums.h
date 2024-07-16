// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DTFluxEnums.generated.h"

/**
 * 
 */
UENUM()
enum EDTFluxParticipantStatusType : uint8
{
	Normal = 0 UMETA(DisplayName="Normal"),
	OutOfRace = 1 UMETA(DisplayName="HorsCourse"),
	DSQ = 2 UMETA(DisplayName="Disqualifié"),
	DNF = 3 UMETA(DisplayName="Abandon"),
	DNS = 4 UMETA(DisplayName="NonPartant"),
	NotLinedUp = 5 UMETA(DisplayName="NonPresentAuDépart"),
};

UENUM()
enum EDTFluxFinisherType : uint8
{
	Winner = 0 UMETA(DisplayName="Winner"),
	Spotter = 1 UMETA(DisplayName="Spotter"),
	Finish = 2 UMETA(DisplayName="Finish"),
};

UENUM(BlueprintType, Category="DTFlux|Server")
enum EDTFluxResponseErrorCode
{
	Unknown_Error UMETA(DisplayName="Unknown Error"),
	InvalidBody_Error UMETA(DisplayName="Invalid Body"),
	InvalidRequest_Error UMETA(DisplayName="Invalid Request"),
	Internal_Error UMETA(DisplayName="Internal Server Error")
};

UENUM(BlueprintType, Category="DTFlux|Subsystem")
enum EDTFluxResponseType: uint8
{
	UnknownResponse = 0 UMETA(DisplayName="UnknownResponse"),
	RaceData = 1 UMETA(DisplayName="RaceData"),
	ContestRanking = 2 UMETA(DisplayName="ContestRanking"),
	StageRanking = 3 UMETA(DisplayName="StageRanking"),
	SplitRanking = 4 UMETA(DisplayName="SplitRanking"),
	TeamList = 5 UMETA(DisplayName="TeamList"),
	TeamUpdate = 6 UMETA(DisplayName="TeamUpdate"),
	SplitSensor = 7 UMETA(DisplayName="SplitSensor"),
	StatusUpdate = 8 UMETA(DisplayName="StatusUpdate"),
	WsConnected = 9 UMETA(DisplayName="WsConnected"),
	WsClosed = 10 UMETA(DisplayName="WsClosed"),
	WsError = 11 UMETA(DisplayName="WsError"),
};

UENUM(BlueprintType, Category="DTFlux|Subsystem")
enum EDTFluxSplitType : uint8
{
	UnknownSplitType = 0 UMETA(DisplayName="UnknownSplitType"),
	NormalSplit = 1 UMETA(DisplayName="NormalSplit"),
	PreFinnishSplit = 2 UMETA(DisplayName="PreFinnishSplit"),
	FinishSplit = 3 UMETA(DisplayName="FinishSplit"),
};


UENUM(BlueprintType, Category="DTFlux|DataStorage")
// ReSharper disable once IdentifierTypo
enum EDTFluxDataStorageEventType : uint8
{
	UnknownEvent = 0 UMETA(DisplayName="ParticipantUpdateEvent"),
	ParticipantCreateEvent = 1 UMETA(DisplayName="ParticipantUpdateEvent"),
	ParticipantUpdateEvent = 2 UMETA(DisplayName="ParticipantUpdateEvent"),
	ParticipantDeleteEvent = 3 UMETA(DisplayName="ParticipantDeleteEvent"),
	ParticipantStatusUpdateEvent = 4 UMETA(DisplayName="ParticipantUpdateEvent"),
	RaceDataCreateEvent = 5 UMETA(DisplayName="RaceDataCreateEvent"),
	RaceDataUpdateEvent = 6 UMETA(DisplayName="RaceDataUpdateEvent"),
	RaceDataDeleteEvent = 7 UMETA(DisplayName="RaceDataDeleteEvent"),
	ContestRankingUpdate = 8 UMETA(DisplayName="ContestRankingUpdate"),
	StageRankingUpdate = 9 UMETA(DisplayName="StageRankingUpdate"),
	SplitRankingUpdate = 10 UMETA(DisplayName="SplitRankingUpdate"),
};


UENUM()
enum EDTFluxTimerEventType : uint8
{
	StageStart = 0 UMETA(DisplayName="StageStart"),
	StageCutOff = 1 UMETA(DisplayName="StageCutOff"),

};

UENUM()
enum EDTFluxStageStatusType : uint8
{
	UnknownStatus = 0 UMETA(DisplayName="UnknownStatus"),
	StageWaiting = 1 UMETA(DisplayName="StageWaiting"),
	StageStarted = 2 UMETA(DisplayName="StageStarted"),
	StageEnded = 3 UMETA(DisplayName="StageCutOff")
};


