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
#include <usr_lg206_p_settings.h>
#include <usr_lg206_p_uart_settings.h>
#include <MAX485TTL.h>

/**
 * @brief OUT is used as a identifier for output parameters
 *
 */
#define OUT

/**
 * @brief Class used to connect and communicate to a LoRa module type USR_LG_206_P
 *
 */
class LoRa
{
public:
    /**
     * @brief Construct a new usr lg 206 p object
     *
     * @param serial the stream to which data needs to be sent to communicate with the module
     */
    explicit LoRa(RS485 *serial);

    /**
     * @brief Destroy the LoRa object
     *
     */
    ~LoRa(void);

    /**
     * @brief Set the settings of the LoRa module to factory settings
     *
     * @return int, true if succesfull and false if unsuccesfull
     */
    int FactoryReset(void);

    // TODO
    int SetSettings(LoRaSettings::LoRaSettings *settings);

    // TODO
    int GetSettings(OUT LoRaSettings::LoRaSettings &settings);

    /**
     * @brief Function to start the AT mode
     *
     * @return int, true if succesfull and false if unsuccesfull
     */
    int BeginAtMode(void);

    /**
     * @brief Function to end the AT mode and start transmission mode
     *
     * @return true if succesfull and false if unsuccesfull
     */
    int EndAtMode(void);

    /**
     * @brief Function used to set the function echo boolean
     *
     * @param isOn the value to which it is set
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaSettings::CommandEchoFunction SetEcho(bool is_on);

    /**
     * @brief Function used to get the currect value of the echo function command on the LoRa module
     *
     * @param isOn OUTPUT variable used to store the requested value
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaSettings::CommandEchoFunction GetEcho();

    /**
     * @brief Function used to restart the LoRa module
     *
     * @return true if succesfull
     */
    int Restart(void);

    /**
     * @brief Function used to set the current settings as new default
     *
     * @return true if succesfull, false if unsuccesfull
     */
    int SaveAsDefault(void);

    /**
     * @brief Function used to reset settings to default
     *
     * @return true if succesfull, false if unsuccesfull
     */
    int ResetToDefault(void);

    /**
     * @brief Get the node id
     *
     * @param node_id output parameter
     * @return true if succesfull, false if unsuccesfull
     */
    int GetNodeId(OUT String &node_id);

    /**
     * @brief Get the firmware version
     *
     * @param firmware_version output parameter
     * @return true if succesfull, false if unsuccesfull
     */
    int GetFirmwareVersion(OUT String &firmware_version);

    /**
     * @brief Function used to set the workmode
     *
     * @param wmode WorkMode (kWorkModeTransparent or kWorkModeFixedPoint)
     * @return true if succesfull, false if unsuccesfull
     */
    int SetWorkMode(LoRaSettings::WorkMode work_mode = LoRaSettings::kWorkModeTransparent);

    /**
     * @brief Function used to get the work mode
     *
     * @param wmode OUTPUT variable used to store the requested value
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaSettings::WorkMode GetWorkMode();

    /**
     * @brief Set the uart settings of the LoRa module
     *
     * @param settings variable containing the requested settings
     * @return true if succesfull, false if unsuccesfull
     */
    int SetUartSettings(LoRaUartSettings::LoRaUartSettings *settings);

    /**
     * @brief Get the uart settings of the LoRa module
     *
     * @param settings OUTPUT variable used to store the requested value
     * @return true if succesfull, false if unsuccesfull
     */
    int GetUartSettings(OUT LoRaUartSettings::LoRaUartSettings &settings);

    /**
     * @brief Set the power consumption mode
     *
     * @param powermode (POWERMODE_RUN, POWERMODE_WU)
     * @return true if succesfull, false if unsuccesfull
     */
    int set_power_consumption_mode(LoRaSettings::PowerConsumptionMode powermode = LoRaSettings::kPowerConsumptionModeRun);

    /**
     * @brief Get the power consumption mode
     *
     * @param powermode (POWERMODE_RUN, POWERMODE_WU)
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaSettings::PowerConsumptionMode get_power_consumption_mode();

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
     * @param speed value for air rate in bits per second
     * @return true if succesfull, false if unsuccesfull
     */
    int set_speed(LoRaSettings::LoRaAirRateLevel air_rate_level = LoRaSettings::kLoRaAirRateLevel21875);

    /**
     * @brief Function used to set the LoRa air rate level
     *
     * @param speed value for air rate in bits per second
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaSettings::LoRaAirRateLevel get_speed();

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
    int set_forward_error_correction(LoRaSettings::ForwardErrorCorrection setting = LoRaSettings::kForwardErrorCorrectionIsOff);

    /**
     * @brief Get the forward error correction value
     *
     * @param isOn OUTPUT true for forward error correction turned on and false for turned off
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaSettings::ForwardErrorCorrection get_forward_error_correction();

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
     *  //TODO send interval
     * @return true if succesfull, false if unsuccesfull
     */
    int set_transmission_interval(int interval);

    /**
     * @brief Get the transmission interval of test data being sent
     * // TODO receive interval
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
    int SetKey(String key = "FFFFFFFFFFFFFFFF");

    /**
     * @brief Function used to retrieve a message from the module
     *
     * @return the data received
     */
    String ReceiveMessage(void);

    /**
     * @brief Function used to send data
     *
     * @param message data that needs to be send
     * @return amount of bytes written
     */
    int SendMessage(char *message);

    /**
     * @brief Function used to send data when fixed point is enabled
     *
     * @param destination_address of the other module
     * @param channel of the other module
     * @param message data that needs to be received
     * @param message_size is the size of data
     * @return int amount of bytes written
     */
    int SendMessage(uint16_t destination_address, uint8_t channel, char *message, uint8_t message_size);

private:
    /**
     * @brief Stream to which the communication with the module is sent
     *
     */
    RS485 *serial_;

    /**
     * @brief Object to keep the settings of the LoRa module
     * This is to counter frequent calls
     */
    LoRaSettings::LoRaSettings *settings_;

    /**
     * @brief Function used to set the value on the LoRa module
     *
     * @param command which is used to set the value
     * @param succesfullResponse is what is displayed on succes, defaulted to "OK"
     * @return int, true if succesfull, false if unsuccesfull
     */
    int SetCommand(String command, String succesfullResponse = "OK");

    /**
     * @brief Function used to get a setting from the LoRa module
     *
     * @param command Command for wich the value is stored
     * @param succesfullResponse is what is displayed on succes, defaulted to "OK"
     * @return String containing this value, empty if not succeeded
     */
    String GetCommand(String command, String succesfullResponse = "OK");

    /**
     * @brief Helper function to send a command and wait for response
     *
     * @param command which needs to be send
     * @return String response
     */
    String SendCommand(String command);
};

#endif // USR_LG206_P_H_