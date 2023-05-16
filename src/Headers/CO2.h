#include "stdint.h"
#include "mh_z19.h"
#include <stdlib.h>
#include <stdio.h>
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include "./Threshold.h"
#include "Alarm.h"

typedef struct co2* co2_t;

co2_t co2_create(threshold_t*);
void co2_destroy(co2_t);
bool co2_get_data(co2_t self);
uint16_t co2_get_value(co2_t);
void co2_update_average(co2_t);
uint16_t co2_get_average(co2_t);
void co2_reset_average(co2_t);
void create_co2_task(co2_t*);
void co2_task( void* );