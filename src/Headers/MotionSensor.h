#include "hcSr501.h"
#include <stdlib.h>
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include "./LoRaWAN.h"

typedef struct motion* motion_t;

motion_t motion_create(hcsr501_p);
void motion_destroy(motion_t self);
bool detecting(motion_t self);
void create_motion_task(motion_t*);
void motion_task(void*);