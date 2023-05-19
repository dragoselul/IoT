#include "gtest/gtest.h"
#include "./FreeRTOS_defs/fff.h"
#include "./FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
    #include "../src/Headers/LoRaWAN.h"
	#include "../src/drivers/lora_driver.h"
	#include "../src/Headers/Threshold.h"
}

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
		RESET_FAKE(vTaskDelay);
		RESET_FAKE(lora_driver_getMaxPayloadSize);
		RESET_FAKE(lora_driver_initialise);
		RESET_FAKE(lora_driver_setOtaaIdentity);
		RESET_FAKE(lora_driver_configureToEu868);
		RESET_FAKE(lora_driver_mapReturnCodeToText);
		RESET_FAKE(lora_driver_setAbpIdentity);
		RESET_FAKE(lora_driver_join);
		RESET_FAKE(lora_driver_sendUploadMessage);
		RESET_FAKE(lora_driver_setDeviceIdentifier);
		RESET_FAKE(lora_driver_setApplicationIdentifier);
		RESET_FAKE(lora_driver_setApplicationKey);
		RESET_FAKE(lora_driver_setNetworkSessionKey);
		RESET_FAKE(lora_driver_setApplicationSessionKey);
		RESET_FAKE(lora_driver_rn2483FactoryReset);
		RESET_FAKE(lora_driver_getRn2483Hweui);
		RESET_FAKE(lora_driver_setReceiveDelay);
		RESET_FAKE(lora_driver_saveMac);
		RESET_FAKE(lora_driver_setAdaptiveDataRate);
		RESET_FAKE(lora_driver_resetRn2483);
		RESET_FAKE(lora_driver_flushBuffers);
		RESET_FAKE(status_leds_initialise);
		RESET_FAKE(status_leds_longPuls);
		RESET_FAKE(status_leds_shortPuls);
		RESET_FAKE(status_leds_slowBlink);
		RESET_FAKE(status_leds_fastBlink);
		RESET_FAKE(status_leds_ledOn);
		RESET_FAKE(status_leds_ledOff);
		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};

TEST_F(LoRaWANTest, tasks_created_after_intiliase_called){
	// Arrange
	MessageBufferHandle_t downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2);
	threshold_t thresh = threshold_create();
	lora_handler_initialise(3, &thresh , &downLinkMessageBufferHandle);

	// Assert/Expect
	ASSERT_EQ(2, xTaskCreate_fake.call_count);
}

TEST_F(LoRaWANTest, create_is_called) 
{
	// Arrange

	// Assert/Expect
	ASSERT_EQ(1, 1);
}

TEST_F(LoRaWANTest, lora_setup_called) 
{
	// Arrange
	lora_driver_join_fake.return_val = LORA_ACCEPTED;
	_lora_setup();
	// Assert/Expect
	ASSERT_EQ(lora_driver_rn2483FactoryReset_fake.call_count, 1);
	ASSERT_EQ(lora_driver_mapReturnCodeToText_fake.call_count,9);
	ASSERT_EQ(lora_driver_configureToEu868_fake.call_count, 1);
	ASSERT_EQ(lora_driver_getRn2483Hweui_fake.call_count, 1);
	ASSERT_EQ(lora_driver_setDeviceIdentifier_fake.call_count, 1);
	ASSERT_EQ(lora_driver_setOtaaIdentity_fake.call_count, 1);
	ASSERT_EQ(lora_driver_saveMac_fake.call_count, 1);
	ASSERT_EQ(lora_driver_setAdaptiveDataRate_fake.call_count, 1);
	ASSERT_EQ(lora_driver_setReceiveDelay_fake.call_count, 1);
	ASSERT_EQ(lora_driver_join_fake.call_count, 1);
	ASSERT_EQ(status_leds_slowBlink_fake.call_count, 1);
	ASSERT_EQ(status_leds_ledOn_fake.call_count, 1);
}