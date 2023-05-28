#include "stdint-gcc.h"
#include "tsl2591.h"
#include <stdlib.h>
#include <stdio.h>
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include "./Threshold.h"
#include "./LED.h"
#include "./LoRaWAN.h"
#include "./Threshold.h"

typedef struct light* light_t;

// Is called when the light sensor measuring is complete
void tsl2591Callback(tsl2591_returnCode_t);

// Creates the structure for the sensor and allocates it memory
light_t light_create(threshold_t*);

// Destroys the structure for the sensor and frees up the memory
void light_destroy(light_t*);

// Measures the light and returns true if it was successful / false otherwise
bool get_light_data(light_t);

// Returns light value
uint16_t get_tmp(light_t);

// Returns lux value
uint16_t get_lux(light_t);

// Creates task for measuring light
void create_light_task(light_t*);

// Implementation of light sensor task
void light_task( void* );