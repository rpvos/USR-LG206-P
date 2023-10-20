#include "usr_lg206_p.h"
#include "unity.h"

/**
 * @brief Test made for production settings testing
 *
 */
void test_setup(LoRa *lora)
{
    LoRaSettings::LoRaSettings settings = LoRaSettings::LoRaSettings(true);
    settings.lora_air_rate_level = LoRaSettings::LoRaAirRateLevel::kLoRaAirRateLevel21875;
    settings.channel = 72; // 470Mhz
    settings.destination_address = 1;
    settings.destination_address_is_set = true;

    TEST_ASSERT_TRUE_MESSAGE(lora->SetSettings(&settings), "Set settings was not succesfull");

    String message = "Hello World!";
    int bytes_written = lora->SendMessage(message.c_str());
    TEST_ASSERT_EQUAL_INT_MESSAGE(message.length(), bytes_written, "Message was not written succesfull");
}