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

    TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->BeginAtMode(), "At mode not entered");

    { // Check sent message
        memory_stream->ReadInput(buffer, buffer_size);
        TEST_ASSERT_EQUAL_STRING("+++", buffer);
        memory_stream->ReadInput(buffer, buffer_size);
        TEST_ASSERT_EQUAL_STRING("a", buffer);
        memory_stream->ReadInput(buffer, buffer_size);
        TEST_ASSERT_EQUAL_STRING("", buffer);
    }

    TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->BeginAtMode(), "At mode was not saved");
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

    TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->EndAtMode(), "At mode not exited");

    { // Check sent message
        memory_stream->ReadInput(buffer, buffer_size);
        TEST_ASSERT_EQUAL_STRING("AT+ENTM\r\n", buffer);
        memory_stream->ReadInput(buffer, buffer_size);
        TEST_ASSERT_EQUAL_STRING("", buffer);
    }

    // Test if returned true when already in Atmode
    TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->EndAtMode(), "At mode not exited");

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

    TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->Restart(), "Restart did not work");

    { // Check sent message
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
    // TEST_ASSERT_TRUE_MESSAGE(lora->SetChannel(66), "Settings not set after saved as default");
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

        LoRaErrorCode response_code = lora->GetEcho(command_echo_function);
        TEST_ASSERT_EQUAL(LoRaErrorCode::kSucces, response_code);
        TEST_ASSERT_EQUAL(LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOn, command_echo_function);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+E\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test set echo function off
        { // Setup
            String response1 = String("AT+E=OFF\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetEcho(LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOff), "Function set did not succeed");

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+E=OFF\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {
        LoRaErrorCode response_code = lora->GetEcho(command_echo_function);
        TEST_ASSERT_EQUAL(LoRaErrorCode::kSucces, response_code);
        TEST_ASSERT_EQUAL(LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOff, command_echo_function);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test set echo function on
        { // Setup
            String response1 = String("AT+E=ON\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetEcho(LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOn), "Function set did not succeed");

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+E=ON\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    { // Test get for echo function on
        LoRaErrorCode response_code = lora->GetEcho(command_echo_function);
        TEST_ASSERT_EQUAL(LoRaErrorCode::kSucces, response_code);
        TEST_ASSERT_EQUAL(LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOn, command_echo_function);

        { // Check sent message
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

        LoRaErrorCode response_code = lora->GetNodeId(node_id);
        TEST_ASSERT_EQUAL(LoRaErrorCode::kSucces, response_code);
        TEST_ASSERT_EQUAL_STRING("FFFFFFFF", node_id.c_str());

        { // Check sent message
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

        LoRaErrorCode response_code = lora->GetNodeId(second_node_id);
        TEST_ASSERT_EQUAL(LoRaErrorCode::kSucces, response_code);
        TEST_ASSERT_EQUAL_STRING("FFFFFFFF", second_node_id.c_str());
        TEST_ASSERT_EQUAL_STRING(node_id.c_str(), second_node_id.c_str());

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}

void test_firmware_version(void)
{
    String firmware_version;
    String second_firmware_version;

    {     // Test get firmware version
        { // Setup
            String response1 = String("\r\n+VER\r\n\r\n+VER:1.1.1\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        // TODO fix get firmware version
        LoRaErrorCode response_code = lora->GetFirmwareVersion(firmware_version);
        TEST_ASSERT_EQUAL(LoRaErrorCode::kSucces, response_code);
        TEST_ASSERT_EQUAL_STRING("1.1.1", firmware_version.c_str());

        { // Check sent message
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

        LoRaErrorCode response_code = lora->GetFirmwareVersion(second_firmware_version);
        TEST_ASSERT_EQUAL(LoRaErrorCode::kSucces, response_code);
        TEST_ASSERT_EQUAL_STRING("1.1.1", second_firmware_version.c_str());
        TEST_ASSERT_EQUAL_STRING(firmware_version.c_str(), second_firmware_version.c_str());

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}

void test_wmode(void)
{
    LoRaSettings::WorkMode value;
    {
        { // Setup
            String response1 = String("\r\nAT+WMODE\r\n\r\n+WMODE:TRANS\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWorkMode(value), "Function get did not succeed");
        // Make sure it defaults to transparent
        TEST_ASSERT_EQUAL(LoRaSettings::WorkMode::kWorkModeTransparent, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+WMODE\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
            String response1 = String("\r\nAT+WMODE=FP\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWorkMode(LoRaSettings::WorkMode::kWorkModeFixedPoint), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWorkMode(value), "Function get did not succeed");

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+WMODE=FP\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWorkMode(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWorkMode(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::WorkMode::kWorkModeFixedPoint, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test set transparent
        { // Setup
            String response1 = String("\r\nAT+WMODE=TRANS\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWorkMode(LoRaSettings::WorkMode::kWorkModeTransparent), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWorkMode(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::WorkMode::kWorkModeTransparent, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+WMODE=TRANS\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test when value already is set
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWorkMode(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWorkMode(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::WorkMode::kWorkModeTransparent, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}

void test_powermode(void)
{
    LoRaSettings::PowerConsumptionMode value;
    {     // Test initial state
        { // Setup
            String response1 = String("\r\nAT+PMODE\r\n\r\n+PMODE:RUN\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerConsumptionMode(value), "Function get did not succeed");
        // Make sure it defaults to RUN
        TEST_ASSERT_EQUAL(LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeRun, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+PMODE\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
            String response1 = String("\r\nAT+PMODE=WU\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerConsumptionMode(LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeWakeUp), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerConsumptionMode(value), "Function get did not succeed");

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+PMODE=WU\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerConsumptionMode(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerConsumptionMode(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeWakeUp, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test set default
        { // Setup
            String response1 = String("\r\nAT+PMODE=RUN\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerConsumptionMode(), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerConsumptionMode(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeRun, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+PMODE=RUN\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test when value already is set
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerConsumptionMode(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerConsumptionMode(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeRun, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}

void test_waking_up_interval(void)
{
    int value;
    {     // Test initial state
        { // Setup
            String response1 = String("\r\nAT+WTM\r\n\r\n+WTM:2000\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWakingUpInterval(value), "Function get did not succeed");
        // Make sure it defaults to 2000
        TEST_ASSERT_EQUAL_INT(2000, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+WTM\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
            String response1 = String("\r\nAT+WTM=1000\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWakingUpInterval(1000), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWakingUpInterval(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(1000, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+WTM=1000\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWakingUpInterval(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWakingUpInterval(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(1000, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test set default
        { // Setup
            String response1 = String("\r\nAT+WTM=2000\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWakingUpInterval(), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWakingUpInterval(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(2000, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+WTM=2000\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test when value already is set
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWakingUpInterval(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWakingUpInterval(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(2000, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}

void test_air_rate_level(void)
{
    LoRaSettings::LoRaAirRateLevel value;
    {     // Test initial state
        { // Setup
            String response1 = String("\r\nAT+SPD\r\n\r\n+SPD:10\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetAirRateLevel(value), "Function get did not succeed");
        // Make sure it defaults to RUN
        TEST_ASSERT_EQUAL(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel21875, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+SPD\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
            String response1 = String("\r\nAT+SPD=9\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetAirRateLevel(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel10937), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetAirRateLevel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel10937, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+SPD=9\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetAirRateLevel(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetAirRateLevel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel10937, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test set transparent
        { // Setup
            String response1 = String("\r\nAT+SPD=10\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetAirRateLevel(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel21875), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetAirRateLevel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel21875, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+SPD=10\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test when value already is set
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetAirRateLevel(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetAirRateLevel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel21875, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}

void test_address(void)
{
    int value;
    {     // Test initial state
        { // Setup
            String response1 = String("\r\nAT+ADDR\r\n\r\n+ADDR:0\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetDestinationAddress(value), "Function get did not succeed");
        // Make sure it defaults to 0
        TEST_ASSERT_EQUAL_INT(0, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+ADDR\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
            String response1 = String("\r\nAT+ADDR=1\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetDestinationAddress(1), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetDestinationAddress(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(1, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+ADDR=1\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetDestinationAddress(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetDestinationAddress(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(1, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test set transparent
        { // Setup
            String response1 = String("\r\nAT+ADDR=0\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetDestinationAddress(0), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetDestinationAddress(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(0, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+ADDR=0\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test when value already is set
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetDestinationAddress(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetDestinationAddress(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(0, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}

void test_channel(void)
{
    int value;
    {     // Test initial state
        { // Setup
            String response1 = String("\r\nAT+CH\r\n\r\n+CH:72\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetChannel(value), "Function get did not succeed");
        // Make sure it defaults to 72
        TEST_ASSERT_EQUAL_INT(72, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+CH\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
            String response1 = String("\r\nAT+CH=70\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetChannel(70), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetChannel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(70, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+CH=70\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetChannel(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetChannel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(70, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test set transparent
        { // Setup
            String response1 = String("\r\nAT+CH=72\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetChannel(72), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetChannel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(72, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+CH=72\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test when value already is set
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetChannel(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetChannel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(72, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}

void test_forward_error_correction(void)
{

    LoRaSettings::ForwardErrorCorrection value;
    {     // Test initial state
        { // Setup
            String response1 = String("\r\nAT+FEC\r\n\r\n+FEC:OFF\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetForwardErrorCorrection(value), "Function get did not succeed");
        // Make sure it defaults to 72
        TEST_ASSERT_EQUAL(LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOff, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+FEC\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
            String response1 = String("\r\nAT+FEC=ON\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetForwardErrorCorrection(LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOn), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetForwardErrorCorrection(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOn, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+FEC=ON\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetForwardErrorCorrection(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetForwardErrorCorrection(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOn, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test set transparent
        { // Setup
            String response1 = String("\r\nAT+FEC=OFF\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetForwardErrorCorrection(LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOff), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetForwardErrorCorrection(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOff, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+FEC=OFF\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test when value already is set
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetForwardErrorCorrection(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetForwardErrorCorrection(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOff, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}

void test_power_transmission_value(void)
{
    int value;
    {     // Test initial state
        { // Setup
            String response1 = String("\r\nAT+PWR\r\n\r\n+PWR:20\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerTransmissionValue(value), "Function get did not succeed");
        // Make sure it defaults to 20
        TEST_ASSERT_EQUAL_INT(20, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+PWR\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
            String response1 = String("\r\nAT+PWR=10\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerTransmissionValue(10), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerTransmissionValue(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(10, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+PWR=10\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerTransmissionValue(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerTransmissionValue(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(10, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test set transparent
        { // Setup
            String response1 = String("\r\nAT+PWR=20\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerTransmissionValue(20), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerTransmissionValue(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(20, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+PWR=20\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     // Test when value already is set
        { // Setup
        }

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerTransmissionValue(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerTransmissionValue(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(20, value);

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}

void test_transmission_interval(void)
{
    {     // Test initial state
        { // Setup
            String response1 = String("\r\nAT+SQT\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
            String response2 = String("\r\nADDR:0 SNR:8 RSSI:-15.742600\r\n");
            memory_stream->AddOutput(response2.c_str(), response2.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->QueryTransmissionInterval(), "Function get did not succeed");
        String value = lora->ReceiveMessage();
        value.trim();
        TEST_ASSERT_EQUAL_STRING("ADDR:0 SNR:8 RSSI:-15.742600", value.c_str());

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+SQT\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }

    {     //  Test the set function
        { // Setup
            String response1 = String("\r\nAT+SQT=500\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetTransmissionInterval(500), "Function set did not succeed");
        TEST_ASSERT_EQUAL_STRING("\r\nADDR:0 SNR:8 RSSI:-15.742600\r\n", lora->ReceiveMessage().c_str());
        TEST_ASSERT_EQUAL_STRING("\r\nADDR:0 SNR:8 RSSI:-15.742600\r\n", lora->ReceiveMessage().c_str());

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+SQT=500\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}
void test_key(void)
{
    {     //  Test the set function
        { // Setup
            String response1 = String("\r\nAT+KEY=FFFFFFFFFFFFFFFF\r\n\r\n\r\nOK\r\n");
            memory_stream->AddOutput(response1.c_str(), response1.length());
        }

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetKey("FFFFFFFFFFFFFFFF"), "Function set did not succeed");

        { // Check sent message
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("AT+KEY=FFFFFFFFFFFFFFFF\r\n", buffer);
            memory_stream->ReadInput(buffer, buffer_size);
            TEST_ASSERT_EQUAL_STRING("", buffer);
        }
    }
}

/**
 * @brief Testing print function
 *
 */
void test_print(void)
{
    TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SendMessage("Hello world!"), "Message could not be send");
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
    RUN_TEST(test_wmode);
    RUN_TEST(test_powermode);
    RUN_TEST(test_waking_up_interval);
    RUN_TEST(test_air_rate_level);
    RUN_TEST(test_address);
    RUN_TEST(test_channel);
    RUN_TEST(test_forward_error_correction);
    RUN_TEST(test_power_transmission_value);
    RUN_TEST(test_transmission_interval); // TODO
    RUN_TEST(test_key);
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
