#ifndef USR_LG206_P_SETTINGS_H_
#define USR_LG206_P_SETTINGS_H_
#include <Arduino.h>
#include "usr_lg206_p_uart_settings.h"

namespace LoRaSettings
{
    enum class AtMode
    {
        kAtModeUndefined = 0,
        kAtModeIsOn = 1,
        kAtModeIsOff = 2,
    };
    enum class CommandEchoFunction
    {
        kCommandEchoFunctionUndefined = 0,
        kCommandEchoFunctionIsOn = 1,
        kCommandEchoFunctionIsOff = 2,
    };
    enum class WorkMode
    {
        kWorkModeUndefined = 0,
        kWorkModeTransparent = 1,
        kWorkModeFixedPoint = 2
    };
    enum class PowerConsumptionMode
    {
        kPowerConsumptionModeUndefined = 0,
        kPowerConsumptionModeRun = 1,
        kPowerConsumptionModeWakeUp = 2,
    };
    enum class LoRaAirRateLevel
    {
        kLoRaAirRateLevelUndefined = 0,
        kLoRaAirRateLevel268 = 1,
        kLoRaAirRateLevel488 = 2,
        kLoRaAirRateLevel537 = 3,
        kLoRaAirRateLevel878 = 4,
        kLoRaAirRateLevel977 = 5,
        kLoRaAirRateLevel1758 = 6,
        kLoRaAirRateLevel3125 = 7,
        kLoRaAirRateLevel6250 = 8,
        kLoRaAirRateLevel10937 = 9,
        kLoRaAirRateLevel21875 = 10,
    };
    enum class ForwardErrorCorrection
    {
        kForwardErrorCorrectionUndefined = 0,
        kForwardErrorCorrectionIsOn = 1,
        kForwardErrorCorrectionIsOff = 2,
    };

    /**
     * @brief Struct defined to store settings of the USR_LG206_P module
     *
     */
    class LoRaSettings
    {
    public:
        AtMode at_mode;
        CommandEchoFunction command_echo_function;
        String node_id;
        String firmware_version;
        WorkMode work_mode;
        PowerConsumptionMode power_consumption_mode;
        int wake_up_interval;
        LoRaAirRateLevel lora_air_rate_level;
        uint16_t destination_address;
        bool destination_address_is_set;
        int channel;
        ForwardErrorCorrection forward_error_correction;
        uint8_t transmitting_power;
        uint8_t test_interval;
        String key; // 16 bytes HEX format character string

        explicit LoRaSettings(bool usingFactorySettings = false);
        LoRaUartSettings::LoRaUartSettings GetUartSettings(void);
        void SetUartSettings(const LoRaUartSettings::LoRaUartSettings &newUARTSettings);

        bool operator==(const LoRaSettings &b) const;
        bool operator!=(const LoRaSettings &b) const;

    private:
        LoRaUartSettings::LoRaUartSettings uart_;
    };
} // namespace LoRaSettings

#endif // USR_LG206_P_SETTINGS_H_