/**
 * @file USR_LG206_P.cpp
 * @author Rik Vos (rik.vos01@gmail.com)
 * @brief Library for using USR-LR206-P
 * @version 0.1
 * @date 2023-09-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef USR_LG_206P_H_
#define USR_LG_206P_H_

#include <Arduino.h>

/**
 * @brief OUT is used as a identifier for output parameters
 *
 */
#define OUT

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
    Baudrate buadrate = baudrate_115200;
    char dataBits = 8;
    char stopBits = 1;
    Parity parity = parity_none;
    Flowcontrol flowControl = flowcontrol_485;

    String toString(void);
    int fromString(String);
};

/**
 * @brief Struct defined to store settings of the USR_LG206_P module
 *
 */
typedef struct
{
    int ATMode;
    int commandEchoFunction;
    String nodeID;
    String firmwareVersion;
    WorkMode workMode;
    USR_LG_206_P_UART_SETTINGS *uart;
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
} USR_LG_206_P_SETTINGS_t;

/**
 * @brief Settings used by the manufacturer
 *
 */
USR_LG_206_P_SETTINGS_t factory_settings = {
    .ATMode = false,
    .commandEchoFunction = false,
    .nodeID = "",
    .firmwareVersion = "",
    .workMode = work_mode_transparent,
    .uart = new USR_LG_206_P_UART_SETTINGS(),
    .powerMode = powermode_run,
    .wakeUpInterval = 2000,
    .loraAirRateLevel = LoRa_air_rate_level_21875,
    .destinationAddress = 0,
    .destinationAddressIsSet = true,
    .channel = 65,
    .forwardErrorCorrection = false,
    .transmittingPower = 20,
    .testInterval = false,
    .key = "FFFFFFFF"};

/**
 * @brief Settings set to undefined to know which settings need to be retrieved from device
 *
 */
USR_LG_206_P_SETTINGS_t undefined_settings = {
    .ATMode = false,
    .commandEchoFunction = SETTING_UNDEFINED,
    .nodeID = "",
    .firmwareVersion = "",
    .workMode = work_mode_undefined,
    .uart = 0,
    .powerMode = powermode_undefined,
    .wakeUpInterval = SETTING_UNDEFINED,
    .loraAirRateLevel = LoRa_air_rate_level_undefined,
    .destinationAddress = 0,
    .destinationAddressIsSet = false,
    .channel = SETTING_UNDEFINED,
    .forwardErrorCorrection = SETTING_UNDEFINED,
    .transmittingPower = SETTING_UNDEFINED,
    .testInterval = SETTING_UNDEFINED,
    .key = ""};

/**
 * @brief Class used to connect and communicate to a LoRa module type USR_LG_206_P
 *
 */
class USR_LG_206_P
{
public:
    /**
     * @brief Construct a new usr lg 206 p object
     *
     * @param serial the stream to which data needs to be sent to communicate with the module
     */
    USR_LG_206_P(Stream *serial);

    /**
     * @brief Set the settings of the LoRa module to factory settings
     *
     * @return int, true if succesfull and false if unsuccesfull
     */
    int factory_reset(void);

    // TODO
    int set_settings(USR_LG_206_P_SETTINGS_t *settings);

    // TODO
    int get_settings(OUT USR_LG_206_P_SETTINGS_t &settings);

    /**
     * @brief Function to start the AT mode
     *
     * @return int, true if succesfull and false if unsuccesfull
     */
    int begin_AT_mode(void);

    /**
     * @brief Function to end the AT mode and start transmission mode
     *
     * @return true if succesfull and false if unsuccesfull
     */
    int end_AT_mode(void);

    /**
     * @brief Function used to set the function echo boolean
     *
     * @param isOn the value to which it is set
     * @return true if succesfull, false if unsuccesfull
     */
    int set_echo(bool isOn);

    /**
     * @brief Function used to get the currect value of the echo function command on the LoRa module
     *
     * @param isOn OUTPUT variable used to store the requested value
     * @return true if succesfull, false if unsuccesfull
     */
    int get_echo(OUT bool &isOn);

    /**
     * @brief Function used to restart the LoRa module
     *
     * @return true if succesfull
     */
    int restart(void);

    /**
     * @brief Function used to set the current settings as new default
     *
     * @return true if succesfull, false if unsuccesfull
     */
    int save_as_default(void);

    /**
     * @brief Function used to reset settings to default
     *
     * @return true if succesfull, false if unsuccesfull
     */
    int reset_to_default(void);

    /**
     * @brief Get the node id
     *
     * @param node_id output parameter
     * @return true if succesfull, false if unsuccesfull
     */
    int get_node_id(OUT String &node_id);

    /**
     * @brief Get the firmware version
     *
     * @param firmware_version output parameter
     * @return true if succesfull, false if unsuccesfull
     */
    int get_firmware_version(OUT String &firmware_version);

    /**
     * @brief Function used to set the workmode
     *
     * @param wmode WorkMode (work_mode_transparent or work_mode_fixed_point)
     * @return true if succesfull, false if unsuccesfull
     */
    int set_wmode(WorkMode wmode);

    /**
     * @brief Function used to get the work mode
     *
     * @param wmode OUTPUT variable used to store the requested value
     * @return true if succesfull, false if unsuccesfull
     */
    int get_wmode(OUT WorkMode &wmode);

    /**
     * @brief Set the uart settings of the LoRa module
     *
     * @param settings variable containing the requested settings
     * @return true if succesfull, false if unsuccesfull
     */
    int set_uart(USR_LG_206_P_UART_SETTINGS *settings);

    /**
     * @brief Get the uart settings of the LoRa module
     *
     * @param settings OUTPUT variable used to store the requested value
     * @return true if succesfull, false if unsuccesfull
     */
    int get_uart(OUT USR_LG_206_P_UART_SETTINGS &settings);

    /**
     * @brief Set the power consumption mode
     *
     * @param powermode (POWERMODE_RUN, POWERMODE_WU)
     * @return true if succesfull, false if unsuccesfull
     */
    int set_power_consumption_mode(PowerConsumptionMode powermode);

    /**
     * @brief Get the power consumption mode
     *
     * @param powermode (POWERMODE_RUN, POWERMODE_WU)
     * @return true if succesfull, false if unsuccesfull
     */
    int get_power_consumption_mode(OUT PowerConsumptionMode &powermode);

    /**
     * @brief Set the waking up interval
     *
     * @param wake_up_interval 500-4000 in ms
     * @return true if succesfull, false if unsuccesfull
     */
    int set_waking_up_interval(int wake_up_interval);

    /**
     * @brief Get the waking up interval
     *
     * @param wake_up_interval in ms
     * @return true if succesfull, false if unsuccesfull
     */
    int get_waking_up_interval(OUT int &wake_up_interval);

    /**
     * @brief Function used to set the LoRa air rate level
     *
     * @param speed value for air rate 1-10 as following values
     * 1: 268bps
     * 2: 488bps
     * 3: 537bps
     * 4: 878bps
     * 5: 977bps
     * 6: 1758bps
     * 7: 3125bps
     * 8: 6250bps
     * 9: 10937bps
     * 10: 21875bps
     * @return true if succesfull, false if unsuccesfull
     */
    int set_speed(LoRaAirRateLevel speed);

    /**
     * @brief Function used to set the LoRa air rate level
     *
     * @param OUTPUT speed value for air rate 1-10 as following values
     * 1: 268bps
     * 2: 488bps
     * 3: 537bps
     * 4: 878bps
     * 5: 977bps
     * 6: 1758bps
     * 7: 3125bps
     * 8: 6250bps
     * 9: 10937bps
     * 10: 21875bps
     * @return true if succesfull, false if unsuccesfull
     */
    int get_speed(OUT LoRaAirRateLevel &speed);

    /**
     * @brief Function used to set the destination address
     *
     * @param address value between 0-65535 where 65535 a broadcast address is
     * @return true if succesfull, false if unsuccesfull
     */
    int set_address(int address);

    /**
     * @brief Function used to set the destination address
     *
     * @param OUTPUT address value between 0-65535 where 65535 a broadcast address is
     * @return true if succesfull, false if unsuccesfull
     */
    int get_address(OUT int &address);

    /**
     * @brief Function used to get the LoRa air rate level
     *
     * @param channel value for channel is as following values in the following ranges
     *  L: 0~127 Working frequency band=(398+ch)MHz (Default is 72, frequency band 470Mhz)
     *  H: 0~127 Working frequency band=(803+ch)Mhz (Default is 65, frequency band 868Mhz)
     * @return true if succesfull, false if unsuccesfull
     */
    int set_channel(int channel);

    /**
     * @brief Function used to get the LoRa air rate level
     *
     * @param OUTPUT channel value for channel is as following values in the following ranges
     *  L: 0~127 Working frequency band=(398+ch)MHz (Default is 72, frequency band 470Mhz)
     *  H: 0~127 Working frequency band=(803+ch)Mhz (Default is 65, frequency band 868Mhz)
     * @return true if succesfull, false if unsuccesfull
     */
    int get_channel(OUT int &channel);

    /**
     * @brief Set the forward error correction on or off
     *
     * @param isOn true for forward error correction turned on and false for turned off
     * @return true if succesfull, false if unsuccesfull
     */
    int set_forward_error_correction(bool isOn);

    /**
     * @brief Get the forward error correction value
     *
     * @param isOn OUTPUT true for forward error correction turned on and false for turned off
     * @return true if succesfull, false if unsuccesfull
     */
    int get_forward_error_correction(OUT bool &isOn);

    /**
     * @brief Set the power transmission value
     *
     * @param power Amount of power in dBm (10~20)
     * @return true if succesfull, false if unsuccesfull
     */
    int set_power_transmission_value(int power);

    /**
     * @brief Set the power transmission value
     *
     * @param power OUTPUT Amount of power in dBm (10~20)
     * @return true if succesfull, false if unsuccesfull
     */
    int get_power_transmission_value(OUT int &power);

    /**
     * @brief Set the transmission interval of test data being sent
     *
     * @return true if succesfull, false if unsuccesfull
     */
    int set_transmission_interval(int interval);

    /**
     * @brief Get the transmission interval of test data being sent
     * // TODO implement a good way of receiving this data
     *
     * @return true if succesfull, false if unsuccesfull
     */
    int get_transmission_interval(OUT int &interval);

    /**
     * @brief Set the key to encrypt the data transmission with
     *
     * @param key 16 bytes HEX format character string
     * @return true if succesfull, false if unsuccesfull
     */
    int set_key(String key);

    /**
     * @brief Function used to retrieve a message from the module
     *
     * @return the data received
     */
    String retrieve_message(void);

    /**
     * @brief Function used to send data
     *
     * @param message data that needs to be send
     * @return amount of bytes written
     */
    int send_message(char *message);

private:
    /**
     * @brief Stream to which the communication with the module is sent
     *
     */
    Stream *serial;

    /**
     * @brief Object to keep the settings of the LoRa module
     * This is to counter frequent calls
     */
    USR_LG_206_P_SETTINGS_t *settings;

    /**
     * @brief Function used to set the value on the LoRa module
     *
     * @param command which is used to set the value
     * @return int, true if succesfull, false if unsuccesfull
     */
    int set_command(String command);

    /**
     * @brief Function used to get a setting from the LoRa module
     *
     * @param command Command for wich the value is stored
     * @return String containing this value
     */
    String get_command(String command);

    /**
     * @brief Function used to log a warning when received data is not what expected
     *
     * @param expected_data How the data should look like
     * @param actual_data What it actually looks like
     */
    void log_warning(String expected_data, String actual_data);
};

#endif // USR_LG206_P_H_