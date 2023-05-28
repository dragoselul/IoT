#pragma once
#include "stdint-gcc.h"
#include <stdlib.h>
#include "../drivers/hih8120.h"
#include "./Threshold.h"
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include "./Alarm.h"
#include "./LoRaWAN.h"
#include "./ExternBooleans.h"
#include "Servo.h"

typedef struct tempAndHum* tempAndHum_t;


// Creates the structure for the sensor and allocates it memory
tempAndHum_t tempAndHum_create(threshold_t*);

// Destroys the structure for the sensor and frees up the memory
void tempAndHum_destroy(tempAndHum_t*);

// Updates the temperature and humidity average values
void update_averages(tempAndHum_t);

// Resets the temperature and humidity average values
void reset_averages(tempAndHum_t);

// Measures the temperature and humidity
bool measure_temp_hum(tempAndHum_t);

// Returns the temperature average values
int16_t get_average_temp(tempAndHum_t);

// Returns the humidity average values
uint16_t get_average_hum(tempAndHum_t);

// Creates the task for measuring temperature and humidity
void create_temp_hum_task(tempAndHum_t*);

// Implementation of the task for measuring temperature and humidity
void temp_hum_task( void* );
