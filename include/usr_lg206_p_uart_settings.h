#ifndef USR_LG206_P_UART_SETTINGS_H_
#define USR_LG206_P_UART_SETTINGS_H_
#include <Arduino.h>

namespace LoRaUartSettings
{
    enum Baudrate
    {
        baudrate_1200 = 1200,
        baudrate_2400 = 2400,
        baudrate_4800 = 4800,
        baudrate_9200 = 9200,
        baudrate_19200 = 19200,
        baudrate_38400 = 38400,
        baudrate_57600 = 57600,
        baudrate_115200 = 115200,
    };
    enum Parity
    {
        parity_undefined = -1,
        parity_none = 1,
        parity_even = 2,
        parity_odd = 3,
    };
    enum Flowcontrol
    {
        flowcontrol_undefined = -1,
        flowcontrol_485 = 1,
        flowcontrol_nfc = 2,
    };
    /**
     * @brief Class used to store set and retrieve UART settings
     *
     */
    class LoRaUartSettings
    {
    public:
        Baudrate buadrate;
        char dataBits;
        char stopBits;
        Parity parity;
        Flowcontrol flowControl;

        explicit LoRaUartSettings(bool usingFactorySettings = false);
        String toString(void);
        int fromString(String);

        bool operator==(const LoRaUartSettings &b) const;
        bool operator!=(const LoRaUartSettings &b) const;
    };

#pragma region helper_functions_for_enums
    inline const char *ToString(Flowcontrol flowcontrol)
    {
        switch (flowcontrol)
        {
        case flowcontrol_485:
            return "485";
        case flowcontrol_nfc:
            return "NFC";
        default:
            return "Undefined flowcontrol";
        }
    };
    inline Flowcontrol FlowcontrolFromString(String data)
    {
        if (data == "485")
        {
            return flowcontrol_485;
        }
        else if (data == "NFC")
        {
            return flowcontrol_nfc;
        }

        return flowcontrol_undefined;
    };

    inline const char *ToString(Parity parity)
    {
        switch (parity)
        {
        case parity_none:
            return "NONE";
        case parity_even:
            return "EVEN";
        case parity_odd:
            return "ODD";
        default:
            return "Undefined parity";
        }
    };
    inline Parity ParityFromString(String data)
    {
        if (data == "NONE")
        {
            return parity_none;
        }
        else if (data == "EVEN")
        {
            return parity_even;
        }
        else if (data == "ODD")
        {
            return parity_odd;
        }

        return parity_undefined;
    };

#pragma endregion helper_functions_for_enums

} // namespace LoRaUartSettings

#endif // USR_LG206_P_UART_SETTINGS_H_