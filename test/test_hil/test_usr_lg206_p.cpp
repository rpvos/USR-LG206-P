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

/**
 * @brief Test for entering AT mode
 *
 */
void test_enter_at(void)
{
    TEST_ASSERT_TRUE_MESSAGE(lora->begin_AT_mode(), "At mode not entered");
}

/**
 * @brief Test for exiting AT mode
 *
 */
void test_exit_at(void)
{
    TEST_ASSERT_TRUE_MESSAGE(lora->end_AT_mode(), "At mode not exited");
}

/**
 * @brief Test made for production settings testing
 *
 */
void test_setup(void)
{
    TEST_ASSERT_TRUE_MESSAGE(lora->set_speed(LoRa_air_rate_level_21875), "Lora speed could not be set");

    TEST_ASSERT_TRUE_MESSAGE(lora->set_channel(72), "Lora channel could not be set");

    TEST_ASSERT_TRUE_MESSAGE(lora->set_address(1), "Lora address could not be set");
}

/**
 * @brief Testing restart
 *
 */
void test_restart(void)
{
    TEST_ASSERT_TRUE_MESSAGE(lora->restart(), "Restart did not work");
}

/**
 * @brief Testing all setting level settings
 *
 */
void test_settings(void)
{
    LoRaSettings settings = LoRaSettings(false);
    LoRaSettings settings2 = LoRaSettings(false);

    // Test 1 for get settings
    TEST_ASSERT_TRUE_MESSAGE(lora->get_settings(settings), "Getting the settings did not work");

    TEST_ASSERT_TRUE_MESSAGE(settings2 == settings, "Standard settings not used");

    // Test 1 for set settings to factory settings
    TEST_ASSERT_TRUE_MESSAGE(lora->set_settings(&LoRaSettings(true)), "Set settings to factory settings did not work");
    settings2 = LoRaSettings(true);
    TEST_ASSERT_TRUE_MESSAGE(lora->get_settings(settings), "Settings not retrieved after setting to factory settings");
    TEST_ASSERT_TRUE_MESSAGE(settings2 == settings, "Settings not set to factory settings succesfully");

    // Test 1 for save as deafult and reset to default
    TEST_ASSERT_TRUE_MESSAGE(lora->get_settings(settings), "Retieving settings did not succeed after factory settings were set");
    TEST_ASSERT_TRUE_MESSAGE(lora->save_as_default(), "Save as default did not work");
    // Alter settings
    TEST_ASSERT_TRUE_MESSAGE(lora->set_channel(66), "Settings not set after saved as default");
    // Load default settings
    TEST_ASSERT_TRUE_MESSAGE(lora->reset_to_default(), "Reset to default did not succeed");
    // Compare current settings them with the previous set settings
    TEST_ASSERT_TRUE_MESSAGE(lora->get_settings(settings2), "Retrieving settings did not succeed after setting to default");
}

/**
 * @brief Test for all set and get settings
 *
 */
void test_set_and_get(void)
{
    // TODO

    // test_set_and_get_individual_template(lora->get_address, lora->set_address, 63);
}

void test_set_and_get_individual_template(int (*get_function)(int &out), int (*set_function)(int value), int value)
{
    // Make sure to test with a different setting then i was already set to
    int out;
    TEST_ASSERT_TRUE_MESSAGE(get_function(out), "Function get did not succeed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(out, value, "Function was already set to this setting");

    // Test the set function
    TEST_ASSERT_TRUE_MESSAGE(set_function(value), "Function set did not succeed");
    int out2;
    TEST_ASSERT_TRUE_MESSAGE(get_function(out2), "Function get did not succeed");
    // New value gotten must be the same as value set to
    TEST_ASSERT_EQUAL_INT_MESSAGE(value, out2, "Set did not work");
}

/**
 * @brief Testing print function
 *
 */
void test_print(void)
{
    TEST_ASSERT_TRUE_MESSAGE(lora->send_message("Hello world!"), "Message could not be send");
}

/**
 * @brief Test for retrieving a message
 *
 */
void test_retrieve(void)
{
    // TODO
    String s = lora->retrieve_message();
    // Serial.print(s);
    TEST_ASSERT_TRUE_MESSAGE(s.length(), "Message could not be send");
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
    RUN_TEST(test_settings);
    RUN_TEST(test_set_and_get);
    RUN_TEST(test_restart);

    // Enter at mode to test the exit at mode
    RUN_TEST(test_enter_at);
    RUN_TEST(test_setup);
    RUN_TEST(test_exit_at);

    RUN_TEST(test_print);

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
