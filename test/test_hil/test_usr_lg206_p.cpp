#include <Arduino.h>
#include <unity.h>
#include <max485ttl.h>
#include <memory_stream.h>

#include "usr_lg206_p.h"

const uint8_t enable_pin = 2;

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
    rs = new RS485(enable_pin, enable_pin, &Serial1, false);
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
}

/**
 * @brief Test for entering AT mode
 *
 */
void test_enter_at(void)
{
    TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->BeginAtMode(), "At mode not entered");
    TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->BeginAtMode(), "At mode was not saved");
}

/**
 * @brief Test for exiting AT mode
 *
 */
void test_exit_at(void)
{
    TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->EndAtMode(), "At mode not exited");

    // Test if returned true when already out Atmode
    TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->EndAtMode(), "At mode exit not saved");
}

/**
 * @brief Testing Restart
 *
 */
void test_restart(void)
{
    TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->Restart(), "Restart did not work");
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

    { // Test get on initialisation

        LoRaErrorCode response_code = lora->GetEcho(command_echo_function);
        TEST_ASSERT_EQUAL(LoRaErrorCode::kSucces, response_code);
        TEST_ASSERT_EQUAL(LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOn, command_echo_function);
    }

    { // Test set echo function off

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetEcho(LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOff), "Function set did not succeed");
    }

    {
        LoRaErrorCode response_code = lora->GetEcho(command_echo_function);
        TEST_ASSERT_EQUAL(LoRaErrorCode::kSucces, response_code);
        TEST_ASSERT_EQUAL(LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOff, command_echo_function);
    }

    { // Test set echo function on

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetEcho(LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOn), "Function set did not succeed");
    }

    { // Test get for echo function on

        LoRaErrorCode response_code = lora->GetEcho(command_echo_function);
        TEST_ASSERT_EQUAL(LoRaErrorCode::kSucces, response_code);
        TEST_ASSERT_EQUAL(LoRaSettings::CommandEchoFunction::kCommandEchoFunctionIsOn, command_echo_function);
    }
}

void test_node_id(void)
{
    String node_id = String();
    String second_node_id = String();

    { // Test get node id

        LoRaErrorCode response_code = lora->GetNodeId(node_id);
        TEST_ASSERT_EQUAL(LoRaErrorCode::kSucces, response_code);
        TEST_ASSERT_EQUAL_STRING("FFFFFFFF", node_id.c_str());
    }

    { // Test get node twice the same

        LoRaErrorCode response_code = lora->GetNodeId(second_node_id);
        TEST_ASSERT_EQUAL(LoRaErrorCode::kSucces, response_code);
        TEST_ASSERT_EQUAL_STRING("FFFFFFFF", second_node_id.c_str());
        TEST_ASSERT_EQUAL_STRING(node_id.c_str(), second_node_id.c_str());
    }
}

void test_firmware_version(void)
{
    String firmware_version;
    String second_firmware_version;

    { // Test get firmware version

        LoRaErrorCode response_code = lora->GetFirmwareVersion(firmware_version);
        TEST_ASSERT_EQUAL(LoRaErrorCode::kSucces, response_code);
        TEST_ASSERT_EQUAL_STRING("1.1.1", firmware_version.c_str());
    }

    { // Test get firmware version

        LoRaErrorCode response_code = lora->GetFirmwareVersion(second_firmware_version);
        TEST_ASSERT_EQUAL(LoRaErrorCode::kSucces, response_code);
        TEST_ASSERT_EQUAL_STRING("1.1.1", second_firmware_version.c_str());
        TEST_ASSERT_EQUAL_STRING(firmware_version.c_str(), second_firmware_version.c_str());
    }
}

void test_wmode(void)
{
    LoRaSettings::WorkMode value;
    {

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWorkMode(value), "Function get did not succeed");
        // Make sure it defaults to transparent
        TEST_ASSERT_EQUAL(LoRaSettings::WorkMode::kWorkModeTransparent, value);
    }

    { //  Test the set function

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWorkMode(LoRaSettings::WorkMode::kWorkModeFixedPoint), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWorkMode(value), "Function get did not succeed");
    }

    { //  Test the set function

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWorkMode(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWorkMode(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::WorkMode::kWorkModeFixedPoint, value);
    }

    { // Test set transparent

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWorkMode(LoRaSettings::WorkMode::kWorkModeTransparent), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWorkMode(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::WorkMode::kWorkModeTransparent, value);
    }

    { // Test when value already is set

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWorkMode(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWorkMode(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::WorkMode::kWorkModeTransparent, value);
    }
}

void test_powermode(void)
{
    LoRaSettings::PowerConsumptionMode value;
    { // Test initial state

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerConsumptionMode(value), "Function get did not succeed");
        // Make sure it defaults to RUN
        TEST_ASSERT_EQUAL(LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeRun, value);
    }

    { //  Test the set function

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerConsumptionMode(LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeWakeUp), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerConsumptionMode(value), "Function get did not succeed");
    }

    { //  Test the set function

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerConsumptionMode(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerConsumptionMode(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeWakeUp, value);
    }

    { // Test set default

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerConsumptionMode(), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerConsumptionMode(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeRun, value);
    }

    { // Test when value already is set

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerConsumptionMode(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerConsumptionMode(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::PowerConsumptionMode::kPowerConsumptionModeRun, value);
    }
}

void test_waking_up_interval(void)
{
    int value;
    { // Test initial state

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWakingUpInterval(value), "Function get did not succeed");
        // Make sure it defaults to 2000
        TEST_ASSERT_EQUAL_INT(2000, value);
    }

    { //  Test the set function

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWakingUpInterval(1000), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWakingUpInterval(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(1000, value);
    }

    { //  Test the set function

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWakingUpInterval(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWakingUpInterval(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(1000, value);
    }

    { // Test set default

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWakingUpInterval(), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWakingUpInterval(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(2000, value);
    }

    { // Test when value already is set

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetWakingUpInterval(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetWakingUpInterval(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(2000, value);
    }
}

void test_air_rate_level(void)
{
    LoRaSettings::LoRaAirRateLevel value;
    { // Test initial state

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetAirRateLevel(value), "Function get did not succeed");
        // Make sure it defaults to RUN
        TEST_ASSERT_EQUAL(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel21875, value);
    }

    { //  Test the set function

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetAirRateLevel(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel10937), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetAirRateLevel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel10937, value);
    }

    { //  Test the set function

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetAirRateLevel(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetAirRateLevel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel10937, value);
    }

    { // Test set transparent

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetAirRateLevel(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel21875), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetAirRateLevel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel21875, value);
    }

    { // Test when value already is set

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetAirRateLevel(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetAirRateLevel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel21875, value);
    }
}

void test_address(void)
{
    int value;
    { // Test initial state

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetDestinationAddress(value), "Function get did not succeed");
        // Make sure it defaults to 0
        TEST_ASSERT_EQUAL_INT(0, value);
    }

    { //  Test the set function

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetDestinationAddress(1), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetDestinationAddress(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(1, value);
    }

    { //  Test the set function

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetDestinationAddress(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetDestinationAddress(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(1, value);
    }

    { // Test set transparent

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetDestinationAddress(0), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetDestinationAddress(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(0, value);
    }

    { // Test when value already is set

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetDestinationAddress(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetDestinationAddress(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(0, value);
    }
}

void test_channel(void)
{
    int value;
    { // Test initial state

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetChannel(value), "Function get did not succeed");
        // Make sure it defaults to 72
        TEST_ASSERT_EQUAL_INT(72, value);
    }

    { //  Test the set function

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetChannel(70), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetChannel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(70, value);
    }

    { //  Test the set function

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetChannel(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetChannel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(70, value);
    }

    { // Test set transparent

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetChannel(72), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetChannel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(72, value);
    }

    { // Test when value already is set

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetChannel(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetChannel(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(72, value);
    }
}

void test_forward_error_correction(void)
{

    LoRaSettings::ForwardErrorCorrection value;
    { // Test initial state

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetForwardErrorCorrection(value), "Function get did not succeed");
        // Make sure it defaults to 72
        TEST_ASSERT_EQUAL(LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOff, value);
    }

    { //  Test the set function

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetForwardErrorCorrection(LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOn), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetForwardErrorCorrection(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOn, value);
    }

    { //  Test the set function

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetForwardErrorCorrection(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetForwardErrorCorrection(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOn, value);
    }

    { // Test set transparent

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetForwardErrorCorrection(LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOff), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetForwardErrorCorrection(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOff, value);
    }

    { // Test when value already is set

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetForwardErrorCorrection(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetForwardErrorCorrection(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL(LoRaSettings::ForwardErrorCorrection::kForwardErrorCorrectionIsOff, value);
    }
}

void test_power_transmission_value(void)
{
    int value;
    { // Test initial state

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerTransmissionValue(value), "Function get did not succeed");
        // Make sure it defaults to 20
        TEST_ASSERT_EQUAL_INT(20, value);
    }

    { //  Test the set function

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerTransmissionValue(10), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerTransmissionValue(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(10, value);
    }

    { //  Test the set function

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerTransmissionValue(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerTransmissionValue(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(10, value);
    }

    { // Test set transparent

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerTransmissionValue(20), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerTransmissionValue(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(20, value);
    }

    { // Test when value already is set

        // Set when it is already set test
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetPowerTransmissionValue(value), "Function set did not succeed");
        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->GetPowerTransmissionValue(value), "Function get did not succeed");
        TEST_ASSERT_EQUAL_INT(20, value);
    }
}

void test_transmission_interval(void)
{
    // TODO make test for transmission viable
    { // Test initial state

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->QueryTransmissionInterval(), "Function get did not succeed");
        String value = lora->ReceiveMessage();
        value.trim();
        TEST_ASSERT_EQUAL_STRING("ADDR:0 SNR:8 RSSI:-15.742600", value.c_str());
    }

    { //  Test the set function

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetTransmissionInterval(500), "Function set did not succeed");
        TEST_ASSERT_EQUAL_STRING("\r\nADDR:0 SNR:8 RSSI:-15.742600\r\n", lora->ReceiveMessage().c_str());
        TEST_ASSERT_EQUAL_STRING("\r\nADDR:0 SNR:8 RSSI:-15.742600\r\n", lora->ReceiveMessage().c_str());
    }
}

void test_key(void)
{
    { //  Test the set function

        TEST_ASSERT_EQUAL_MESSAGE(LoRaErrorCode::kSucces, lora->SetKey("FFFFFFFFFFFFFFFF"), "Function set did not succeed");
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
}

/**
 * @brief Do nothing after all tests have succeeded
 *
 */
void loop()
{
    delay(2000);
}
