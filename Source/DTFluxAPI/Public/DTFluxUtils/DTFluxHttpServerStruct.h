// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DTFluxHttpServerStruct.generated.h"

UENUM(BlueprintType)
enum EDTFluxHttpServerVerbs : uint8
{
	HTTP_GET,
	HTTP_POST,
	HTTP_PUT,
	HTTP_PATCH,
	HTTP_DELETE,
};

// USTRUCT(BlueprintType, meta=(DisplayName="DT Http Server Headers", HasNativeBreak = "DTHttpServer.DTHttpServerBPLib.BreakHeaders"))
USTRUCT(BlueprintType, Category="DTFlux|Server", meta=(HasNativeBreak = "DTFluxAPI.DTFluxHttpServerBPFn.BreakHeaders"))
struct FDTFluxHttpServerHeaders
{
	GENERATED_BODY()
	TMap<FString, FString> Headers;
};

USTRUCT(BlueprintType, Category="DTFlux|Server", meta=(HasNativeBreak = "DTFluxAPI.DTFluxHttpServerBPFn.BreakBody"))
struct FDTFluxHttpServerBody
{
	GENERATED_BODY()
	TArray<uint8> ReqBody;
};

// USTRUCT(BlueprintType, meta=(DisplayName="DTFlux Server Params", HasNativeBreak = "DTHttpServer.DTHttpServerBPLib.BreakParams"))
USTRUCT(BlueprintType, Category="DTFlux|Server", meta=(HasNativeBreak = "DTFluxAPI.DTFluxHttpServerBPFn.BreakParams"))
struct FDTFluxHttpServerParams
{
	GENERATED_BODY()
	TMap<FString, FString> Params;
};

UCLASS(NotBlueprintable, NotBlueprintType)
class DTFLUXAPI_API UDTFluxHttpServerBPFn : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Break DTFlux Http Server Params
	UFUNCTION(BlueprintPure, meta = (DisplayName="Break Http Server Params"), Category = "DT Http Server|Params")
	static void BreakParams(const FDTFluxHttpServerParams& HttpServerParams, TMap<FString, FString>& Params);

	// Find DTFlux Http Server Params
	UFUNCTION(BlueprintPure, meta = (DisplayName="Find Http Server Params"), Category = "DT Http Server|Params")
	static void FindParam(const FDTFluxHttpServerParams& HttpServerParams, const FString & Key, FString & Param );
	
	// Break DTFlux Http Server Headers
	UFUNCTION(BlueprintPure, meta = (DisplayName="Break Http Server Headers"), Category = "DT Http Server|Headers")
	static void BreakHeaders(const FDTFluxHttpServerHeaders& HttpServerHeaders, TMap<FString, FString>& Headers);

	// Break DTFlux Request Body
	UFUNCTION(BlueprintPure, meta = (DisplayName="Break Http Server Headers"), Category = "DT Http Server|Headers")
	static void BreakBody(const FDTFluxHttpServerBody& HttpBody, TArray<uint8> RawBody);
	
	// Find DTFlux Http Server Headers
	UFUNCTION(BlueprintPure, meta = (DisplayName="Find Http Server Headers"), Category = "DT Http Server|Headers")
	static void FindHeader(const FDTFluxHttpServerHeaders& HttpServerHeaders, const FString & Key, FString & Header );
};
