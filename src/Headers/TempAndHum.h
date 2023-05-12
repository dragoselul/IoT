#pragma once
#include "stdint-gcc.h"
#include <stdlib.h>
#include "../drivers/hih8120.h"
#include "./Average.h"
#include "./Threshold.h"
#include "ATMEGA_FreeRTOS.h"
#include "task.h"

typedef struct tempAndHum* tempAndHum_t;

tempAndHum_t tempAndHum_create();
void tempAndHum_destroy(tempAndHum_t*);
void update_averages(tempAndHum_t);
void reset_averages(tempAndHum_t);
bool measure_temp_hum(tempAndHum_t);
int16_t get_average_temp(tempAndHum_t);
uint16_t get_average_hum(tempAndHum_t);
void create_temp_hum_task(tempAndHum_t*);
void temp_hum_task( void* );
int16_t get_average_temp(tempAndHum_t);

bool hum_threshold_surpassed(tempAndHum_t);
uint16_t hum_get_threshold(tempAndHum_t);
void hum_set_threshold(tempAndHum_t, uint16_t);

bool temp_threshold_surpassed(tempAndHum_t);
uint16_t temp_get_threshold(tempAndHum_t);
void temp_set_threshold(tempAndHum_t, uint16_t);
