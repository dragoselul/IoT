#include "gtest/gtest.h"
#include "./FreeRTOS_defs/fff.h"
#include "./FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
    #include "../src/Headers/LoRaWAN.h"
	#include "../src/drivers/lora_driver.h"
	#include "../src/Headers/Threshold.h"
}

/*
FAKE_VALUE_FUNC(threshold_t, threshold_create);
FAKE_VOID_FUNC(threshold_destroy, threshold_t);
FAKE_VALUE_FUNC(int16_t, get_temperature_threshold, threshold_t*);
FAKE_VALUE_FUNC(uint16_t, get_humidity_threshold, threshold_t*);
FAKE_VALUE_FUNC(uint16_t, get_c02_threshold, threshold_t*);
FAKE_VALUE_FUNC(uint16_t, get_light_threshold, threshold_t*);

FAKE_VOID_FUNC(set_temperature_threshold, threshold_t*, int16_t);
FAKE_VOID_FUNC(set_humidity_threshold, threshold_t*, uint16_t);
FAKE_VOID_FUNC(set_light_threshold, threshold_t*, uint16_t);
FAKE_VOID_FUNC(set_co2_threshold, threshold_t*, uint16_t);
*/
FAKE_VALUE_FUNC(uint8_t ,lora_driver_getMaxPayloadSize);
FAKE_VOID_FUNC(lora_driver_initialise, serial_comPort_t, MessageBufferHandle_t);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_setOtaaIdentity, char*, char*, char*);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_configureToEu868);
FAKE_VALUE_FUNC(char*, lora_driver_mapReturnCodeToText, lora_driver_returnCode_t);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_setAbpIdentity, char*, char*, char*);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_join, lora_driver_joinMode_t);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_sendUploadMessage, bool, lora_driver_payload_t*);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_setDeviceIdentifier,const char*);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_setApplicationIdentifier,const char*);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_setApplicationKey, const char*);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_setNetworkSessionKey, const char*);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_setApplicationSessionKey, const char*);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_rn2483FactoryReset);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_getRn2483Hweui, char*);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_setReceiveDelay, uint16_t);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_saveMac);
FAKE_VALUE_FUNC(lora_driver_returnCode_t, lora_driver_setAdaptiveDataRate, lora_driver_adaptiveDataRate_t);
FAKE_VOID_FUNC(lora_driver_resetRn2483, uint8_t);
FAKE_VOID_FUNC(lora_driver_flushBuffers);
FAKE_VOID_FUNC(status_leds_initialise, UBaseType_t);
FAKE_VOID_FUNC(status_leds_longPuls, status_leds_t);
FAKE_VOID_FUNC(status_leds_shortPuls, status_leds_t);
FAKE_VOID_FUNC(status_leds_slowBlink, status_leds_t);
FAKE_VOID_FUNC(status_leds_fastBlink, status_leds_t);
FAKE_VOID_FUNC(status_leds_ledOn, status_leds_t);
FAKE_VOID_FUNC(status_leds_ledOff, status_leds_t);

class LoRaWANTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		RESET_FAKE(xTaskCreate);
		RESET_FAKE(xTaskGetTickCount);
		RESET_FAKE(xTaskDelayUntil);
		RESET_FAKE(vTaskDelay)/*
		RESET_FAKE(threshold_create);
		RESET_FAKE(threshold_destroy);
		RESET_FAKE(get_temperature_threshold);
		RESET_FAKE(get_humidity_threshold);
		RESET_FAKE(get_c02_threshold);
		RESET_FAKE(get_light_threshold);
		RESET_FAKE(set_temperature_threshold);
		RESET_FAKE(set_humidity_threshold);
		RESET_FAKE(set_light_threshold);
        RESET_FAKE(set_co2_threshold); */
		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};

TEST_F(LoRaWANTest, tasks_created_after_intiliase_called){
	// Arrange
	MessageBufferHandle_t downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2);
	lora_driver_initialise(ser_USART1, downLinkMessageBufferHandle);

	// Assert/Expect
	ASSERT_EQ(2, xTaskCreate_fake.call_count);
}

TEST_F(LoRaWANTest, create_is_called) 
{
	// Arrange

	// Assert/Expect
	ASSERT_EQ(1, 1);
}
