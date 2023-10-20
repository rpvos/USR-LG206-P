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
#include "usr_lg206_p_uart_settings.h"
#include <Arduino.h>
#include <MAX485TTL.h>

LoRa::LoRa(RS485 *serial)
{
    this->serial_ = serial;
    this->settings_ = new LoRaSettings::LoRaSettings(false);
};

LoRa::~LoRa(void)
{
    this->serial_ = nullptr;
    delete this->settings_;
};

int LoRa::FactoryReset(void)
{
    LoRaSettings::LoRaSettings factory_settings = LoRaSettings::LoRaSettings(true);
    return SetSettings(&factory_settings);
};

int LoRa::SetSettings(LoRaSettings::LoRaSettings *settings)
{
    delete this->settings_;
    this->settings_ = settings;
    // TODO add setter for every setting
    return false;
}

int LoRa::GetSettings(OUT LoRaSettings::LoRaSettings &settings)
{
    // TODO use getter for every setting
    return false;
}

int LoRa::BeginAtMode(void)
{
    // Check if the LoRa module is already in AT mode
    if (settings_->at_mode)
    {
        return true;
    }

    String received_data = "";
    String sent_data = "+++";
    received_data = SendCommand(sent_data);

    String expected_data = "a";
    if (received_data.indexOf(expected_data) < 0)
    {
        delay(1000);
        return false;
    }

    sent_data = "a";
    received_data = SendCommand(sent_data);

    expected_data = "+OK";
    if (received_data.indexOf(expected_data) < 0)
    {
        return false;
    }

    settings_->at_mode = LoRaSettings::kAtModeIsOn;
    return true;
};

int LoRa::EndAtMode(void)
{
    // Check if LoRa module was already out of AT mode
    if (!(settings_->at_mode))
    {
        return true;
    }

    String command = "+ENTM";
    return SetCommand(command, "OK");
};

LoRaSettings::CommandEchoFunction LoRa::SetEcho(bool isOn)
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
        LoRaSettings::CommandEchoFunction setting;
        if (isOn)
        {
            setting = LoRaSettings::kEchoFunctionIsOn;
        }
        else
        {
            setting = LoRaSettings::kEchoFunctionIsOn;
        }
        settings_->command_echo_function = setting;
        return setting;
    }

    return LoRaSettings::kEchoFunctionUndefined;
};

LoRaSettings::CommandEchoFunction LoRa::GetEcho()
{
    if (settings_->command_echo_function != LoRaSettings::kEchoFunctionUndefined)
    {
        return settings_->command_echo_function;
    }

    String command = "+E";

    serial_->println("AT" + command);
    String expected_data = command;
    serial_->WaitForInput();
    String received_data = serial_->readString();
    // TODO check for contain
    if (received_data != expected_data)
    {
        return LoRaSettings::kEchoFunctionUndefined;
    }

    serial_->WaitForInput();
    received_data = serial_->readString();

    expected_data = "OK=";

    String subStringed = received_data.substring(0, received_data.indexOf('='));

    if (subStringed != expected_data)
    {
        return LoRaSettings::kEchoFunctionUndefined;
    }

    String value = received_data.substring(received_data.indexOf('='));
    if (value == "ON")
    {
        settings_->command_echo_function = LoRaSettings::kEchoFunctionIsOn;
        return settings_->command_echo_function;
    }
    else if (value == "OFF")
    {
        settings_->command_echo_function = LoRaSettings::kEchoFunctionIsOff;
        return settings_->command_echo_function;
    }

    return LoRaSettings::kEchoFunctionUndefined;
};

int LoRa::Restart(void)
{
    String command = "+Z\n";
    int response = SetCommand(command);

    // TODO Check for LoRa start

    if (response)
    {
        settings_->at_mode = LoRaSettings::kAtModeIsOff;
    }

    return response;
};

int LoRa::SaveAsDefault(void)
{
    String command = "+CFGTF";
    String succes_message = "+CFGTF:SAVED";
    return SetCommand(command, succes_message);
};

int LoRa::ResetToDefault(void)
{
    String command = "AT+RELD";
    serial_->println(command);
    serial_->WaitForInput();
    String received_data = serial_->readString();
    String expected_data = command;

    if (received_data != expected_data)
    {
        return false;
    }

    serial_->WaitForInput();
    received_data = serial_->readString();
    expected_data = "REBOOTING";

    if (received_data != expected_data)
    {
        return false;
    }

    return true;
};

int LoRa::GetNodeId(OUT String &node_id)
{
    if (settings_->node_id != "")
    {
        node_id = settings_->node_id;
        return true;
    }

    String command = "+NID";
    String value = GetCommand(command);

    if (value)
    {
        node_id = value;
        settings_->node_id = value;
        return true;
    }

    return false;
};

int LoRa::GetFirmwareVersion(OUT String &firmware_version)
{
    if (settings_->firmware_version != "")
    {
        firmware_version = settings_->firmware_version;
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

int LoRa::SetWorkMode(LoRaSettings::WorkMode wmode)
{
    String command = "+WMODE=";
    if (wmode == LoRaSettings::kWorkModeTransparent)
    {
        command += "TRANS";
    }
    else if (wmode == LoRaSettings::kWorkModeFixedPoint)
    {
        command += "FP";
    }
    else
    {
        return false;
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        settings_->work_mode = wmode;
        return true;
    }

    return false;
};

LoRaSettings::WorkMode LoRa::GetWorkMode()
{
    if (settings_->work_mode != LoRaSettings::kWorkModeUndefined)
    {
        return settings_->work_mode;
    }

    String command = "+WMODE";
    String value = GetCommand(command);

    if (value == "TRANS")
    {
        settings_->work_mode = LoRaSettings::kWorkModeTransparent;
    }
    else if (value == "FP")
    {
        settings_->work_mode = LoRaSettings::kWorkModeFixedPoint;
    }

    return settings_->work_mode;
};

int LoRa::SetUartSettings(LoRaUartSettings::LoRaUartSettings *uart_settings)
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
        settings_->SetUartSettings(uart_settings);
        return true;
    }

    return false;
};

int LoRa::GetUartSettings(OUT LoRaUartSettings::LoRaUartSettings &uart_settings)
{
    if (settings_->GetUartSettings() != 0)
    {
        uart_settings = *settings_->GetUartSettings();
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

int LoRa::set_power_consumption_mode(LoRaSettings::PowerConsumptionMode powermode)
{
    String command = "+PMODE=";
    if (powermode == LoRaSettings::kPowerConsumptionModeRun)
    {
        command += "RUN";
    }
    if (powermode == LoRaSettings::kPowerConsumptionModeWakeUp)
    {
        command += "WU";
    }
    else
    {
        return false;
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings_->power_mode = powermode;
        return true;
    }

    return false;
};

LoRaSettings::PowerConsumptionMode LoRa::get_power_consumption_mode()
{
    if (settings_->power_mode != LoRaSettings::kPowerConsumptionModeUndefined)
    {
        return settings_->power_mode;
    }

    String command = "+PMODE";
    String value = GetCommand(command);

    if (value == "RUN")
    {
        settings_->power_mode = LoRaSettings::kPowerConsumptionModeRun;
    }
    else if (value == "WU")
    {
        settings_->power_mode = LoRaSettings::kPowerConsumptionModeWakeUp;
    }

    return settings_->power_mode;
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
        return false;
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings_->wake_up_interval = wake_up_interval;
        return true;
    }

    return false;
};
int LoRa::get_waking_up_interval(OUT int &wake_up_interval)
{
    // TODO find what undefined is
    if (settings_->wake_up_interval != -1)
    {
        wake_up_interval = settings_->wake_up_interval;
        return true;
    }

    String command = "+WTM";
    String value = GetCommand(command);

    if (value)
    {
        settings_->wake_up_interval = value.toInt();
        wake_up_interval = value.toInt();
        return true;
    }

    return false;
};

int LoRa::set_speed(LoRaSettings::LoRaAirRateLevel speed)
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
        settings_->lora_air_rate_level = speed;
        return true;
    }

    return false;
};

LoRaSettings::LoRaAirRateLevel LoRa::get_speed()
{
    if (settings_->lora_air_rate_level != LoRaSettings::kLoRaAirRateLevelUndefined)
    {
        return settings_->lora_air_rate_level;
    }

    String command = "+SPD";
    String value = GetCommand(command);

    if (value)
    {
        settings_->lora_air_rate_level = LoRaSettings::LoRaAirRateLevel(value.toInt());
    }

    return settings_->lora_air_rate_level;
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
        settings_->destination_address = address;
        settings_->destination_address_is_set = true;
        return true;
    }

    return false;
};

int LoRa::get_address(OUT int &address)
{
    if (settings_->destination_address_is_set)
    {
        address = settings_->destination_address;
        return true;
    }

    String command = "+ADDR";
    String value = GetCommand(command);

    if (value)
    {
        settings_->destination_address = value.toInt();
        settings_->destination_address_is_set = true;
        address = value.toInt();
        return true;
    }

    return false;
}

int LoRa::set_channel(int channel)
{
    String command = "+CH=";
    if (0 <= channel && channel <= 127)
    {
        command += channel;
    }
    else
    {
        return false;
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings_->channel = channel;
        return true;
    }

    return false;
};

int LoRa::get_channel(OUT int &channel)
{
    if (settings_->channel != -1)
    {
        channel = settings_->channel;
        return true;
    }

    String command = "+CH";
    String value = GetCommand(command);

    if (value)
    {
        settings_->channel = value.toInt();
        channel = value.toInt();
        return true;
    }

    return false;
};

int LoRa::set_forward_error_correction(LoRaSettings::ForwardErrorCorrection setting)
{
    String command = "+FEC=";
    if (setting == LoRaSettings::kForwardErrorCorrectionIsOn)
    {
        command += "ON";
    }
    else if (setting == LoRaSettings::kForwardErrorCorrectionIsOff)
    {
        command += "OFF";
    }
    else
    {
        return false;
    }

    int received_data = SetCommand(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings_->forward_error_correction = setting;
        return true;
    }

    return false;
}

LoRaSettings::ForwardErrorCorrection LoRa::get_forward_error_correction()
{

    if (settings_->forward_error_correction != LoRaSettings::kForwardErrorCorrectionUndefined)
    {
        return settings_->forward_error_correction;
    }

    String command = "+FEC";
    String value = GetCommand(command);

    if (value == "ON")
    {
        settings_->forward_error_correction = LoRaSettings::kForwardErrorCorrectionIsOn;
    }
    else if (value == "OFF")
    {
        settings_->forward_error_correction = LoRaSettings::kForwardErrorCorrectionIsOff;
    }

    return settings_->forward_error_correction;
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
        settings_->transmitting_power = power;
        return true;
    }

    return false;
};

int LoRa::get_power_transmission_value(OUT int &power)
{
    if (settings_->transmitting_power != 0)
    {
        power = settings_->transmitting_power;
        return true;
    }

    String command = "+PWR";
    String value = GetCommand(command);

    if (value)
    {
        settings_->transmitting_power = value.toInt();
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
        settings_->test_interval = interval;
        return true;
    }

    return false;
};

int LoRa::get_transmission_interval(OUT int &interval)
{
    if (settings_->test_interval != 0)
    {
        interval = settings_->test_interval;
        return true;
    }

    String command = "+SQT";
    // TODO probably other way of receiving data then normal
    String value = GetCommand(command);

    if (value)
    {
        settings_->test_interval = value.toInt();
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
        settings_->key = key;
        return true;
    }

    return false;
};

String LoRa::ReceiveMessage(void)
{
    // Wait for data to be received
    serial_->WaitForInput();
    if (serial_->available())
    {
        return serial_->readString();
    }

    return "";
};

int LoRa::SendMessage(char *message)
{
    if (this->settings_->work_mode != LoRaSettings::kWorkModeTransparent)
    {
        return 2;
    }

    int amountOfBytesWritten = serial_->println(message);
    if (amountOfBytesWritten)
    {
        // Make sure the full message is sent and set mode back to input so other messeages can be received
        serial_->flush();
        serial_->SetMode(INPUT);
        return amountOfBytesWritten;
    }

    return false;
};

int LoRa::SendMessage(uint16_t destination_address, uint8_t channel, char *message, uint8_t message_size)
{
    if (this->settings_->work_mode != LoRaSettings::kWorkModeFixedPoint)
    {
        return 2;
    }

    const uint8_t destination_address_size = sizeof(uint16_t);
    const uint8_t channel_size = sizeof(uint8_t);
    char message_buffer[destination_address_size + channel_size + message_size];
    // Copy destination address into message buffer
    memcpy(message_buffer, &destination_address, destination_address_size);
    // Copy channel into message buffer
    memcpy(message_buffer + destination_address_size, &channel, channel_size);
    // Copy message into message buffer
    memcpy(message_buffer + destination_address_size + channel_size, message, message_size);

    size_t amount_of_bytes_written = serial_->write(message, destination_address_size + channel_size + message_size);
    if (amount_of_bytes_written == destination_address_size + channel_size + message_size)
    {
        // Make sure the full message is sent and set mode back to input so other messeages can be received
        serial_->flush();
        serial_->SetMode(INPUT);
        return EXIT_SUCCESS;
    }

    return 1;
};

#pragma region private functions

String LoRa::SendCommand(String command)
{
    serial_->print(command);
    serial_->flush();

    serial_->WaitForInput();
    if (serial_->available())
    {
        return serial_->readString();
    }

    return "";
}

int LoRa::SetCommand(String command, String succesfullResponse)
{
    command = "AT" + command + "\n";
    String expected_data = command;
    String received_data = SendCommand(command);

    // If echo is enabled check for the repeated command
    if (this->settings_->command_echo_function)
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
        if (this->settings_->command_echo_function)
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
    if (this->settings_->command_echo_function)
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
        if (this->settings_->command_echo_function)
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

#pragma endregion