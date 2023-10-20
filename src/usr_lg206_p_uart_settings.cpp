#include "usr_lg206_p_uart_settings.h"
#include <Arduino.h>

LoRaUartSettings::LoRaUartSettings::LoRaUartSettings(bool usingFactorySettings)
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

bool LoRaUartSettings::LoRaUartSettings::operator!=(const LoRaUartSettings &b) const
{
    return !(*this == b);
}

bool LoRaUartSettings::LoRaUartSettings::operator==(const LoRaUartSettings &b) const
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

String LoRaUartSettings::LoRaUartSettings::toString(void)
{
    return String(this->buadrate) + "," + String(this->dataBits) + "," + String(this->stopBits) + "," + ToString(this->parity) + "," + ToString(this->flowControl);
};

int LoRaUartSettings::LoRaUartSettings::fromString(String input)
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
