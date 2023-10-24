enum class LoRaErrorCode
{
    kSucces = 0,
    // Errors returned by LoRa
    kError1InvalidCommandFormat = 1,
    kError2InvallidCommand = 2,
    kError3InvalidOperator = 3,
    kError4InvalidParameter = 4,
    kError5OperationIsNotAllowed = 5,
    // Errors returned by microcontroller
    kInvalidResponse = 10, // Response was not what was expected
    kNoResponse,           // No response was received in TimeOutInMillisecond
    kInvalidParameter,
    kCommandEchoNotReceived,
    kMissingOk,
    kMissingSettingClarification,
};
