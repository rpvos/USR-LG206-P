#include "USR_LG206_P_settings.h"
#include <Arduino.h>

USR_LG_206_P_UART_SETTINGS::USR_LG_206_P_UART_SETTINGS(bool usingFactorySettings)
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

String USR_LG_206_P_UART_SETTINGS::toString(void)
{
    return String(this->buadrate) + "," + String(this->dataBits) + "," + String(this->stopBits) + "," + ToString(this->parity) + "," + ToString(this->flowControl);
};

int USR_LG_206_P_UART_SETTINGS::fromString(String input)
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

USR_LG_206_P_SETTINGS::USR_LG_206_P_SETTINGS(bool usingFactorySettings)
{
    if (usingFactorySettings)
    {
        this->ATMode = false;
        this->commandEchoFunction = false;
        this->nodeID = "";
        this->firmwareVersion = "";
        this->workMode = work_mode_transparent;
        this->uart = new USR_LG_206_P_UART_SETTINGS(usingFactorySettings);
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
        this->uart = new USR_LG_206_P_UART_SETTINGS(false);
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

USR_LG_206_P_UART_SETTINGS *USR_LG_206_P_SETTINGS::get_uart(void)
{
    return this->uart;
};

void USR_LG_206_P_SETTINGS::set_uart(USR_LG_206_P_UART_SETTINGS *newUARTSettings)
{
    delete this->uart;
    this->uart = newUARTSettings;
};