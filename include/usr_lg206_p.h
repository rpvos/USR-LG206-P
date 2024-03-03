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
#include <MAX485TTL.hpp>

#include "usr_lg206_p_error_code.h"
#include "usr_lg206_p_settings.h"
#include "usr_lg206_p_uart_settings.h"

/**
 * @brief OUT is used as a identifier for output parameters
 *
 */
#define OUT

/**
 * @brief Class used to connect and communicate to a LoRa module type USR_LG_206_P
 *
 */
class UsrLg206P
{
public:
    /**
     * @brief Construct a new usr lg 206 p object
     *
     * @param serial the stream to which data needs to be sent to communicate with the module
     */
    UsrLg206P(RS485 *serial);

    /**
     * @brief Destroy the LoRa object
     *
     */
    ~UsrLg206P(void);

    /**
     * @brief Set the settings of the LoRa module to factory settings
     *
     * @return int, true if succesfull and false if unsuccesfull
     */
    LoRaErrorCode FactoryReset(void);

    // TODO set settings
    LoRaErrorCode SetSettings(LoRaSettings::LoRaSettings settings);

    // TODO get settings
    LoRaErrorCode GetSettings(OUT LoRaSettings::LoRaSettings &settings);

    /**
     * @brief Function to start the AT mode
     *
     * @return int, true if succesfull and false if unsuccesfull
     */
    LoRaErrorCode BeginAtMode(void);

    /**
     * @brief Function to end the AT mode and start transmission mode
     *
     * @return true if succesfull and false if unsuccesfull
     */
    LoRaErrorCode EndAtMode(void);

    /**
     * @brief Function used to set the function echo boolean
     *
     * @param isOn the value to which it is set
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode SetEcho(LoRaSettings::CommandEchoFunction command_echo_function);

    /**
     * @brief Function used to get the currect value of the echo function command on the LoRa module
     *
     * @param isOn OUTPUT variable used to store the requested value
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode GetEcho(LoRaSettings::CommandEchoFunction &setting);

    /**
     * @brief Function used to restart the LoRa module
     *
     * @return true if succesfull
     */
    LoRaErrorCode Restart(void);

    /**
     * @brief Function used to set the current settings as new default
     *
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode SaveAsDefault(void);

    /**
     * @brief Function used to reset settings to default
     *
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode ResetToDefault(void);

    /**
     * @brief Get the node id
     *
     * @param node_id output parameter
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode GetNodeId(OUT String &node_id);

    /**
     * @brief Get the firmware version
     *
     * @param firmware_version output parameter
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode GetFirmwareVersion(OUT String &firmware_version);

    /**
     * @brief Function used to set the workmode
     *
     * @param wmode WorkMode (kWorkModeTransparent or kWorkModeFixedPoint)
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode SetWorkMode(LoRaSettings::WorkMode work_mode = LoRaSettings::WorkMode::kWorkModeTransparent);

    /**
     * @brief Function used to get the work mode
     *
     * @param wmode OUTPUT variable used to store the requested value
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode GetWorkMode(LoRaSettings::WorkMode &setting);

    /**
     * @brief Set the uart settings of the LoRa module
     *
     * @param settings variable containing the requested settings
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode SetUartSettings(LoRaUartSettings::LoRaUartSettings *setting);

    /**
     * @brief Get the uart settings of the LoRa module
     *
     * @param settings OUTPUT variable used to store the requested value
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode GetUartSettings(OUT LoRaUartSettings::LoRaUartSettings *setting);

    /**
     * @brief Set the power consumption mode
     *
     * @param powermode (POWERMODE_RUN, POWERMODE_WU)
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode SetPowerConsumptionMode(LoRaSettings::PowerConsumptionMode powermode = LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeRun);

    /**
     * @brief Get the power consumption mode
     *
     * @param powermode (POWERMODE_RUN, POWERMODE_WU)
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode GetPowerConsumptionMode(LoRaSettings::PowerConsumptionMode &setting);

    /**
     * @brief Set the waking up interval
     *
     * @param wake_up_interval 500-4000 in ms
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode SetWakingUpInterval(int setting = 2000);

    /**
     * @brief Get the waking up interval
     *
     * @param wake_up_interval in ms
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode GetWakingUpInterval(OUT int &setting);

    /**
     * @brief Function used to set the LoRa air rate level
     *
     * @param speed value for air rate in bits per second
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode SetAirRateLevel(LoRaSettings::LoRaAirRateLevel setting = LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel21875);

    /**
     * @brief Function used to set the LoRa air rate level
     *
     * @param speed value for air rate in bits per second
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode GetAirRateLevel(LoRaSettings::LoRaAirRateLevel &setting);

    /**
     * @brief Function used to set the destination address
     *
     * @param address value between 0-65535 where 65535 a broadcast address is
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode SetDestinationAddress(const uint16_t address = 0);

    /**
     * @brief Function used to set the destination address
     *
     * @param OUTPUT address value between 0-65535 where 65535 a broadcast address is
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode GetDestinationAddress(OUT int &address);

    LoRaErrorCode SetChannel(int channel = 72);

    LoRaErrorCode GetChannel(OUT int &channel);

    /**
     * @brief Set the forward error correction on or off
     *
     * @param isOn true for forward error correction turned on and false for turned off
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode SetForwardErrorCorrection(LoRaSettings::ForwardErrorCorrection setting = LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOff);

    /**
     * @brief Get the forward error correction value
     *
     * @param isOn OUTPUT true for forward error correction turned on and false for turned off
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode GetForwardErrorCorrection(LoRaSettings::ForwardErrorCorrection &setting);

    /**
     * @brief Set the power transmission value
     *
     * @param power Amount of power in dBm (10~20)
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode SetPowerTransmissionValue(int power = 20);

    /**
     * @brief Set the power transmission value
     *
     * @param power OUTPUT Amount of power in dBm (10~20)
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode GetPowerTransmissionValue(OUT int &power);

    /**
     * @brief Set the transmission interval of test data being sent
     *
     * @return LoRaErrorCode
     */
    LoRaErrorCode SetTransmissionInterval(int interval = 2000);

    /**
     * @brief Query transmission interval of test data being received
     *
     * @return LoRaErrorCode
     */
    LoRaErrorCode QueryTransmissionInterval();

    /**
     * @brief Set the key to encrypt the data transmission with
     *
     * @param key 16 bytes HEX format character string
     * @return true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode SetKey(String key = "FFFFFFFFFFFFFFFF");

    /**
     * @brief Function used to check if messages are received
     *
     * @return int amount of bytes available
     */
    int Available(void);

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
    int SendMessage(const uint8_t *message, size_t length);

    int SendMessage(const char *const message, const size_t length);

    /**
     * @brief Function used to send data when fixed point is enabled
     *
     * @param message data that needs to be received
     * @param message_size is the size of data
     * @param destination_address of the other module
     * @param channel of the other module
     * @return int amount of bytes written
     */
    int SendMessage(const char *message, const size_t message_size, const uint16_t destination_address, const uint8_t channel);

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
    LoRaSettings::LoRaSettings settings_;

    /**
     * @brief Function used to set the value on the LoRa module
     *
     * @param command which is used to set the value
     * @param succesfullResponse is what is displayed on succes, defaulted to "OK"
     * @return int, true if succesfull, false if unsuccesfull
     */
    LoRaErrorCode SetCommand(String command, String succesfullResponse = "OK");

    /**
     * @brief Function used to get a setting from the LoRa module
     *
     * @param command Command for wich the value is stored
     * @param succesfullResponse is what is displayed on succes, defaulted to "OK"
     * @return String containing this value, empty if not succeeded
     */
    LoRaErrorCode GetCommand(String command, OUT String &ouput, bool using_colon = true, String succesfullResponse = "OK");

    /**
     * @brief Helper function to send a command and wait for response
     *
     * @param command which needs to be send
     * @return String response
     */
    String SendCommand(String command);
};

#endif // USR_LG206_P_H_