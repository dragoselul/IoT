#include "../Headers/LoRaWAN.h"

#define LORA_appEUI "9276B3CF3B069355"
#define LORA_appKEY "84860CBA5C5116F9EC56E1B4346CA899"

void lora_handler_task( void *pvParameters );
void lora_downlink_task( void *pvParameters );

SemaphoreHandle_t gateKeeper = NULL;
MessageBufferHandle_t downlink_buffer;

static lora_driver_payload_t _uplink_payload;
static lora_driver_payload_t _downlink_payload;

void lora_handler_initialise(UBaseType_t lora_handler_task_priority, void* thresholds, void* buffer)
{
	if ( gateKeeper == NULL )  // Check to confirm that the Semaphore has not already been created.
	{
		gateKeeper = xSemaphoreCreateMutex();  // Create a mutex semaphore.
	}
	
	downlink_buffer = *(MessageBufferHandle_t*) buffer;
	
	xTaskCreate(
	lora_handler_task
	,  "LRHand"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+200  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  lora_handler_task_priority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
	xTaskCreate(
	lora_downlink_task
	,	"LRDownLink"
	,	configMINIMAL_STACK_SIZE+200
	,	thresholds
	,	lora_handler_task_priority
	,	NULL );
}

static void _lora_setup(void)
{
	char _out_buf[20];
	lora_driver_returnCode_t rc;
	_uplink_payload.len = 10;
	_downlink_payload.len = 10;
	status_leds_slowBlink(led_ST2); // OPTIONAL: Led the green led blink slowly while we are setting up LoRa
	// Factory reset the transceiver
	printf("FactoryReset >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_rn2483FactoryReset()));
	
	// Configure to EU868 LoRaWAN standards
	printf("Configure to EU868 >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_configureToEu868()));

	// Get the transceivers HW EUI
	rc = lora_driver_getRn2483Hweui(_out_buf);
	printf("Get HWEUI >%s<: %s\n",lora_driver_mapReturnCodeToText(rc), _out_buf);

	// Set the HWEUI as DevEUI in the LoRaWAN software stack in the transceiver
	printf("Set DevEUI: %s >%s<\n", _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setDeviceIdentifier(_out_buf)));

	// Set Over The Air Activation parameters to be ready to join the LoRaWAN
	printf("Set OTAA Identity appEUI:%s appKEY:%s devEUI:%s >%s<\n", LORA_appEUI, LORA_appKEY, _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setOtaaIdentity(LORA_appEUI,LORA_appKEY,_out_buf)));

	// Save all the MAC settings in the transceiver
	printf("Save mac >%s<\n",lora_driver_mapReturnCodeToText(lora_driver_saveMac()));

	// Enable Adaptive Data Rate
	printf("Set Adaptive Data Rate: ON >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_setAdaptiveDataRate(LORA_ON)));

	// Set receiver window1 delay to 500 ms - this is needed if down-link messages will be used
	printf("Set Receiver Delay: %d ms >%s<\n", 500, lora_driver_mapReturnCodeToText(lora_driver_setReceiveDelay(500)));

	// Join the LoRaWAN
	uint8_t maxJoinTriesLeft = 100;
	
	do {
		rc = lora_driver_join(LORA_OTAA);
		printf("Join Network TriesLeft:%d >%s<\n", maxJoinTriesLeft, lora_driver_mapReturnCodeToText(rc));

		if ( rc != LORA_ACCEPTED)
		{
			// Make the red led pulse to tell something went wrong
			status_leds_longPuls(led_ST1); // OPTIONAL
			// Wait 5 sec and lets try again
			vTaskDelay(pdMS_TO_TICKS(5000UL));
		}
		else
		{
			break;
		}
	} while (--maxJoinTriesLeft);

	if (rc == LORA_ACCEPTED)
	{
		// Connected to LoRaWAN :-)
		// Make the green led steady
		status_leds_ledOn(led_ST2); // OPTIONAL
	}
	else
	{
		// Something went wrong
		// Turn off the green led
		status_leds_ledOff(led_ST2); // OPTIONAL
		// Make the red led blink fast to tell something went wrong
		status_leds_fastBlink(led_ST1); // OPTIONAL

		// Lets stay here
		while (1)
		{
			taskYIELD();
		}
	}
}

bool switchGarageId = true;

void add_to_payload(uint16_t data, uint8_t byte_pos1, uint8_t byte_pos2, uint8_t bit_pos)
{
	if(xSemaphoreTake(gateKeeper, portMAX_DELAY))
	{
		
		printf("\n Data is: %d\n", data);
		if(byte_pos1 == 0)
		puts("CO2 writing payload\n");
		if(byte_pos1 == 2)
		puts("Temp writing payload\n");
		if(byte_pos1 == 4)
		puts("Hum writing payload\n");
		if(byte_pos1 == 6)
		puts("Lux writing payload\n");
		if(byte_pos1 == 8)
		{
			if(bit_pos == 0)
			puts("Servo changing bit\n");
			if(bit_pos == 1)
			puts("Motion changing bit\n");
			if(bit_pos == 2)
			puts("Sound changing bit\n");
			if(bit_pos == 3)
			puts("Alarm changing bit\n");
			_uplink_payload.bytes[byte_pos1] = data>>bit_pos;
		}
		else
		{
			_uplink_payload.bytes[byte_pos1] = data >> 8;
			_uplink_payload.bytes[byte_pos2] = data & 0xFF;
		}
		if(switchGarageId)
		{
			uint16_t hash = 5381;
			int c;
			unsigned char* str = "0004A30B00251001";
			while (c = *str++) {
				hash = ((hash << 5) + hash) + c;
			}
			_uplink_payload.bytes[9] = hash;
			switchGarageId = false;
	}
	//printf("[0]: %d \n [1]: %d \n [2]: %d \n [3]: %d \n [4]: %d \n [5]: %d \n [6]: %d \n [7]: %d \n [8]: %d \n [9]: %d \n", _uplink_payload.bytes[0], _uplink_payload.bytes[1], _uplink_payload.bytes[2], _uplink_payload.bytes[3], _uplink_payload.bytes[4], _uplink_payload.bytes[5], _uplink_payload.bytes[6], _uplink_payload.bytes[7], _uplink_payload.bytes[8], _uplink_payload.bytes[9]);
		vTaskDelay(pdMS_TO_TICKS(50UL));
		xSemaphoreGive(gateKeeper);
	}
	else
	{
		puts("Could not take mutex");
	}
}

/*-----------------------------------------------------------*/
void lora_handler_task( void *pvParameters )
{
	// Hardware reset of LoRaWAN transceiver
	lora_driver_resetRn2483(1);
	vTaskDelay(2);
	lora_driver_resetRn2483(0);
	// Give it a chance to wakeup
	vTaskDelay(150);

	lora_driver_flushBuffers(); // get rid of first version string from module after reset!

	_lora_setup();
	
	_uplink_payload.portNo = 2;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(300000UL); // Upload message every 5 minutes (300000 ms)
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );	

		status_leds_shortPuls(led_ST4);  // OPTIONAL
		printf("Upload Message >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &_uplink_payload)));
	}
}

void lora_downlink_task( void *pvParameters )
{
	threshold_t thresholds = *(threshold_t*) pvParameters;
	for(;;)
	{
		xMessageBufferReceive(downlink_buffer, &_downlink_payload, sizeof(lora_driver_payload_t), portMAX_DELAY);
		_downlink_payload.portNo = 1;
		//printf("DOWN LINK: from port: %d with %d bytes received!", _downlink_payload.portNo, _downlink_payload.len); // Just for Debug
		if (10 == _downlink_payload.len && _downlink_payload.bytes[9] == 120) // Check that we have got the expected 10 bytes and the id of the garage is 120
		{
			
			//printf("%d %d %d %d %d %d %d %d %d %d %d", _downlink_payload.bytes[0], _downlink_payload.bytes[1], _downlink_payload.bytes[2], _downlink_payload.bytes[3], _downlink_payload.bytes[4], 
			//_downlink_payload.bytes[5], _downlink_payload.bytes[6], _downlink_payload.bytes[7], _downlink_payload.bytes[8], _downlink_payload.bytes[9]);
			
			uint16_t co2 = (uint16_t)(_downlink_payload.bytes[0]) << 8 | _downlink_payload.bytes[1];
			int16_t temp = (int16_t)(_downlink_payload.bytes[2]) << 8 | _downlink_payload.bytes[3];
			uint16_t hum = (uint16_t)(_downlink_payload.bytes[4]) << 8 | _downlink_payload.bytes[5];
			uint16_t lux = (uint16_t)(_downlink_payload.bytes[6]) << 8 | _downlink_payload.bytes[7];
			
			set_co2_threshold(&thresholds,co2);
			set_temperature_threshold(&thresholds,temp);
			set_humidity_threshold(&thresholds,hum);
			set_light_threshold(&thresholds, lux);
			
			//set_co2_threshold(&thresholds,(uint16_t)(_downlink_payload.bytes[0]) << 8 | _downlink_payload.bytes[1]);
			//set_temperature_threshold(&thresholds,(uint16_t)(_downlink_payload.bytes[2]) << 8 | _downlink_payload.bytes[3]);
			//set_humidity_threshold(&thresholds,(uint16_t)(_downlink_payload.bytes[4]) << 8 | _downlink_payload.bytes[5]);
			//set_light_threshold(&thresholds,(uint16_t)(_downlink_payload.bytes[6]) << 8 | _downlink_payload.bytes[7]);
			
			printf("%d %d %d %d\n", co2, temp, hum, lux);
			
			//printf("%d %d %d %d\n", get_co2_threshold(&thresholds), get_temperature_threshold(&thresholds), get_humidity_threshold(&thresholds), get_light_threshold(&thresholds));
			
			
			
			
			//set_co2_threshold(&thresholds,(uint16_t)(_downlink_payload.bytes[0]) << 8 | _downlink_payload.bytes[1]);
			//set_temperature_threshold(&thresholds,(uint16_t)(_downlink_payload.bytes[2]) << 8 | _downlink_payload.bytes[3]);
			//set_humidity_threshold(&thresholds,(uint16_t)(_downlink_payload.bytes[4]) << 8 | _downlink_payload.bytes[5]);
			//set_light_threshold(&thresholds,(uint16_t)(_downlink_payload.bytes[6]) << 8 | _downlink_payload.bytes[7]);
			
			//printf("%d %d %d %d", (uint16_t)(_downlink_payload.bytes[0]) << 8 | _downlink_payload.bytes[1],
			//(uint16_t)(_downlink_payload.bytes[2]) << 8 | _downlink_payload.bytes[3],
			//(uint16_t)(_downlink_payload.bytes[4]) << 8 | _downlink_payload.bytes[5],
			//(uint16_t)(_downlink_payload.bytes[6]) << 8 | _downlink_payload.bytes[7],
			//(uint16_t)(_downlink_payload.bytes[7]) << 8 | _downlink_payload.bytes[8]
			//);
			
			//printf("%d %d %d %d", get_co2_threshold(&thresholds), get_temperature_threshold(&thresholds), get_humidity_threshold(&thresholds), get_light_threshold(&thresholds));
			
			
			
		}
	}
}