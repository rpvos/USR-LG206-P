#include <Arduino.h>
#include <MAX485TTL.hpp>
#include <usr_lg206_p.h>

const bool kIsSensorNode = false;
const uint8_t kSensorChannel = 36;
const uint8_t kBaseChannel = 37;
const uint16_t kSensorAddress = 3;
const uint16_t kBaseAddress = 2;

const uint8_t kEnablePin = 2;

RS485 rs = RS485(kEnablePin, kEnablePin, &Serial1);
UsrLg206P lora = UsrLg206P(&rs);
int state = 0;

void SetupRandomCode()
{
    randomSeed(analogRead(0));
}

String GetRandomCode()
{
    // String code = String(random(10000, 99999));
    const int buffer_size = 15;
    char buffer[buffer_size] = "ABCDEFGHIJKLMN";

    // for (size_t i = 0; i < buffer_size; i++)
    // {
    //     buffer[i] = 'A' + random(0, 26);
    // }

    String code = String(buffer);
    // String code = String(random(10000, 99999));
    return code;
}

void setup()
{
    Serial.begin(9600);
    // Serial using 8 bits, No parity and 1 stop bit
    Serial1.begin(115200, SERIAL_8N1);

    SetupRandomCode();
}

void loop()
{
    LoRaErrorCode response_code = LoRaErrorCode::kSucces;
    switch (state)
    {
    case 0:
    {
        response_code = lora.BeginAtMode();
        if (response_code == LoRaErrorCode::kSucces)
        { // Create distance between state completes
            Serial.println("At mode entered");
            state++;
        }
        else
        {
            delay(1000);
        }
        break;
    }
    case 1:
    {
        response_code = lora.SetWorkMode(LoRaSettings::WorkMode::kWorkModeFixedPoint);
        if (response_code == LoRaErrorCode::kSucces)
        {
            Serial.println("Workmode set");
            state++;
        }
        else
        {
            delay(1000);
        }
        break;
    }
    case 2:
    {
        response_code = lora.SetAirRateLevel(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel268);
        if (response_code == LoRaErrorCode::kSucces)
        {
            Serial.println("Air rate level set");
            state++;
        }
        else
        {
            delay(1000);
        }
        break;
    }
    case 3:
    {
        if (kIsSensorNode)
        {
            response_code = lora.SetChannel(kSensorChannel);
        }
        else
        {
            response_code = lora.SetChannel(kBaseChannel);
        }

        if (response_code == LoRaErrorCode::kSucces)
        {
            Serial.println("Channel set");
            state++;
        }
        else
        {
            delay(1000);
        }
        break;
    }
    case 4:
    {
        if (kIsSensorNode)
        {
            response_code = lora.SetDestinationAddress(kSensorAddress);
        }
        else
        {
            response_code = lora.SetDestinationAddress(kBaseAddress);
        }

        if (response_code == LoRaErrorCode::kSucces)
        {
            Serial.println("Destination address set");
            state++;
        }
        else
        {
            delay(1000);
        }
        break;
    }
    case 5:
    {
        response_code = lora.SetPowerTransmissionValue(10);
        if (response_code == LoRaErrorCode::kSucces)
        {
            Serial.println("Power transmission value set");
            state++;
        }
        else
        {
            delay(1000);
        }
        break;
    }
        // Here
        // case 5:
        // {
        //     response_code = lora.SetTransmissionInterval();
        //     if (response_code == LoRaErrorCode::kSucces)
        //     {
        //         state += 2;
        //     }
        //     break;
        // }
        // case 6:
        // {
        //     response_code = lora.EndAtMode();
        //     // If register is succefully retrieved
        //     if (response_code == LoRaErrorCode::kSucces)
        //     {
        //         Serial.println("At mode exited");
        //         state += 1;
        //     }
        //     break;
        // }
        // case 7:
        // {
        //     Serial.println(lora.ReceiveMessage());
        //     break;
        // }
        // Here

    case 6:
    {
        response_code = lora.EndAtMode();
        // If register is succefully retrieved
        if (response_code == LoRaErrorCode::kSucces)
        {
            Serial.println("At mode exited");
            if (kIsSensorNode)
            {
                state += 2;
            }
            else
            {
                state++;
            }
        }
        else
        {
            delay(1000);
        }
        break;
    }
    case 7:
    {
        // String message = "AAAA";
        String message = GetRandomCode();
        int time_send = millis();
        int bytes = lora.SendMessage(message.c_str(), message.length(), kSensorAddress, kSensorChannel);

        String received_data;
        while (true)
        {
            received_data = lora.ReceiveMessage();
            if (received_data.length())
            {
                break;
            }
        }

        int time_received = millis();
        Serial.print("Time in millis: ");
        Serial.println(time_received - time_send);

        // Only if something is received write it in the loggs
        if (received_data.length())
        {
            Serial.print("S:");
            Serial.println(message);
            Serial.print("R:");
            Serial.println(received_data);

            if (!message.equals(received_data))
            {
                Serial.println("Incorrect message received");
            }
            else
            {
                Serial.println("Correct message received");
            }
        }

        delay(1000);
        break;
    }
    case 8:
    {
        String s = lora.ReceiveMessage();
        if (s.length())
        {
            lora.SendMessage(kBaseAddress, kBaseChannel, s.c_str(), s.length());
            Serial.println(s);
        }

        break;
    }
    default:
    {
        Serial.println("Unknown state");
        delay(1000);
        break;
    }
    } // switch state
    // Print error
    if (response_code != LoRaErrorCode::kSucces)
    {
        Serial.print("State: ");
        Serial.println(state);
        Serial.print("Response code: ");
        Serial.println(static_cast<int>(response_code));
    }
}
