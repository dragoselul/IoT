#include "sen14262.h"
#include <stdlib.h>
#include <stdio.h>
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include "LoRaWAN.h"

typedef struct sound* sound_t;

//Creates a sound
sound_t sound_create();

//Destroys the created sound
void sound_destroy(sound_t self);

//Gets the gate of the sound
bool get_sound_gate_state();

//Detects the sound around the sensor
bool soundDetection(sound_t self);

//Creates the sound task for measuring sound
void create_sound_task(sound_t*);

//Task implementation for the sound
void sound_task(void*);