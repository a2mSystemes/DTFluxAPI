# DTFluxAPI plugin doc

## DESCRIPTION

This document describe the usage of the plugin.

## USAGE
### Plugin Project Settings

#### Race Result API

- Race Result Port (int) : Port of the Race Result API (Local Server ONLY)
- Race Result Url (FString) : Race Result API URL (Local Server ONLY)
- Access Is Local (bool) : Race Result API only in local Mode (prevent polling limits)
- RaceResultSessionID (FString) : Race result session ID
- Start List Access Token (FString) : Token to access Start List endpoint
- General Classification Access Token (FString) : Token to access General Classification endpoint
- Live Stage Results Access Token (FString) : Token to access Live Stage Results endpoint

#### Chrono Proxy

- ProxyAddress
- ProxyRootPath
- ProxyPort

#### Server Config

- InPort (int) : Listening port of the embedded server 
- Endpoints (FString[] ) : Array of endpoints to be served (Wil be modified in the future)

#### Objects provided

- EDTFluxProxyRoute : Routes type for the Proxy
- EDTFluxAPIRoute : Routes type for the API Race Result
- FSearchFilters : struct that contain a ContestId, a StageId and a gender to filter api searches

#### Functions provided

All the function provided by the Project Settings are available both in blueprint and in c++

- FString GetAPIPath(APIRouteType, FString Filters )
- GetAPIPathFiltered(APIRouteType, FSearchFilters Filters )
- GetProxyPath( ProxyRouteType, int ContestId, int StageId)


### Model

### DataStorage


#### Public Data

##### Collections

###### Chrono

- Chrono of each stage (count down)
- Current Stage and current contest

###### Participant

- Current progression (pourcentage done in stage -> number of checkpoints done)
- 




### Subsystem


##### Delegates :

- 

- 
