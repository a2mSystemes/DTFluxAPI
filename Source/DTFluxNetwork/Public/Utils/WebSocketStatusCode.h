#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EWebSocketCloseCode : int32
{
    // No Error
    NoError = 0 UMETA(DisplayName="No Errors"),
    
    // Normal Closure.
    Normal = 1000 UMETA(DisplayName="Normal Closure"),

    // The endpoint is going away, either because of a server shutdown or because the browser is navigating away from the page that opened the connection.
    GoingAway = 1001 UMETA(DisplayName="Going Away"),

    // A protocol error occurred.
    ProtocolError = 1002 UMETA(DisplayName="Protocol Error"),

    // Unsupported data type received.
    UnsupportedData = 1003 UMETA(DisplayName="Unsupported Data"),

    // Reserved for future use.
    Reserved = 1004 UMETA(DisplayName="Reserved"),

    // Indicates no status code was provided (must not be sent over the wire).
    NoStatus = 1005 UMETA(DisplayName="No Status Received"),

    // Connection closed abnormally without receiving a close code.
    Abnormal = 1006 UMETA(DisplayName="Abnormal Closure"),

    // Invalid frame payload data (e.g., invalid UTF-8 text).
    InvalidPayload = 1007 UMETA(DisplayName="Invalid Frame Payload Data"),

    // Message violates policy; rejected.
    PolicyViolation = 1008 UMETA(DisplayName="Policy Violation"),

    // Message too big for processing.
    MessageTooBig = 1009 UMETA(DisplayName="Message Too Big"),

    // Mandatory extension not supported by server.
    MandatoryExtension = 1010 UMETA(DisplayName="Mandatory Extension"),

    // Internal server error while processing the connection.
    InternalServerError = 1011 UMETA(DisplayName="Internal Server Error"),

    // Reserved for future use.
    ServiceRestart = 1012 UMETA(DisplayName="Service Restart"),

    // Try again later.
    TryAgainLater = 1013 UMETA(DisplayName="Try Again Later"),

    // TLS handshake failure.
    TlsHandshake = 1015 UMETA(DisplayName="TLS Handshake")
};

/**
 * EDTFluxProtocolError
 * @brief Enum designed for Protocol link errors
 * @author Ange-Marie MAURIN
 */
UENUM()
enum class EDTFluxProtocolError : uint8
{
    NoErrors = 0,
	UnknownError = 1,
	ConnectionError = 2,
	MaxConnectionsError = 3,
	TimoutError=4,
	SerializationError = 5,
	DeserializationError=6
};
