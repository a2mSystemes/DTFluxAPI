// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#include "DTFluxUtils/DTFluxHttpServerStruct.h"

void UDTFluxHttpServerBPFn::BreakParams(const FDTFluxHttpServerParams& HttpServerParams, TMap<FString, FString>& Params)
{
	Params = HttpServerParams.Params;
}

void UDTFluxHttpServerBPFn::FindParam(const FDTFluxHttpServerParams& HttpServerParams, const FString& Key, FString& Param)
{
	Param.Empty();
	if ( const FString * pParam = HttpServerParams.Params.Find(Key) )
	{
		Param = *pParam;
	}
}

void UDTFluxHttpServerBPFn::BreakHeaders(const FDTFluxHttpServerHeaders& HttpServerHeaders, TMap<FString, FString>& Headers)
{
	Headers = HttpServerHeaders.Headers;
}

void UDTFluxHttpServerBPFn::BreakBody(const FDTFluxHttpServerBody& HttpBody, TArray<uint8> RawBody)
{
	RawBody = HttpBody.ReqBody;
}

void UDTFluxHttpServerBPFn::FindHeader(const FDTFluxHttpServerHeaders& HttpServerHeaders, const FString& Key, FString& Header)
{
	Header.Empty();
	if ( const FString * pHeader = HttpServerHeaders.Headers.Find(Key) )
	{
		Header = *pHeader;
	}
}

