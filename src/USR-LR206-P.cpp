/**
 * @file USR-LR206-P.cpp
 * @author Rik Vos (rik.vos01@gmail.com)
 * @brief Library for using USR-LR206-P
 * @version 0.1
 * @date 2023-09-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "USR-LR206-P.h"
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

    String data = "";
    String send = "+++";
    serial->println(send);

    data = serial->readString();

    String expected = "a";
    if (data != expected)
    {
        Log.warningln("Send: \"%s\" \t Expected: \"%s\" \t Recieved: \"%s\"", send.c_str(), expected.c_str(), data.c_str());
        delay(1000);
        return false;
    }

    send = "a";
    serial->println(send);
    data = serial->readString();
    expected = "+OK";
    if (data != expected)
    {
        Log.warningln("Send: \"%s\" \t Expected: \"%s\" \t Recieved: \"%s\"", send.c_str(), expected.c_str(), data.c_str());
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
    String data = serial->readString();
    String expected = "OK";
    if (data != expected)
    {
        Log.warningln("Received data is not \"%s\" but \"%s\"", expected.c_str(), data.c_str());
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

    int response = set_command(command);

    // If the set command was done succesfull
    if (response)
    {
        // Set the setting to the set value
        settings->commandEchoFunction = isOn;
    }

    return response;
};

int USR_LG_206_P::get_echo(OUT bool &isOn)
{
    if (settings->commandEchoFunction != SETTING_UNDEFINED)
    {
        return settings->commandEchoFunction;
    }

    String command = "+E";

    serial->println("AT" + command);
    String expected = command;
    String data = serial->readString();

    if (data != expected)
    {
        Log.warningln("Received data is not \"%s\" but \"%s\"", expected.c_str(), data.c_str());
        return false;
    }

    data = serial->readString();

    expected = "OK=";

    String subStringed = data.substring(0, data.indexOf('='));

    if (subStringed != expected)
    {
        Log.warningln("Received data is not \"%s\" but \"%s\"", expected.c_str(), subStringed.c_str());
        return false;
    }

    String value = data.substring(data.indexOf('='));
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
    String response = serial->readString();
    String expected = command;

    if (response != expected)
    {
        Log.warningln("Received data is not \"%s\" but \"%s\"", expected.c_str(), response.c_str());
        return false;
    }

    response = serial->readString();

    expected = "OK";

    if (response != expected)
    {
        Log.warningln("Received data is not \"%s\" but \"%s\"", expected.c_str(), response.c_str());
        return false;
    }

    return true;
};

int USR_LG_206_P::save_as_default(void)
{
    String command = "AT+CFGTF";
    serial->println(command);
    String response = serial->readString();
    String expected = command;

    if (response != expected)
    {
        Log.warningln("Received data is not \"%s\" but \"%s\"", expected.c_str(), response.c_str());
        return false;
    }

    response = serial->readString();

    expected = "+CFGTF:SAVED";

    if (response != expected)
    {
        Log.warningln("Received data is not \"%s\" but \"%s\"", expected.c_str(), response.c_str());
        return false;
    }

    return true;
};

int USR_LG_206_P::reset_to_default(void)
{
    String command = "AT+RELD";
    serial->println(command);
    String response = serial->readString();
    String expected = command;

    if (response != expected)
    {
        Log.warningln("Received data is not \"%s\" but \"%s\"", expected.c_str(), response.c_str());
        return false;
    }

    response = serial->readString();

    expected = "REBOOTING";

    if (response != expected)
    {
        Log.warningln("Received data is not \"%s\" but \"%s\"", expected.c_str(), response.c_str());
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

    int response = set_command(command);

    // If the set command was done succesfull
    if (response)
    {
        // Set the setting to the set value
        settings->workMode = response;
    }

    return response;
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

        int response = set_command(command);

        // If the set command was done succesfull
        if (response)
        {
            // Set the setting to the set value
            settings->loraAirRateLevel = response;
        }

        return response;
    }

    return false;
};

int USR_LG_206_P::get_speed(void){};

int USR_LG_206_P::set_address(int address = 0)
{
    if (0 <= address && address <= 65535)
    {
        String command = "+ADDR=" + address;

        int response = set_command(command);

        // If the set command was done succesfull
        if (response)
        {
            // Set the setting to the set value
            settings->destinationAddress = response;
        }

        return response;
    }

    return false;
};

int USR_LG_206_P::set_channel(int channel = 65)
{
    if (0 <= channel && channel <= 127)
    {
        String command = "+CH=" + channel;

        int response = set_command(command);

        // If the set command was done succesfull
        if (response)
        {
            // Set the setting to the set value
            settings->channel = response;
        }

        return response;
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
    String expected = command;
    String data = serial->readString();
    if (data != expected)
    {
        Log.warningln("Received data is not \"%s\" but \"%s\"", expected.c_str(), data.c_str());
        return false;
    }

    data = serial->readString();
    expected = "OK";
    if (data != expected)
    {
        Log.warningln("Received data is not \"%s\" but \"%s\"", expected.c_str(), data.c_str());
        return false;
    }

    return true;
};

String USR_LG_206_P::get_command(String command)
{
    serial->println("AT" + command);
    String expected = command;
    String data = serial->readString();

    if (data != expected)
    {
        Log.warningln("Received data is not \"%s\" but \"%s\"", expected.c_str(), data.c_str());
        return "";
    }

    data = serial->readString();

    expected = command + ":";

    String subStringed = data.substring(0, data.indexOf(':'));

    if (subStringed != expected)
    {
        Log.warningln("Received data is not \"%s\" but \"%s\"", expected.c_str(), data.c_str());
        return "";
    }

    String value = data.substring(data.indexOf(':'), data.indexOf("OK"));

    return value;
};