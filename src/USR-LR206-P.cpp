#include "USR-LR206-P.h"
#include <Arduino.h>
#include <ArduinoLog.h>

USR_LG_206_P::USR_LG_206_P(Stream *serial)
{
    this->serial = serial;
};

int USR_LG_206_P::retrieve_settings()
{
    USR_LG_206_P_SETTINGS settings;
    settings.ATMode = true;

    settings.commandEchoFunction = get_echo();
    // TODO add other settings

    this->settings = &settings;
};

/// @brief Function used to set the value on the LoRa module
/// @param command which is used to set the value
/// @return true of succesfull, false if unsuccesfull
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

/// @brief Function used to get a value from the LoRa module
/// @param command Command for wich the value is stored
/// @return String containing this value
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

/// @brief Function to start the AT mode
/// @return true if succesfull and false if unsuccesfull
int USR_LG_206_P::begin_AT_mode()
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

/// @brief Function to end the AT mode and start transmission mode
/// @return true if succesfull and false if unsuccesfull
int USR_LG_206_P::end_AT_mode()
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
}

/// @brief Function used to set the function echo boolean
/// @param isOn the value to which it is set
/// @return true of succesfull, false if unsuccesfull
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

/// @brief Function used to get the currect value of the echo function command on the LoRa module,
/// this can't be done using the get_command because the returned string is unconventional
/// @return The value of the echo command or -1 if not succeeded
int USR_LG_206_P::get_echo()
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
        return -1;
    }

    data = serial->readString();

    expected = "OK=";

    String subStringed = data.substring(0, data.indexOf('='));

    if (subStringed != expected)
    {
        Log.warningln("Received data is not \"%s\" but \"%s\"", expected.c_str(), subStringed.c_str());
        return -1;
    }

    String value = data.substring(data.indexOf('='));
    if (value == "ON")
    {
        settings->commandEchoFunction = true;
        return true;
    }
    else if (value == "OFF")
    {
        settings->commandEchoFunction = false;
        return false;
    }

    return -1;
};

/// @brief Function used to restart the LoRa module
/// @return true of succesfull, false if unsuccesfull
int USR_LG_206_P::restart()
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

/// @brief Function used to set the current settings as new default
/// @return true of succesfull, false if unsuccesfull
int USR_LG_206_P::save_as_default()
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

/// @brief Function used to set the workmode
/// @param wmode workmode (WMODE_TRANS = 1, WMODE_FP = 2)
/// @return true of succesfull, false if unsuccesfull
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

/// @brief Function used to set the LoRa air rate level
/// @param speed value for air rate 1-10 as following values
/// 1: 268bps
/// 2: 488bps
/// 3: 537bps
/// 4: 878bps
/// 5: 977bps
/// 6: 1758bps
/// 7: 3125bps
/// 8: 6250bps
/// 9: 10937bps
/// 10: 21875bp
/// @return true of succesfull, false if unsuccesfull
int USR_LG_206_P::set_speed(int speed = 10)
{
    if (1 <= speed <= 10)
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
}

/// @brief Function used to set the destination address
/// @param address value between 0-65535
/// @return true of succesfull, false if unsuccesfull
int USR_LG_206_P::set_address(int address = 0)
{
    if (0 <= address <= 65535)
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
}

/// @brief Function used to set the LoRa air rate level
/// @param channel value for channel is as following values in the following ranges
/// L: 0~127 Working frequency band=(398+ch)MHz (Default is 72, frequency band 470Mhz)
/// H: 0~127 Working frequency band=(803+ch)Mhz (Default is 65, frequency band 868Mhz)
/// @return true of succesfull, false if unsuccesfull
int USR_LG_206_P::set_channel(int channel = 65)
{
    if (0 <= channel <= 127)
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
}

/// @brief Function used to retrieve a message from the module
/// @return the data received
String USR_LG_206_P::retrieve_message()
{
    if (serial->available())
    {
        return serial->readString();
    }

    return "";
};

/// @brief Function used to send data
/// @param message data that needs to be send
/// @return amount of bytes written
int USR_LG_206_P::send_message(char *message)
{

    int amountOfBytesWritten = serial->println(message);
    if (amountOfBytesWritten)
    {
        return amountOfBytesWritten;
    }

    return false;
}