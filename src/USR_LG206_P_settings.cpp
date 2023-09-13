#include "USR_LG206_P_settings.h"
#include <Arduino.h>

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