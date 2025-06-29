// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/**
 * 
 */
class DTFLUXUTILITIES_API DTFluxDatesUtilities
{
public:
	DTFluxDatesUtilities();
	~DTFluxDatesUtilities();
	
	static bool CompileDateAndTime(const FString& Time, const FString& Date, FDateTime& OutDateTime);
	static bool CompileDateAndTime(const FDateTime& Time, const FDateTime& Date, FDateTime& OutDateTime);
};
