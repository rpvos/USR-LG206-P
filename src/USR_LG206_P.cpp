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

#include "usr_lg206_p.h"
#include "usr_lg206_p_settings.h"
#include <Arduino.h>
#include <MAX485TTL.h>
#include <Regexp.h>

// #define LOGGER_

LoRa::LoRa(RS485 *serial)
{
    this->serial = serial;
    this->settings = new LoRaSettings(false);
};

LoRa::~LoRa(void)
{
    this->serial = nullptr;
    delete this->settings;
};

int LoRa::factory_reset(void)
{
    LoRaSettings settings = LoRaSettings(true);
    return set_settings(&settings);
}

int LoRa::set_settings(LoRaSettings *settings)
{
    delete this->settings;
    this->settings = settings;
    // TODO add setter for every setting
    return false;
}

int LoRa::get_settings(OUT LoRaSettings &settings)
{
    // TODO use getter for every setting
    return false;
}

int LoRa::begin_AT_mode(void)
{
    // Check if the LoRa module is already in AT mode
    if (settings->ATMode)
    {
        return true;
    }

    String received_data = "";
    String sent_data = "+++";
    received_data = SendCommand(sent_data);

    String expected_data = "a";
    if (received_data.indexOf(expected_data) < 0)
    {
        log_warning(expected_data, received_data);
        delay(1000);
        return false;
    }

    sent_data = "a";
    received_data = SendCommand(sent_data);

    expected_data = "+OK";
    if (received_data.indexOf(expected_data) < 0)
    {
        log_warning(expected_data, received_data);
        return false;
    }

    settings->ATMode = true;
    return true;
};

int LoRa::end_AT_mode(void)
{
    // Check if LoRa module was already out of AT mode
    if (!(settings->ATMode))
    {
        return true;
    }

    String command = "+ENTM";
    return SetCommand(command, "OK");
};

int LoRa::set_echo(bool isOn)
{
    String command = "+E=";
    if (isOn)
    {
        command += "ON";
    }
    else
    {
        command += "OFF";
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->commandEchoFunction = isOn;
        return true;
    }

    return false;
};

int LoRa::get_echo(OUT bool &isOn)
{
    if (settings->commandEchoFunction != SETTING_UNDEFINED)
    {
        return settings->commandEchoFunction;
    }

    String command = "+E";

    // TODO
    //  serial->println("AT" + command);
    //  String expected_data = command;
    //  serial->WaitForInput();
    //  String received_data = serial->readString();

    // if (received_data != expected_data)
    // {
    //     log_warning(expected_data, received_data);
    //     return false;
    // }

    // serial->WaitForInput();
    // received_data = serial->readString();

    // expected_data = "OK=";

    // String subStringed = received_data.substring(0, received_data.indexOf('='));

    // if (subStringed != expected_data)
    // {
    //     log_warning(expected_data, received_data);
    //     return false;
    // }

    // String value = received_data.substring(received_data.indexOf('='));
    // if (value == "ON")
    // {
    //     settings->commandEchoFunction = true;
    //     isOn = true;
    //     return true;
    // }
    // else if (value == "OFF")
    // {
    //     settings->commandEchoFunction = false;
    //     isOn = false;
    //     return true;
    // }

    return false;
};

int LoRa::restart(void)
{
    String command = "+Z\n";
    int response = SetCommand(command);

    // TODO Check for LoRa start

    if (response)
    {
        settings->ATMode = false;
    }

    return response;
};

int LoRa::save_as_default(void)
{
    String command = "+CFGTF";
    String succes_message = "+CFGTF:SAVED";
    return SetCommand(command, succes_message);
};

int LoRa::reset_to_default(void)
{
    String command = "AT+RELD";
    serial->println(command);
    String received_data = serial->readString();
    String expected_data = command;

    if (received_data != expected_data)
    {
        log_warning(expected_data, received_data);
        return false;
    }

    received_data = serial->readString();

    expected_data = "REBOOTING";

    if (received_data != expected_data)
    {
        log_warning(expected_data, received_data);
        return false;
    }

    return true;
};

int LoRa::get_node_id(OUT String &node_id)
{
    if (settings->nodeID != "")
    {
        node_id = settings->nodeID;
        return true;
    }

    String command = "+NID";
    String value = GetCommand(command);

    if (value)
    {
        node_id = value;
        return true;
    }

    return false;
};

int LoRa::get_firmware_version(OUT String &firmware_version)
{
    if (settings->firmwareVersion != "")
    {
        firmware_version = settings->firmwareVersion;
        return true;
    }

    String command = "+VER";
    String value = GetCommand(command);

    if (value)
    {
        firmware_version = value;
        return true;
    }

    return false;
};

int LoRa::set_wmode(WorkMode wmode = work_mode_transparent)
{
    String command = "+WMODE=";
    if (wmode == work_mode_transparent)
    {
        command += "TRANS";
    }
    if (wmode == work_mode_fixed_point)
    {
        command += "FP";
    }
    else
    {
#ifdef LOGGER_
        Log.warningln("Wmode is not defined (\"%d\")", wmode);
#endif
        return false;
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        settings->workMode = wmode;
        return true;
    }

    return false;
};

int LoRa::get_wmode(OUT WorkMode &wmode)
{
    if (settings->workMode != work_mode_undefined)
    {
        wmode = settings->workMode;
        return true;
    }

    String command = "+WMODE";
    String value = GetCommand(command);

    if (value == "TRANS")
    {
        settings->workMode = work_mode_transparent;
        wmode = work_mode_transparent;
        return true;
    }
    else if (value == "FP")
    {
        settings->workMode = work_mode_fixed_point;
        wmode = work_mode_fixed_point;
        return true;
    }

    return false;
};

int LoRa::set_uart(LoRaUartSettings *uart_settings)
{
    if (uart_settings == 0)
    {
        return false;
    }

    String command = "+UART=";
    command += uart_settings->toString();

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->set_uart(uart_settings);
        return true;
    }

    return false;
};

int LoRa::get_uart(OUT LoRaUartSettings &uart_settings)
{
    if (settings->get_uart() != 0)
    {
        uart_settings = *settings->get_uart();
        return true;
    }

    String command = "+UART";
    String value = GetCommand(command);

    if (uart_settings.fromString(value))
    {
        return true;
    }

    return false;
};

int LoRa::set_power_consumption_mode(PowerConsumptionMode powermode = powermode_run)
{
    String command = "+PMODE=";
    if (powermode == powermode_run)
    {
        command += "RUN";
    }
    if (powermode == powermode_wake_up)
    {
        command += "WU";
    }
    else
    {
#ifdef LOGGER_
        Log.warningln("Powermode is not defined (\"%d\")", powermode);
#endif
        return false;
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->powerMode = powermode;
        return true;
    }

    return false;
};

int LoRa::get_power_consumption_mode(OUT PowerConsumptionMode &powermode)
{
    if (settings->powerMode != powermode_undefined)
    {
        powermode = settings->powerMode;
        return true;
    }

    String command = "+PMODE";
    String value = GetCommand(command);

    if (value == "RUN")
    {
        settings->powerMode = powermode_run;
        powermode = powermode_run;
        return true;
    }
    else if (value == "WU")
    {
        settings->powerMode = powermode_wake_up;
        powermode = powermode_wake_up;
        return true;
    }

    return false;
};

int LoRa::set_waking_up_interval(int wake_up_interval = 2000)
{
    String command = "+WTM=";
    if (500 <= wake_up_interval && wake_up_interval <= 4000)
    {
        command += wake_up_interval;
    }
    else
    {
#ifdef LOGGER_
        Log.warningln("Wake up interval is out of bounds (\"%d\")", wake_up_interval);
#endif
        return false;
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->wakeUpInterval = wake_up_interval;
        return true;
    }

    return false;
};
int LoRa::get_waking_up_interval(OUT int &wake_up_interval)
{
    if (settings->wakeUpInterval != SETTING_UNDEFINED)
    {
        wake_up_interval = settings->wakeUpInterval;
        return true;
    }

    String command = "+WTM";
    String value = GetCommand(command);

    if (value)
    {
        settings->wakeUpInterval = value.toInt();
        wake_up_interval = value.toInt();
        return true;
    }

    return false;
};

int LoRa::set_speed(LoRaAirRateLevel speed = LoRa_air_rate_level_21875)
{
    String command = "+SPD=";
    if (1 <= speed && speed <= 10)
    {
        command += speed;
    }
    else
    {
        return -1;
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->loraAirRateLevel = speed;
        return true;
    }

    return false;
};

int LoRa::get_speed(OUT LoRaAirRateLevel &speed)
{
    if (settings->loraAirRateLevel != LoRa_air_rate_level_undefined)
    {
        speed = settings->loraAirRateLevel;
        return true;
    }

    String command = "+SPD";
    String value = GetCommand(command);

    if (value)
    {
        settings->loraAirRateLevel = LoRaAirRateLevel(value.toInt());
        speed = LoRaAirRateLevel(value.toInt());
        return true;
    }

    return false;
};

int LoRa::set_address(int address = 0)
{
    String command = "+ADDR=";
    if (1 <= address && address <= 65535)
    {
        command += address;
    }
    else
    {
        return -1;
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->destinationAddress = address;
        settings->destinationAddressIsSet = true;
        return true;
    }

    return false;
};

int LoRa::get_address(OUT int &address)
{
    if (settings->destinationAddressIsSet)
    {
        address = settings->destinationAddress;
        return true;
    }

    String command = "+ADDR";
    String value = GetCommand(command);

    if (value)
    {
        settings->destinationAddress = value.toInt();
        settings->destinationAddressIsSet = true;
        address = value.toInt();
        return true;
    }

    return false;
}

int LoRa::set_channel(int channel = 65)
{
    String command = "+CH=";
    if (0 <= channel && channel <= 127)
    {
        command += channel;
    }
    else
    {
#ifdef LOGGER_
        Log.warningln("Channel is out of bounds (\"%d\")", channel);
#endif
        return false;
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->channel = channel;
        return true;
    }

    return false;
};

int LoRa::get_channel(OUT int &channel)
{
    if (settings->channel != SETTING_UNDEFINED)
    {
        channel = settings->channel;
        return true;
    }

    String command = "+CH";
    String value = GetCommand(command);

    if (value)
    {
        settings->channel = value.toInt();
        channel = value.toInt();
        return true;
    }

    return false;
};

int LoRa::set_forward_error_correction(bool isOn = false)
{
    String command = "+FEC=";
    if (isOn)
    {
        command += "ON";
    }
    else
    {
        command += "OFF";
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->forwardErrorCorrection = isOn;
        return true;
    }

    return false;
}

int LoRa::get_forward_error_correction(OUT bool &isOn)
{

    if (settings->forwardErrorCorrection != SETTING_UNDEFINED)
    {
        isOn = settings->forwardErrorCorrection;
        return true;
    }

    String command = "+FEC";
    String value = GetCommand(command);

    if (value == "ON")
    {
        settings->forwardErrorCorrection = true;
        isOn = true;
        return true;
    }
    else if (value == "OFF")
    {
        settings->forwardErrorCorrection = false;
        isOn = false;
        return true;
    }

    return false;
}

int LoRa::set_power_transmission_value(int power = 20)
{
    String command = "+PWR=";
    if (10 <= power && power <= 20)
    {
        command += power;
    }
    else
    {
#ifdef LOGGER_
        Log.warningln("power is out of bounds (\"%d\")", power);
#endif
        return false;
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->transmittingPower = power;
        return true;
    }

    return false;
};

int LoRa::get_power_transmission_value(OUT int &power)
{
    if (settings->transmittingPower != SETTING_UNDEFINED)
    {
        power = settings->transmittingPower;
        return true;
    }

    String command = "+PWR";
    String value = GetCommand(command);

    if (value)
    {
        settings->transmittingPower = value.toInt();
        power = value.toInt();
        return true;
    }

    return false;
};

int LoRa::set_transmission_interval(int interval = 2000)
{
    String command = "+SQT=";
    if ((100 <= interval && interval <= 6000) || false)
    {
        command += interval;
    }
    else
    {
#ifdef LOGGER_
        Log.warningln("interval is out of bounds (\"%d\")", interval);
#endif
        return false;
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->testInterval = interval;
        return true;
    }

    return false;
};

int LoRa::get_transmission_interval(OUT int &interval)
{
    if (settings->testInterval != SETTING_UNDEFINED)
    {
        interval = settings->testInterval;
        return true;
    }

    String command = "+SQT";
    // TODO probably other way of receiving data then normal
    String value = GetCommand(command);

    if (value)
    {
        settings->testInterval = value.toInt();
        interval = value.toInt();
        return true;
    }

    return false;
};

int LoRa::SetKey(String key)
{
    String command = "+KEY=";
    if (key.length() == 8)
    {
        command += key;
    }
    else
    {
        return -1;
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->key = key;
        return true;
    }

    return false;
};

String LoRa::ReceiveMessage(void)
{
    // Wait for data to be received
    serial->WaitForInput();
    if (serial->available())
    {
        return serial->readString();
    }

    return "";
};

int LoRa::SendMessage(char *message)
{
    int amountOfBytesWritten = serial->println(message);
    if (amountOfBytesWritten)
    {
        // Make sure the full message is sent and set mode back to input so other messeages can be received
        serial->flush();
        serial->SetMode(INPUT);
        return amountOfBytesWritten;
    }

    return false;
};

#pragma region private functions

String LoRa::SendCommand(String command)
{
    serial->print(command);
    serial->flush();

    serial->WaitForInput();
    if (serial->available())
    {
        return serial->readString();
    }

    return "";
}

int LoRa::SetCommand(String command, String succesfullResponse)
{
    command = "AT" + command + "\n";
    String expected_data = command;
    String received_data = SendCommand(command);

    // TODO regex
    MatchState ms;
    char outputBuffer[64];

    char temp[received_data.length()];
    received_data.toCharArray(temp, received_data.length());

    // string we are searching
    ms.Target(temp);

    // search it
    char result = ms.Match("AT%+([A-Z]+)=(%w+)[%s]*(%w+[-%d]*)", 0);

    if (result == REGEXP_MATCHED)
    {
        for (int i = 0; i < ms.level; i++)
        {
            Serial.println("Succes");
            Serial.println(ms.GetCapture(outputBuffer, i));
        }
    }
    else
    {
        if (result == REGEXP_NOMATCH)
        {
            Serial.println("No match");
        }
        else
        {
            Serial.println("Error");
            Serial.println(result, DEC);
        }
    }

    // If echo is enabled check for the repeated command
    if (this->settings->commandEchoFunction)
    {
        String returned_command = received_data.substring(0, command.length());
        if (returned_command != command)
        {
            return false;
        }
    }

    // Check for error
    int indexError = received_data.indexOf("ERR");
    if (indexError != -1)
    {
        Serial.println("On error");
        Serial.println(received_data);
        // On error retry
        received_data = SendCommand(command);
        // If echo is enabled check for the repeated command
        if (this->settings->commandEchoFunction)
        {
            String returned_command = received_data.substring(0, command.length());
            if (returned_command != command)
            {
                Serial.println("On error");
                Serial.println(received_data);
                return false;
            }
        }
    }

    int index = received_data.indexOf(succesfullResponse);
    if (index == -1)
    {
        Serial.println("Response unsuccesfull");
        return false;
    }

    return true;
};

String LoRa::GetCommand(String command, String succesfullResponse)
{
    String returnValue = "";
    command = "AT" + command + "\n";
    String expected_data = command;
    String received_data = SendCommand(command);

    // If echo is enabled check for the repeated command
    if (this->settings->commandEchoFunction)
    {
        String returned_command = received_data.substring(0, command.length());
        if (returned_command != command)
        {
            return returnValue;
        }
    }

    // Check for error
    int indexError = received_data.indexOf("ERR");
    if (indexError != -1)
    {
        // On error retry
        received_data = SendCommand(command);
        // If echo is enabled check for the repeated command
        if (this->settings->commandEchoFunction)
        {
            String returned_command = received_data.substring(0, command.length());
            if (returned_command != command)
            {
                return returnValue;
            }
        }
    }

    // Check if : is present and if OK is present
    String gottenSetting = command + ":";
    int index = received_data.indexOf(gottenSetting);
    int okInResponse = received_data.indexOf(succesfullResponse);
    if (index == -1 || okInResponse == -1)
    {
        Serial.println("Response unsuccesfull");
        return returnValue;
    }

    // Return the part containing the setting
    return received_data.substring(index, okInResponse - 1);
};

void LoRa::log_warning(String expected_data, String actual_data)
{
#ifdef LOGGER_
    Serial.print("Received received_data is not\n" + expected_data + "but\n" + actual_data);
#endif
};

#pragma endregion