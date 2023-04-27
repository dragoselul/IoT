/*
 * Sound.h
 *
 * Created: 4/27/2023 1:15:28 PM
 *  Author: Jagerinho
 */ 
typedef struct sound_t;

sound_t sound_create();
void sound_destroy();
bool get_sound_state();
bool soundDetection(sound_t self);