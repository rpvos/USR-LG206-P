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

#define DE_PORT 2
#define RE_PORT 3

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
}

/**
 * @brief Clean up the test by deleting the used memory and giving it back
 *
 */
void tearDown(void)
{
}

void test_enter_at(void)
{
    TEST_ASSERT_TRUE_MESSAGE(lora->begin_AT_mode(), "At mode not entered");
}

void test_exit_at(void)
{
    TEST_ASSERT_TRUE_MESSAGE(lora->end_AT_mode(), "At mode not exited");
}

void test_settings(void)
{
    LoRaSettings settings = LoRaSettings(false);
    LoRaSettings settings2 = LoRaSettings(false);

    lora->get_settings(settings);

    TEST_ASSERT_TRUE_MESSAGE(settings2 == settings, "Standard settings not used");

    lora->set_settings(&LoRaSettings(true));
    settings2 = LoRaSettings(true);

    lora->get_settings(settings);
    TEST_ASSERT_TRUE_MESSAGE(settings2 == settings, "Settings not set succesfully");

    // TODO add factory settings test
}

void test_setup(void)
{
    TEST_ASSERT_TRUE_MESSAGE(lora->set_speed(LoRa_air_rate_level_21875), "Lora speed could not be set");

    TEST_ASSERT_TRUE_MESSAGE(lora->set_channel(72), "Lora channel could not be set");

    TEST_ASSERT_TRUE_MESSAGE(lora->set_address(1), "Lora address could not be set");
}

void test_restart(void)
{
    TEST_ASSERT_TRUE_MESSAGE(lora->restart(), "Restart did not work");
}

void test_save_settings(void)
{
    TEST_ASSERT_TRUE_MESSAGE(lora->save_as_default(), "Save as default did not work");
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

    Serial1.begin(115200);
    rs = new RS485(DE_PORT, RE_PORT, &Serial1);
    lora = new LoRa(rs);

    UNITY_BEGIN(); // Start unit testing

    RUN_TEST(test_enter_at);
    // RUN_TEST(test_settings);
    RUN_TEST(test_setup);
    RUN_TEST(test_save_settings);
    RUN_TEST(test_restart);

    // Enter at mode to test the exit at mode
    RUN_TEST(test_enter_at);
    RUN_TEST(test_exit_at);

    UNITY_END(); // Stop unit testing

    delete lora;
    delete rs;
    Serial1.end();
}

/**
 * @brief Do nothing after all tests have succeeded
 *
 */
void loop()
{
    delay(2000);
}
