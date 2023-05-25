#include "hcSr501.h"
#include <stdlib.h>
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include "./LoRaWAN.h"

typedef struct motion* motion_t;

// Creates the structure for the sensor and allocates it memory
motion_t motion_create(hcsr501_p);

// Destroys the structure for the sensor and frees up the memory
void motion_destroy(motion_t* self);

// Returns "true" if motion is detected and "false" if it is not
bool detecting(motion_t self);

// Creates the task for the motion sensor to detect and update values
void create_motion_task(motion_t*);

// The function that the motion task will execute in a loop
// Checks if motion was detected and updates the LoRaWAN payload
void motion_task(void*);