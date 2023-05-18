#include "gtest/gtest.h"
#include "./FreeRTOS_defs/fff.h"
#include "./FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
    #include "../src/Headers/TempAndHum.h"
	#include "../src/drivers/hih8120.h"
	#include "../src/Headers/Threshold.h"
}

FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_initialise);
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_destroy);
FAKE_VALUE_FUNC(bool, hih8120_isReady);	
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_wakeup);
FAKE_VALUE_FUNC(hih8120_driverReturnCode_t, hih8120_measure);
FAKE_VALUE_FUNC(uint16_t, hih8120_getHumidityPercent_x10);
FAKE_VALUE_FUNC(int16_t, hih8120_getTemperature_x10);
FAKE_VALUE_FUNC(float, hih8120_getHumidity);
FAKE_VALUE_FUNC(float, hih8120_getTemperature);
// FAKE_VALUE_FUNC(threshold_t, threshold_create);
// FAKE_VOID_FUNC(threshold_destroy, threshold_t);
// FAKE_VALUE_FUNC(int16_t, get_temperature_threshold, threshold_t*);
// FAKE_VALUE_FUNC(uint16_t, get_humidity_threshold, threshold_t*);
// FAKE_VALUE_FUNC(uint16_t, get_c02_threshold, threshold_t*);
// FAKE_VALUE_FUNC(uint16_t, get_light_threshold, threshold_t*);
// FAKE_VOID_FUNC(set_temperature_threshold, threshold_t*, int16_t);
// FAKE_VOID_FUNC(set_humidity_threshold, threshold_t*, uint16_t);
// FAKE_VOID_FUNC(set_light_threshold, threshold_t*, uint16_t);
// FAKE_VOID_FUNC(set_co2_threshold, threshold_t*, uint16_t);
FAKE_VOID_FUNC(alarm_turn_on);
FAKE_VOID_FUNC(alarm_turn_off);

class TempHumTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		RESET_FAKE(xTaskCreate);
		RESET_FAKE(xTaskGetTickCount);
		RESET_FAKE(xTaskDelayUntil);
		RESET_FAKE(vTaskDelay)
		RESET_FAKE(hih8120_initialise);
		RESET_FAKE(hih8120_destroy);
		RESET_FAKE(hih8120_isReady);
		RESET_FAKE(hih8120_wakeup);
		RESET_FAKE(hih8120_measure);
		RESET_FAKE(hih8120_getHumidityPercent_x10);
		RESET_FAKE(hih8120_getTemperature_x10);
		RESET_FAKE(hih8120_getHumidity);
		RESET_FAKE(hih8120_getTemperature);
		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};

TEST_F(TempHumTest, create_is_called_with_no_arg) 
{
	// Arrange
	tempAndHum_t tempHum = tempAndHum_create(NULL);
	// Assert/Expect
	ASSERT_EQ(0, hih8120_initialise_fake.call_count);
	ASSERT_TRUE(tempHum==NULL);
}

/*
TEST_F(TempHumTest, create_is_called_with_arg) 
{
	// Arrange
	threshold_t thresh = threshold_create_fake.return_val;
	tempAndHum_t tempHum = tempAndHum_create(&thresh);
	// Assert/Expect
	ASSERT_EQ(1, hih8120_initialise_fake.call_count);
	//ASSERT_EQ(1, threshold_create_fake.call_count);
	ASSERT_TRUE((tempHum!=NULL));
	ASSERT_TRUE((thresh!=NULL));
}*/

TEST_F(TempHumTest, destroy_is_called_with_no_arg) 
{
	// Arrange
	tempAndHum_destroy(NULL);
	// Assert/Expect
	ASSERT_EQ(1, hih8120_destroy_fake.call_count);
}

TEST_F(TempHumTest, destroy_is_called_with_arg) 
{
	// Arrange
	threshold_t thresh = threshold_create();
	tempAndHum_t tempHum = tempAndHum_create(&thresh);
	tempAndHum_destroy(&tempHum);

	// Assert/Expect
	ASSERT_EQ(1, hih8120_destroy_fake.call_count);
}

TEST_F(TempHumTest, update_averages_no_args) 
{
	// Arrange
	threshold_t thresh = threshold_create();
	tempAndHum_t tempHum = tempAndHum_create(&thresh);
	update_averages(NULL);
	// Assert/Expect
	ASSERT_EQ(0, get_average_temp(tempHum));
	ASSERT_EQ(0, get_average_hum(tempHum));
}

TEST_F(TempHumTest, update_averages_args) 
{
	// Arrange
	threshold_t thresh = threshold_create();
	tempAndHum_t tempHum = tempAndHum_create(&thresh);
	ASSERT_EQ(0, get_average_temp(tempHum));
	ASSERT_EQ(0, get_average_hum(tempHum));
	update_averages(tempHum);
	// Assert/Expect
	ASSERT_EQ(0, get_average_temp(tempHum));
	ASSERT_EQ(0, get_average_hum(tempHum));
	//ASSERT_EQ(2, calculate_average_fake.call_count);
}

// TEST_F(TempHumTest, reset_averages_no_args) 
// {
// 	// Arrange
// 	reset_averages(NULL);
// 	// Assert/Expect
// 	ASSERT_EQ(0, average_destroy_fake.call_count);
// 	ASSERT_EQ(0, average_create_fake.call_count);
// }

// TEST_F(TempHumTest, reset_averages_args) 
// {
// 	// Arrange
// 	tempAndHum_t tempAndHum = tempAndHum_create();
// 	reset_averages(tempAndHum);
// 	// Assert/Expect
// 	ASSERT_EQ(2, average_destroy_fake.call_count);
// 	ASSERT_EQ(4, average_create_fake.call_count);
// }

TEST_F(TempHumTest, measure_temp_hum_no_args) 
{
	// Arrange
	ASSERT_FALSE(measure_temp_hum(NULL));
}

TEST_F(TempHumTest, measure_temp_hum_args) 
{
	// Arrange
	threshold_t thresh = threshold_create();
	tempAndHum_t tempHum = tempAndHum_create(&thresh);
	// Assert/Expect
	ASSERT_TRUE(measure_temp_hum(tempHum));
	ASSERT_EQ(1, hih8120_wakeup_fake.call_count);
	ASSERT_EQ(2, vTaskDelay_fake.call_count);
	ASSERT_EQ(1, hih8120_measure_fake.call_count);
	ASSERT_EQ(1, hih8120_getHumidityPercent_x10_fake.call_count);
	ASSERT_EQ(1, hih8120_getTemperature_x10_fake.call_count);
}


TEST_F(TempHumTest, get_average_hum_no_args) 
{
	// Assert/Expect
	ASSERT_EQ(get_average_hum(NULL), 0);
}

TEST_F(TempHumTest, get_average_hum_args_without_measurement) 
{
	// Arrange
	threshold_t thresh = threshold_create();
	tempAndHum_t tempHum = tempAndHum_create(&thresh);
	// Assert/Expect
	ASSERT_EQ(0,get_average_hum(tempHum));
}

TEST_F(TempHumTest, get_average_temp_no_args) 
{
	// Assert/Expect
	ASSERT_EQ(get_average_temp(NULL), 0);
}

TEST_F(TempHumTest, get_average_temp_args_without_measurement) 
{
	// Arrange
	threshold_t thresh = threshold_create();
	tempAndHum_t tempHum = tempAndHum_create(&thresh);
	// Assert/Expect
	ASSERT_EQ(0,get_average_temp(tempHum));
}

TEST_F(TempHumTest, create_temp_hum_task_no_args) 
{
	// Arrange
	create_temp_hum_task(NULL);
	// Assert/Expect
	ASSERT_EQ(0,xTaskCreate_fake.call_count);
}

TEST_F(TempHumTest, create_temp_hum_task_args) 
{
	// Arrange
	threshold_t thresh = threshold_create();
	tempAndHum_t tempHum = tempAndHum_create(&thresh);
	create_temp_hum_task(&tempHum);
	// Assert/Expect
	ASSERT_EQ(1,xTaskCreate_fake.call_count);
	ASSERT_EQ(xTaskCreate_fake.arg0_val, &temp_hum_task);
	ASSERT_EQ(strncmp(xTaskCreate_fake.arg1_val, "Temperature and Humidity task", 30), 0);
	ASSERT_EQ(xTaskCreate_fake.arg2_val, configMINIMAL_STACK_SIZE);
	ASSERT_EQ(xTaskCreate_fake.arg3_val, &tempHum);
	ASSERT_EQ(xTaskCreate_fake.arg4_val, 1);
	ASSERT_EQ(xTaskCreate_fake.arg5_val, nullptr);
}