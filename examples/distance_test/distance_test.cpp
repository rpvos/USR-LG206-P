#include <Arduino.h>
#include <max485ttl.hpp>
#include <usr_lg206_p.h>

// #define DEBUG_PRINT

const bool kIsSensorNode = false;

const uint8_t kSensorChannel = 40;
const uint8_t kBaseChannel = 37;
const uint16_t kSensorAddress = 1;
const uint16_t kBaseAddress = 2;

const uint8_t kEnablePin = 2;
const size_t kMessageFrequency = 10000;

RS485 rs = RS485(kEnablePin, kEnablePin, &Serial1);
UsrLg206P lora = UsrLg206P(&rs);
int state = 0;

const size_t buffer_size = 64;
uint8_t buffer[buffer_size];

void SetupRandomCode()
{
    randomSeed(analogRead(0));
}

String GetRandomCode()
{
    return String(random(10000, 99999));
}

void setup()
{
// Sensor node doesn't communicate with Serial
#ifdef DEBUG_PRINT
    Serial.begin(9600);
#endif

    if (!kIsSensorNode)
    {
        Serial.begin(9600);
    }

    // Serial using 8 bits, No parity and 1 stop bit for LoRa module
    Serial1.begin(115200, SERIAL_8N1);

    SetupRandomCode();
}

long amount_correct = 0;

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
#ifdef DEBUG_PRINT
            Serial.println("At mode entered");
#endif
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
#ifdef DEBUG_PRINT
            Serial.println("Workmode set");
#endif
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
#ifdef DEBUG_PRINT
            Serial.println("Air rate level set");
#endif

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
#ifdef DEBUG_PRINT
            Serial.println("Channel set");
#endif

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
#ifdef DEBUG_PRINT
            Serial.println("Destination address set");
#endif
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
#ifdef DEBUG_PRINT
            Serial.println("Power transmission value set");
#endif
            state++;
        }
        else
        {
            delay(1000);
        }
        break;
    }
    case 6:
    {
        response_code = lora.EndAtMode();
        // If register is succefully retrieved
        if (response_code == LoRaErrorCode::kSucces)
        {
#ifdef DEBUG_PRINT
            Serial.println("At mode exited");
#endif
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
        // Base station code
        String message = GetRandomCode();
        int time_send = millis();
        int bytes = lora.SendMessage(message.c_str(), message.length(), kSensorAddress, kSensorChannel);
        if (!bytes)
        {
#ifdef DEBUG_PRINT
            Serial.println("Message not sent correctly");
#endif
            return;
        }

        size_t length = lora.ReceiveMessage(buffer, buffer_size);
        if (length)
        {
            if (length < buffer_size - 1)
            {
                buffer[length] = '\0';
            }
            else
            {
                buffer[buffer_size - 1] = '\0';
            }
        }

        int time_received = millis();

        // Only if something is received write it in the loggs
        if (length)
        {
#ifdef DEBUG_PRINT
            Serial.print("Time in millis: ");
            Serial.println(time_received - time_send);
            Serial.print("S:");
            Serial.println(message);
            Serial.print("R:");
            Serial.println(reinterpret_cast<char *>(buffer));
#endif
            if (!message.equals(reinterpret_cast<char *>(buffer)))
            {
                Serial.println("Incorrect message received");
            }
            else
            {
                Serial.print("Correct messages: ");
                Serial.println(++amount_correct);
            }
        }
        else
        {
#ifdef DEBUG_PRINT
            Serial.println("Nothing received");
#endif
        }

        size_t time_remaining = 0;
        if ((time_received - time_send) < kMessageFrequency)
        {
            time_remaining = kMessageFrequency - (time_received - time_send);
        }

        delay(time_remaining);
        break;
    }
    case 8:
    {
        // Sender node
        size_t length = lora.ReceiveMessage(buffer, buffer_size);
        if (length)
        {
            if (length < buffer_size - 1)
            {
                buffer[length] = '\0';
#ifdef DEBUG_PRINT
                Serial.print("Length: ");
                Serial.print((int)length);
                Serial.print(':');
                Serial.println(reinterpret_cast<char *>(buffer));
#endif
                int bytes = lora.SendMessage(reinterpret_cast<char *>(buffer), length, kBaseAddress, kBaseChannel);
                if (bytes != length + 3)
                {
#ifdef DEBUG_PRINT
                    Serial.println("Message not sent correctly");
#endif
                }
            }
        }

        break;
    }
    default:
    {
#ifdef DEBUG_PRINT
        Serial.println("Unknown state");
#endif
        delay(1000);
        break;
    }
    } // switch state

    // Print error
    if (response_code != LoRaErrorCode::kSucces)
    {
#ifdef DEBUG_PRINT
        Serial.print("State: ");
        Serial.println(state);
        Serial.print("Response code: ");
        Serial.println(static_cast<int>(response_code));
#endif
    }
}
