#include "usr_lg206_p_settings.h"

LoRaSettings::LoRaSettings::LoRaSettings(bool usingFactorySettings)
{
    if (usingFactorySettings)
    {
        this->at_mode = kAtModeIsOff;
        this->command_echo_function = kEchoFunctionIsOn;
        this->node_id = "";
        this->firmware_version = "";
        this->work_mode = kWorkModeTransparent;
        this->uart_ = new LoRaUartSettings::LoRaUartSettings(usingFactorySettings);
        this->power_mode = kPowerConsumptionModeRun;
        this->wake_up_interval = 2000;
        this->lora_air_rate_level = kLoRaAirRateLevel21875;
        this->destination_address = 0;
        this->destination_address_is_set = true;
        this->channel = 65;
        this->forward_error_correction = kForwardErrorCorrectionIsOff;
        this->transmitting_power = 20;
        this->test_interval = false;
        this->key = "FFFFFFFF";
    }
    else
    {
        this->at_mode = kAtModeUndefined;
        this->command_echo_function = kEchoFunctionUndefined;
        this->node_id = "";
        this->firmware_version = "";
        this->work_mode = kWorkModeUndefined;
        this->uart_ = new LoRaUartSettings::LoRaUartSettings(false);
        this->power_mode = kPowerConsumptionModeUndefined;
        this->wake_up_interval = -1;
        this->lora_air_rate_level = kLoRaAirRateLevelUndefined;
        this->destination_address = 0;
        this->destination_address_is_set = false;
        this->channel = -1;
        this->forward_error_correction = kForwardErrorCorrectionUndefined;
        this->transmitting_power = 0;
        this->test_interval = 0;
        this->key = "";
    }
};

LoRaUartSettings::LoRaUartSettings *LoRaSettings::LoRaSettings::GetUartSettings(void)
{
    return this->uart_;
};

void LoRaSettings::LoRaSettings::SetUartSettings(LoRaUartSettings::LoRaUartSettings *newUARTSettings)
{
    if (this->uart_ != nullptr)
    {
        delete this->uart_;
    }

    this->uart_ = newUARTSettings;
};

bool LoRaSettings::LoRaSettings::operator!=(const LoRaSettings &b) const
{
    return !(*this == b);
}

bool LoRaSettings::LoRaSettings::operator==(const LoRaSettings &b) const
{
    if (this->at_mode != b.at_mode)
    {
        return false;
    }
    if (this->command_echo_function != b.command_echo_function)
    {
        return false;
    }
    if (this->node_id != b.node_id)
    {
        return false;
    }
    if (this->firmware_version != b.firmware_version)
    {
        return false;
    }
    if (this->work_mode != b.work_mode)
    {
        return false;
    }
    if (*(this->uart_) != *(b.uart_))
    {
        return false;
    }
    if (this->power_mode != b.power_mode)
    {
        return false;
    }
    if (this->wake_up_interval != b.wake_up_interval)
    {
        return false;
    }
    if (this->lora_air_rate_level != b.lora_air_rate_level)
    {
        return false;
    }
    if (this->destination_address != b.destination_address)
    {
        return false;
    }
    if (this->destination_address_is_set != b.destination_address_is_set)
    {
        return false;
    }
    if (this->channel != b.channel)
    {
        return false;
    }
    if (this->forward_error_correction != b.forward_error_correction)
    {
        return false;
    }
    if (this->transmitting_power != b.transmitting_power)
    {
        return false;
    }
    if (this->test_interval != b.test_interval)
    {
        return false;
    }

    return true;
}