/*
 * SoundImpl.c
 *
 * Created: 18.04.2023 10:37:04
 *  Author: Danny
 */ 
#include "Sound.h"
#include <sen14262.h>

void sen14262_initialise();

typedef struct sound
{
	uint16_t _sound;	
}sound;

sound_t sound_create(){
	uint16_t lastSoundValue;
	lastSoundValue = sen14262_envelope();
	return lastSoundValue;
}

bool get_sound_state()
{
	return sen14262_gate();
}

void sound_destroy(sound_t self)
{
	if (NULL != self)
	free(self);
}

bool soundDetection(sound self)
{
	uint16_t lastSoundValue;
	if (self = sen14262_envelope(lastSoundValue))
	{
		return true;
	}
	else
	{
		return false;
	}
}
