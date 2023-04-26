/*
* main.c
* Author : IHA
*
* Example main file including LoRaWAN setup
* Just for inspiration :)
*/
#include "stdint-gcc.h"
#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <serial.h>

 // Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>

//Drivers
#include "./Headers/CO2.h"
#include "./Headers/Light.h"
#include "./Headers/TempAndHum.h"
#include "./Headers/MotionSensor.h"
#include "display_7seg.h"

// define two Tasks
void displayTask( void *pvParameters );
void tempAndHumidityTask( void *pvParameters );
void lightTask(void *pvParameters);
void motionTask(void *pvParameters);
void co2Task(void *pvParameters);

// define semaphore handle
SemaphoreHandle_t gateKeeper = NULL;

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

// sensor variables
tempAndHum_t temp_hum;
light_t light_sensor;
motion_t motion_sensor;
//co2_t co2_sensor;

/*-----------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if ( gateKeeper == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		gateKeeper = xSemaphoreCreateMutex();  // Create a mutex semaphore.
	}
	
	/*
	xTaskCreate(
	lightTask
	,  "Light Task"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	*/
	
	
	/*
	xTaskCreate(
	tempAndHumidityTask
	,  "Temperature and Humidity"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	*/
	
	
	xTaskCreate(
	co2Task
	,  "CO2 Task"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	
	/*
	xTaskCreate(
	motionTask
	,  "Motion sensor task"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
		*/
	
}


/*-----------------------------------------------------------*/
void motionTask(void *pvParameters)
{
	TickType_t xLastWakeTime;
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	
	if(motion_sensor != NULL)
	{
		for(;;)
		{
			if(!detecting(motion_sensor))
			{
				puts("Nothing detected...\n");
			}
			else
			{
				puts("Detecting something...\n");
			}
			xTaskDelayUntil( &xLastWakeTime, 1000/portTICK_PERIOD_MS); // 10 ms
		}
	}
}

/*-----------------------------------------------------------*/
void lightTask(void *pvParameters)
{
	TickType_t xLastWakeTime;
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	
	if(light_sensor != NULL) 
	{
		for(;;)
		{
			if(power_up_sensor() && xSemaphoreTake(gateKeeper, 5000/portTICK_PERIOD_MS))
			{
				puts("Light task took the semaphore");
				get_light_data(light_sensor);
				xTaskDelayUntil( &xLastWakeTime, 10/portTICK_PERIOD_MS); // 10 ms
				display_7seg_display(get_tmp(light_sensor),0);
				xTaskDelayUntil( &xLastWakeTime, 2000/portTICK_PERIOD_MS); // 1000 ms
				display_7seg_display(get_lux(light_sensor),1);
				power_down_sensor();
				xTaskDelayUntil( &xLastWakeTime, 2000/portTICK_PERIOD_MS); // 10 ms
				display_7seg_display(0,0);
				xSemaphoreGive(gateKeeper);
			}
		}		
	}
}


void co2Task(void *pvParameters)
{
	TickType_t xLastWakeTime;
	// Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{		
		take_measuring();
		//xTaskDelayUntil( &xLastWakeTime, 10/portTICK_PERIOD_MS); // 10 ms
		display_7seg_display((float)get_value(), 0);
		printf("[CO2 Sensor]: There is %d particles of CO2 per million particles of air\n", get_value());
		printf("[CO2 Sensor]: Average for last %d measurements is %d\n", get_measurements(), get_average());
		
		//printf("[CO2 Sensor]: Value: %d, Threshold: %d, Surpassed: %d", get_value(), get_threshold(), threshold_surpassed());
		if(threshold_surpassed()){
			// START SERVO
			printf("\n[CO2 Sensor]: Threshold of %d ppm surpassed\n", get_threshold());
		}
		
		xTaskDelayUntil( &xLastWakeTime, 1000/portTICK_PERIOD_MS); // 500 ms
		// What is this for?
		PORTA ^= _BV(PA1);
	}
	
}

/*-----------------------------------------------------------*/
void tempAndHumidityTask( void *pvParameters )
{
	TickType_t xLastWakeTime;
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	if(temp_hum!= NULL)
	{
		for(;;)
		{	
			if(wakeup_sensor())
			{
				//It takes the sensor around 50 ms to wake up
				xTaskDelayUntil( &xLastWakeTime, 50/portTICK_PERIOD_MS ); // 50 ms
				if(measure_temp_hum() && xSemaphoreTake(gateKeeper, 5000/portTICK_PERIOD_MS));
				{
					puts("Temp task took the semaphore");
					//It takes the sensor around 1 ms to measure up something
					xTaskDelayUntil( &xLastWakeTime, 1/portTICK_PERIOD_MS); // 1 ms
					display_7seg_display(get_temperature_float(), 2);
					xTaskDelayUntil( &xLastWakeTime, 2000/portTICK_PERIOD_MS); // 1000 ms
					display_7seg_display(get_humidity_float(), 2);
					xTaskDelayUntil( &xLastWakeTime, 2000/portTICK_PERIOD_MS ); //1000 ms
					xSemaphoreGive(gateKeeper);
				}
			}
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
	co2_initialize();
	//Temp and humidity sensor
	temp_hum = tempAndHum_create();
	//Light sensor
	light_sensor = light_create();
	//Motion sensor
	motion_sensor = motion_create();
	
	//co2 sensor
	set_threshold(1000);
/*
	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Display initialization
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_initialise(1, NULL);
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3);
	*/
	
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