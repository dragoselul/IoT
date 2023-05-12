#include "stdint-gcc.h"
#include "tsl2591.h"
#include <stdlib.h>
#include <stdio.h>
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include "./Average.h"
#include "./Threshold.h"

typedef struct light* light_t;

light_t light_create();
void light_destroy(light_t);
bool get_light_data(light_t);
uint16_t get_tmp(light_t);
float get_lux(light_t);
uint16_t get_average_light(light_t);
void reset_average_light(light_t);

/*
bool light_threshold_surpassed(light_t);
uint16_t light_get_threshold(light_t);
void light_set_threshold(light_t, uint16_t);
*/