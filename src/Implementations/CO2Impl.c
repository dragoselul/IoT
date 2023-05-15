#include "../Headers/CO2.h"

typedef struct co2
{
	uint16_t val;
	float avg_co2;
	uint8_t measurements;
	threshold_t threshold;
} co2;

uint16_t val = 0;
mh_z19_returnCode_t rc = MHZ19_NO_MEASSURING_AVAILABLE;

void mhz19_callback(uint16_t ppm){
	val = ppm;
}

co2_t co2_create(){
	co2_t _new_co2 = (co2_t) calloc (1, sizeof(co2));
	if(NULL == _new_co2)
		return NULL;
	mh_z19_initialise(ser_USART3);
	mh_z19_injectCallBack(mhz19_callback);

	_new_co2->val = 0;
	_new_co2->threshold = threshold_create();
	_new_co2->avg_co2 = 0.0;
	_new_co2->measurements = 0;
	return _new_co2;
}
void co2_destroy(co2_t self){
	if (NULL != self)
	free(self);
}
void co2_measure()
{
	rc = mh_z19_takeMeassuring();
}
bool co2_get_data(co2_t self){
	if(rc == MHZ19_OK){
		self->val = val;
		co2_update_average(self);
		return true;
	}
	return false;

}

uint16_t co2_get_value(co2_t self){
	return self->val;
}
uint16_t co2_get_average(co2_t self){
	return (uint16_t)self->avg_co2;
}
void co2_reset_average(co2_t self){
	if(&self == NULL)
		return;
	self->avg_co2 = 0.0;
	self->measurements = 0;
}
void co2_update_average(co2_t self){
	if(&self == NULL)
		return;
	if(self->measurements <= 4)
	{
		self->avg_co2 = self->val;
		return;
	}
	self->avg_co2 = self->avg_co2 + (self->val - self->avg_co2) / (self->measurements + 1);
}

// bool co2_threshold_surpassed(co2_t self){
// 	return threshold_surpassed(self->threshold, self->val);
// }
// uint16_t co2_get_threshold(co2_t self){
// 	return get_threshold(self->threshold);
// }
// void co2_set_threshold(co2_t self, uint16_t newThreshold){
// 	set_threshold(self->threshold, newThreshold);
// }

void log_errors(mh_z19_returnCode_t code){
	if(code == MHZ19_OK){
		printf("[CO2 Sensor]: MHZ19_OK\n");
		}if(code == MHZ19_NO_SERIAL){
		printf("[CO2 Sensor]: MHZ19_NO_SERIAL\n");
		}if(code == MHZ19_NO_MEASSURING_AVAILABLE){
		printf("[CO2 Sensor]: MHZ19_NO_MEASSURING_AVAILABLE\n");
		}if(code == MHZ19_PPM_MUST_BE_GT_999){
		printf("[CO2 Sensor]: MHZ19_PPM_MUST_BE_GT_999\n");
	}
}

void create_co2_task(co2_t* self)
{
	xTaskCreate(
	co2_task
	,  "CO2 Task"
	,  configMINIMAL_STACK_SIZE
	,  self
	,  1
	,  NULL );
}
void co2_task( void* pvParameters)
{
	TickType_t xLastWakeTime = xTaskGetTickCount();
	co2_t co2_sensor = *((co2_t*)(pvParameters));
	for(;;)
	{
		vTaskDelay(pdMS_TO_TICKS(50UL));
		co2_measure();
		xTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(60UL)); // 10 ms
		co2_get_data(co2_sensor);
		//	printf("\n [CO2 Sensor]:  %d \n", co2_get_average(co2_sensor));
		//	printf("[CO2 Sensor]: Average for last %d measurements is %d\n", get_measurements(), get_average_co2());
		
		//printf("[CO2 Sensor]: Value: %d, Threshold: %d, Surpassed: %d", get_value(), get_threshold(), threshold_surpassed());
// 		if(co2_threshold_surpassed(co2_sensor))
// 		{
// 			rc_servo(100);
// 			add_to_payload(1,8,NULL,0);
// 			// START SERVO
// 			//printf("\n[CO2 Sensor]: Threshold of %d ppm surpassed\n", get_threshold());
// 			}else{
// 			rc_servo(-100);
// 		}
		
		add_to_payload(co2_get_average(co2_sensor), 0,1, NULL);
		vTaskDelay(pdMS_TO_TICKS(4000UL)); // 500 ms
	}
}