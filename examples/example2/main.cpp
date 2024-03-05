#include <Arduino.h>
#include <max485ttl.hpp>
#include "usr_lg206_p.h"

#define SENDER

RS485 rs = RS485(2, 2, &Serial1);
UsrLg206P lora = UsrLg206P(&rs);

void setup()
{
    Serial.begin(9600);
    // Serial using 8 bits, No parity and 1 stop bit
    Serial1.begin(115200, SERIAL_8N1);
}

String m = "Hello world!";
#ifdef SENDER
void loop()
{
    int status = lora.SendMessage((uint8_t *)m.c_str(), m.length());
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