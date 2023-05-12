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
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

// sensor variables
tempAndHum_t temp_hum;
light_t light_sensor;
motion_t motion_sensor;
sound_t sound_sensor;
co2_t co2_sensor;

/*
void display(TickType_t ms, void *data, uint8_t decimal_places)
{
	if(xSemaphoreTake(gateKeeper, portMAX_DELAY))
	{
		puts("Display took the semaphore!\n");
		display_7seg_display(*((float*)data), decimal_places);
		vTaskDelay(ms);
		xSemaphoreGive(gateKeeper);
	}
	else
	{
		puts("Could not take the display!\n");
	}
}
*/

/*-----------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.

/*	
	xTaskCreate(
	co2Task
	,  "CO2 Task"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	motionTask
	,  "Motion sensor task"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	*/
}

// SERVO JC14 = 0, JC13 = 1
void rc_servo(uint16_t percentage){
	rc_servo_setPosition(1, percentage);
}


/*-----------------------------------------------------------*/
void motionTask(void *pvParameters)
{
	TickType_t xLastWakeTime;
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	uint16_t is_detecting;
	
	if(motion_sensor != NULL)
	{
		for(;;)
		{
			if(!detecting(motion_sensor))
			{
				is_detecting = 0;
				add_to_payload(is_detecting, 8,NULL, 1);
			}
			else
			{
				is_detecting = 1;
				add_to_payload(is_detecting, 8,NULL, 1);
			}
			vTaskDelay(4000/portTICK_PERIOD_MS); // 1000 ms
		}
	}
}

/*-----------------------------------------------------------*/
void soundTask( void *pvParameters )
{
	TickType_t xLastWakeTime;
	//Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	uint16_t sound_detecting;
	
	if(sound_sensor != NULL)
	{
		for(;;)
		{
			if(!soundDetection(sound_sensor))
			{
				sound_detecting = 0;
				add_to_payload(sound_detecting, 8,NULL, 2);
			}
			else
			{
				sound_detecting = 1;
				add_to_payload(sound_detecting, 8,NULL, 2);
			}
			xTaskDelayUntil( &xLastWakeTime, 4000/portTICK_PERIOD_MS); // 10 ms
		}
	}
}

/*-----------------------------------------------------------*/
void initialiseSystem()
{
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);

	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	// Let's create some tasks
	create_tasks_and_semaphores();
	
	display_7seg_initialise(NULL);
	display_7seg_powerUp();
	rc_servo_initialise();
	rc_servo(-100);
	//Temp and humidity sensor
	temp_hum = tempAndHum_create();
	create_temp_hum_task(&temp_hum);
	//Light sensor
	light_sensor = light_create();
	create_light_task(&light_sensor);
	//Motion sensor
	motion_sensor = motion_create();
	//Sound sensor
	sound_sensor = sound_create();
	//CO2 sensor
	co2_sensor = co2_create();
	create_co2_task(co2_sensor);
	//co2 sensor
	co2_set_threshold(co2_sensor,1000);
	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Display initialization
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_initialise(1, NULL);
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3);
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