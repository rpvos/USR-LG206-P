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
#include <unity.h>

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

LoRaErrorCode LoRa::FactoryReset(void)
{
    LoRaSettings::LoRaSettings factory_settings = LoRaSettings::LoRaSettings(true);
    return SetSettings(&factory_settings);
};

LoRaErrorCode LoRa::SetSettings(LoRaSettings::LoRaSettings *settings)
{
    delete this->settings_;
    this->settings_ = settings;
    // TODO add setter for every setting
    return LoRaErrorCode::kNoResponse;
}

LoRaErrorCode LoRa::GetSettings(OUT LoRaSettings::LoRaSettings &settings)
{
    // TODO use getter for every setting
    return LoRaErrorCode::kNoResponse;
}

LoRaErrorCode LoRa::BeginAtMode(void)
{
    // Check if the LoRa module is already in AT mode
    if (settings_->at_mode == LoRaSettings::AtMode::kAtModeIsOn)
    {
        return LoRaErrorCode::kSucces;
    }

    String received_data = "";
    String sent_data = "+++";
    received_data = SendCommand(sent_data);
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
    received_data = SendCommand(sent_data);
    if (received_data.length() < 1)
    {
        return LoRaErrorCode::kNoResponse;
    }

    expected_data = "+OK";
    if (received_data.indexOf(expected_data) < 0)
    {
        return LoRaErrorCode::kInvalidResponse;
    }

    settings_->at_mode = LoRaSettings::AtMode::kAtModeIsOn;
    return LoRaErrorCode::kSucces;
};

LoRaErrorCode LoRa::EndAtMode(void)
{
    // Check if LoRa module was already out of AT mode
    if (settings_->at_mode == LoRaSettings::AtMode::kAtModeIsOff)
    {
        return LoRaErrorCode::kSucces;
    }

    String command = "+ENTM";
    LoRaErrorCode response_code = SetCommand(command, "OK");

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_->at_mode = LoRaSettings::AtMode::kAtModeIsOff;
    }
    return response_code;
};

LoRaErrorCode LoRa::SetEcho(LoRaSettings::CommandEchoFunction setting)
{
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
        settings_->command_echo_function = setting;
    }

    return response_code;
};

LoRaErrorCode LoRa::GetEcho(LoRaSettings::CommandEchoFunction &setting)
{
    if (settings_->command_echo_function != LoRaSettings::CommandEchoFunction::kCommandEchoFunctionUndefined)
    {
        setting = settings_->command_echo_function;
        return LoRaErrorCode::kSucces;
    }

    String command = "+E";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value, false);

    if (response_code == LoRaErrorCode::kSucces)
    {
        if (value.indexOf("ON") != -1)
        {
            settings_->command_echo_function = LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOn;
            setting = settings_->command_echo_function;
        }
        else if (value.indexOf("OFF") != -1)
        {
            settings_->command_echo_function = LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOff;
            setting = settings_->command_echo_function;
        }
    }

    return response_code;
};

LoRaErrorCode LoRa::Restart(void)
{
    String command = "+Z";
    LoRaErrorCode response = SetCommand(command);

    // TODO Check for LoRa start

    if (response == LoRaErrorCode::kSucces)
    {
        settings_->at_mode = LoRaSettings::AtMode::kAtModeIsOff;
    }

    return response;
};

LoRaErrorCode LoRa::SaveAsDefault(void)
{
    String command = "+CFGTF";
    String succes_message = "+CFGTF:SAVED";
    return SetCommand(command, succes_message);
};

LoRaErrorCode LoRa::ResetToDefault(void)
{
    String command = "+RELD";
    return SetCommand(command, "REBOOTING");
};

LoRaErrorCode LoRa::GetNodeId(OUT String &node_id)
{
    if (settings_->node_id != "")
    {
        node_id = settings_->node_id;
        return LoRaErrorCode::kSucces;
    }

    String command = "+NID";
    LoRaErrorCode response_code = GetCommand(command, node_id);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_->node_id = node_id;
    }

    return response_code;
};

LoRaErrorCode LoRa::GetFirmwareVersion(OUT String &setting)
{
    if (settings_->firmware_version.length())
    {
        setting = settings_->firmware_version;
        return LoRaErrorCode::kSucces;
    }

    String command = "+VER";
    LoRaErrorCode response_code = GetCommand(command, setting);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_->firmware_version = setting;
    }

    return response_code;
};

LoRaErrorCode LoRa::SetWorkMode(LoRaSettings::WorkMode setting)
{
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
        settings_->work_mode = setting;
    }

    return response_code;
};

LoRaErrorCode LoRa::GetWorkMode(LoRaSettings::WorkMode &setting)
{
    if (settings_->work_mode != LoRaSettings::WorkMode::kWorkModeUndefined)
    {
        setting = settings_->work_mode;
        return LoRaErrorCode::kSucces;
    }

    String command = "+WMODE";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        if (value.indexOf("TRANS") != -1)
        {
            settings_->work_mode = LoRaSettings::WorkMode::kWorkModeTransparent;
            setting = settings_->work_mode;
        }
        else if (value.indexOf("FP") != -1)
        {
            settings_->work_mode = LoRaSettings::WorkMode::kWorkModeFixedPoint;
            setting = settings_->work_mode;
        }
    }

    return response_code;
};

LoRaErrorCode LoRa::SetUartSettings(LoRaUartSettings::LoRaUartSettings *setting)
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
        settings_->SetUartSettings(setting);
    }

    return response_code;
};

LoRaErrorCode LoRa::GetUartSettings(OUT LoRaUartSettings::LoRaUartSettings *setting)
{
    if (settings_->GetUartSettings() != 0)
    {
        setting = settings_->GetUartSettings();
        return LoRaErrorCode::kSucces;
    }

    String command = "+UART";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        setting->fromString(value);
        settings_->SetUartSettings(setting);
    }

    return response_code;
};

LoRaErrorCode LoRa::SetPowerConsumptionMode(LoRaSettings::PowerConsumptionMode setting)
{
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
        settings_->power_consumption_mode = setting;
    }

    return response_code;
};

LoRaErrorCode LoRa::GetPowerConsumptionMode(LoRaSettings::PowerConsumptionMode &setting)
{
    if (settings_->power_consumption_mode != LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeUndefined)
    {
        setting = settings_->power_consumption_mode;
        return LoRaErrorCode::kSucces;
    }

    String command = "+PMODE";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        if (value.indexOf("RUN") != -1)
        {
            settings_->power_consumption_mode = LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeRun;
            setting = settings_->power_consumption_mode;
        }
        else if (value.indexOf("WU") != -1)
        {
            settings_->power_consumption_mode = LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeWakeUp;
            setting = settings_->power_consumption_mode;
        }
    }

    return response_code;
};

LoRaErrorCode LoRa::set_waking_up_interval(int setting = 2000)
{
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
        settings_->wake_up_interval = setting;
    }

    return response_code;
};

LoRaErrorCode LoRa::get_waking_up_interval(OUT int &setting)
{
    if (settings_->wake_up_interval != -1)
    {
        setting = settings_->wake_up_interval;
        return LoRaErrorCode::kSucces;
    }

    String command = "+WTM";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        setting = value.toInt();
        settings_->wake_up_interval = setting;
    }

    return response_code;
};

LoRaErrorCode LoRa::set_speed(LoRaSettings::LoRaAirRateLevel setting)
{
    String command = "+SPD=";
    if (setting == LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevelUndefined)
    {
        return LoRaErrorCode::kInvalidParameter;
    }

    LoRaErrorCode response_code = SetCommand(command);

    if (response_code == LoRaErrorCode::kSucces)
    {
        settings_->lora_air_rate_level = setting;
    }

    return response_code;
};

LoRaErrorCode LoRa::get_speed(LoRaSettings::LoRaAirRateLevel &setting)
{
    if (settings_->lora_air_rate_level != LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevelUndefined)
    {
        setting = settings_->lora_air_rate_level;
        return LoRaErrorCode::kSucces;
    }

    String command = "+SPD";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        setting = LoRaSettings::LoRaAirRateLevel(value.toInt());
        settings_->lora_air_rate_level = setting;
    }

    return response_code;
};

LoRaErrorCode LoRa::set_address(int address = 0)
{
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
        settings_->destination_address = address;
        settings_->destination_address_is_set = true;
    }

    return response_code;
};

LoRaErrorCode LoRa::get_address(OUT int &address)
{
    if (settings_->destination_address_is_set)
    {
        address = settings_->destination_address;
        return LoRaErrorCode::kSucces;
    }

    String command = "+ADDR";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        address = value.toInt();
        settings_->destination_address = address;
        settings_->destination_address_is_set = true;
    }

    return response_code;
}

LoRaErrorCode LoRa::set_channel(int channel)
{
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
        settings_->channel = channel;
    }

    return response_code;
};

LoRaErrorCode LoRa::get_channel(OUT int &channel)
{
    if (settings_->channel != -1)
    {
        channel = settings_->channel;
        return LoRaErrorCode::kSucces;
    }

    String command = "+CH";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        channel = value.toInt();
        settings_->channel = channel;
    }

    return response_code;
};

LoRaErrorCode LoRa::set_forward_error_correction(LoRaSettings::ForwardErrorCorrection setting)
{

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
        settings_->forward_error_correction = setting;
    }

    return response_code;
}

LoRaErrorCode LoRa::get_forward_error_correction(LoRaSettings::ForwardErrorCorrection &setting)
{
    if (settings_->forward_error_correction != LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionUndefined)
    {
        setting = settings_->forward_error_correction;
        return LoRaErrorCode::kSucces;
    }

    String command = "+FEC";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        if (value == "ON")
        {
            settings_->forward_error_correction = LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOn;
        }
        else if (value == "OFF")
        {
            settings_->forward_error_correction = LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOff;
        }
    }

    return response_code;
}

LoRaErrorCode LoRa::set_power_transmission_value(int setting)
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
        settings_->transmitting_power = setting;
    }

    return response_code;
};

LoRaErrorCode LoRa::get_power_transmission_value(OUT int &setting)
{

    if (settings_->transmitting_power != 0)
    {
        setting = settings_->transmitting_power;
        return LoRaErrorCode::kSucces;
    }

    String command = "+PWR";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        setting = value.toInt();
        settings_->transmitting_power = setting;
    }

    return response_code;
};

LoRaErrorCode LoRa::set_transmission_interval(int interval = 2000)
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
        settings_->test_interval = interval;
    }

    return response_code;
};

LoRaErrorCode LoRa::get_transmission_interval(OUT int &setting)
{
    if (settings_->test_interval != 0)
    {
        setting = settings_->test_interval;
        return LoRaErrorCode::kSucces;
    }

    String command = "+SQT";
    String value;
    LoRaErrorCode response_code = GetCommand(command, value);

    if (response_code == LoRaErrorCode::kSucces)
    {
        setting = value.toInt();
        settings_->test_interval = setting;
    }

    return response_code;
};

LoRaErrorCode LoRa::SetKey(String key)
{
    String command = "+KEY=";
    if (key.length() == 8)
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
        settings_->key = key;
    }

    return response_code;
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

int LoRa::SendMessage(const char *message)
{
    if (this->settings_->work_mode != LoRaSettings::WorkMode::kWorkModeTransparent)
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

int LoRa::SendMessage(uint16_t destination_address, uint8_t channel, const char *message, uint8_t message_size)
{
    if (this->settings_->work_mode != LoRaSettings::WorkMode::kWorkModeFixedPoint)
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
    if (amount_of_bytes_written == (destination_address_size + channel_size + message_size))
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

LoRaErrorCode LoRa::SetCommand(String command, String succesfullResponse)
{
    command = "AT" + command + "\r\n";
    String expected_data = command;
    String received_data = SendCommand(command);

    // If echo is enabled check for the repeated command
    if (this->settings_->command_echo_function == LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOn)
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

    int index = received_data.indexOf(succesfullResponse);
    if (index == -1)
    {
        return LoRaErrorCode::kInvalidResponse;
    }

    return LoRaErrorCode::kSucces;
};

LoRaErrorCode LoRa::GetCommand(String command, OUT String &value, bool using_colon, String succesfullResponse)
{
    String returnValue = "";
    String querry = "AT" + command + "\r\n";
    String expected_data = querry;
    String received_data = SendCommand(querry);

    // If echo is enabled check for the repeated command
    if (this->settings_->command_echo_function == LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOn)
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