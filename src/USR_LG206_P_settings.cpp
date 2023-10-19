#include "usr_lg206_p_settings.h"
#include <Arduino.h>

LoRaUartSettings::LoRaUartSettings(bool usingFactorySettings)
{
    if (usingFactorySettings)
    {
        this->buadrate = baudrate_115200;
        this->dataBits = 8;
        this->stopBits = 1;
        this->parity = parity_none;
        this->flowControl = flowcontrol_485;
    }
    else
    {
        this->buadrate = baudrate_115200;
        this->dataBits = 8;
        this->stopBits = 1;
        this->parity = parity_none;
        this->flowControl = flowcontrol_485;
    };
};

bool LoRaUartSettings::operator!=(const LoRaUartSettings &b) const
{
    return !(*this == b);
}

bool LoRaUartSettings::operator==(const LoRaUartSettings &b) const
{
    if (this->buadrate != b.buadrate)
    {
        return false;
    }
    if (this->dataBits != b.dataBits)
    {
        return false;
    }
    if (this->stopBits != b.stopBits)
    {
        return false;
    }
    if (this->parity != b.parity)
    {
        return false;
    }
    if (this->flowControl != b.flowControl)
    {
        return false;
    }

    return true;
}

String LoRaUartSettings::toString(void)
{
    return String(this->buadrate) + "," + String(this->dataBits) + "," + String(this->stopBits) + "," + ToString(this->parity) + "," + ToString(this->flowControl);
};

int LoRaUartSettings::fromString(String input)
{
    int index = input.indexOf(',');
    String baudrate = input.substring(0, index);
    input = input.substring(index);
    this->buadrate = Baudrate(baudrate.toInt());

    index = input.indexOf(',');
    String dataBits = input.substring(0, index);
    input = input.substring(index);
    this->dataBits = dataBits.toInt();

    index = input.indexOf(',');
    String stopBits = input.substring(0, index);
    input = input.substring(index);
    this->stopBits = stopBits.toInt();

    index = input.indexOf(',');
    String parity = input.substring(0, index);
    input = input.substring(index);
    this->parity = ParityFromString(input);

    this->flowControl = FlowcontrolFromString(input);
    return true;
};

LoRaSettings::LoRaSettings(bool usingFactorySettings)
{
    if (usingFactorySettings)
    {
        this->ATMode = false;
        this->commandEchoFunction = true;
        this->nodeID = "";
        this->firmwareVersion = "";
        this->workMode = work_mode_transparent;
        this->uart = new LoRaUartSettings(usingFactorySettings);
        this->powerMode = powermode_run;
        this->wakeUpInterval = 2000;
        this->loraAirRateLevel = LoRa_air_rate_level_21875;
        this->destinationAddress = 0;
        this->destinationAddressIsSet = true;
        this->channel = 65;
        this->forwardErrorCorrection = false;
        this->transmittingPower = 20;
        this->testInterval = false;
        this->key = "FFFFFFFF";
    }
    else
    {
        this->ATMode = false;
        this->commandEchoFunction = SETTING_UNDEFINED;
        this->nodeID = "";
        this->firmwareVersion = "";
        this->workMode = work_mode_undefined;
        this->uart = new LoRaUartSettings(false);
        this->powerMode = powermode_undefined;
        this->wakeUpInterval = SETTING_UNDEFINED;
        this->loraAirRateLevel = LoRa_air_rate_level_undefined;
        this->destinationAddress = 0;
        this->destinationAddressIsSet = false;
        this->channel = SETTING_UNDEFINED;
        this->forwardErrorCorrection = SETTING_UNDEFINED;
        this->transmittingPower = SETTING_UNDEFINED;
        this->testInterval = SETTING_UNDEFINED;
        this->key = "";
    }
};

LoRaUartSettings *LoRaSettings::get_uart(void)
{
    return this->uart;
};

void LoRaSettings::set_uart(LoRaUartSettings *newUARTSettings)
{
    delete this->uart;
    this->uart = newUARTSettings;
};

bool LoRaSettings::operator!=(const LoRaSettings &b) const
{
    return !(*this == b);
}

bool LoRaSettings::operator==(const LoRaSettings &b) const
{
    if (this->ATMode != b.ATMode)
    {
        return false;
    }
    if (this->commandEchoFunction != b.commandEchoFunction)
    {
        return false;
    }
    if (this->nodeID != b.nodeID)
    {
        return false;
    }
    if (this->firmwareVersion != b.firmwareVersion)
    {
        return false;
    }
    if (this->workMode != b.workMode)
    {
        return false;
    }
    if (*(this->uart) != *(b.uart))
    {
        return false;
    }
    if (this->powerMode != b.powerMode)
    {
        return false;
    }
    if (this->wakeUpInterval != b.wakeUpInterval)
    {
        return false;
    }
    if (this->loraAirRateLevel != b.loraAirRateLevel)
    {
        return false;
    }
    if (this->destinationAddress != b.destinationAddress)
    {
        return false;
    }
    if (this->destinationAddressIsSet != b.destinationAddressIsSet)
    {
        return false;
    }
    if (this->channel != b.channel)
    {
        return false;
    }
    if (this->forwardErrorCorrection != b.forwardErrorCorrection)
    {
        return false;
    }
    if (this->transmittingPower != b.transmittingPower)
    {
        return false;
    }
    if (this->testInterval != b.testInterval)
    {
        return false;
    }

    return true;
}