/*
* main.c
* Author : IHA
*
* Example main file including LoRaWAN setup
* Just for inspiration :)
*/

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
#include <display_7seg.h>
#include <hih8120.h>

// define two Tasks
void displayTask( void *pvParameters );
void tempAndHumidity( void *pvParameters );

// define semaphore handle
SemaphoreHandle_t xTestSemaphore;

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

/*-----------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{
	// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	// because it is sharing a resource, such as the Serial port.
	// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	if ( xTestSemaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		xTestSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		if ( ( xTestSemaphore ) != NULL )
		{
			xSemaphoreGive( ( xTestSemaphore ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		}
	}
/*
	xTaskCreate(
	displayTask
	,  "Display-Task"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	*/
	xTaskCreate(
	tempAndHumidity
	,  "Task2"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
}

/*-----------------------------------------------------------*/
void displayTask(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 500/portTICK_PERIOD_MS; // 500 ms
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		display_7seg_displayHex("FFFF");
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		display_7seg_displayHex("");
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		PORTA ^= _BV(PA0);
	}
}

/*-----------------------------------------------------------*/
void tempAndHumidity( void *pvParameters )
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 50/portTICK_PERIOD_MS; // 1000 ms
	float humidity = 0.0;
	float temperature = 0.0;
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		if ( HIH8120_OK != hih8120_wakeup() )
		{
			// Something went wrong
			// Investigate the return code further
		}
		else{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		if ( HIH8120_OK !=  hih8120_measure() )
		{
			// Something went wrong
			// Investigate the return code further
		}
		else {
		xTaskDelayUntil( &xLastWakeTime, 3 );
		humidity = hih8120_getHumidity();
		temperature = hih8120_getTemperature();
		display_7seg_display(temperature, 2);
		xTaskDelayUntil( &xLastWakeTime, 250 );
		display_7seg_display(humidity, 2);
		xTaskDelayUntil( &xLastWakeTime, 250 );
		}
		}
		
		PORTA ^= _BV(PA7);
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
	//Temp and humidity sensor
	if ( HIH8120_OK == hih8120_initialise() )
	{
		// Driver initialised OK
		// Always check what hih8120_initialise() returns
	}

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Display initialization
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_initialise(1, NULL);
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3);
	
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

