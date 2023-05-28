#include "gtest/gtest.h"
#include "./FreeRTOS_defs/fff.h"
#include "./FreeRTOS_defs/FreeRTOS_FFF_MocksDeclaration.h"

extern "C" 
{
    #include "../src/Headers/CO2.h"
    #include "../src/drivers/mh_z19.h"
    #include "../src/Headers/Threshold.h"
    #include "../src/Headers/Servo.h"
    #include "../src/Headers/Alarm.h"
    #include "../src/Headers/ExternBooleans.h"
}

FAKE_VOID_FUNC(mh_z19_initialise, serial_comPort_t);
FAKE_VALUE_FUNC(mh_z19_returnCode_t, mh_z19_getCo2Ppm, uint16_t*);
FAKE_VALUE_FUNC(mh_z19_returnCode_t, mh_z19_takeMeassuring);
FAKE_VOID_FUNC(alarm_turn_on);
FAKE_VOID_FUNC(alarm_turn_off);
FAKE_VOID_FUNC(servo_open);
FAKE_VOID_FUNC(servo_close);

// FAKE_VOID_FUNC(set_temperature_threshold, threshold_t*, int16_t);
// FAKE_VOID_FUNC(set_humidity_threshold, threshold_t*, uint16_t);
// FAKE_VOID_FUNC(set_light_threshold, threshold_t*, uint16_t);
// FAKE_VALUE_FUNC(uint16_t, get_light_threshold, threshold_t*);
// FAKE_VALUE_FUNC(int16_t, get_temperature_threshold, threshold_t*);
// FAKE_VALUE_FUNC(uint16_t, get_humidity_threshold, threshold_t*);

class Co2Test: public ::testing::Test
{
    protected:
        void SetUp() override
        {
            // RESET Task Functions
            RESET_FAKE(xTaskCreate);
            RESET_FAKE(xTaskGetTickCount);
            RESET_FAKE(vTaskDelayUntil);
            RESET_FAKE(vTaskDelay);

            // RESET CO2 SENSOR DRIVER FAKES
            RESET_FAKE(mh_z19_initialise);
            RESET_FAKE(mh_z19_getCo2Ppm);
            RESET_FAKE(mh_z19_takeMeassuring);

            // RESET ALARM FAKES
            RESET_FAKE(alarm_turn_on);
            RESET_FAKE(alarm_turn_off);

            // RESET SERVO FAKES
            RESET_FAKE(servo_open);
            RESET_FAKE(servo_close);
            FFF_RESET_HISTORY();
        }
        void TearDown() override
        {}
};

// TEST IF CO2_CREATE IS PROPERLY CALLED AND INITIALIZES THE SENSOR
TEST_F(Co2Test, create_is_called)
{
  // Arange
  threshold_t thresh = threshold_create();
  co2_t co2 = co2_create(&thresh);

  // Assert/Expect
  ASSERT_EQ(1, mh_z19_initialise_fake.call_count);
  ASSERT_TRUE(co2 != NULL);
}

// TEST IF CO2_CREATE INITIALIZED SENSOR STRUCT PROPERLY
TEST_F(Co2Test, struct_has_default_values_after_create)
{
  // Arange
  threshold_t threshold = threshold_create();
  co2_t co2 = co2_create(&threshold);

  // Assert/Expect
  ASSERT_TRUE(co2 != NULL);
  ASSERT_TRUE(co2->val == 0);
  ASSERT_TRUE(co2->avg_co2 == 0.0);
  ASSERT_TRUE(co2->measurements == 0);
  ASSERT_TRUE(co2->th_point == &threshold);
}

// TEST THAT GET_DATA RETURNS TRUE IF THERE IS A MEASURING AVAILABLE
TEST_F(Co2Test, get_data_after_successful_measure)
{
  // Arrange
  mh_z19_getCo2Ppm_fake.return_val = MHZ19_OK;
  threshold_t threshold = threshold_create();
  co2_t co2 = co2_create(&threshold);

  bool measured = co2_get_data(co2);

  // Assert/Expect
  ASSERT_TRUE(measured);
  ASSERT_EQ(1, mh_z19_getCo2Ppm_fake.call_count);
}

// TEST THAT GET_DATA RETURNS FALSE IF THERE IS NO MEASURING AVAILABLE
TEST_F(Co2Test, get_data_after_unsuccessful_measure)
{
  // Arrange
  mh_z19_getCo2Ppm_fake.return_val = MHZ19_NO_MEASSURING_AVAILABLE;
  threshold_t threshold = threshold_create();
  co2_t co2 = co2_create(&threshold);

  bool measured = co2_get_data(co2);

  // Assert/Expect
  ASSERT_FALSE(measured);
  ASSERT_EQ(1, mh_z19_takeMeassuring_fake.call_count);
  ASSERT_EQ(1, mh_z19_getCo2Ppm_fake.call_count);
}

// TEST IF TAKE MEASURE AND GET CO2 PPM FUNCTIONS ARE CALLED
TEST_F(Co2Test, get_value_returns_correct_value)
{

  // Arrange
  threshold_t threshold = threshold_create();
  co2_t co2 = co2_create(&threshold);
  co2->val = 420;
  
  uint16_t val = co2_get_value(co2);

  // Assert/Expect
  ASSERT_EQ(420, val);

}

// TEST IF ALARM IS TURNED ON WHEN CO2 SURPASSES THRESHOLD
TEST_F(Co2Test, alarm_turned_on_after_threshold_surpassed){
  // Arrange
  threshold_t threshold = threshold_create();
  co2_t co2 = co2_create(&threshold);
  set_co2_threshold(&threshold, 800);
  co2->val = 900;

  co2_evaluate_threshold(co2);

  // Assert/Expect
  ASSERT_EQ(1, alarm_turn_on_fake.call_count);
}

// TEST IF ALARM IS TURNED OFF WHEN CO2 IS BELOW THRESHOLD
TEST_F(Co2Test, alarm_turned_off_after_threshold_not_surpassed){
  // Arrange
  threshold_t threshold = threshold_create();
  co2_t co2 = co2_create(&threshold);
  set_co2_threshold(&threshold, 800);
  co2->val = 700;

  co2_evaluate_threshold(co2);

  // Assert/Expect
  ASSERT_EQ(1, alarm_turn_off_fake.call_count);
}

// TEST IF DOOR IS OPENED WHEN CO2 SURPASSES THRESHOLD
TEST_F(Co2Test, door_opened_after_threshold_surpassed){
  // Arrange
  threshold_t threshold = threshold_create();
  co2_t co2 = co2_create(&threshold);
  set_co2_threshold(&threshold, 800);
  co2->val = 900;

  co2_evaluate_threshold(co2);

  // Assert/Expect
  ASSERT_EQ(1, servo_open_fake.call_count);
}

// TEST IF DOOR IS OPENED WHEN CO2 SURPASSES THRESHOLD
TEST_F(Co2Test, door_closed_after_threshold_not_surpassed){
  // Arrange
  threshold_t threshold = threshold_create();
  co2_t co2 = co2_create(&threshold);
  set_co2_threshold(&threshold, 800);
  co2->val = 700;

  co2_evaluate_threshold(co2);

  // Assert/Expect
  ASSERT_EQ(1, servo_close_fake.call_count);
}

TEST_F(Co2Test, get_average_returns_average)
{
    // Arrange
    threshold_t threshold = threshold_create();
    co2_t co2 = co2_create(&threshold);
    co2->avg_co2 = 100;

    // Assert/Expect
    ASSERT_EQ(100, co2_get_average(co2));
}

// TEST IF THE AVERAGE IS CORRECTLY CALCULATED AFTER A MEASURE
TEST_F(Co2Test, correct_average_after_1_measure)
{

  threshold_t threshold = threshold_create();
  co2_t co2 = co2_create(&threshold);

  co2->val = 100;
  co2_update_average(co2);

  ASSERT_EQ(100, co2_get_average(co2));

}

// TEST IF THE AVERAGE IS CORRECTLY CALCULATED AFTER MULTIPLE MEASURES
TEST_F(Co2Test, correct_average_after_multiple_measures)
{

  threshold_t threshold = threshold_create();
  co2_t co2 = co2_create(&threshold);

  co2->val = 100;
  co2_update_average(co2);

  // 100 / 1 = 100
  ASSERT_EQ(100, co2_get_average(co2));

  co2->val = 200;
  co2_update_average(co2);

  // (100 + 200) / 2 = 150
  ASSERT_EQ(150, co2_get_average(co2));

  co2->val = 300;
  co2_update_average(co2);

  // (100 + 200 + 300) / 3 = 200
  ASSERT_EQ(200, co2_get_average(co2));

  co2->val = 400;
  co2_update_average(co2);

  // (100 + 200 + 300 + 400) / 4 = 250
  ASSERT_EQ(250, co2_get_average(co2));

  co2->val = 500;
  co2_update_average(co2);

  // (100 + 200 + 300 + 400 + 500) / 5 = 300
  ASSERT_EQ(300, co2_get_average(co2));

}

// TESTS IF THE AVERAGE AND MEASURMENTS ARE RESET AFTER RESET IS CALLED
TEST_F(Co2Test, reset_average_defaults_values)
{

  threshold_t threshold = threshold_create();
  co2_t co2 = co2_create(&threshold);

  co2->val = 100;
  co2_update_average(co2);
  ASSERT_EQ(100, co2_get_average(co2));

  co2_reset_average(co2);
  ASSERT_EQ(0.0, co2->avg_co2);
  ASSERT_EQ(0, co2->measurements);

}

// TESTS IF THE TASK FOR CO2 MEASURING IS SUCCESSFULLY CREATED
TEST_F(Co2Test, create_co2_task) 
{
	// Arrange
	threshold_t thresh = threshold_create();
	co2_t co2 = co2_create(&thresh);
	create_co2_task(&co2);
	// Assert/Expect
	ASSERT_EQ(1,xTaskCreate_fake.call_count);
	ASSERT_EQ(xTaskCreate_fake.arg0_val, &co2_task);
	ASSERT_EQ(strncmp(xTaskCreate_fake.arg1_val, "CO2 Task", 9), 0);
	ASSERT_EQ(xTaskCreate_fake.arg2_val, configMINIMAL_STACK_SIZE);
	ASSERT_EQ(xTaskCreate_fake.arg3_val, &co2);
	ASSERT_EQ(xTaskCreate_fake.arg4_val, 1);
	ASSERT_EQ(xTaskCreate_fake.arg5_val, nullptr);
}