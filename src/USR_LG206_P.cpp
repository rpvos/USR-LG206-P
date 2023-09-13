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

#include "USR_LG206_P.h"
#include "USR_LG206_P_settings.h"
#include <Arduino.h>
#include <ArduinoLog.h>

USR_LG_206_P::USR_LG_206_P(Stream *serial)
{
    this->serial = serial;
    USR_LG_206_P_SETTINGS settings = USR_LG_206_P_SETTINGS(false);
    this->settings = &settings;
};

int USR_LG_206_P::factory_reset(void)
{
    USR_LG_206_P_SETTINGS settings = USR_LG_206_P_SETTINGS(true);
    return set_settings(&settings);
}

int USR_LG_206_P::set_settings(USR_LG_206_P_SETTINGS *settings)
{
    // TODO add setter for every setting
    return false;
}

int USR_LG_206_P::get_settings(OUT USR_LG_206_P_SETTINGS &settings)
{
    // TODO use getter for every setting
    return false;
}

int USR_LG_206_P::begin_AT_mode(void)
{
    // Check if the LoRa module is already in AT mode
    if (settings->ATMode)
    {
        return true;
    }

    String received_data = "";
    String sent_data = "+++";
    serial->println(sent_data);

    received_data = serial->readString();

    String expected_data = "a";
    if (received_data != expected_data)
    {
        log_warning(expected_data, received_data);
        delay(1000);
        return false;
    }

    sent_data = "a";
    serial->println(sent_data);
    received_data = serial->readString();
    expected_data = "+OK";
    if (received_data != expected_data)
    {
        log_warning(expected_data, received_data);
        return false;
    }

    settings->ATMode = true;
    return true;
};

int USR_LG_206_P::end_AT_mode(void)
{
    // Check if LoRa module was already out of AT mode
    if (!(settings->ATMode))
    {
        return true;
    }

    serial->println("AT+ENTM");
    String received_data = serial->readString();
    String expected_data = "OK";
    if (received_data != expected_data)
    {
        log_warning(expected_data, received_data);
        return false;
    }
    else
    {
        settings->ATMode = false;
        return true;
    }
};

int USR_LG_206_P::set_echo(bool isOn)
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

    int received_data = set_command(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->commandEchoFunction = isOn;
        return true;
    }

    return false;
};

int USR_LG_206_P::get_echo(OUT bool &isOn)
{
    if (settings->commandEchoFunction != SETTING_UNDEFINED)
    {
        return settings->commandEchoFunction;
    }

    String command = "+E";

    serial->println("AT" + command);
    String expected_data = command;
    String received_data = serial->readString();

    if (received_data != expected_data)
    {
        log_warning(expected_data, received_data);
        return false;
    }

    received_data = serial->readString();

    expected_data = "OK=";

    String subStringed = received_data.substring(0, received_data.indexOf('='));

    if (subStringed != expected_data)
    {
        log_warning(expected_data, received_data);
        return false;
    }

    String value = received_data.substring(received_data.indexOf('='));
    if (value == "ON")
    {
        settings->commandEchoFunction = true;
        isOn = true;
        return true;
    }
    else if (value == "OFF")
    {
        settings->commandEchoFunction = false;
        isOn = false;
        return true;
    }

    return false;
};

int USR_LG_206_P::restart(void)
{
    String command = "AT+Z";
    serial->println(command);
    String received_data = serial->readString();
    String expected_data = command;

    if (received_data != expected_data)
    {
        log_warning(expected_data, received_data);
        return false;
    }

    received_data = serial->readString();

    expected_data = "OK";

    if (received_data != expected_data)
    {
        log_warning(expected_data, received_data);
        return false;
    }

    return true;
};

int USR_LG_206_P::save_as_default(void)
{
    String command = "AT+CFGTF";
    serial->println(command);
    String received_data = serial->readString();
    String expected_data = command;

    if (received_data != expected_data)
    {
        log_warning(expected_data, received_data);
        return false;
    }

    received_data = serial->readString();

    expected_data = "+CFGTF:SAVED";

    if (received_data != expected_data)
    {
        log_warning(expected_data, received_data);
        return false;
    }

    return true;
};

int USR_LG_206_P::reset_to_default(void)
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

int USR_LG_206_P::get_node_id(OUT String &node_id)
{
    if (settings->nodeID != "")
    {
        node_id = settings->nodeID;
        return true;
    }

    String command = "+NID";
    String value = get_command(command);

    if (value)
    {
        node_id = value;
        return true;
    }

    return false;
};

int USR_LG_206_P::get_firmware_version(OUT String &firmware_version)
{
    if (settings->firmwareVersion != "")
    {
        firmware_version = settings->firmwareVersion;
        return true;
    }

    String command = "+VER";
    String value = get_command(command);

    if (value)
    {
        firmware_version = value;
        return true;
    }

    return false;
};

int USR_LG_206_P::set_wmode(WorkMode wmode = work_mode_transparent)
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
        Log.warningln("Wmode is not defined (\"%d\")", wmode);
        return false;
    }

    int received_data = set_command(command);

    // If the set command was done succesfull
    if (received_data)
    {
        settings->workMode = wmode;
        return true;
    }

    return false;
};

int USR_LG_206_P::get_wmode(OUT WorkMode &wmode)
{
    if (settings->workMode != work_mode_undefined)
    {
        wmode = settings->workMode;
        return true;
    }

    String command = "+WMODE";
    String value = get_command(command);

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

int USR_LG_206_P::set_uart(USR_LG_206_P_UART_SETTINGS *uart_settings)
{
    if (uart_settings == 0)
    {
        return false;
    }

    String command = "+UART=";
    command += uart_settings->toString();

    int received_data = set_command(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->set_uart(uart_settings);
        return true;
    }

    return false;
};

int USR_LG_206_P::get_uart(OUT USR_LG_206_P_UART_SETTINGS &uart_settings)
{
    if (settings->get_uart() != 0)
    {
        uart_settings = *settings->get_uart();
        return true;
    }

    String command = "+UART";
    String value = get_command(command);

    if (uart_settings.fromString(value))
    {
        return true;
    }

    return false;
};

int USR_LG_206_P::set_power_consumption_mode(PowerConsumptionMode powermode = powermode_run)
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
        Log.warningln("Powermode is not defined (\"%d\")", powermode);
        return false;
    }

    int received_data = set_command(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->powerMode = powermode;
        return true;
    }

    return false;
};

int USR_LG_206_P::get_power_consumption_mode(OUT PowerConsumptionMode &powermode)
{
    if (settings->powerMode != powermode_undefined)
    {
        powermode = settings->powerMode;
        return true;
    }

    String command = "+PMODE";
    String value = get_command(command);

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

int USR_LG_206_P::set_waking_up_interval(int wake_up_interval = 2000)
{
    String command = "+WTM=";
    if (500 <= wake_up_interval && wake_up_interval <= 4000)
    {
        command += wake_up_interval;
    }
    else
    {
        Log.warningln("Wake up interval is out of bounds (\"%d\")", wake_up_interval);
        return false;
    }

    int received_data = set_command(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->wakeUpInterval = wake_up_interval;
        return true;
    }

    return false;
};
int USR_LG_206_P::get_waking_up_interval(OUT int &wake_up_interval)
{
    if (settings->wakeUpInterval != SETTING_UNDEFINED)
    {
        wake_up_interval = settings->wakeUpInterval;
        return true;
    }

    String command = "+WTM";
    String value = get_command(command);

    if (value)
    {
        settings->wakeUpInterval = value.toInt();
        wake_up_interval = value.toInt();
        return true;
    }

    return false;
};

int USR_LG_206_P::set_speed(LoRaAirRateLevel speed = LoRa_air_rate_level_21875)
{
    String command = "+SPD=";
    if (1 <= speed && speed <= 10)
    {
        command += speed;
    }
    else
    {
        Log.warningln("LoRa air rate is out of bounds (\"%d\")", speed);
        return false;
    }

    int received_data = set_command(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->loraAirRateLevel = speed;
        return true;
    }

    return false;
};

int USR_LG_206_P::get_speed(OUT LoRaAirRateLevel &speed)
{
    if (settings->loraAirRateLevel != LoRa_air_rate_level_undefined)
    {
        speed = settings->loraAirRateLevel;
        return true;
    }

    String command = "+SPD";
    String value = get_command(command);

    if (value)
    {
        settings->loraAirRateLevel = LoRaAirRateLevel(value.toInt());
        speed = LoRaAirRateLevel(value.toInt());
        return true;
    }

    return false;
};

int USR_LG_206_P::set_address(int address = 0)
{
    String command = "+ADDR=";
    if (1 <= address && address <= 65535)
    {
        command += address;
    }
    else
    {
        Log.warningln("Address is out of bounds (\"%d\")", address);
        return false;
    }

    int received_data = set_command(command);

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

int USR_LG_206_P::get_address(OUT int &address)
{
    if (settings->destinationAddressIsSet)
    {
        address = settings->destinationAddress;
        return true;
    }

    String command = "+ADDR";
    String value = get_command(command);

    if (value)
    {
        settings->destinationAddress = value.toInt();
        settings->destinationAddressIsSet = true;
        address = value.toInt();
        return true;
    }

    return false;
}

int USR_LG_206_P::set_channel(int channel = 65)
{
    String command = "+CH=";
    if (0 <= channel && channel <= 127)
    {
        command += channel;
    }
    else
    {
        Log.warningln("Channel is out of bounds (\"%d\")", channel);
        return false;
    }

    int received_data = set_command(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->channel = channel;
        return true;
    }

    return false;
};

int USR_LG_206_P::get_channel(OUT int &channel)
{
    if (settings->channel != SETTING_UNDEFINED)
    {
        channel = settings->channel;
        return true;
    }

    String command = "+CH";
    String value = get_command(command);

    if (value)
    {
        settings->channel = value.toInt();
        channel = value.toInt();
        return true;
    }

    return false;
};

int USR_LG_206_P::set_forward_error_correction(bool isOn = false)
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

    int received_data = set_command(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->forwardErrorCorrection = isOn;
        return true;
    }

    return false;
}

int USR_LG_206_P::get_forward_error_correction(OUT bool &isOn)
{

    if (settings->forwardErrorCorrection != SETTING_UNDEFINED)
    {
        isOn = settings->forwardErrorCorrection;
        return true;
    }

    String command = "+FEC";
    String value = get_command(command);

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

int USR_LG_206_P::set_power_transmission_value(int power = 20)
{
    String command = "+PWR=";
    if (10 <= power && power <= 20)
    {
        command += power;
    }
    else
    {
        Log.warningln("power is out of bounds (\"%d\")", power);
        return false;
    }

    int received_data = set_command(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->transmittingPower = power;
        return true;
    }

    return false;
};

int USR_LG_206_P::get_power_transmission_value(OUT int &power)
{
    if (settings->transmittingPower != SETTING_UNDEFINED)
    {
        power = settings->transmittingPower;
        return true;
    }

    String command = "+PWR";
    String value = get_command(command);

    if (value)
    {
        settings->transmittingPower = value.toInt();
        power = value.toInt();
        return true;
    }

    return false;
};

int USR_LG_206_P::set_transmission_interval(int interval = 2000)
{
    String command = "+SQT=";
    if ((100 <= interval && interval <= 6000) || false)
    {
        command += interval;
    }
    else
    {
        Log.warningln("interval is out of bounds (\"%d\")", interval);
        return false;
    }

    int received_data = set_command(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->testInterval = interval;
        return true;
    }

    return false;
};

int USR_LG_206_P::get_transmission_interval(OUT int &interval)
{
    if (settings->testInterval != SETTING_UNDEFINED)
    {
        interval = settings->testInterval;
        return true;
    }

    String command = "+SQT";
    // TODO probably other way of receiving data then normal
    String value = get_command(command);

    if (value)
    {
        settings->testInterval = value.toInt();
        interval = value.toInt();
        return true;
    }

    return false;
};

int USR_LG_206_P::set_key(String key = "FFFFFFFF")
{
    String command = "+KEY=";
    if (key.length() == 8)
    {
        command += key;
    }
    else
    {
        Log.warningln("Key length is not correct (\"%s\")", key);
        return false;
    }

    int received_data = set_command(command);

    // If the set command was done succesfull
    if (received_data)
    {
        // Set the setting to the set value
        settings->key = key;
        return true;
    }

    return false;
};

String USR_LG_206_P::retrieve_message(void)
{
    if (serial->available())
    {
        return serial->readString();
    }

    return "";
};

int USR_LG_206_P::send_message(char *message)
{

    int amountOfBytesWritten = serial->println(message);
    if (amountOfBytesWritten)
    {
        return amountOfBytesWritten;
    }

    return false;
};

int USR_LG_206_P::set_command(String command)
{
    serial->println("AT" + command);
    String expected_data = command;
    String received_data = serial->readString();
    if (received_data != expected_data)
    {
        log_warning(expected_data, received_data);
        return false;
    }

    received_data = serial->readString();
    expected_data = "OK";
    if (received_data != expected_data)
    {
        log_warning(expected_data, received_data);
        return false;
    }

    return true;
};

String USR_LG_206_P::get_command(String command)
{
    serial->println("AT" + command);
    String expected_data = command;
    String received = serial->readString();

    if (received != expected_data)
    {
        log_warning(expected_data, received);
        return "";
    }

    received = serial->readString();

    expected_data = command + ":";

    String subStringed = received.substring(0, received.indexOf(':'));

    if (subStringed != expected_data)
    {
        log_warning(expected_data, received);
        return "";
    }

    String value = received.substring(received.indexOf(':'), received.indexOf("OK"));

    return value;
};

void USR_LG_206_P::log_warning(String expected_data, String actual_data)
{
    Log.warningln("Received received_data is not \"%s\" but \"%s\"", expected_data.c_str(), actual_data.c_str());
};