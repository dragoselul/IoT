#include "gtest/gtest.h"
#include "./FreeRTOS_defs/fff.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"

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
/*
FAKE_VALUE_FUNC(tempAndHum_t, tempAndHum_create);
FAKE_VOID_FUNC(tempAndHum_destroy, tempAndHum_t);
FAKE_VOID_FUNC(reset_averages,tempAndHum_t);
FAKE_VOID_FUNC(update_averages, tempAndHum_t);
FAKE_VALUE_FUNC(bool, measure_temp_hum, tempAndHum_t);
FAKE_VALUE_FUNC(float, get_humidity_float);
FAKE_VALUE_FUNC(float, get_temperature_float);
FAKE_VALUE_FUNC(uint16_t, get_humidity_int,tempAndHum_t);
FAKE_VALUE_FUNC(uint16_t, get_temperature_int, tempAndHum_t);
FAKE_VALUE_FUNC(uint16_t, get_average_hum, tempAndHum_t);
FAKE_VALUE_FUNC(uint16_t, get_average_temp, tempAndHum_t);
*/

class TempHumTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		RESET_FAKE(hih8120_initialise);
		RESET_FAKE(average_create);
		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};

TEST_F(TempHumTest, Test_hal_create_is_called) {
	// Arrange
	// Act
	tempAndHum_create();

	// Assert/Expect
	ASSERT_EQ(1, hih8120_initialise_fake.call_count);
}

TEST_F(TempHumTest, Test_tempAndHum_destroy_is_called) {
	// Arrange
	// Act
	tempAndHum_t tempAndHum = tempAndHum_create();
	tempAndHum_destroy(tempAndHum);

	// Assert/Expect
	ASSERT_EQ(1, hih8120_destroy_fake.call_count);
}