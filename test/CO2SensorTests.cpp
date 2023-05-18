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
}

FAKE_VOID_FUNC(mh_z19_initialise, serial_comPort_t);
FAKE_VALUE_FUNC(mh_z19_returnCode_t, mh_z19_getCo2Ppm, uint16_t*);
FAKE_VALUE_FUNC(mh_z19_returnCode_t, mh_z19_takeMeassuring);
// FAKE_VALUE_FUNC(threshold_t, threshold_create);
// FAKE_VOID_FUNC(threshold_destroy, threshold_t);
// FAKE_VOID_FUNC(set_co2_threshold, threshold_t*, uint16_t);
// FAKE_VALUE_FUNC(uint16_t, get_c02_threshold, threshold_t*);
// FAKE_VOID_FUNC(alarm_turn_on);
// FAKE_VOID_FUNC(alarm_turn_off);
FAKE_VOID_FUNC(rc_servo, uint16_t);

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

            // RESET CO2 Sensor Driver Functions
            RESET_FAKE(mh_z19_initialise);
            RESET_FAKE(mh_z19_getCo2Ppm);
            RESET_FAKE(mh_z19_takeMeassuring);

            FFF_RESET_HISTORY();
        }
        void TearDown() override
        {}
};


TEST_F(Co2Test, create_is_called)
{
    // Arange
    threshold_t thresh = threshold_create();
    co2_t co2 = co2_create(&thresh);

    // Assert/Expect
    ASSERT_EQ(1, mh_z19_initialise_fake.call_count);
   // ASSERT_EQ(1, threshold_create_fake.call_count);
    ASSERT_TRUE(co2 != NULL);
  //  ASSERT_TRUE(threshold != NULL);
}
 
TEST_F(Co2Test, struct_has_default_values_after_create)
{
    // Arange
    threshold_t threshold = threshold_create();
    co2_t co2 = co2_create(&threshold);

    // Assert/Expect
    ASSERT_TRUE(co2 != NULL);
   // ASSERT_TRUE(co2->val == 0);
   // ASSERT_TRUE(co2->avg_co2 == 0.0);
   // ASSERT_TRUE(co2->measurements == 0);
   // ASSERT_TRUE(co2->th_point == &threshold);
}

/*
TEST_F(Co2Test, destroy_is_called)
{
    // Arrange
    threshold_t threshold = threshold_create();
    co2_t co2 = co2_create(&threshold);
    co2_destroy(co2);

    // Assert/Expect
    ASSERT_EQ(NULL, &co2);
}
*/

TEST_F(Co2Test, get_data_after_measure)
{
    // Arrange
    threshold_t threshold = threshold_create();
    co2_t co2 = co2_create(&threshold);

  //  uint16_t old_val = co2->val;
    
    
    // Assert/Expect
    ASSERT_EQ(1, mh_z19_takeMeassuring_fake.call_count);

    // Check if value is changed
  //  ASSERT_NE(old_val, co2->val);
}