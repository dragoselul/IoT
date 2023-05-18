#include "gtest/gtest.h"
#include "./FreeRTOS_defs/fff.h"
#include "FreeRTOS_FFF_MocksDeclaration.h"

extern "C"
{
    #include "../src/Headers/Sound.h"
	#include "../src/drivers/sen14262.h"
}

FAKE_VOID_FUNC(sen14262_initialise);
FAKE_VALUE_FUNC(uint16_t, sen14262_envelope);
FAKE_VALUE_FUNC(bool, sen14262_gate);

class SoundTest : public ::testing::Test
{
    protected:
    void SetUp() override
    {
        RESET_FAKE(sen14262_initialise);
        RESET_FAKE(sen14262_envelope);
        RESET_FAKE(sen14262_gate);
        FFF_RESET_HISTORY();
    }
    void TearDown() override
    {}
};

TEST_F(SoundTest, Test_sen_create_is_called){
    // Arrange
    // Act
    sound_create();

    // Assert/Expect
    ASSERT_EQ(1, sen14262_initialise_fake.call_count);
}

TEST_F(SoundTest, Test_sound_is_detected){
    // Arrange
    // Act
   // soundDetection(NULL);

    // Assert/Expect
    ASSERT_EQ(1, sen14262_envelope_fake.call_count);
}

TEST_F(SoundTest, Test_sound_get_gate){
    // Arrange
    // Act
   // get_sound_gate_state();

    // Assert/Expect
    ASSERT_EQ(1, sen14262_gate_fake.call_count);
}