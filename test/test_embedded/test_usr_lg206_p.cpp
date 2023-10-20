#include <Arduino.h>
#include <unity.h>
#include <max485ttl.h>
#include <memory_stream.h>

#include "usr_lg206_p.h"

const uint8_t enable_pin = 2;

MemoryStream *memory_stream;
RS485 *rs;

/**
 * @brief The object being tested against
 *
 */
LoRa *lora;

const int buffer_size = 64;
char buffer[buffer_size];

/**
 * @brief Set the test up by creating a new instance of, the mock stream, the rs485 module and the LoRa module
 *
 */
void setUp(void)
{
    memory_stream = new MemoryStream(true);
    rs = new RS485(enable_pin, enable_pin, memory_stream, false);
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
    delete memory_stream;
}

/**
 * @brief Test for entering AT mode
 *
 */
void test_enter_at(void)
{
    { // Setup
        String response1 = String("a");
        memory_stream->AddOutput(response1.c_str(), response1.length());
        String response2 = String("+OK");
        memory_stream->AddOutput(response2.c_str(), response2.length());
    }

    TEST_ASSERT_TRUE_MESSAGE(lora->BeginAtMode(), "At mode not entered");

    { // Check message handling
        memory_stream->ReadInput(buffer, buffer_size);
        TEST_ASSERT_EQUAL_STRING("+++", buffer);
        memory_stream->ReadInput(buffer, buffer_size);
        TEST_ASSERT_EQUAL_STRING("a", buffer);
        memory_stream->ReadInput(buffer, buffer_size);
        TEST_ASSERT_EQUAL_STRING("", buffer);
    }

    TEST_ASSERT_TRUE_MESSAGE(lora->BeginAtMode(), "At mode not entered");
    memory_stream->ReadInput(buffer, buffer_size);
    TEST_ASSERT_EQUAL_STRING("", buffer);
}

/**
 * @brief Test for exiting AT mode
 *
 */
void test_exit_at(void)
{
    { // Setup
        String response1 = String("AT+ENTM\r\n\r\n\r\nOK\r\n");
        memory_stream->AddOutput(response1.c_str(), response1.length());
    }

    TEST_ASSERT_TRUE_MESSAGE(lora->EndAtMode(), "At mode not exited");

    { // Check message handling
        memory_stream->ReadInput(buffer, buffer_size);
        TEST_ASSERT_EQUAL_STRING("AT+ENTM\r\n", buffer);
        memory_stream->ReadInput(buffer, buffer_size);
        TEST_ASSERT_EQUAL_STRING("", buffer);
    }

    // Test if returned true when already in Atmode
    TEST_ASSERT_TRUE_MESSAGE(lora->EndAtMode(), "At mode not exited");

    // Nothing must be send
    memory_stream->ReadInput(buffer, buffer_size);
    TEST_ASSERT_EQUAL_STRING("", buffer);
}

/**
 * @brief Testing Restart
 *
 */
void test_restart(void)
{
    { // Setup
        String response1 = String("AT+Z\r\n\r\n\r\nOK\r\n");
        memory_stream->AddOutput(response1.c_str(), response1.length());
    }

    TEST_ASSERT_TRUE_MESSAGE(lora->Restart(), "Restart did not work");

    { // Check message handling
        memory_stream->ReadInput(buffer, buffer_size);
        TEST_ASSERT_EQUAL_STRING("AT+Z\r\n", buffer);
        memory_stream->ReadInput(buffer, buffer_size);
        TEST_ASSERT_EQUAL_STRING("", buffer);
    }
}

/**
 * @brief Testing all setting level settings
 *
 */
void test_settings(void)
{
    // LoRaSettings settings = LoRaSettings(false);
    // LoRaSettings settings2 = LoRaSettings(false);

    // // Test get settings
    // TEST_ASSERT_TRUE_MESSAGE(lora->GetSettings(settings), "Getting the settings did not work");

    // TEST_ASSERT_TRUE_MESSAGE(settings2 == settings, "Standard settings not used");

    // // Test factory settings
    // TEST_ASSERT_TRUE_MESSAGE(lora->SetSettings(&LoRaSettings(true)), "Set settings to factory settings did not work");
    // settings2 = LoRaSettings(true);
    // TEST_ASSERT_TRUE_MESSAGE(lora->GetSettings(settings), "Settings not retrieved after setting to factory settings");
    // TEST_ASSERT_TRUE_MESSAGE(settings2 == settings, "Settings not set to factory settings succesfully");

    // // Test factory reset function
    // TEST_ASSERT_TRUE_MESSAGE(lora->FactoryReset(), "Settings not retrieved after setting to factory settings");
    // TEST_ASSERT_TRUE_MESSAGE(lora->GetSettings(settings2), "Settings not retrieved after setting to factory settings");
    // TEST_ASSERT_TRUE_MESSAGE(settings2 == settings, "Settings not set to factory settings succesfully");

    // // Test save as deafult and reset to default
    // TEST_ASSERT_TRUE_MESSAGE(lora->GetSettings(settings), "Retieving settings did not succeed after factory settings were set");
    // TEST_ASSERT_TRUE_MESSAGE(lora->SaveAsDefault(), "Save as default did not work");
    // // Alter settings
    // TEST_ASSERT_TRUE_MESSAGE(lora->set_channel(66), "Settings not set after saved as default");
    // // Load default settings
    // TEST_ASSERT_TRUE_MESSAGE(lora->ResetToDefault(), "Reset to default did not succeed");
    // // Compare current settings them with the previous set settings
    // TEST_ASSERT_TRUE_MESSAGE(lora->GetSettings(settings2), "Retrieving settings did not succeed after setting to default");
    // TEST_ASSERT_TRUE_MESSAGE(settings2 == settings, "Settings not set to factory settings succesfully");
}

void test_echo(void)
{
    LoRaSettings::CommandEchoFunction command_echo_function;

    {     // Test get on initialisation
        { // Setup
            String response1 = String("AT+E\r\n\r\nOK=ON\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        command_echo_function = lora->GetEcho();
        TEST_ASSERT_EQUAL(LoRaSettings::kEchoFunctionIsOn, command_echo_function);

        { // Check message handling
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+E\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test set echo function off
        { // Setup
            String response1 = String("AT+E\r\n\r\nOK=OFF\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_TRUE_MESSAGE(lora->SetEcho(LoRaSettings::kEchoFunctionIsOff), "Function set did not succeed");

        { // Check message handling
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+E=OFF\r\n\r\nOK\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test get for echo function off
        { // Setup
            String response1 = String("AT+E\r\n\r\nOK=OFF\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        command_echo_function = lora->GetEcho();
        TEST_ASSERT_EQUAL(LoRaSettings::kEchoFunctionIsOff, command_echo_function);

        { // Check message handling
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+E\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test set echo function on
        { // Setup
            String response1 = String("AT+E\r\n\r\nOK=ON\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_TRUE_MESSAGE(lora->SetEcho(LoRaSettings::kEchoFunctionIsOn), "Function set did not succeed");

        { // Check message handling
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+E=ON\r\n\r\nOK\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test get for echo function on
        { // Setup
            String response1 = String("AT+E\r\n\r\nOK=ON\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        command_echo_function = lora->GetEcho();
        TEST_ASSERT_EQUAL(LoRaSettings::kEchoFunctionIsOff, command_echo_function);

        { // Check message handling
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+E\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}

void test_node_id(void)
{
    String node_id = String();
    String second_node_id = String();

    {     // Test get node id
        { // Setup
            String response1 = String("\r\n+NID\r\n\r\n+NID:FFFFFFFF\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_TRUE(lora->GetNodeId(node_id));
        TEST_ASSERT_EQUAL_STRING("FFFFFFFF", node_id.c_str());

        { // Check message handling
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+NID\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test get node twice the same
        { // Setup
            String response1 = String("\r\n+NID\r\n\r\n+NID:FFFFFFFF\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_TRUE(lora->GetNodeId(second_node_id));
        TEST_ASSERT_EQUAL_STRING(node_id.c_str(), second_node_id.c_str());

        { // Check message handling
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+NID\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}

void test_firmware_version(void)
{
    char firmware_version[8];
    char second_firmware_version[8];

    {     // Test get firmware version
        { // Setup
            String response1 = String("\r\n+VER\r\n\r\n+VER:1.1.1\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_TRUE(lora->GetFirmwareVersion(firmware_version));
        TEST_ASSERT_EQUAL_STRING("1.1.1", firmware_version);

        { // Check message handling
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+VER\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test get firmware version
        { // Setup
            String response1 = String("\r\n+VER\r\n\r\n+VER:1.1.1\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_TRUE(lora->GetFirmwareVersion(second_firmware_version));
        TEST_ASSERT_EQUAL_STRING(firmware_version, second_firmware_version);

        { // Check message handling
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+VER\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}

// void test_wmode(void)
// {

//     Workmode::WorkMode value = Workmode::kWorkModeFixedPoint;
//     // Make sure to test with a different setting then the currect setting
//     Workmode::WorkMode original;
//     TEST_ASSERT_TRUE_MESSAGE(lora->GetWorkMode(original), "Function get did not succeed");
//     // Make sure to chose a new value that is in range
//     if (original == value)
//     {
//         value = Workmode::WorkMode(original - 1);
//         if (value == 0)
//         {
//             value = Workmode::WorkMode(original + 1);
//         }
//     }

//     // Test the set function
//     TEST_ASSERT_TRUE_MESSAGE(lora->SetWorkMode(value), "Function set did not succeed");
//     Workmode::WorkMode out;
//     TEST_ASSERT_TRUE_MESSAGE(lora->GetWorkMode(out), "Function get did not succeed");
//     // New value gotten must be the same as value set to
//     TEST_ASSERT_EQUAL_INT_MESSAGE(value, out, "Set did not work");

//     // Set it back to original value
//     TEST_ASSERT_TRUE_MESSAGE(lora->SetWorkMode(original), "Function set did not succeed");
// }

// void test_powermode(void)
// {
//     PowerConsumptionMode::PowerConsumptionMode value = PowerConsumptionMode::kPowerConsumptionModeWakeUp;
//     // Make sure to test with a different setting then the currect setting
//     PowerConsumptionMode::PowerConsumptionMode original;
//     TEST_ASSERT_TRUE_MESSAGE(lora->get_power_consumption_mode(original), "Function get did not succeed");
//     // Make sure to chose a new value that is in range
//     if (original == value)
//     {
//         value = PowerConsumptionMode::PowerConsumptionMode(original - 1);
//         if (value == 0)
//         {
//             value = PowerConsumptionMode::PowerConsumptionMode(original + 1);
//         }
//     }

//     // Test the set function
//     TEST_ASSERT_TRUE_MESSAGE(lora->set_power_consumption_mode(value), "Function set did not succeed");
//     PowerConsumptionMode::PowerConsumptionMode out;
//     TEST_ASSERT_TRUE_MESSAGE(lora->get_power_consumption_mode(out), "Function get did not succeed");
//     // New value gotten must be the same as value set to
//     TEST_ASSERT_EQUAL_INT_MESSAGE(value, out, "Set did not work");

//     // Set it back to original value
//     TEST_ASSERT_TRUE_MESSAGE(lora->set_power_consumption_mode(original), "Function set did not succeed");
// }
// void test_wake_up_interval(void) {}

// void test_speed(void)
// {
//     LoRaAirRateLevel::LoRaAirRateLevel value = LoRaAirRateLevel::kLoRaAirRateLevel10937;
//     // Make sure to test with a different setting then the currect setting
//     LoRaAirRateLevel::LoRaAirRateLevel original;
//     TEST_ASSERT_TRUE_MESSAGE(lora->get_speed(original), "Function get did not succeed");
//     // Make sure to chose a new value that is in range
//     if (original == value)
//     {
//         value = LoRaAirRateLevel::LoRaAirRateLevel(original - 1);
//         if (value == 0)
//         {
//             value = LoRaAirRateLevel::LoRaAirRateLevel(original + 1);
//         }
//     }

//     // Test the set function
//     TEST_ASSERT_TRUE_MESSAGE(lora->set_speed(value), "Function set did not succeed");
//     LoRaAirRateLevel::LoRaAirRateLevel out;
//     TEST_ASSERT_TRUE_MESSAGE(lora->get_speed(out), "Function get did not succeed");
//     // New value gotten must be the same as value set to
//     TEST_ASSERT_EQUAL_INT_MESSAGE(value, out, "Set did not work");

//     // Set it back to original value
//     TEST_ASSERT_TRUE_MESSAGE(lora->set_speed(original), "Function set did not succeed");
// }

// void test_address(void)
// {
//     int value = 2;
//     // Make sure to test with a different setting then the currect setting
//     int original;
//     TEST_ASSERT_TRUE_MESSAGE(lora->get_address(original), "Function get did not succeed");
//     // Make sure to chose a new value that is in range
//     if (original == value)
//     {
//         value = 3;
//     }

//     // Test the set function
//     TEST_ASSERT_TRUE_MESSAGE(lora->set_address(value), "Function set did not succeed");
//     int out;
//     TEST_ASSERT_TRUE_MESSAGE(lora->get_address(out), "Function get did not succeed");
//     // New value gotten must be the same as value set to
//     TEST_ASSERT_EQUAL_INT_MESSAGE(value, out, "Set did not work");

//     // Set it back to original value
//     TEST_ASSERT_TRUE_MESSAGE(lora->set_address(original), "Function set did not succeed");
// }

// void test_channel(void)
// {
//     int value = 2;
//     // Make sure to test with a different setting then the currect setting
//     int original;
//     TEST_ASSERT_TRUE_MESSAGE(lora->get_channel(original), "Function get did not succeed");
//     // Make sure to chose a new value that is in range
//     if (original == value)
//     {
//         value = 3;
//     }

//     // Test the set function
//     TEST_ASSERT_TRUE_MESSAGE(lora->set_channel(value), "Function set did not succeed");
//     int out;
//     TEST_ASSERT_TRUE_MESSAGE(lora->get_channel(out), "Function get did not succeed");
//     // New value gotten must be the same as value set to
//     TEST_ASSERT_EQUAL_INT_MESSAGE(value, out, "Set did not work");

//     // Set it back to original value
//     TEST_ASSERT_TRUE_MESSAGE(lora->set_channel(original), "Function set did not succeed");
// }

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
 * @brief Test for all set and get settings
 *
 */
void test_set_and_get(void)
{
    RUN_TEST(test_echo);
    RUN_TEST(test_node_id);
    RUN_TEST(test_firmware_version);
    // RUN_TEST(test_wmode);
    // RUN_TEST(test_powermode);
    // RUN_TEST(test_wake_up_interval);
    // RUN_TEST(test_speed);
    // RUN_TEST(test_address);
    // RUN_TEST(test_channel);
    RUN_TEST(test_forward_error_correction); // TODO
    RUN_TEST(test_power_transmission_value); // TODO
    RUN_TEST(test_transmission_interval);    // TODO
    RUN_TEST(test_key);                      // TODO
}

void RunAllTests(void)
{
    RUN_TEST(test_enter_at);
    RUN_TEST(test_settings); // TODO settings
    test_set_and_get();
    RUN_TEST(test_restart);
    RUN_TEST(test_enter_at);
    RUN_TEST(test_exit_at);
    // RUN_TEST(test_print);
    // RUN_TEST(test_receive);
    // RUN_TEST(test_fixed_point_transmission);
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

    RunAllTests();

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
