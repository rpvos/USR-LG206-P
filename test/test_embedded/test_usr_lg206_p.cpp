/**
 * @file test_max485ttl.cpp
 * @author rpvos (mr.rv.asd@gmail.com)
 * @brief Unit tests for the max485ttl.cpp
 * @version 0.1
 * @date 2023-09-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Arduino.h>
#include <unity.h>
#include "USR_LG206_P.h"
#include "max485ttl.h"
#include "memory_stream.h"

#define DE_PORT 2
#define RE_PORT 3

/**
 * @brief Stream used as mock for serial stream
 *
 */
MemoryStream *stream;

/**
 * @brief The object that is being used for sending data to the LoRa module
 *
 */
RS485 *rs;

/**
 * @brief The object being tested against
 *
 */
LoRa *lora;

/**
 * @brief Set the test up by creating a new instance of, the mock stream, the rs485 module and the LoRa module
 *
 */
void setUp(void)
{
    stream = new MemoryStream();
    rs = new RS485(DE_PORT, RE_PORT, stream);
    lora = new LoRa(rs);
}

/**
 * @brief Clean up the test by deleting the used memory and giving it back
 *
 */
void tearDown(void)
{
    delete lora;
    delete rs;
    delete stream;
}

void test_initilisation(void)
{
    LoRaSettings settings = LoRaSettings(false);
    LoRaSettings settings2 = LoRaSettings(false);

    lora->get_settings(settings);

    TEST_ASSERT_TRUE_MESSAGE(settings2 == settings, "Standard settings not used");
}

void test_factory_reset(void)
{
    LoRaSettings settings = LoRaSettings(false);
    LoRaSettings settings2 = LoRaSettings(true);

    lora->begin_AT_mode();
    lora->factory_reset();
    lora->get_settings(settings);

    // TODO
    //  TEST_ASSERT_TRUE_MESSAGE(settings2 == settings, "Factory settings not set correctly");
}

void test_at_mode(void)
{
    int response;
    for (size_t attempts = 0; attempts < 10; attempts++)
    {
        response = lora->begin_AT_mode();
        if (response)
        {
            break;
        }
    }

    TEST_ASSERT_TRUE_MESSAGE(response, "At mode not entered after 10 attempts");

    for (size_t attempts = 0; attempts < 10; attempts++)
    {
        response = lora->end_AT_mode();
        if (response)
        {
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(response, "At mode not exited after 10 attempts");
}

/**
 * @brief Entry point to start all tests
 *
 */
void setup()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN(); // Start unit testing

    // RUN_TEST(test_initilisation);
    // RUN_TEST(test_factory_reset);
    // RUN_TEST(test_at_mode);

    UNITY_END(); // Stop unit testing
}

/**
 * @brief Do nothing after all tests have succeeded
 *
 */
void loop()
{
    delay(2000);
}
