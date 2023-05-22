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
        RESET_FAKE(xTaskCreate);
		RESET_FAKE(xTaskGetTickCount);
		RESET_FAKE(xTaskDelayUntil);
		RESET_FAKE(vTaskDelay);
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
    sound_t sound = sound_create();

    // Assert/Expect
    ASSERT_EQ(1, sen14262_initialise_fake.call_count);
    ASSERT_FALSE(sound == NULL);
}

TEST_F(SoundTest, Test_sound_is_detected_no_arg){
    // Arrange
    // Act
    bool succesfullDetection = soundDetection(NULL);

    // Assert/Expect
    ASSERT_FALSE(succesfullDetection);
    ASSERT_EQ(0, sen14262_envelope_fake.call_count);
}

TEST_F(SoundTest, Test_sound_is_detected_arg){
    // Arrange
    // Act
    sound_t sound = sound_create();
    sen14262_envelope_fake.return_val = 10;
    bool succesfullDetection = soundDetection(sound);

    // Assert/Expect
    ASSERT_TRUE(succesfullDetection);
    ASSERT_EQ(1, sen14262_envelope_fake.call_count);
}

TEST_F(SoundTest, Test_sound_get_gate_true){
    // Arrange
    // Act
    sen14262_gate_fake.return_val = true;
    bool soundGate = get_sound_gate_state();

    // Assert/Expect
    ASSERT_EQ(1, sen14262_gate_fake.call_count);
    ASSERT_TRUE(soundGate);
}

TEST_F(SoundTest, Test_sound_get_gate_false){
    // Arrange
    // Act
    sen14262_gate_fake.return_val = false;
    bool soundGate = get_sound_gate_state();

    // Assert/Expect
    ASSERT_EQ(1, sen14262_gate_fake.call_count);
    ASSERT_FALSE(soundGate);
}

TEST_F(SoundTest, create_sound_task_no_args) 
{
	// Arrange
	create_sound_task(NULL);
	// Assert/Expect
	ASSERT_EQ(0,xTaskCreate_fake.call_count);
}

TEST_F(SoundTest, create_sound_task_args) 
{
	// Arrange
	sound_t sound = sound_create();
	create_sound_task(&sound);
	// Assert/Expect
	ASSERT_EQ(1,xTaskCreate_fake.call_count);
	ASSERT_EQ(xTaskCreate_fake.arg0_val, &sound_task);
	ASSERT_EQ(strncmp(xTaskCreate_fake.arg1_val, "Sound sensor task", 18), 0);
	ASSERT_EQ(xTaskCreate_fake.arg2_val, configMINIMAL_STACK_SIZE);
	ASSERT_EQ(xTaskCreate_fake.arg3_val, &sound);
	ASSERT_EQ(xTaskCreate_fake.arg4_val, 1);
	ASSERT_EQ(xTaskCreate_fake.arg5_val, nullptr);
}