#include "stdint-gcc.h"
#include "tsl2591.h"
#include <stdlib.h>
#include <stdio.h>
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include "./Threshold.h"
#include "./LED.h"
#include "./LoRaWAN.h"

typedef struct light* light_t;

void tsl2591Callback(tsl2591_returnCode_t);
light_t light_create();
void light_destroy(light_t);
bool get_light_data(light_t);
uint16_t get_tmp(light_t);
uint16_t get_lux(light_t);
void create_light_task(light_t*);
void light_task( void* );