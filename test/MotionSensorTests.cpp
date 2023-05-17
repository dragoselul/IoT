#include "gtest/gtest.h"
#include "./FreeRTOS_defs/fff.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"


extern "C"
{
    #include "../src/drivers/hcSr501.h"
    #include "../src/Headers/hal.h"
}

FAKE_VALUE_FUNC(bool, hcsr501_isDetecting, hcsr501_p);
FAKE_VALUE_FUNC(hcsr501_p, hcsr501_create, volatile uint8_t* , uint8_t);
FAKE_VOID_FUNC(hcsr501_destroy,hcsr501_p);
FAKE_VALUE_FUNC(motion_t, motion_create);
FAKE_VOID_FUNC(motion_destroy, motion_t);


class MotionTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
        RESET_FAKE(hcsr501_create);
        RESET_FAKE(motion_create);
        RESET_FAKE(motion_destroy);
		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};

TEST_F(MotionTest, test_motion_create_is_called){
    // Arrange
	// Act
    motion_create();

    // Assert/Expect
    ASSERT_EQ(1, hcsr501_create_fake.call_count);
}
TEST_F(MotionTest, Test_motion_destroy_is_called){
    // Arrange
	// Act
    motion_t motionSen = motion_create();
    motion_destroy(motionSen);

    ASSERT_EQ(1, hcsr501_destroy_fake.call_count);
}