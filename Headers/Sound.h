/*
 * Sound.h
 *
 * Created: 18.04.2023 10:38:17
 *  Author: Danny
 */ 
#include <sen14262.h>

typedef struct sound* sound_t;

sound_t sound_create();
void sound_destroy();
bool get_sound_state();