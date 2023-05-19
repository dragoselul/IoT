#include "gtest/gtest.h"
#include "./FreeRTOS_defs/fff.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"


extern "C"
{
    #include "../src/drivers/hcSr501.h"
    #include "../src/Headers/MotionSensor.h"
}

FAKE_VALUE_FUNC(bool, hcsr501_isDetecting, hcsr501_p);
FAKE_VALUE_FUNC(hcsr501_p, hcsr501_create, volatile uint8_t* , uint8_t);
FAKE_VOID_FUNC(hcsr501_destroy,hcsr501_p);



class MotionTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
        RESET_FAKE(xTaskCreate);
		RESET_FAKE(xTaskGetTickCount);
		RESET_FAKE(xTaskDelayUntil);
		RESET_FAKE(vTaskDelay);
        RESET_FAKE(hcsr501_create);
        RESET_FAKE(hcsr501_destroy);
        RESET_FAKE(hcsr501_isDetecting);
		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};

TEST_F(MotionTest, create_is_called){
    // Arrange
    volatile uint8_t mockport = 0x01;
    uint8_t mockPortPin = 3;
    hcsr501_p sensor = hcsr501_create(&mockport, mockPortPin);
	// Act
    motion_t motion = motion_create(sensor);

    // Assert/Expect
    ASSERT_EQ(1, hcsr501_create_fake.call_count);
}
TEST_F(MotionTest, destroy_is_called){
    // Arrange
    hcsr501_p fakeHCSR501 = (hcsr501_p)calloc(1, sizeof(hcsr501_p));
    motion_t motion = motion_create(fakeHCSR501);

    motion_destroy(&motion);
    // Assert/Expect
    ASSERT_EQ(1, hcsr501_destroy_fake.call_count);
}
TEST_F(MotionTest, detecting_is_called_with_no_args){
    ASSERT_FALSE(detecting(NULL));
    ASSERT_EQ(0, hcsr501_isDetecting_fake.call_count);
    // Assert/Expect
}
TEST_F(MotionTest, detecting_is_called_with_args){
    // Arrange
    hcsr501_p fakeHCSR501 = (hcsr501_p)calloc(1, sizeof(hcsr501_p));
	// Act
    motion_t motion = motion_create(fakeHCSR501);
    hcsr501_isDetecting_fake.return_val = true;
    // Asser/Expect
    ASSERT_TRUE(detecting(motion));
    ASSERT_EQ(1, hcsr501_isDetecting_fake.call_count);
}