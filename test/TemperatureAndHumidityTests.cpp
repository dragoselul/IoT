#include "gtest/gtest.h"
#include "./FreeRTOS_defs/fff.h"
#include "./FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
    #include "../src/Headers/TempAndHum.h"
	#include "../src/drivers/hih8120.h"
	#include "../src/Headers/Average.h"
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
FAKE_VALUE_FUNC(average_t, average_create);
FAKE_VOID_FUNC(average_destroy, average_t);
FAKE_VOID_FUNC(calculate_average, uint16_t, average_t);
FAKE_VALUE_FUNC(uint16_t, get_average, average_t);

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
		RESET_FAKE(average_create);
		RESET_FAKE(average_destroy);
		RESET_FAKE(calculate_average);
		RESET_FAKE(get_average);
		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};

TEST_F(TempHumTest, Test_tempAndHum_create_is_called) 
{
	// Arrange
	 tempAndHum_t tempHum = tempAndHum_create();

	// Assert/Expect
	ASSERT_EQ(1, hih8120_initialise_fake.call_count);
	ASSERT_EQ(2, average_create_fake.call_count);
	ASSERT_TRUE((tempHum!=NULL));
}

TEST_F(TempHumTest, Test_tempAndHum_destroy_is_called_with_NULL_arg) 
{
	// Arrange
	tempAndHum_destroy(NULL);

	// Assert/Expect
	ASSERT_EQ(1, hih8120_destroy_fake.call_count);
}

TEST_F(TempHumTest, Test_tempAndHum_destroy_is_called_with_arg) 
{
	// Arrange
	tempAndHum_destroy(tempAndHum_create());

	// Assert/Expect
	ASSERT_EQ(1, hih8120_destroy_fake.call_count);
}

TEST_F(TempHumTest, Test_tempAndHum_update_averages_no_args) 
{
	// Arrange
	update_averages(NULL);
	// Assert/Expect
	ASSERT_EQ(2, calculate_average_fake.call_count);
}

TEST_F(TempHumTest, Test_tempAndHum_update_averages_args) 
{
	// Arrange
	tempAndHum_t tempAndHum = tempAndHum_create();
	update_averages(tempAndHum);
	// Assert/Expect
	ASSERT_EQ(2, calculate_average_fake.call_count);
}

TEST_F(TempHumTest, Test_tempAndHum_reset_averages_no_args) 
{
	// Arrange
	reset_averages(NULL);
	// Assert/Expect
	ASSERT_EQ(2, average_destroy_fake.call_count);
	ASSERT_EQ(2, average_create_fake.call_count);
}

TEST_F(TempHumTest, Test_tempAndHum_reset_averages_args) 
{
	// Arrange
	tempAndHum_t tempAndHum = tempAndHum_create();
	reset_averages(tempAndHum);
	// Assert/Expect
	ASSERT_EQ(2, average_destroy_fake.call_count);
	ASSERT_EQ(2, average_create_fake.call_count);
}

TEST_F(TempHumTest, Test_tempAndHum_measure_temp_hum_no_args) 
{
	// Arrange
	ASSERT_FALSE(measure_temp_hum(NULL));
	// Assert/Expect
}

TEST_F(TempHumTest, Test_tempAndHum_measure_temp_hum_args) 
{
	// Arrange
	tempAndHum_t tempAndHum = tempAndHum_create();
	// Assert/Expect
	ASSERT_TRUE(measure_temp_hum(tempAndHum));
	ASSERT_EQ(1, hih8120_wakeup_fake.call_count);
	ASSERT_EQ(2, vTaskDelay_fake.call_count);
	ASSERT_EQ(1, hih8120_measure_fake.call_count);
	ASSERT_EQ(1, hih8120_getHumidityPercent_x10_fake.call_count);
	ASSERT_EQ(1, hih8120_getTemperature_x10_fake.call_count);
}