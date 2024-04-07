#include <Arduino.h>
#include <max485ttl.hpp>
#include "usr_lg206_p.h"

#define SENDER

RS485 rs = RS485(2, 2, &Serial1);
UsrLg206P lora = UsrLg206P(&rs);

bool setup_complete = false;

void setup()
{
    Serial.begin(9600);
    // Serial using 8 bits, No parity and 1 stop bit
    Serial1.begin(115200, SERIAL_8N1);
}

#ifdef SENDER

const char m[] = "Hello world!";
const size_t length = sizeof(m) / sizeof(char);

void loop()
{
    if (!setup_complete)
    {
        lora.BeginAtMode();
        lora.SetWorkMode(LoRaSettings::WorkMode::kWorkModeTransparent);
        lora.SetAirRateLevel(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel268);
        lora.SetChannel(40);
        lora.SetDestinationAddress(1);
        lora.EndAtMode();
    }

    int status = lora.SendMessage((uint8_t *)m, length);
    Serial.println(status);
    delay(1000);
}
#else
void loop()
{
    if (lora.Available())
    {
        String r = lora.ReceiveMessage();
        Serial.println(r);
    }
    delay(10);
}
#endif