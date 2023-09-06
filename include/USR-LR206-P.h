/**
 * @file USR-LR206-P.cpp
 * @author Rik Vos (rik.vos01@gmail.com)
 * @brief Library for using USR-LR206-P
 * @version 0.1
 * @date 2023-09-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef USRLR206P_H_
#define USRLR206P_H_

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

// All settings for workmode
#define WMODE_TRANS 1 // Transparent transmission
#define WMODE_FP 2    // Fixed-point transmitting

// All settings for baudrate
#define BAUDRATE_1200 1
#define BAUDRATE_2400 2
#define BAUDRATE_4800 3
#define BAUDRATE_9200 4
#define BAUDRATE_19200 5
#define BAUDRATE_38400 6
#define BAUDRATE_57600 7
#define BAUDRATE_115200 8

// All settings for parity
#define PARITY_NONE 1
#define PARITY_EVEN 2
#define PARITY_ODD 3

// All settings for flowcontrol
#define FLOWCONTROL_485 1 // 485 Flow Control
#define FLOWCONTROL_NFC 2 // No Flow Control

// All settings for power consumption mode
#define POWERMODE_RUN 1 // Run mode
#define POWERMODE_WU 2  // Wake up mode

// All settings for LoRa air rate level (Speed)
#define LoRaAirRateLevel_268 1
#define LoRaAirRateLevel_488 2
#define LoRaAirRateLevel_537 3
#define LoRaAirRateLevel_878 4
#define LoRaAirRateLevel_977 5
#define LoRaAirRateLevel_1758 6
#define LoRaAirRateLevel_3125 7
#define LoRaAirRateLevel_6250 8
#define LoRaAirRateLevel_10937 9
#define LoRaAirRateLevel_21875 10

class USR_LG_206_P_UART_SETTINGS
{
    char buadrate = BAUDRATE_115200;
    char dataBits = 8;
    char stopBits = 1;
    char parity = PARITY_NONE;
    char flowControl = FLOWCONTROL_485;
};

typedef struct
{
    int ATMode;
    int commandEchoFunction;
    int nodeID;
    String firmwareVersion;
    int workMode;
    USR_LG_206_P_UART_SETTINGS *uart;
    int powerMode;
    int wakingUpInterval;
    int loraAirRateLevel;
    unsigned int destinationAddress;
    int channel;
    int forwardErrorCorrection;
    int transmittingPower;
    char key[16]; // 16 bytes HEX format character string
} USR_LG_206_P_SETTINGS_t;

USR_LG_206_P_SETTINGS_t factory_settings = {
    .ATMode = false,
    .commandEchoFunction = false,
    .nodeID = -1,
    .firmwareVersion = "",
    .workMode = WMODE_TRANS,
    .uart = new USR_LG_206_P_UART_SETTINGS(),
    .powerMode = POWERMODE_RUN,
    .wakingUpInterval = 2000,
    .loraAirRateLevel = LoRaAirRateLevel_21875,
    .destinationAddress = 0,
    .channel = 65,
    .forwardErrorCorrection = false,
    .transmittingPower = 20,
    // .key = {}
};

USR_LG_206_P_SETTINGS_t undefined_settings = {
    .ATMode = false,
    .commandEchoFunction = SETTING_UNDEFINED,
    .nodeID = SETTING_UNDEFINED,
    .firmwareVersion = "",
    .workMode = SETTING_UNDEFINED,
    .uart = 0,
    .powerMode = SETTING_UNDEFINED,
    .wakingUpInterval = SETTING_UNDEFINED,
    .loraAirRateLevel = SETTING_UNDEFINED,
    .destinationAddress = 0,
    .channel = SETTING_UNDEFINED,
    .forwardErrorCorrection = SETTING_UNDEFINED,
    .transmittingPower = SETTING_UNDEFINED,
    // .key[16] = {}
};

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

    // TODO
    int retrieve_settings(void);

    // TODO ADD reset to factory settings

    // TODO
    //  int set_settings(USR_LG_206_P_SETTINGS* settings);

    /**
     * @brief Function to start the AT mode
     *
     * @return int, true if succesfull and false if unsuccesfull
     */
    int begin_AT_mode(void);

    /// @brief Function to end the AT mode and start transmission mode
    /// @return true if succesfull and false if unsuccesfull
    int end_AT_mode(void);

    /// @brief Function used to set the function echo boolean
    /// @param isOn the value to which it is set
    /// @return true of succesfull, false if unsuccesfull
    int set_echo(bool isOn);

    /// @brief Function used to get the currect value of the echo function command on the LoRa module
    /// @param isOn OUTPUT variable used to store the requested value
    /// @return true of succesfull, false if unsuccesfull
    int get_echo(OUT bool &isOn);

    /// @brief Function used to restart the LoRa module
    /// @return true if succesfull
    int restart(void);

    /// @brief Function used to set the current settings as new default
    /// @return true of succesfull, false if unsuccesfull
    int save_as_default(void);

    // TODO
    int reset_to_default(void);

    // TODO
    int get_node_id(void);
    // TODO
    int get_firmware_version(void);

    /// @brief Function used to set the workmode
    /// @param wmode workmode (WMODE_TRANS = 1, WMODE_FP = 2)
    /// @return true of succesfull, false if unsuccesfull
    int set_wmode(int wmode);

    /// @brief Function used to get the work mode
    /// @param wmode OUTPUT variable used to store the requested value
    /// @return true of succesfull, false if unsuccesfull
    int get_wmode(OUT int &wmode);

    // TODO
    int set_uart(USR_LG_206_P_UART_SETTINGS *settings);

    // TODO
    /// @brief Function used to get the uart settings
    /// @param settings OUTPUT variable used to store the requested value
    /// @return true of succesfull, false if unsuccesfull
    int get_uart(OUT USR_LG_206_P_UART_SETTINGS &settings);

    // TODO
    int set_power_consumption_mode(void);
    // TODO
    int get_power_consumption_mode(void);
    // TODO
    int set_waking_up_interval(void);
    // TODO
    int get_waking_up_interval(void);

    /// @brief Function used to set the LoRa air rate level
    /// @param speed value for air rate 1-10 as following values
    /// 1: 268bps
    /// 2: 488bps
    /// 3: 537bps
    /// 4: 878bps
    /// 5: 977bps
    /// 6: 1758bps
    /// 7: 3125bps
    /// 8: 6250bps
    /// 9: 10937bps
    /// 10: 21875bp
    /// @return true of succesfull, false if unsuccesfull
    int set_speed(int speed);

    // TODO
    int get_speed(void);

    /// @brief Function used to set the destination address
    /// @param address value between 0-65535
    /// @return true of succesfull, false if unsuccesfull
    int set_address(int address);

    // TODO
    // int get_address(void);

    /// @brief Function used to set the LoRa air rate level
    /// @param channel value for channel is as following values in the following ranges
    /// L: 0~127 Working frequency band=(398+ch)MHz (Default is 72, frequency band 470Mhz)
    /// H: 0~127 Working frequency band=(803+ch)Mhz (Default is 65, frequency band 868Mhz)
    /// @return true of succesfull, false if unsuccesfull
    int set_channel(int channel);

    // TODO
    // int get_channel(void);
    // TODO
    // int set_forward_error_correction(void);
    // TODO
    // int get_forward_error_correction(void);
    // TODO
    // int set_power_transmission_mode(void);
    // TODO
    // int get_power_transmission_mode(void);
    // TODO
    // int set_transmission_interval(void);
    // TODO
    // int get_transmission_interval(void);
    // TODO
    // int set_key(void);

    /// @brief Function used to retrieve a message from the module
    /// @return the data received
    String retrieve_message(void);

    /// @brief Function used to send data
    /// @param message data that needs to be send
    /// @return amount of bytes written
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
};

#endif // USRLR206P_H_