enum class LoRaErrorCode
{
    kSucces = 0,
    // Errors returned by LoRa
    kError1InvalidCommandFormat = 1,
    kError2InvallidCommand,
    kError3InvalidOperator,
    kError4InvalidParameter,
    kError5OperationIsNotAllowed,
    // Errors returned by microcontroller
    kInvalidResponse = 10, // Response was not what was expected
    kNoResponse,           // No response was received in TimeOutInMillisecond
    kInvalidParameter,
    kCommandEchoNotReceived,
    kMissingOk,
    kMissingSettingClarification,
};
