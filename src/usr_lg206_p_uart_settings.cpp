#include "usr_lg206_p_uart_settings.h"

LoRaUartSettings::LoRaUartSettings::LoRaUartSettings(bool usingFactorySettings)
{
    if (usingFactorySettings)
    {
        this->buadrate = Baudrate::baudrate_115200;
        this->dataBits = 8;
        this->stopBits = 1;
        this->parity = Parity::parity_none;
        this->flowControl = Flowcontrol::flowcontrol_485;
    }
    else
    {
        this->buadrate = Baudrate::baudrate_undefined;
        this->dataBits = 0;
        this->stopBits = 0;
        this->parity = Parity::parity_undefined;
        this->flowControl = Flowcontrol::flowcontrol_undefined;
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

String LoRaUartSettings::LoRaUartSettings::toString(void) const
{
    return String(ToString(this->buadrate)) + "," + String(this->dataBits) + "," + String(this->stopBits) + "," + ToString(this->parity) + "," + ToString(this->flowControl);
};

int LoRaUartSettings::LoRaUartSettings::fromString(String input)
{
    int index = input.indexOf(',');
    String baudrate = input.substring(0, index);
    input = input.substring(index);
    this->buadrate = Baudrate(baudrate.toInt());

    index = input.indexOf(',');
    String input_data_bits = input.substring(0, index);
    input = input.substring(index);
    this->dataBits = input_data_bits.toInt();

    index = input.indexOf(',');
    String input_stop_bits = input.substring(0, index);
    input = input.substring(index);
    this->stopBits = input_stop_bits.toInt();

    index = input.indexOf(',');
    String parity = input.substring(0, index);
    input = input.substring(index);
    this->parity = ParityFromString(input);

    this->flowControl = FlowcontrolFromString(input);
    return true;
};
