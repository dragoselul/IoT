#include "stdint-gcc.h"
#include <stdio.h>
#include <avr/io.h>
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <stdio_driver.h>
#include <serial.h>

// LoRaWAN Initialization
#include <lora_driver.h>
#include <status_leds.h>

// headers for sensors
#include "./Headers/CO2.h"
#include "./Headers/Light.h"
#include "./Headers/TempAndHum.h"
#include "./Headers/MotionSensor.h"
#include "./Headers/Sound.h"
#include "./Headers/Servo.h"

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority, void* thresh, void* downlink_buffer);

// Structures for the sensors
tempAndHum_t temp_hum;
light_t light_sensor;
motion_t motion_sensor;
sound_t sound_sensor;
co2_t co2_sensor;
threshold_t thresholds;

/*
	Initializes all drivers and sensors and creates structures and thresholds for the sensors
	as well as initializes the LoRaWAN driver for communication with back-end 
*/
void initialiseSystem(){
	
	// TODO Delete if not necessary
	// Set output ports for LEDs used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);
	
	// Make it possible to turn on and off the Alarm
	alarm_initialize();
	
	// Make it possible to use the LED connected to PIN 15 for lighting
	LED_initialize();
	
	// Make it possible to use the RC-Servo to control the door
	servo_intialize();

	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	
	// Initialize Thresholds
	thresholds = threshold_create();
	
	// Temp and humidity sensor
	temp_hum = tempAndHum_create(&thresholds);
	create_temp_hum_task(&temp_hum);
	
	// Light sensor
	light_sensor = light_create(&thresholds);
	create_light_task(&light_sensor);
	
	// Motion sensor
	motion_sensor = motion_create(hcsr501_create(&PORTE, PE5));
	create_motion_task(&motion_sensor);
	
	// Sound sensor
	sound_sensor = sound_create();
	create_sound_task(&sound_sensor);
	
	// CO2 sensor
	co2_sensor = co2_create(&thresholds);
	create_co2_task(&co2_sensor);
	
	/* --- LoRaWAN initialization --- */
	// Initialize Status LEDs driver
	status_leds_initialise(5); // Priority 5 for internal task
	
	// Create MessageBuffer
	MessageBufferHandle_t downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t)*2); // Here I make room for two downlink messages in the message buffer
	
	// TODO - Add some comments about functionality
	
	// Initialize the LoRaWAN driver without down-link buffer
	
	// The parameter is the USART port the RN2483 module is connected to - in this case USART1 - here no message buffer for down-link messages are defined
	lora_driver_initialise(ser_USART1, downLinkMessageBufferHandle); 
	
	
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3, &thresholds, &downLinkMessageBufferHandle);
	
	
	printf("Program Started!!\n");
}

/*-----------------------------------------------------------*/
int main(void){
	// Initialize all drivers, sensors and PINs
	initialiseSystem();
	// Initialize and run the FreeRTOS scheduler. (Should never return)
	vTaskStartScheduler(); 
}