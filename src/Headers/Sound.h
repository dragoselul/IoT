#include "sen14262.h"
#include <stdlib.h>
#include <stdio.h>
#include "ATMEGA_FreeRTOS.h"
#include "task.h"

typedef struct sound* sound_t;

sound_t sound_create();
void sound_destroy(sound_t self);
bool get_sound_gate_state();
bool soundDetection(sound_t self);
void create_sound_task(sound_t*);
void sound_task(void*);