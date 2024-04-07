#ifndef USR_LG206_P_UART_SETTINGS_H_
#define USR_LG206_P_UART_SETTINGS_H_
#include <Arduino.h>

namespace LoRaUartSettings
{
    enum class Baudrate : long
    {
        baudrate_undefined = 0,
        baudrate_1200 = 1200,
        baudrate_2400 = 2400,
        baudrate_4800 = 4800,
        baudrate_9200 = 9200,
        baudrate_19200 = 19200,
        baudrate_38400 = 38400,
        baudrate_57600 = 57600,
        baudrate_115200 = 115200,
    };
    enum class Parity
    {
        parity_undefined = 0,
        parity_none = 1,
        parity_even = 2,
        parity_odd = 3,
    };
    enum class Flowcontrol
    {
        flowcontrol_undefined = 0,
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
        String toString(void) const;
        int fromString(String);

        bool operator==(const LoRaUartSettings &b) const;
        bool operator!=(const LoRaUartSettings &b) const;
    };

#pragma region helper_functions_for_enums
    inline const char *ToString(Flowcontrol flowcontrol)
    {
        switch (flowcontrol)
        {
        case Flowcontrol::flowcontrol_485:
            return "485";
        case Flowcontrol::flowcontrol_nfc:
            return "NFC";
        default:
            return "Undefined flowcontrol";
        }
    };
    inline Flowcontrol FlowcontrolFromString(String data)
    {
        if (data == "485")
        {
            return Flowcontrol::flowcontrol_485;
        }
        else if (data == "NFC")
        {
            return Flowcontrol::flowcontrol_nfc;
        }

        return Flowcontrol::flowcontrol_undefined;
    };

    inline const char *ToString(Parity parity)
    {
        switch (parity)
        {
        case Parity::parity_none:
            return "NONE";
        case Parity::parity_even:
            return "EVEN";
        case Parity::parity_odd:
            return "ODD";
        default:
            return "Undefined parity";
        }
    };
    inline Parity ParityFromString(String data)
    {
        if (data == "NONE")
        {
            return Parity::parity_none;
        }
        else if (data == "EVEN")
        {
            return Parity::parity_even;
        }
        else if (data == "ODD")
        {
            return Parity::parity_odd;
        }

        return Parity::parity_undefined;
    };
    inline const char *ToString(Baudrate baudrate)
    {
        switch (baudrate)
        {
        case Baudrate::baudrate_1200:
            return "1200";
        case Baudrate::baudrate_2400:
            return "2400";
        case Baudrate::baudrate_4800:
            return "4800";
        case Baudrate::baudrate_9200:
            return "9200";
        case Baudrate::baudrate_19200:
            return "19200";
        case Baudrate::baudrate_38400:
            return "38400";
        case Baudrate::baudrate_57600:
            return "57600";
        case Baudrate::baudrate_115200:
            return "115200";
        default:
            return "Undefined parity";
        }
    };
    inline Baudrate BaudrateFromString(String data)
    {
        if (data == "1200")
        {
            return Baudrate::baudrate_1200;
        }
        else if (data == "2400")
        {
            return Baudrate::baudrate_2400;
        }
        else if (data == "4800")
        {
            return Baudrate::baudrate_4800;
        }
        else if (data == "9200")
        {
            return Baudrate::baudrate_9200;
        }
        else if (data == "19200")
        {
            return Baudrate::baudrate_19200;
        }
        else if (data == "38400")
        {
            return Baudrate::baudrate_38400;
        }
        else if (data == "57600")
        {
            return Baudrate::baudrate_57600;
        }
        else if (data == "115200")
        {
            return Baudrate::baudrate_115200;
        }

        return Baudrate::baudrate_undefined;
    };

#pragma endregion helper_functions_for_enums

} // namespace LoRaUartSettings

#endif // USR_LG206_P_UART_SETTINGS_H_