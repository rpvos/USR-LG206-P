#include <unity.h>

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void RUN_UNITY_TESTS()
{
    UNITY_BEGIN();
    // RUN_TEST(test_Crc32);
    UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>
void setup()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    RUN_UNITY_TESTS();
}

void loop()
{
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
}

#else

int main(int argc, char **argv)
{
    RUN_UNITY_TESTS();
    return 0;
}

#endif
