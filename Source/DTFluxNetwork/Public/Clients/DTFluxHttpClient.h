// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

/**
 * 
 */
class DTFLUXNETWORK_API FDTFluxHttpClient: public TSharedFromThis<FDTFluxHttpClient>
{
public:
	FDTFluxHttpClient();
	~FDTFluxHttpClient();
};
