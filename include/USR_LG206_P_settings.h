#ifndef USR_LG206_P_SETTINGS_H_
#define USR_LG206_P_SETTINGS_H_
#include <Arduino.h>

/**
 * @brief SETTING_UNDEFINED is used as a identifier for not yet retrieved or set settings
 *
 */
#define SETTING_UNDEFINED -1

/**
 * @brief All settings for workmode
 *
 */
enum WorkMode
{
    work_mode_undefined = -1,
    work_mode_transparent = 1,
    work_mode_fixed_point = 2
};

/**
 * @brief All settings for baudrate
 *
 */
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

/**
 * @brief All settings for parity
 *
 */
enum Parity
{
    parity_undefined = -1,
    parity_none = 1,
    parity_even = 2,
    parity_odd = 3,
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

/**
 * @brief All settings for flowcontrol
 *
 */
enum Flowcontrol
{
    flowcontrol_undefined = -1,
    flowcontrol_485 = 1,
    flowcontrol_nfc = 2,
};
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

/**
 * @brief All settings for power consumption mode
 *
 */
enum PowerConsumptionMode
{
    powermode_undefined = -1,
    powermode_run = 1,
    powermode_wake_up = 2,
};

/**
 * @brief All settings for LoRa air rate level (Speed)
 *
 */
enum LoRaAirRateLevel
{
    LoRa_air_rate_level_undefined = -1,
    LoRa_air_rate_level_268 = 1,
    LoRa_air_rate_level_488 = 2,
    LoRa_air_rate_level_537 = 3,
    LoRa_air_rate_level_878 = 4,
    LoRa_air_rate_level_977 = 5,
    LoRa_air_rate_level_1758 = 6,
    LoRa_air_rate_level_3125 = 7,
    LoRa_air_rate_level_6250 = 8,
    LoRa_air_rate_level_10937 = 9,
    LoRa_air_rate_level_21875 = 10,
};

/**
 * @brief Class used to store set and retrieve UART settings
 *
 */
class USR_LG_206_P_UART_SETTINGS
{
public:
    Baudrate buadrate;
    char dataBits;
    char stopBits;
    Parity parity;
    Flowcontrol flowControl;

    USR_LG_206_P_UART_SETTINGS(bool usingFactorySettings);
    String toString(void);
    int fromString(String);
};

/**
 * @brief Struct defined to store settings of the USR_LG206_P module
 *
 */
class USR_LG_206_P_SETTINGS
{
public:
    int ATMode;
    int commandEchoFunction;
    String nodeID;
    String firmwareVersion;
    WorkMode workMode;
    PowerConsumptionMode powerMode;
    int wakeUpInterval;
    LoRaAirRateLevel loraAirRateLevel;
    unsigned int destinationAddress;
    bool destinationAddressIsSet;
    int channel;
    int forwardErrorCorrection;
    int transmittingPower;
    int testInterval;
    String key; // 16 bytes HEX format character string

    USR_LG_206_P_SETTINGS(bool usingFactorySettings);
    USR_LG_206_P_UART_SETTINGS *get_uart(void);
    void set_uart(USR_LG_206_P_UART_SETTINGS *newUARTSettings);

private:
    USR_LG_206_P_UART_SETTINGS *uart;
};

#endif // USR_LG206_P_SETTINGS_H_