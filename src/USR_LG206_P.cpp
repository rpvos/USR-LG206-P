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
#include <Arduino.h>
#include <ArduinoLog.h>

USR_LG_206_P::USR_LG_206_P(Stream *serial)
{
    this->serial = serial;
    USR_LG_206_P_SETTINGS_t settings = undefined_settings;
    this->settings = &settings;
};

int USR_LG_206_P::retrieve_settings(void)
{
    USR_LG_206_P_SETTINGS_t settings;
    settings.ATMode = true;

    bool isOn;
    if (get_echo(isOn))
    {
        settings.commandEchoFunction = isOn;
    }
    // TODO add other settings

    this->settings = &settings;

    return true;
};

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
    if (!settings->ATMode)
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
    }

    return received_data;
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

int USR_LG_206_P::get_node_id(void){

};

int USR_LG_206_P::get_firmware_version(void){

};

int USR_LG_206_P::set_wmode(int wmode = WMODE_TRANS)
{
    String command = "+WMODE=";
    if (wmode == WMODE_TRANS)
    {
        command += "TRANS";
    }
    if (wmode == WMODE_FP)
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
        // Set the setting to the set value
        settings->workMode = received_data;
    }

    return received_data;
};

int USR_LG_206_P::get_wmode(OUT int &wmode)
{
    if (settings->workMode != SETTING_UNDEFINED)
    {
        return settings->workMode;
    }

    String command = "+WMODE";
    String value = get_command(command);

    if (value == "TRANS")
    {
        settings->workMode = WMODE_TRANS;
        wmode = WMODE_TRANS;
        return true;
    }
    else if (value == "FP")
    {
        settings->workMode = WMODE_FP;
        wmode = WMODE_TRANS;
        return true;
    }

    return false;
};

int USR_LG_206_P::set_uart(USR_LG_206_P_UART_SETTINGS *settings){

};

int USR_LG_206_P::get_uart(OUT USR_LG_206_P_UART_SETTINGS &settings){

};

int USR_LG_206_P::set_power_consumption_mode(void){};

int USR_LG_206_P::get_power_consumption_mode(void){};

int USR_LG_206_P::set_waking_up_interval(void){};
int USR_LG_206_P::get_waking_up_interval(void){};

int USR_LG_206_P::set_speed(int speed = 10)
{
    if (1 <= speed && speed <= 10)
    {
        String command = "+SPD=" + speed;

        int received_data = set_command(command);

        // If the set command was done succesfull
        if (received_data)
        {
            // Set the setting to the set value
            settings->loraAirRateLevel = received_data;
        }

        return received_data;
    }
    else
    {
        log_warning("0-10", "" + speed);
    }

    return false;
};

int USR_LG_206_P::get_speed(void){};

int USR_LG_206_P::set_address(int address = 0)
{
    if (0 <= address && address <= 65535)
    {
        String command = "+ADDR=" + address;

        int received_data = set_command(command);

        // If the set command was done succesfull
        if (received_data)
        {
            // Set the setting to the set value
            settings->destinationAddress = received_data;
        }

        return received_data;
    }
    else
    {
        log_warning("0-65535", "" + address);
    }

    return false;
};

int USR_LG_206_P::set_channel(int channel = 65)
{
    if (0 <= channel && channel <= 127)
    {
        String command = "+CH=" + channel;

        int received_data = set_command(command);

        // If the set command was done succesfull
        if (received_data)
        {
            // Set the setting to the set value
            settings->channel = received_data;
        }

        return received_data;
    }
    else
    {
        log_warning("0-127", "" + channel);
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