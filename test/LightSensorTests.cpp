#include "gtest/gtest.h"
#include "./FreeRTOS_defs/fff.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"


extern "C"
{
    #include "../src/Headers/Light.h"
    #include "../src/drivers/tsl2591.h"
    #include "../src/Headers/Threshold.h"
}
typedef void (*callback)(tsl2591_returnCode_t rc);

FAKE_VALUE_FUNC(tsl2591_returnCode_t, tsl2591_initialise, callback);
FAKE_VALUE_FUNC(tsl2591_returnCode_t, tsl2591_destroy);
FAKE_VALUE_FUNC(tsl2591_returnCode_t, tsl2591_enable);
FAKE_VALUE_FUNC(tsl2591_returnCode_t, tsl2591_disable);
FAKE_VALUE_FUNC(tsl2591_returnCode_t, tsl2591_fetchDeviceId);
FAKE_VALUE_FUNC(uint8_t, tsl2591_getDeviceId);
FAKE_VALUE_FUNC(tsl2591_returnCode_t, tsl2591_setGainAndIntegrationTime, tsl2591_gain_t, tsl2591_integrationTime_t);
FAKE_VALUE_FUNC(tsl2591_gain_t, tsl2591_getGain);
FAKE_VALUE_FUNC(tsl2591_integrationTime_t, tsl2591_getIntegrationTime);
FAKE_VALUE_FUNC(tsl2591_returnCode_t, tsl2591_fetchData);
FAKE_VALUE_FUNC(tsl2591_returnCode_t, tsl259_getVisibleRaw, uint16_t* );
FAKE_VALUE_FUNC(tsl2591_returnCode_t, tsl2591_getInfraredRaw, uint16_t*);
FAKE_VALUE_FUNC(tsl2591_returnCode_t, tsl2591_getFullSpectrumRaw, uint16_t*);
FAKE_VALUE_FUNC(tsl2591_returnCode_t, tsl2591_getCombinedDataRaw, tsl2591_combinedData_t*);
FAKE_VALUE_FUNC(tsl2591_returnCode_t, tsl2591_getLux, float*);
FAKE_VOID_FUNC(LED_turn_on);
FAKE_VOID_FUNC(LED_turn_off);


class LightTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
        RESET_FAKE(xTaskCreate);
		RESET_FAKE(xTaskGetTickCount);
		RESET_FAKE(xTaskDelayUntil);
		RESET_FAKE(vTaskDelay);
        RESET_FAKE(tsl2591_initialise);
        RESET_FAKE(tsl2591_destroy);
        RESET_FAKE(tsl2591_enable);
        RESET_FAKE(tsl2591_disable);
        RESET_FAKE(tsl2591_fetchDeviceId);
        RESET_FAKE(tsl2591_getDeviceId);
        RESET_FAKE(tsl2591_setGainAndIntegrationTime);
        RESET_FAKE(tsl2591_getIntegrationTime);
        RESET_FAKE(tsl2591_getGain);
        RESET_FAKE(tsl2591_fetchData);
        RESET_FAKE(tsl259_getVisibleRaw);
        RESET_FAKE(tsl2591_getInfraredRaw);
        RESET_FAKE(tsl2591_getFullSpectrumRaw);
        RESET_FAKE(tsl2591_getCombinedDataRaw);
        RESET_FAKE(tsl2591_getLux);
		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};

TEST_F(LightTest, create_is_called_with_no_arg){
    // Arrange
	// Act
    light_t light = light_create(NULL);

    // Assert/Expect
    ASSERT_EQ(0, tsl2591_initialise_fake.call_count);
    ASSERT_TRUE(light==NULL);
}

TEST_F(LightTest, create_is_called_with_arg){
    // Arrange
    threshold_t thresh = threshold_create();
    // Act
    light_t light = light_create(&thresh);
    // Assert/Expect
    ASSERT_EQ(1, tsl2591_initialise_fake.call_count);
    ASSERT_TRUE(light!=NULL);
    ASSERT_TRUE(thresh!=NULL);
}

TEST_F(LightTest, destroy_is_called_with_no_arg){
    // Arrange
	// Act
    light_destroy(NULL);
    // Assert/Expect
    ASSERT_EQ(0, tsl2591_destroy_fake.call_count);
}

TEST_F(LightTest, destroy_is_called_with_arg){
    // Arrange
    threshold_t thresh = threshold_create();
    light_t light = light_create(&thresh);
    // Act
    light_destroy(&light);
    // Assert/Expect
    ASSERT_EQ(1, tsl2591_destroy_fake.call_count);
}

TEST_F(LightTest, get_light_data_no_args){
    // Assert/Expect
    bool get_light_data_succesfully = get_light_data(NULL);
    ASSERT_FALSE(get_light_data_succesfully);
}

tsl2591_returnCode_t getVisibleRawFake(uint16_t* value) {
    *value = 150;
    return TSL2591_OK;
}

tsl2591_returnCode_t getLuxFake(float* value) {
    // Simulate the behavior of getting lux data
    *value = 250.0;
    return TSL2591_OK;
}

TEST_F(LightTest, get_light_data_with_args_data){
    // Arrange
    threshold_t thresh = threshold_create();
    light_t light = light_create(&thresh);
    
    // Set the behavior of the fake functions
    tsl259_getVisibleRaw_fake.return_val = TSL2591_OK;
    tsl259_getVisibleRaw_fake.custom_fake = getVisibleRawFake;  

    tsl2591_getLux_fake.return_val = TSL2591_OK;
    tsl2591_getLux_fake.custom_fake = getLuxFake;

    tsl2591_enable_fake.return_val = TSL2591_OK;
    tsl2591_fetchData_fake.return_val = TSL2591_OK;
    tsl2591_disable_fake.return_val = TSL2591_OK;
    
    tsl2591Callback(TSL2591_DATA_READY);
    bool get_light_data_succesfully = get_light_data(light);
    // Verify the behavior of the fake functions
    ASSERT_EQ(tsl259_getVisibleRaw_fake.call_count, 1);
    ASSERT_EQ(tsl2591_getLux_fake.call_count, 1);
    ASSERT_EQ(get_tmp(light), 150);
    ASSERT_EQ(get_lux(light), 2500);
    // Assert/Expect
    ASSERT_EQ(1, tsl2591_enable_fake.call_count);
    ASSERT_EQ(1, tsl2591_fetchData_fake.call_count);
    ASSERT_EQ(1, tsl2591_disable_fake.call_count);
    ASSERT_TRUE(get_light_data_succesfully);
}

TEST_F(LightTest, get_tmp_no_args){
    // Assert/Expect
    ASSERT_EQ(get_tmp(NULL), 0);
}
TEST_F(LightTest, get_tmp_with_args_no_data){
    // Arrange
    threshold_t thresh = threshold_create();
    light_t light = light_create(&thresh);
    // Assert/Expect
    ASSERT_EQ(get_tmp(light), 0);
}

TEST_F(LightTest, get_tmp_with_args_with_data){
    // Arrange
    threshold_t thresh = threshold_create();
    light_t light = light_create(&thresh);
    
    // Set the behavior of the fake functions
    tsl259_getVisibleRaw_fake.return_val = TSL2591_OK;
    tsl259_getVisibleRaw_fake.custom_fake = getVisibleRawFake;  

    tsl2591_getLux_fake.return_val = TSL2591_OK;
    tsl2591_getLux_fake.custom_fake = getLuxFake;

    tsl2591_enable_fake.return_val = TSL2591_OK;
    tsl2591_fetchData_fake.return_val = TSL2591_OK;
    tsl2591_disable_fake.return_val = TSL2591_OK;
    
    tsl2591Callback(TSL2591_DATA_READY);
    bool get_light_data_succesfully = get_light_data(light);
    // Verify the behavior of the fake functions
    ASSERT_EQ(tsl259_getVisibleRaw_fake.call_count, 1);
    ASSERT_EQ(tsl2591_getLux_fake.call_count, 1);
    ASSERT_EQ(get_tmp(light), 150);
}

TEST_F(LightTest, get_lux_no_args){
    // Assert/Expect
    ASSERT_EQ(get_lux(NULL), 0);
}

TEST_F(LightTest, get_lux_with_args_no_data){
    // Arrange
    threshold_t thresh = threshold_create();
    light_t light = light_create(&thresh);
    // Assert/Expect
    ASSERT_EQ(get_lux(light), 0);
}

TEST_F(LightTest, get_lux_with_args_with_data){
    // Arrange
    threshold_t thresh = threshold_create();
    light_t light = light_create(&thresh);
    
    // Set the behavior of the fake functions
    tsl259_getVisibleRaw_fake.return_val = TSL2591_OK;
    tsl259_getVisibleRaw_fake.custom_fake = getVisibleRawFake;  

    tsl2591_getLux_fake.return_val = TSL2591_OK;
    tsl2591_getLux_fake.custom_fake = getLuxFake;

    tsl2591_enable_fake.return_val = TSL2591_OK;
    tsl2591_fetchData_fake.return_val = TSL2591_OK;
    tsl2591_disable_fake.return_val = TSL2591_OK;
    
    tsl2591Callback(TSL2591_DATA_READY);
    bool get_light_data_succesfully = get_light_data(light);
    // Verify the behavior of the fake functions
    ASSERT_EQ(tsl259_getVisibleRaw_fake.call_count, 1);
    ASSERT_EQ(tsl2591_getLux_fake.call_count, 1);
    ASSERT_EQ(get_lux(light), 2500);
}

TEST_F(LightTest, create_light_task_no_args) 
{
	// Arrange
	create_light_task(NULL);
	// Assert/Expect
	ASSERT_EQ(0,xTaskCreate_fake.call_count);
}

TEST_F(LightTest, create_light_task_args) 
{
	// Arrange
	threshold_t thresh = threshold_create();
	light_t light = light_create(&thresh);
	create_light_task(&light);
	// Assert/Expect
	ASSERT_EQ(1,xTaskCreate_fake.call_count);
	ASSERT_EQ(xTaskCreate_fake.arg0_val, &light_task);
	ASSERT_EQ(strncmp(xTaskCreate_fake.arg1_val, "Light Task", 11), 0);
	ASSERT_EQ(xTaskCreate_fake.arg2_val, configMINIMAL_STACK_SIZE);
	ASSERT_EQ(xTaskCreate_fake.arg3_val, &light);
	ASSERT_EQ(xTaskCreate_fake.arg4_val, 1);
	ASSERT_EQ(xTaskCreate_fake.arg5_val, nullptr);
}