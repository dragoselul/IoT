#pragma once
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include "ATMEGA_FreeRTOS.h"
#include "../drivers/lora_driver.h"
#include "../drivers/status_leds.h"
#include "semphr.h"
#include "./Threshold.h"

void lora_handler_initialise(UBaseType_t, void*, void*);
void _lora_setup(void);
void add_to_payload(uint16_t, uint8_t, uint8_t, uint8_t);
void lora_handler_task( void *pvParameters );
void lora_downlink_task( void *pvParameters );