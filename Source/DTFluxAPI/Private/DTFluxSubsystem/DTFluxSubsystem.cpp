// Fill out your copyright notice in the Description page of Project Settings.

#include "DTFluxSubsystem/DTFluxSubsystem.h"
#include "DTFluxProjectSettings/DTFluxProjectSettings.h"
#include "DTFluxModel/DTFluxModel.h"
#include "HttpServerModule.h"
#include "HttpRouteHandle.h"
#include "DTFluxAPILog.h"
#include "IHttpRouter.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"

// TODO: Not implemented
bool UDTFluxSubsystem::OnRequest(const FHttpServerRequest& Request)
{
	return true;
}
// TODO: Not implemented
void UDTFluxSubsystem::HandleRequest(const FString& Route,const FHttpServerRequest& Request, FHttpResultCallback OnComplete)
{
	// creating payload string
	FString ReqPayload;
	if (Request.Body.Num() > 0)
	{
		const std::string RawBody((char*)Request.Body.GetData(), Request.Body.Num());
		ReqPayload = UTF8_TO_TCHAR(RawBody.c_str());
	}
	TSharedPtr<FJsonObject> JsonPayload;
	if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(ReqPayload), JsonPayload))
	{
		UE_LOG(LogDTFluxAPI, Error, TEXT("unable to parse JSON Payload\n%s"), *ReqPayload);
	}
	//Checking path

	UE_LOG(LogDTFluxAPI,Log, TEXT("Request Received for Route %s"), *Route);

		// EventStart
	if(Route == TEXT("/event/start"))
	{
		FDTFluxStartStagePayload StartStagePayload;
		FJsonObjectConverter::JsonObjectToUStruct<FDTFluxStartStagePayload>(JsonPayload.ToSharedRef(), &StartStagePayload, 0, 0);
		OnEventStartReceived.Broadcast(StartStagePayload);
	}
	else if(Route == TEXT("/team/create"))
	{
		// /team/create route
	}
	else if(Route == TEXT("/team/update"))
	{
		// /team/create route
	}	else if(Route == TEXT("/team/create"))
	{
		// /team/create route
	}
	// Default Route
	else
	{
		
	}

	//Preparing Response Header
	TUniquePtr<FHttpServerResponse> Response = CreateHttpServerResponse();

	// Adding Response Body
	FDTFluxResponseBody RespBody;
	RespBody.Success = TEXT("OK");
	std::string RespBody_c;
	RespBody_c = TCHAR_TO_UTF8(*RespBody.Deserialize());
	Response->Body.Append((const uint8*)RespBody_c.c_str(), RespBody_c.length());
	// Return the response
	OnComplete(MoveTemp(Response));

}

void UDTFluxSubsystem::OnUpdateStartList(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogDTFluxAPI, Error, TEXT("RaceResult Request failed"));
		return;
	}
	// Tricks because Payload root is an array
	const FString ModifiedData = FString::Printf(TEXT("{\"Participants\":%s}"), *Response->GetContentAsString());
	TSharedPtr<FJsonObject> Payload;
	if(!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(ModifiedData), Payload))
	{
		UE_LOG(LogDTFluxAPI, Error, TEXT("unable to parse JSON Payload****"));
	}else
	{
		FDTFluxStartListPayload StartList;
		if (FJsonObjectConverter::JsonObjectToUStruct<FDTFluxStartListPayload>(Payload.ToSharedRef(), &StartList, 0, 0))
		{
			UE_LOG(LogDTFluxAPI, Log, TEXT("Success Reading"))

		for(const auto& Participant : StartList.Participants)
		{
			// Creating a new Contest
				if(!Contests.Contests.Contains(Participant.ContestName))
				{
					FDTFluxContest NewContest;
					NewContest.ContestID = Participant.ContestID;
					NewContest.ContestName = Participant.ContestName;
					Contests.Contests.Add(Participant.ContestName, NewContest);
				}
				FDTFluxContest* CurrentContest = Contests.Contests.Find(Participant.ContestName);
			
				FDTFluxTeam Team;
				Team.Bib = Participant.Bib;
				FDTFluxParticipant ContestParticipant;
				ContestParticipant.FirstName = Participant.FirstName;
				ContestParticipant.LastName = Participant.LastName;
				ContestParticipant.Gender = Participant.Gender;
				ContestParticipant.Club = Participant.Club;
				ContestParticipant.Category = Participant.Category;

				Team.Participants.Add(ContestParticipant);
				// if we have LastName2 it is a two personne Team
				if(Participant.LastName2 != "")
				{
					FDTFluxParticipant ContestParticipant2;
					ContestParticipant2.FirstName = Participant.FirstName2;
					ContestParticipant2.LastName = Participant.LastName2;
					ContestParticipant2.Gender = Participant.Gender2;
					ContestParticipant2.Club = Participant.Club2;
					ContestParticipant2.Category = Participant.Category;
					Team.Participants.Add(ContestParticipant);
					Team.TeamName = Participant.TeamName;
					UE_LOG(LogDTFluxAPI, Log, TEXT("Participant is a team : TeamName \"%s\" "), *Team.TeamName);
				}
				else
				{
					Team.TeamName = Participant.FirstName + TEXT(" ") + Participant.LastName.ToUpper();
				}
			// check if Participant already exists

				if(!CurrentContest->TeamAlreadyExist(Team))
				{
					// Add it to the TeamList
					CurrentContest->AddTeam(Team);
					UE_LOG(LogDTFluxAPI, Log, TEXT("Adding Team  \"%s\" "), *Team.TeamName);
				}

			}
			UE_LOG(LogDTFluxAPI, Log, TEXT("Contest has now %i elements"), Contests.Contests.Num());


		}
		else
		{
			UE_LOG(LogDTFluxAPI, Error, TEXT("Error Deserializing Payload \n*%s*\n"), *ModifiedData);

		}
	}
	for(const auto& Element : Contests.Contests)
	{
		UE_LOG(LogDTFluxAPI, Log, TEXT("Contest %s"), *Element.Key);
		FDTFluxContest Contest = Element.Value;
		UE_LOG(LogDTFluxAPI, Log, TEXT("Number of participants in this contest %i"), Contest.TeamParticipants.Num());

	}
	
}

TUniquePtr<FHttpServerResponse> UDTFluxSubsystem::CreateHttpServerResponse() const
{
	// Create a Response to be returned by the server
	TUniquePtr<FHttpServerResponse> Response = MakeUnique<FHttpServerResponse>();
	Response->Code = EHttpServerResponseCodes::Ok;
	// Response Header
	Response->Headers.Add(TEXT("Content-Type"), { TEXT("application/json;charset=utf-8") });
	Response->Headers.Add(TEXT("Access-Control-Allow-Origin"), { TEXT("*") });
	Response->Headers.Add(TEXT("Access-Control-Allow-Methods"), { TEXT("GET,POST,PUT,PATCH,DELETE,OPTIONS") });
	Response->Headers.Add(TEXT("Access-Control-Allow-Headers"), { TEXT("Origin,X-Requested-With,Content-Type,Accept") });
	Response->Headers.Add(TEXT("Access-Control-Max-Age"), { TEXT("600") });
	Response->Headers.Add(TEXT("Access-Control-Allow-Credentials"), { TEXT("true") });
	Response->Headers.Add(TEXT("Server"), { TEXT("UE 5.4.1 EMBBEDED") });

	return Response;
}

void UDTFluxSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	mSettings = UDTFluxProjectSettings::GetDTFluxAPIProjectSettings();
	if(mSettings)
	{
		// Setting up request		
		HttpRequest = &FHttpModule::Get();
		HttpRouter = FHttpServerModule::Get().GetHttpRouter(mSettings->InPort);
		if(!HttpRouter)
		{
			UE_LOG(LogDTFluxAPI, Type::Error, TEXT("Invalid Http Router for port : %i"), mSettings->InPort)
		}
		// Setting Up Routes
		for(const TArray<FString> Routes = mSettings->Endpoints; const FString& Route : Routes)
		{
			const FHttpRequestHandler OptionRequestHandler = FHttpRequestHandler::CreateLambda([this](const FHttpServerRequest &Request, const FHttpResultCallback& OnComplete)
			{
				OnComplete(CreateHttpServerResponse());
				return true;
			});
			FHttpRouteHandle OptionRouteHandle = HttpRouter->BindRoute(Route, EHttpServerRequestVerbs::VERB_OPTIONS, OptionRequestHandler);
			HttpMountedMap.Add(Route + TEXT("HTTPOption"), OptionRouteHandle);

			const FHttpRequestHandler RequestHandler = FHttpRequestHandler::CreateLambda([this, Route](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
			{
				// Processing Request to Raw OnRequestReceived firing
				FDTFluxHttpServerHeaders Headers;
				for(const auto &Header: Request.Headers)
				{
					Headers.Headers.Add(Header.Key, FString::Join(Header.Value, TEXT(",")));
				}
				FDTFluxHttpServerParams Params;
				Params.Params = Request.QueryParams;
					
				FDTFluxHttpServerBody Payload;
				Payload.ReqBody = Request.Body;

				// Raw broadcasting Received event
				OnRequestReceived.Broadcast(Headers, Params, Payload);
				HandleRequest(Route, Request, OnComplete);
				return true;
			});
			// Binding Routes
			FHttpRouteHandle RouteHandle = HttpRouter->BindRoute(Route, EHttpServerRequestVerbs::VERB_POST, RequestHandler);
			HttpMountedMap.Add(Route, RouteHandle);
		}

	}
	
}

void UDTFluxSubsystem::Deinitialize()
{
	StopServer();
	UE_LOG(LogDTFluxAPI, Log, TEXT("Route Num %i"), HttpMountedMap.Num());
	for( auto const& Route: HttpMountedMap)
	{
		HttpRouter->UnbindRoute(Route.Value);
	}
	HttpMountedMap.Empty();
	HttpRouter.Reset();
	Super::Deinitialize();
}

TArray<FString> UDTFluxSubsystem::GetMountedRoutes() const
{
	TArray<FString> Mounted = TArray<FString>();
	for(const auto Route : HttpMountedMap)
	{
		Mounted.Add(Route.Key);
	}
	return Mounted;
}

void UDTFluxSubsystem::StartServer()
{
	if(bIsListening){return ;}
	FHttpServerModule::Get().StartAllListeners();
	bIsListening = true;
	OnServerListening.Broadcast();
}

TArray<FDTFluxTeam> UDTFluxSubsystem::GetParticipantsByContestId(const int ContestId)
{
	if(Contests.Contests.Num() != 0)
	{
		FString ContestName = Contests.GetContestName(ContestId);
		UE_LOG(LogDTFluxAPI, Log, TEXT("Getting Participants for Contest %s"), *ContestName);
		return GetParticipantsByContestName(ContestName);
	}
	else
	{
		UE_LOG(LogDTFluxAPI, Error, TEXT("No Contest Yet !!!!"));
		TArray<FDTFluxTeam> EmptyTeam;
		return EmptyTeam;
	}

}

TArray<FDTFluxTeam> UDTFluxSubsystem::GetParticipantsByContestName(const FString ContestName)
{
	if(Contests.Contests.Num() != 0)
	{
		return Contests.Contests[ContestName].TeamParticipants;

	}
	else
	{
		UE_LOG(LogDTFluxAPI, Error, TEXT("No Contest Yet !!!!"));
		TArray<FDTFluxTeam> EmptyTeam;
		return EmptyTeam;
	}
}

FString UDTFluxSubsystem::GetContestName(const int ContestId)
{
	if(Contests.GetContestName(ContestId) != "")
	{
		return Contests.GetContestName(ContestId);
	}
	return TEXT("");
}

void UDTFluxSubsystem::UpdateStartList()
{
	const TSharedRef<IHttpRequest> Req = HttpRequest->CreateRequest();
	Req->SetVerb("GET");
	Req->SetURL(mSettings->GetAPIPath(EDTFluxAPIRoute::Starters));
	Req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Req->OnProcessRequestComplete().BindUObject(this, &UDTFluxSubsystem::OnUpdateStartList);

	Req->ProcessRequest();
}

void UDTFluxSubsystem::UpdateClassification(const int& ContestId, const int& StageId)
{
}

TArray<FDTFluxParticipant> UDTFluxSubsystem::GetClassification(const int& ContestId, const int& StageId)
{
	return TArray<FDTFluxParticipant>();
}

void UDTFluxSubsystem::StopServer()
{
	FHttpServerModule::Get().StopAllListeners();
	bIsListening = false;
	OnServerStopped.Broadcast();
}
