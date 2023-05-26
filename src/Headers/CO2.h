#pragma once
#include "stdint.h"
#include "mh_z19.h"
#include <stdlib.h>
#include <stdio.h>
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include "./Threshold.h"
#include "Alarm.h"
#include "Servo.h"
#include "LoRaWAN.h"

typedef struct co2* co2_t;

// Structure that holds the co2 value, threshold and the average
typedef struct co2 {
	uint16_t val;
	float avg_co2;
	uint8_t measurements;
	threshold_t* th_point;
} co2;

// Creates the structure for the sensor and allocates it memory
co2_t co2_create(threshold_t*);

// Destroys the structure for the sensor and frees up the memory
void co2_destroy(co2_t);

// Measures the co2 and returns true if it was successful / false otherwise
bool co2_get_data(co2_t);

// Evaluates if the co2 surpassed the threshold and turns on alarm/opens door if it did
void co2_evaluate_threshold(co2_t);

// Returns the last measured co2 value
uint16_t co2_get_value(co2_t);

// Updates the co2 average value
void co2_update_average(co2_t);

// Returns the co2 average for the past measurements
uint16_t co2_get_average(co2_t);

// Resets the average value of the sensor for the past measurements
void co2_reset_average(co2_t);

// Creates the task for measuring co2
void create_co2_task(co2_t*);

// Task implementation for the CO2
void co2_task(void*);