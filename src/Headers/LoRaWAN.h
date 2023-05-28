#pragma once
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include "ATMEGA_FreeRTOS.h"
#include "semphr.h"
#include "../drivers/lora_driver.h"
#include "../drivers/status_leds.h"
#include "./Threshold.h"
#include "./ExternBooleans.h"

// Initializes the Semaphore and the LoRa up-link and down-link tasks
void lora_handler_initialise(UBaseType_t, void*, void*);

// Sets up the necessary configurations in order to establish connection
void _lora_setup(void);

// Adds data from sensors to the payload so it can be send through LoRa
void add_to_payload(int32_t, uint8_t, uint8_t, uint8_t);

// Task for handling LoRa up-link
void lora_handler_task( void *pvParameters );

// Task for handling LoRa down-link
void lora_downlink_task( void *pvParameters );

// Returns LoRa up-link payload - mainly for testing purposes
lora_driver_payload_t get_uplink_payload();

// Returns LoRa down-link payload - mainly for testing purposes
lora_driver_payload_t get_downlink_payload();