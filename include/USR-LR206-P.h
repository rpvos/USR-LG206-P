#ifndef USRLR206P_H
#define USRLR206P_H

#include <Arduino.h>

class USR_LG_206_P
{
private:
    Stream *serial;

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

    String retrieve_message();
    int send_message(char *message);
};

#endif