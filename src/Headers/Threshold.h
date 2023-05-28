#pragma once
#include "stdint-gcc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct threshold* threshold_t;

// Creates the structure for the threshold values and allocates them memory
threshold_t threshold_create();

// Destroys the structure for the threshold values and frees up the memory
void threshold_destroy(threshold_t);

// Returns the threshold for temperature sensor
int16_t get_temperature_threshold(threshold_t*);

// Returns the threshold for humidity sensor
uint16_t get_humidity_threshold(threshold_t*);

// Returns the threshold for co2 sensor
uint16_t get_co2_threshold(threshold_t*);

// Returns the threshold for light sensor
uint16_t get_light_threshold(threshold_t*);

// Returns the current state of the automatic lights feature
bool get_automatic_lights(threshold_t*);

// Sets the threshold for the temperature sensor
void set_temperature_threshold(threshold_t*, int16_t);

// Sets the threshold for the humidity sensor
void set_humidity_threshold(threshold_t*, uint16_t);

// Sets the threshold for the co2 sensor
void set_co2_threshold(threshold_t*, uint16_t);

// Sets the threshold for the light sensor
void set_light_threshold(threshold_t*, uint16_t);

// Sets the status of the automatic lights feature
void set_automatic_lights(threshold_t*, bool);
