#include "gtest/gtest.h"
#include "./FreeRTOS_defs/fff.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"


extern "C"
{
    #include "../src/Headers/Light.h"
    #include "../src/drivers/tsl2591.h"
}

FAKE_VALUE_FUNC(tsl2591_returnCode_t, tsl2591_initialise, void(tsl2591_returnCode_t));
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








class MotionTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
        RESET_FAKE(tsl2591_initialise);
		FFF_RESET_HISTORY();
	}
	void TearDown() override
	{}
};

TEST_F(MotionTest, test_light_create_is_called){
    // Arrange
	// Act
    light_create();

    // Assert/Expect
    ASSERT_EQ(1, tsl2591_initialise_fake.call_count);
}
TEST_F(MotionTest, Test_sound_destroy_is_called){
    // Arrange
	// Act
    light_t lightSen = light_create();
    light_destroy(lightSen);

    ASSERT_EQ(1, tsl2591_initialise_fake.call_count);
}