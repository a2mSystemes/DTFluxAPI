#pragma once

#include "CoreMinimal.h"
#include "HttpStatusCode.generated.h"

UENUM()
enum class EHttpResponseCode: int32
{
    // status code not set yet
    Unknown = 0 UMETA(DisplayName="Unknown"),
    
    // the request can be continued.
    Continue = 100 UMETA(DisplayName="Continue"),
    
    // the server has switched protocols in an upgrade header.
    SwitchProtocol = 101 UMETA(DisplayName="Switching Protocols"),
    
    // the request completed successfully.
    Ok = 200 UMETA(DisplayName="OK"),
    
    // the request has been fulfilled and resulted in the creation of a new resource.
    Created = 201 UMETA(DisplayName="Created"),
    
    // the request has been accepted for processing, but the processing has not been completed.
    Accepted = 202 UMETA(DisplayName="Accepted"),
    
    // the returned meta information in the entity-header is not the definitive set available from the origin server.
    Partial = 203 UMETA(DisplayName="Non-Authoritative Information"),
    
    // the server has fulfilled the request, but there is no new information to send back.
    NoContent = 204 UMETA(DisplayName="No Content"),
    
    // the request has been completed, and the client program should reset the document view that caused the request to be sent to allow the user to easily initiate another input action.
    ResetContent = 205 UMETA(DisplayName="Reset Content"),
    
    // the server has fulfilled the partial get request for the resource.
    PartialContent = 206 UMETA(DisplayName="Partial Content"),
    
    // the server couldn't decide what to return.
    Ambiguous = 300 UMETA(DisplayName="Multiple Choices"),
    
    // the requested resource has been assigned to a new permanent uri (uniform resource identifier), and any future references to this resource should be done using one of the returned uris.
    Moved = 301 UMETA(DisplayName="Moved Permanently"),
    
    // the requested resource resides temporarily under a different uri (uniform resource identifier).
    Redirect = 302 UMETA(DisplayName="Found"),
    
    // the response to the request can be found under a different uri (uniform resource identifier) and should be retrieved using a get http verb on that resource.
    RedirectMethod = 303 UMETA(DisplayName="See Other"),
    
    // the requested resource has not been modified.
    NotModified = 304 UMETA(DisplayName="Not Modified"),
    
    // the requested resource must be accessed through the proxy given by the location field.
    UseProxy = 305 UMETA(DisplayName="Use Proxy"),
    
    // the redirected request keeps the same http verb. http/1.1 behavior.
    RedirectKeepVerb = 307 UMETA(DisplayName="Temporary Redirect"),
    
    // the request could not be processed by the server due to invalid syntax.
    BadRequest = 400 UMETA(DisplayName="Bad Request"),
    
    // the requested resource requires user authentication.
    Denied = 401 UMETA(DisplayName="Unauthorized"),
    
    // not currently implemented in the http protocol.
    PaymentReq = 402 UMETA(DisplayName="Payment Required"),
    
    // the server understood the request, but is refusing to fulfill it.
    Forbidden = 403 UMETA(DisplayName="Forbidden"),
    
    // the server has not found anything matching the requested uri (uniform resource identifier).
    NotFound = 404 UMETA(DisplayName="Not Found"),
    
    // the http verb used is not allowed.
    BadMethod = 405 UMETA(DisplayName="Method Not Allowed"),
    
    // no responses acceptable to the client were found.
    NoneAcceptable = 406 UMETA(DisplayName="Not Acceptable"),
    
    // proxy authentication required.
    ProxyAuthReq = 407 UMETA(DisplayName="Proxy Authentication Required"),
    
    // the server timed out waiting for the request.
    RequestTimeout = 408 UMETA(DisplayName="Request Timeout"),
    
    // the request could not be completed due to a conflict with the current state of the resource. the user should resubmit with more information.
    Conflict = 409 UMETA(DisplayName="Conflict"),
    
    // the requested resource is no longer available at the server, and no forwarding address is known.
    Gone = 410 UMETA(DisplayName="Gone"),
    
    // the server refuses to accept the request without a defined content length.
    LengthRequired = 411 UMETA(DisplayName="Length Required"),
    
    // the precondition given in one or more of the request header fields evaluated to false when it was tested on the server.
    PrecondFailed = 412 UMETA(DisplayName="Precondition Failed"),
    
    // the server is refusing to process a request because the request entity is larger than the server is willing or able to process.
    RequestTooLarge = 413 UMETA(DisplayName="Payload Too Large"),
    
    // the server is refusing to service the request because the request uri (uniform resource identifier) is longer than the server is willing to interpret.
    UriTooLong = 414 UMETA(DisplayName="URI Too Long"),
    
    // the server is refusing to service the request because the entity of the request is in a format not supported by the requested resource for the requested method.
    UnsupportedMedia = 415 UMETA(DisplayName="Unsupported Media Type"),
    
    // too many requests, the server is throttling
    TooManyRequests = 429 UMETA(DisplayName="Too Many Requests"),
    
    // the request should be retried after doing the appropriate action.
    RetryWith = 449 UMETA(DisplayName="Retry With"),
    
    // the server encountered an unexpected condition that prevented it from fulfilling the request.
    ServerError = 500 UMETA(DisplayName="Internal Server Error"),
    
    // the server does not support the functionality required to fulfill the request.
    NotSupported = 501 UMETA(DisplayName="Not Implemented"),
    
    // the server, while acting as a gateway or proxy, received an invalid response from the upstream server it accessed in attempting to fulfill the request.
    BadGateway = 502 UMETA(DisplayName="Bad Gateway"),
    
    // the service is temporarily overloaded.
    ServiceUnavailable = 503 UMETA(DisplayName="Service Unavailable"),
    
    // the request was timed out waiting for a gateway.
    GatewayTimeout = 504 UMETA(DisplayName="Gateway Timeout"),
    
    // the server does not support, or refuses to support, the http protocol version that was used in the request message.
    VersionNotSup = 505 UMETA(DisplayName="HTTP Version Not Supported")
};
