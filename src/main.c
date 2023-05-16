/*
* main.c
* Author : gRPC
*/
#include "stdint-gcc.h"
#include <stdio.h>
#include <avr/io.h>
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <stdio_driver.h>
#include <serial.h>

 // Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>
#include <rc_servo.h>

//Drivers
#include "./Headers/CO2.h"
#include "./Headers/Light.h"
#include "./Headers/TempAndHum.h"
#include "./Headers/MotionSensor.h"
#include "./Headers/Sound.h"
#include "display_7seg.h"

// define the tasks
void displayTask( void *pvParameters );
void motionTask(void *pvParameters);


// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority, void* thresh, void* downlink_buffer);

// sensor variables
tempAndHum_t temp_hum;
light_t light_sensor;
motion_t motion_sensor;
sound_t sound_sensor;
co2_t co2_sensor;
threshold_t thresholds;

// SERVO JC14 = 0, JC13 = 1
void rc_servo(uint16_t percentage){
	rc_servo_setPosition(1, percentage);
}

/*-----------------------------------------------------------*/
void initialiseSystem()
{
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);

	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	
	display_7seg_initialise(NULL);
	display_7seg_powerUp();
	rc_servo_initialise();
	rc_servo(-100);
	//Threshold init
	thresholds = threshold_create();
	//Temp and humidity sensor
	temp_hum = tempAndHum_create(&thresholds);
	create_temp_hum_task(&temp_hum);
	//Light sensor
	light_sensor = light_create(&thresholds);
	create_light_task(&light_sensor);
	//Motion sensor
	motion_sensor = motion_create();
	create_motion_task(&motion_sensor);
	//Sound sensor
	sound_sensor = sound_create();
	create_sound_task(&sound_sensor);
	//CO2 sensor
	co2_sensor = co2_create(&thresholds);
	create_co2_task(&co2_sensor);
	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Buffer
	MessageBufferHandle_t downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2); // Here I make room for two downlink messages in the message buffer
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_initialise(ser_USART1, downLinkMessageBufferHandle); // The parameter is the USART port the RN2483 module is connected to - in this case USART1 - here no message buffer for down-link messages are defined
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3, &thresholds, &downLinkMessageBufferHandle);
	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing!!
	printf("Program Started!!\n");
	vTaskStartScheduler(); // Initialise and run the freeRTOS scheduler. Execution should never return from here.

	/* Replace with your application code */
	while (1)
	{
	}
}