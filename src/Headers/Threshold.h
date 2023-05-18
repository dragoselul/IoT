#pragma once
#include "stdint-gcc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct threshold* threshold_t;


threshold_t threshold_create();
void threshold_destroy(threshold_t);
int16_t get_temperature_threshold(threshold_t*);
uint16_t get_humidity_threshold(threshold_t*);
uint16_t get_co2_threshold(threshold_t*);
uint16_t get_light_threshold(threshold_t*);
void set_temperature_threshold(threshold_t*, int16_t);
void set_humidity_threshold(threshold_t*, uint16_t);
void set_co2_threshold(threshold_t*, uint16_t);
void set_light_threshold(threshold_t*, uint16_t);
