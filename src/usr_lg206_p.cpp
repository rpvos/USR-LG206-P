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
#include <Arduino.h>

#ifndef kDelayTimeAfterSwitch
#define kDelayTimeAfterSwitch 10
#endif

#ifndef kDelayTimeBetweenChars
#define kDelayTimeBetweenChars 20
#endif

UsrLg206P::UsrLg206P(RS485 *serial)
{
    this->serial_ = serial;
    this->settings_ = LoRaSettings::LoRaSettings(false);
};

UsrLg206P::~UsrLg206P(void)
{
    this->serial_ = nullptr;
};

LoRaErrorCode UsrLg206P::FactoryReset(void)
{
    LoRaSettings::LoRaSettings factory_settings = LoRaSettings::LoRaSettings(true);
    return SetSettings(factory_settings);
};

LoRaErrorCode UsrLg206P::SetSettings(LoRaSettings::LoRaSettings settings)
{
    this->settings_ = settings;
    // TODO add setter for every setting
    return LoRaErrorCode::kNoResponse;
}

LoRaErrorCode UsrLg206P::GetSettings(OUT LoRaSettings::LoRaSettings &settings)
{
    // TODO use getter for every setting

    return LoRaErrorCode::kNoResponse;
}

LoRaErrorCode UsrLg206P::BeginAtMode(void)
{
    // Check if the LoRa module is already in AT mode
    if (settings_.at_mode == LoRaSettings::AtMode::kAtModeIsOn)
    {
        return LoRaErrorCode::kSucces;
    }

    String sent_data = "+++";
    size_t bytes_written = SendCommand(sent_data);

    const size_t buffer_size = 128;
    uint8_t buffer[buffer_size];
    size_t size = ReceiveMessage(buffer, buffer_size);
    String received_data = String((char *)buffer);
    if (received_data.length() < 1)
    {
        return LoRaErrorCode::kNoResponse;
    }

    String expected_data = "a";
    if (received_data.indexOf(expected_data) < 0)
    {
        return LoRaErrorCode::kInvalidResponse;
    }

    sent_data = "a";
    bytes_written = SendCommand(sent_data);

    size = ReceiveMessage(buffer, buffer_size);
    received_data = String((char *)buffer);
    if (received_data.length() < 1)
    {
        return LoRaErrorCode::kNoResponse;
    }

    expected_data = "+OK";
    if (received_data.indexOf(expected_data) < 0)
    {
        return LoRaErrorCode::kInvalidResponse;
    }

    settings_.at_mode = LoRaSettings::AtMode::kAtModeIsOn;
    return LoRaErrorCode::kSucces;
};

LoRaErrorCode UsrLg206P::EndAtMode(void)
{
    // Check if LoRa module was already out of AT mode
    if (settings_.at_mode == LoRaSettings::AtMode::kAtModeIsOff)
    {
        return LoRaErrorCode::kSucces;
    }

    String command = "+ENTM";
    LoRaErrorCode response_code = SetCommand(command, "OK");

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.at_mode = LoRaSettings::AtMode::kAtModeIsOff;
    }
    return response_code;
};

LoRaErrorCode UsrLg206P::SetEcho(LoRaSettings::CommandEchoFunction setting)
{
    if (setting == settings_.command_echo_function)
    {
        return LoRaErrorCode::kSucces;
    }

    String command = "+E=";
    if (setting == LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOn)
    {
        command += "ON";
    }
    else if (setting == LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOff)
    {
        command += "OFF";
    }
    else
    {
        return LoRaErrorCode::kInvalidParameter;
    }

    LoRaErrorCode response_code = SetCommand(command);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.command_echo_function = setting;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::GetEcho(LoRaSettings::CommandEchoFunction &setting)
{
    if (settings_.command_echo_function != LoRaSettings::CommandEchoFunction::kCommandEchoFunctionUndefined)
    {
        setting = settings_.command_echo_function;
        return LoRaErrorCode::kSucces;
    }

    String command = "+E";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value, false);

    if (response_code == LoRaErrorCode::kSucces)
    {
        if (value.indexOf("ON") != -1)
        {
            settings_.command_echo_function = LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOn;
            setting = settings_.command_echo_function;
        }
        else if (value.indexOf("OFF") != -1)
        {
            settings_.command_echo_function = LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOff;
            setting = settings_.command_echo_function;
        }
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::Restart(void)
{
    String command = "+Z";
    LoRaErrorCode response = SetCommand(command);

    // TODO Check for LoRa start

    if (response == LoRaErrorCode::kSucces)
    {
        settings_.at_mode = LoRaSettings::AtMode::kAtModeIsOff;
    }

    return response;
};

LoRaErrorCode UsrLg206P::SaveAsDefault(void)
{
    String command = "+CFGTF";
    String succes_message = "+CFGTF:SAVED";
    return SetCommand(command, succes_message);
};

LoRaErrorCode UsrLg206P::ResetToDefault(void)
{
    String command = "+RELD";
    return SetCommand(command, "REBOOTING");
};

LoRaErrorCode UsrLg206P::GetNodeId(OUT String &node_id)
{
    if (settings_.node_id != "")
    {
        node_id = settings_.node_id;
        return LoRaErrorCode::kSucces;
    }

    String command = "+NID";
    LoRaErrorCode response_code = GetCommand(command, node_id);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.node_id = node_id;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::GetFirmwareVersion(OUT String &setting)
{
    if (settings_.firmware_version.length())
    {
        setting = settings_.firmware_version;
        return LoRaErrorCode::kSucces;
    }

    String command = "+VER";
    LoRaErrorCode response_code = GetCommand(command, setting);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.firmware_version = setting;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::SetWorkMode(LoRaSettings::WorkMode setting)
{
    if (setting == settings_.work_mode)
    {
        return LoRaErrorCode::kSucces;
    }

    String command = "+WMODE=";
    if (setting == LoRaSettings::WorkMode::kWorkModeTransparent)
    {
        command += "TRANS";
    }
    else if (setting == LoRaSettings::WorkMode::kWorkModeFixedPoint)
    {
        command += "FP";
    }
    else
    {
        return LoRaErrorCode::kInvalidParameter;
    }

    LoRaErrorCode response_code = SetCommand(command);

    // If the set command was done succesfull
    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.work_mode = setting;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::GetWorkMode(LoRaSettings::WorkMode &setting)
{
    if (settings_.work_mode != LoRaSettings::WorkMode::kWorkModeUndefined)
    {
        setting = settings_.work_mode;
        return LoRaErrorCode::kSucces;
    }

    String command = "+WMODE";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        if (value.indexOf("TRANS") != -1)
        {
            settings_.work_mode = LoRaSettings::WorkMode::kWorkModeTransparent;
            setting = settings_.work_mode;
        }
        else if (value.indexOf("FP") != -1)
        {
            settings_.work_mode = LoRaSettings::WorkMode::kWorkModeFixedPoint;
            setting = settings_.work_mode;
        }
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::SetUartSettings(LoRaUartSettings::LoRaUartSettings *setting)
{
    String command = "+UART=";
    if (setting != nullptr)
    {
        command += setting->toString();
    }
    else
    {
        return LoRaErrorCode::kInvalidParameter;
    }

    LoRaErrorCode response_code = SetCommand(command);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.SetUartSettings(setting);
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::GetUartSettings(OUT LoRaUartSettings::LoRaUartSettings *setting)
{
    if (settings_.GetUartSettings() != 0)
    {
        setting = settings_.GetUartSettings();
        return LoRaErrorCode::kSucces;
    }

    String command = "+UART";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        setting->fromString(value);
        settings_.SetUartSettings(setting);
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::SetPowerConsumptionMode(LoRaSettings::PowerConsumptionMode setting)
{
    if (setting == settings_.power_consumption_mode)
    {
        return LoRaErrorCode::kSucces;
    }

    String command = "+PMODE=";
    if (setting == LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeRun)
    {
        command += "RUN";
    }
    else if (setting == LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeWakeUp)
    {
        command += "WU";
    }
    else
    {
        return LoRaErrorCode::kInvalidParameter;
    }

    LoRaErrorCode response_code = SetCommand(command);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.power_consumption_mode = setting;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::GetPowerConsumptionMode(LoRaSettings::PowerConsumptionMode &setting)
{
    if (settings_.power_consumption_mode != LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeUndefined)
    {
        setting = settings_.power_consumption_mode;
        return LoRaErrorCode::kSucces;
    }

    String command = "+PMODE";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        if (value.indexOf("RUN") != -1)
        {
            settings_.power_consumption_mode = LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeRun;
            setting = settings_.power_consumption_mode;
        }
        else if (value.indexOf("WU") != -1)
        {
            settings_.power_consumption_mode = LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeWakeUp;
            setting = settings_.power_consumption_mode;
        }
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::SetWakingUpInterval(int setting)
{
    if (setting == settings_.wake_up_interval)
    {
        return LoRaErrorCode::kSucces;
    }

    String command = "+WTM=";
    if (500 <= setting && setting <= 4000)
    {
        command += setting;
    }
    else
    {
        return LoRaErrorCode::kInvalidParameter;
    }

    LoRaErrorCode response_code = SetCommand(command);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.wake_up_interval = setting;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::GetWakingUpInterval(OUT int &setting)
{
    if (settings_.wake_up_interval != -1)
    {
        setting = settings_.wake_up_interval;
        return LoRaErrorCode::kSucces;
    }

    String command = "+WTM";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        setting = value.toInt();
        settings_.wake_up_interval = setting;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::SetAirRateLevel(LoRaSettings::LoRaAirRateLevel setting)
{
    if (setting == settings_.lora_air_rate_level)
    {
        return LoRaErrorCode::kSucces;
    }

    if (setting == LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevelUndefined)
    {
        return LoRaErrorCode::kInvalidParameter;
    }

    String command = "+SPD=";
    command += static_cast<int>(setting);
    LoRaErrorCode response_code = SetCommand(command);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.lora_air_rate_level = setting;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::GetAirRateLevel(LoRaSettings::LoRaAirRateLevel &setting)
{
    if (settings_.lora_air_rate_level != LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevelUndefined)
    {
        setting = settings_.lora_air_rate_level;
        return LoRaErrorCode::kSucces;
    }

    String command = "+SPD";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        setting = LoRaSettings::LoRaAirRateLevel(value.toInt());
        settings_.lora_air_rate_level = setting;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::SetDestinationAddress(const uint16_t address)
{
    if (address == settings_.destination_address)
    {
        return LoRaErrorCode::kSucces;
    }

    String command = "+ADDR=";
    if (0 <= address && address <= 65535)
    {
        command += address;
    }
    else
    {
        return LoRaErrorCode::kInvalidParameter;
    }

    LoRaErrorCode response_code = SetCommand(command);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.destination_address = address;
        settings_.destination_address_is_set = true;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::GetDestinationAddress(OUT int &address)
{
    if (settings_.destination_address_is_set)
    {
        address = settings_.destination_address;
        return LoRaErrorCode::kSucces;
    }

    String command = "+ADDR";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        address = value.toInt();
        settings_.destination_address = address;
        settings_.destination_address_is_set = true;
    }

    return response_code;
}

LoRaErrorCode UsrLg206P::SetChannel(int channel)
{
    if (channel == settings_.channel)
    {
        return LoRaErrorCode::kSucces;
    }

    String command = "+CH=";
    if (0 <= channel && channel <= 127)
    {
        command += channel;
    }
    else
    {
        return LoRaErrorCode::kInvalidParameter;
    }

    LoRaErrorCode response_code = SetCommand(command);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.channel = channel;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::GetChannel(OUT int &channel)
{
    if (settings_.channel != -1)
    {
        channel = settings_.channel;
        return LoRaErrorCode::kSucces;
    }

    String command = "+CH";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        channel = value.toInt();
        settings_.channel = channel;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::SetForwardErrorCorrection(LoRaSettings::ForwardErrorCorrection setting)
{
    if (setting == settings_.forward_error_correction)
    {
        return LoRaErrorCode::kSucces;
    }

    String command = "+FEC=";
    if (setting == LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOn)
    {
        command += "ON";
    }
    else if (setting == LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOff)
    {
        command += "OFF";
    }
    else
    {
        return LoRaErrorCode::kInvalidParameter;
    }

    LoRaErrorCode response_code = SetCommand(command);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.forward_error_correction = setting;
    }

    return response_code;
}

LoRaErrorCode UsrLg206P::GetForwardErrorCorrection(LoRaSettings::ForwardErrorCorrection &setting)
{
    if (settings_.forward_error_correction != LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionUndefined)
    {
        setting = settings_.forward_error_correction;
        return LoRaErrorCode::kSucces;
    }

    String command = "+FEC";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        if (value.indexOf("ON") != -1)
        {
            settings_.forward_error_correction = LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOn;
        }
        else if (value.indexOf("OFF") != -1)
        {
            settings_.forward_error_correction = LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOff;
        }
        setting = settings_.forward_error_correction;
    }

    return response_code;
}

LoRaErrorCode UsrLg206P::SetPowerTransmissionValue(int setting)
{
    String command = "+PWR=";
    if (10 <= setting && setting <= 20)
    {
        command += setting;
    }
    else
    {
        return LoRaErrorCode::kInvalidParameter;
    }

    LoRaErrorCode response_code = SetCommand(command);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.transmitting_power = setting;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::GetPowerTransmissionValue(OUT int &setting)
{

    if (settings_.transmitting_power != 0)
    {
        setting = settings_.transmitting_power;
        return LoRaErrorCode::kSucces;
    }

    String command = "+PWR";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        setting = value.toInt();
        settings_.transmitting_power = setting;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::SetTransmissionInterval(int interval)
{
    String command = "+SQT=";
    if ((100 <= interval && interval <= 6000) || false)
    {
        command += interval;
    }
    else
    {
        return LoRaErrorCode::kInvalidParameter;
    }

    LoRaErrorCode response_code = SetCommand(command);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.test_interval = interval;
    }

    return response_code;
};

LoRaErrorCode UsrLg206P::QueryTransmissionInterval()
{
    String returnValue = "";
    String querry = "AT+SQT\r\n";
    String expected_data = querry;
    size_t bytes_written = SendCommand(querry);

    const size_t buffer_size = 128;
    uint8_t buffer[buffer_size];
    size_t size = ReceiveMessage(buffer, buffer_size);
    String received_data = String((char *)buffer);

    // If echo is enabled check for the repeated command
    if (this->settings_.command_echo_function == LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOn)
    {
        if (received_data.indexOf(querry) == -1)
        {
            return LoRaErrorCode::kCommandEchoNotReceived;
        }
    }

    int indexError = received_data.indexOf("ERR");
    if (indexError != -1)
    {
        int error = received_data.substring(indexError + 4, indexError + 5).toInt();
        return LoRaErrorCode(error);
    }

    return LoRaErrorCode::kSucces;
};

LoRaErrorCode UsrLg206P::SetKey(String key)
{
    String command = "+KEY=";
    if (key.length() == 16)
    {
        command += key;
    }
    else
    {
        return LoRaErrorCode::kInvalidParameter;
    }

    LoRaErrorCode response_code = SetCommand(command);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_.key = key;
    }

    return response_code;
}
int UsrLg206P::Available(void)
{
    return serial_->available();
};

size_t UsrLg206P::ReceiveMessage(uint8_t *buffer, size_t buffer_size)
{
    // Wait for data to be received
    serial_->WaitForInput();
    int cursor = 0;
    while (serial_->available())
    {
        // TODO: Check if wait is necessary
        delay(10);

        size_t length = serial_->available();

        // Control for overflow
        if (cursor + length > buffer_size - 1)
        {
            return cursor;
        }

        for (size_t i = 0; i < length; i++)
        {
            buffer[cursor + i] = serial_->read();
        }
        cursor += length;
        buffer[cursor + 1] = '\0';
    }
    Serial.println();

    return cursor;
};

int UsrLg206P::SendMessage(const uint8_t *message, size_t length)
{
    return SendMessage(reinterpret_cast<const char *>(message), length);
};

int UsrLg206P::SendMessage(const char *const message, const size_t length)
{
    // if (this->settings_.work_mode != LoRaSettings::WorkMode::kWorkModeTransparent)
    // {
    //     return -2;
    // }

    serial_->SetMode(OUTPUT);
    int amountOfBytesWritten = serial_->write(message, length);
    serial_->flush();
    serial_->SetMode(INPUT);

    return amountOfBytesWritten;
};

int UsrLg206P::SendMessage(const char *message, const size_t message_size, const uint16_t destination_address, const uint8_t channel)
{
    if (this->settings_.work_mode != LoRaSettings::WorkMode::kWorkModeFixedPoint)
    {
        return -2;
    }

    const size_t destination_address_size = sizeof(destination_address);
    const size_t channel_size = sizeof(channel);

    const size_t total_message_size = (destination_address_size + channel_size + message_size);

    char message_buffer[total_message_size];
    // Copy destination address into message buffer
    message_buffer[0] = (destination_address & 0xFF00) >> 8;
    message_buffer[1] = (destination_address & 0xFF);
    // TODO: Check id memcpy is also working
    //  memcpy(message_buffer, &destination_address, destination_address_size);
    // Copy channel into message buffer
    memcpy(message_buffer + destination_address_size, &channel, channel_size);
    // Copy message into message buffer
    memcpy(message_buffer + destination_address_size + channel_size, message, message_size);

    serial_->SetMode(OUTPUT);
    delay(kDelayTimeAfterSwitch);
    int bytes = serial_->write(message_buffer, total_message_size);

    serial_->flush();
    serial_->SetMode(INPUT);
    return bytes;
};

#pragma region private functions

size_t UsrLg206P::SendCommand(String command)
{
    serial_->SetMode(OUTPUT);
    delay(kDelayTimeAfterSwitch);
    size_t bytes_written = serial_->write(command.c_str(), command.length());
    serial_->flush();
    serial_->SetMode(INPUT);
    return bytes_written;
}

LoRaErrorCode UsrLg206P::SetCommand(String command, String succesfullResponse)
{
    command = "AT" + command + "\r\n";
    String expected_data = command;
    size_t bytes_written = SendCommand(command);

    const size_t buffer_size = 128;
    uint8_t buffer[buffer_size];
    size_t size = ReceiveMessage(buffer, buffer_size);
    String received_data = String((char *)buffer);

    // If echo is enabled check for the repeated command
    // TODO: this->settings_.command_echo_function == LoRaSettings::CommandEchoFunction::kCommandEchoFunctionUndefined
    if (this->settings_.command_echo_function == LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOn)
    {
        String returned_command = received_data.substring(0, command.length());
        if (returned_command != command)
        {
            return LoRaErrorCode::kCommandEchoNotReceived;
        }
    }

    // Check for error
    // TODO verify error message
    int indexError = received_data.indexOf("ERR");
    if (indexError != -1)
    {
        int error = received_data.substring(indexError + 4, indexError + 5).toInt();
        return LoRaErrorCode(error);
    }

    // TODO: Verify if this code is correct
    int index = received_data.indexOf(succesfullResponse);
    if (index == -1)
    {
        // TODO: Message might be wrong first time around after  enter AT command and exit AT command
        return LoRaErrorCode::kInvalidResponse;
    }

    return LoRaErrorCode::kSucces;
};

LoRaErrorCode UsrLg206P::GetCommand(String command, OUT String &value, bool using_colon, String succesfullResponse)
{
    String returnValue = "";
    String querry = "AT" + command + "\r\n";
    String expected_data = querry;
    size_t bytes_written = SendCommand(querry);

    const size_t buffer_size = 128;
    uint8_t buffer[buffer_size];
    size_t size = ReceiveMessage(buffer, buffer_size);
    String received_data = String((char *)buffer);

    // If echo is enabled check for the repeated command
    if (this->settings_.command_echo_function == LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOn)
    {
        String returned_command = received_data.substring(0, command.length());
        if (returned_command != querry)
        {
            return LoRaErrorCode::kCommandEchoNotReceived;
        }
    }

    int indexError = received_data.indexOf("ERR");
    if (indexError != -1)
    {
        int error = received_data.substring(indexError + 4, indexError + 5).toInt();
        return LoRaErrorCode(error);
    }

    // Check if : is present and if OK is present
    String gottenSetting;
    if (using_colon)
    {
        gottenSetting = command + ':';
    }
    else
    {
        gottenSetting = succesfullResponse + '=';
    }

    int index = received_data.indexOf(gottenSetting);
    int okInResponse = received_data.indexOf(succesfullResponse);
    if (index == -1)
    {
        return LoRaErrorCode::kMissingSettingClarification;
    }

    if (okInResponse == -1)
    {
        return LoRaErrorCode::kMissingOk;
    }

    // Return the part containing the setting
    if (using_colon)
    {
        value = received_data.substring(index + gottenSetting.length(), okInResponse - String("\r\n\r\n").length());
    }
    else
    {
        value = received_data.substring(index + gottenSetting.length(), received_data.length() - String("\r\n").length());
    }
    return LoRaErrorCode::kSucces;
};

#pragma endregion