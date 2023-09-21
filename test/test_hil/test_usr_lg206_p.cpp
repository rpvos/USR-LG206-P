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
    LoRaSettings settings = LoRaSettings(true);
    settings.loraAirRateLevel = LoRa_air_rate_level_21875;
    settings.channel = 72; // 470Mhz
    settings.destinationAddress = 1;
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

    // Test get settings
    TEST_ASSERT_TRUE_MESSAGE(lora->get_settings(settings), "Getting the settings did not work");

    TEST_ASSERT_TRUE_MESSAGE(settings2 == settings, "Standard settings not used");

    // Test factory settings
    TEST_ASSERT_TRUE_MESSAGE(lora->set_settings(&LoRaSettings(true)), "Set settings to factory settings did not work");
    settings2 = LoRaSettings(true);
    TEST_ASSERT_TRUE_MESSAGE(lora->get_settings(settings), "Settings not retrieved after setting to factory settings");
    TEST_ASSERT_TRUE_MESSAGE(settings2 == settings, "Settings not set to factory settings succesfully");

    // Test factory reset function
    TEST_ASSERT_TRUE_MESSAGE(lora->factory_reset(), "Settings not retrieved after setting to factory settings");
    TEST_ASSERT_TRUE_MESSAGE(lora->get_settings(settings2), "Settings not retrieved after setting to factory settings");
    TEST_ASSERT_TRUE_MESSAGE(settings2 == settings, "Settings not set to factory settings succesfully");

    // Test save as deafult and reset to default
    TEST_ASSERT_TRUE_MESSAGE(lora->get_settings(settings), "Retieving settings did not succeed after factory settings were set");
    TEST_ASSERT_TRUE_MESSAGE(lora->save_as_default(), "Save as default did not work");
    // Alter settings
    TEST_ASSERT_TRUE_MESSAGE(lora->set_channel(66), "Settings not set after saved as default");
    // Load default settings
    TEST_ASSERT_TRUE_MESSAGE(lora->reset_to_default(), "Reset to default did not succeed");
    // Compare current settings them with the previous set settings
    TEST_ASSERT_TRUE_MESSAGE(lora->get_settings(settings2), "Retrieving settings did not succeed after setting to default");
    TEST_ASSERT_TRUE_MESSAGE(settings2 == settings, "Settings not set to factory settings succesfully");
}

/**
 * @brief Test for all set and get settings
 *
 */
void test_set_and_get(void)
{
    RUN_TEST(test_echo);
    RUN_TEST(test_node_id);
    RUN_TEST(test_firmware_version);
    RUN_TEST(test_wmode);
    RUN_TEST(test_powermode);
    RUN_TEST(test_wake_up_interval); // TODO
    RUN_TEST(test_speed);
    RUN_TEST(test_address);
    RUN_TEST(test_channel);
    RUN_TEST(test_forward_error_correction); // TODO
    RUN_TEST(test_power_transmission_value); // TODO
    RUN_TEST(test_transmission_interval);    // TODO
    RUN_TEST(test_key);                      // TODO
}

void test_echo(void)
{
    bool value = false;
    // Make sure to test with a different setting then the currect setting
    bool original;
    TEST_ASSERT_TRUE_MESSAGE(lora->get_echo(original), "Function get did not succeed");
    // Make sure to chose a new value that is in range
    if (original == value)
    {
        value = !value;
    }

    // Test the set function
    TEST_ASSERT_TRUE_MESSAGE(lora->set_echo(value), "Function set did not succeed");
    bool out;
    TEST_ASSERT_TRUE_MESSAGE(lora->get_echo(out), "Function get did not succeed");
    // New value gotten must be the same as value set to
    TEST_ASSERT_EQUAL_INT_MESSAGE(value, out, "Set did not work");

    // Set it back to original value
    TEST_ASSERT_TRUE_MESSAGE(lora->set_echo(original), "Function set did not succeed");
}

void test_node_id(void)
{
    String original;
    String secondOriginal;
    TEST_ASSERT_TRUE_MESSAGE(lora->get_node_id(original), "Function get did not succeed");
    TEST_ASSERT_TRUE_MESSAGE(lora->get_node_id(secondOriginal), "Function get did not succeed");

    TEST_ASSERT_EQUAL_STRING_MESSAGE(original.c_str(), secondOriginal.c_str(), "Node id was not the same");
}

void test_firmware_version(void)
{
    String original;
    String secondOriginal;
    TEST_ASSERT_TRUE_MESSAGE(lora->get_firmware_version(original), "Function get did not succeed");
    TEST_ASSERT_TRUE_MESSAGE(lora->get_firmware_version(secondOriginal), "Function get did not succeed");

    TEST_ASSERT_EQUAL_STRING_MESSAGE(original.c_str(), secondOriginal.c_str(), "Firmware version was not the same");
}

void test_wmode(void)
{

    WorkMode value = work_mode_fixed_point;
    // Make sure to test with a different setting then the currect setting
    WorkMode original;
    TEST_ASSERT_TRUE_MESSAGE(lora->get_wmode(original), "Function get did not succeed");
    // Make sure to chose a new value that is in range
    if (original == value)
    {
        value = WorkMode(original - 1);
        if (value == 0)
        {
            value = WorkMode(original + 1);
        }
    }

    // Test the set function
    TEST_ASSERT_TRUE_MESSAGE(lora->set_wmode(value), "Function set did not succeed");
    WorkMode out;
    TEST_ASSERT_TRUE_MESSAGE(lora->get_wmode(out), "Function get did not succeed");
    // New value gotten must be the same as value set to
    TEST_ASSERT_EQUAL_INT_MESSAGE(value, out, "Set did not work");

    // Set it back to original value
    TEST_ASSERT_TRUE_MESSAGE(lora->set_wmode(original), "Function set did not succeed");
}

void test_powermode(void)
{
    PowerConsumptionMode value = powermode_wake_up;
    // Make sure to test with a different setting then the currect setting
    PowerConsumptionMode original;
    TEST_ASSERT_TRUE_MESSAGE(lora->get_power_consumption_mode(original), "Function get did not succeed");
    // Make sure to chose a new value that is in range
    if (original == value)
    {
        value = WorkMode(original - 1);
        if (value == 0)
        {
            value = WorkMode(original + 1);
        }
    }

    // Test the set function
    TEST_ASSERT_TRUE_MESSAGE(lora->set_power_consumption_mode(value), "Function set did not succeed");
    PowerConsumptionMode out;
    TEST_ASSERT_TRUE_MESSAGE(lora->get_power_consumption_mode(out), "Function get did not succeed");
    // New value gotten must be the same as value set to
    TEST_ASSERT_EQUAL_INT_MESSAGE(value, out, "Set did not work");

    // Set it back to original value
    TEST_ASSERT_TRUE_MESSAGE(lora->set_power_consumption_mode(original), "Function set did not succeed");
}
void test_wake_up_interval(void) {}

void test_speed(void)
{
    LoRaAirRateLevel value = LoRa_air_rate_level_10937;
    // Make sure to test with a different setting then the currect setting
    LoRaAirRateLevel original;
    TEST_ASSERT_TRUE_MESSAGE(lora->get_speed(original), "Function get did not succeed");
    // Make sure to chose a new value that is in range
    if (original == value)
    {
        value = LoRaAirRateLevel(original - 1);
        if (value == 0)
        {
            value = LoRaAirRateLevel(original + 1);
        }
    }

    // Test the set function
    TEST_ASSERT_TRUE_MESSAGE(lora->set_speed(value), "Function set did not succeed");
    LoRaAirRateLevel out;
    TEST_ASSERT_TRUE_MESSAGE(lora->get_speed(out), "Function get did not succeed");
    // New value gotten must be the same as value set to
    TEST_ASSERT_EQUAL_INT_MESSAGE(value, out, "Set did not work");

    // Set it back to original value
    TEST_ASSERT_TRUE_MESSAGE(lora->set_speed(original), "Function set did not succeed");
}

void test_address(void)
{
    int value = 2;
    // Make sure to test with a different setting then the currect setting
    int original;
    TEST_ASSERT_TRUE_MESSAGE(lora->get_address(original), "Function get did not succeed");
    // Make sure to chose a new value that is in range
    if (original == value)
    {
        value = original - 1;
        if (value == 0)
        {
            value = original + 1;
        }
    }

    // Test the set function
    TEST_ASSERT_TRUE_MESSAGE(lora->set_address(value), "Function set did not succeed");
    int out;
    TEST_ASSERT_TRUE_MESSAGE(lora->get_address(out), "Function get did not succeed");
    // New value gotten must be the same as value set to
    TEST_ASSERT_EQUAL_INT_MESSAGE(value, out, "Set did not work");

    // Set it back to original value
    TEST_ASSERT_TRUE_MESSAGE(lora->set_address(original), "Function set did not succeed");
}

void test_channel(void)
{
    int value = 2;
    // Make sure to test with a different setting then the currect setting
    int original;
    TEST_ASSERT_TRUE_MESSAGE(lora->get_channel(original), "Function get did not succeed");
    // Make sure to chose a new value that is in range
    if (original == value)
    {
        value = original - 1;
        if (value == 0)
        {
            value = original + 1;
        }
    }

    // Test the set function
    TEST_ASSERT_TRUE_MESSAGE(lora->set_channel(value), "Function set did not succeed");
    int out;
    TEST_ASSERT_TRUE_MESSAGE(lora->get_channel(out), "Function get did not succeed");
    // New value gotten must be the same as value set to
    TEST_ASSERT_EQUAL_INT_MESSAGE(value, out, "Set did not work");

    // Set it back to original value
    TEST_ASSERT_TRUE_MESSAGE(lora->set_channel(original), "Function set did not succeed");
}

void test_forward_error_correction(void) {}
void test_power_transmission_value(void) {}
void test_transmission_interval(void) {}
void test_key(void) {}

/**
 * @brief Testing print function
 *
 */
void test_print(void)
{
    TEST_ASSERT_TRUE_MESSAGE(lora->SendMessage("Hello world!"), "Message could not be send");
}

/**
 * @brief Test for retrieving a message
 *
 */
void test_receive(void)
{
    // TODO
    String s = lora->ReceiveMessage();
    Serial.println(s);
    TEST_ASSERT_TRUE_MESSAGE(s.length(), "Message could not be received");
}

void test_fixed_point_transmission(void)
{
    // TODO fixed point test
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
    RUN_TEST(test_receive);

    RUN_TEST(test_fixed_point_transmission);

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
