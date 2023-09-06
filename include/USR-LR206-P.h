#ifndef USRLR206P_H
#define USRLR206P_H

#include <Arduino.h>

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
#define FLOWCONTROL_485 1
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

// struct USR_LG_206_P_SETTINGS_DEFAULT
// {
//     bool ATMode = false;
//     bool commandEchoFunction = false;
//     int nodeID = -1;
//     String firmwareVersion = "";
//     char workMode = WMODE_TRANS;
//     USR_LG_206_P_UART_SETTINGS uart;
//     char powerMode = POWERMODE_RUN;
//     int wakingUpInterval = 2000;
//     char loraAirRateLevel = LoRaAirRateLevel_21875;
//     unsigned int destinationAddress = 0;
//     char channel = 65;
//     bool forwardErrorCorrection = false;
//     char transmittingPower = 20;
//     char key[16]; // 16 bytes HEX format character string
// };

class USR_LG_206_P
{
public:
    /// @brief Class used to store the settings of the UART of the LoRa module
    class USR_LG_206_P_UART_SETTINGS
    {
    public:
        char buadrate = BAUDRATE_115200;
        char dataBits = 8;
        char stopBits = 1;
        char parity = PARITY_NONE;
        char flowControl = FLOWCONTROL_485;
    };

    /// @brief Class used to store the settings of the LoRa module
    class USR_LG_206_P_SETTINGS
    {
    public:
        int ATMode = false;
        int commandEchoFunction = SETTING_UNDEFINED;
        int nodeID = SETTING_UNDEFINED;
        String firmwareVersion = "";
        int workMode = SETTING_UNDEFINED;
        USR_LG_206_P_UART_SETTINGS *uart;
        int powerMode = SETTING_UNDEFINED;
        int wakingUpInterval = SETTING_UNDEFINED;
        int loraAirRateLevel = SETTING_UNDEFINED;
        unsigned int destinationAddress = SETTING_UNDEFINED;
        int channel = SETTING_UNDEFINED;
        bool forwardErrorCorrection = SETTING_UNDEFINED;
        int transmittingPower = SETTING_UNDEFINED;
        char key[16]; // 16 bytes HEX format character string
    };

private:
    Stream *serial;
    USR_LG_206_P_SETTINGS *settings;

    /// @brief Function used to set the value on the LoRa module
    /// @param command which is used to set the value
    /// @return true of succesfull, false if unsuccesfull
    int set_command(String command);

    /// @brief Function used to get a value from the LoRa module
    /// @param command Command for wich the value is stored
    /// @return String containing this value
    String get_command(String command);

public:
    USR_LG_206_P(Stream *serial);

    // TODO
    int retrieve_settings();

    // TODO ADD reset to factory settings

    // TODO
    //  int set_settings(USR_LG_206_P_SETTINGS* settings);

    /// @brief Function to start the AT mode
    /// @return true if succesfull and false if unsuccesfull
    int begin_AT_mode();

    /// @brief Function to end the AT mode and start transmission mode
    /// @return true if succesfull and false if unsuccesfull
    int end_AT_mode();

    /// @brief Function used to set the function echo boolean
    /// @param isOn the value to which it is set
    /// @return true of succesfull, false if unsuccesfull
    int set_echo(bool isOn);

    /// @brief Function used to get the currect value of the echo function command on the LoRa module
    /// @return The value of the echo command or -1 if not succeeded
    int get_echo();

    /// @brief Function used to restart the LoRa module
    /// @return true if succesfull
    int restart();

    /// @brief Function used to set the current settings as new default
    /// @return true of succesfull, false if unsuccesfull
    int save_as_default();

    // TODO
    // int reset_to_default();
    // TODO
    // int get_node_id();
    // TODO
    // int get_firmware_version();

    /// @brief Function used to set the workmode
    /// @param wmode workmode (WMODE_TRANS = 1, WMODE_FP = 2)
    /// @return true of succesfull, false if unsuccesfull
    int set_wmode(int wmode);
    // TODO
    // int get_wmode();
    // TODO
    // int set_uart();
    // TODO
    // int get_uart();
    // TODO
    // int set_power_consumption_mode();
    // TODO
    // int get_power_consumption_mode();
    // TODO
    // int get_waking_up_interval();
    // TODO
    // int set_waking_up_interval();

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
    // int get_speed();

    /// @brief Function used to set the destination address
    /// @param address value between 0-65535
    /// @return true of succesfull, false if unsuccesfull
    int set_address(int address);

    // TODO
    // int get_address();

    /// @brief Function used to set the LoRa air rate level
    /// @param channel value for channel is as following values in the following ranges
    /// L: 0~127 Working frequency band=(398+ch)MHz (Default is 72, frequency band 470Mhz)
    /// H: 0~127 Working frequency band=(803+ch)Mhz (Default is 65, frequency band 868Mhz)
    /// @return true of succesfull, false if unsuccesfull
    int set_channel(int channel);

    // TODO
    // int get_channel();
    // TODO
    // int set_forward_error_correction();
    // TODO
    // int get_forward_error_correction();
    // TODO
    // int set_power_transmission_mode();
    // TODO
    // int get_power_transmission_mode();
    // TODO
    // int set_transmission_interval();
    // TODO
    // int get_transmission_interval();
    // TODO
    // int set_key();

    /// @brief Function used to retrieve a message from the module
    /// @return the data received
    String retrieve_message();

    /// @brief Function used to send data
    /// @param message data that needs to be send
    /// @return amount of bytes written
    int send_message(char *message);
};

#endif