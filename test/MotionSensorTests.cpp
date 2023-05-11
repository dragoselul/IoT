#include "gtest/gtest.h"
#include "./FreeRTOS_defs/fff.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
    #include "../src/Headers/MotionSensor.h"
    #include "../src/drivers/hcSr501.h"
}
 

FAKE_VALUE_FUNC(bool, hcsr501_isDetecting);
FAKE_VALUE_FUNC(hcsr501_p, hcsr501_create());
FAKE_VALUE_FUNC(hcsr501_p, hcsr501_destroy());


class MotionTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};

TEST_F(MotionTest test_motion_create_is_called){
    // Arrange
	// Act
    motion_create();

    // Assert/Expect
    ASSERT_EQ(1, hcsr501_create_fake.call_count);
}
TEST_F(MotionTest Test_motion_destroy_is_called){
    // Arrange
	// Act
    motion_t motionSen = motion_create();
    motion_destroy(motionSen);

    ASSERT_EQ(1, hcsr501_destroy_fake.call_count);
}