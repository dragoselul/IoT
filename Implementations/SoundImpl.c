/*
 * SoundImpl.c
 *
 * Created: 18.04.2023 10:37:04
 *  Author: Danny
 */ 
#include "Sound.h"
#include <sen14262.h>

void sen14262_initialise();

void sen14262_envelope(){
	uint16_t lastSoundValue;
	lastSoundValue = sen14262_envelope();
	return lastSoundValue;
}

}

sound_t sound_create();
void sound_destroy();
uint16_t get_sound_level();
bool get_sound_state()
{
	return sen14262_gate();
}