#include "../Headers/CO2.h"
#include "../Headers/Threshold.h"

typedef struct co2
{
	uint16_t val;
	float avg_co2;
	uint8_t measurements;
	threshold_t* th_point;
} co2;

mh_z19_returnCode_t rc;


co2_t co2_create(threshold_t* point){
	co2_t _new_co2 = (co2_t) calloc (1, sizeof(co2));
	if(NULL == _new_co2)
		return NULL;
	mh_z19_initialise(ser_USART3);
	rc = MHZ19_NO_MEASSURING_AVAILABLE;
	_new_co2->val = 0;
	_new_co2->th_point = point;
	_new_co2->avg_co2 = 0.0;
	_new_co2->measurements = 0;
	return _new_co2;
}
void co2_destroy(co2_t self){
	if (NULL != self)
	free(self);
}

bool co2_get_data(co2_t self){
	if(mh_z19_takeMeassuring() == MHZ19_OK){
		vTaskDelay(pdMS_TO_TICKS(50UL));
		if(mh_z19_getCo2Ppm(&self->val) == MHZ19_OK)
		{
			vTaskDelay(pdMS_TO_TICKS(10UL));
			co2_update_average(self);
			return true;
		}
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
	if(self == NULL)
		return;
	self->avg_co2 = 0.0;
	self->measurements = 0;
}
void co2_update_average(co2_t self){
	if(self == NULL)
		return;
	if(self->measurements <= 8)
	{
		self->avg_co2 = self->val;
		return;
	}
	self->avg_co2 = self->avg_co2 + (self->val - self->avg_co2) / (self->measurements + 1);
}

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
	co2_t co2_sensor = *((co2_t*)pvParameters);
	threshold_t co2_th = *(co2_sensor->th_point);
	for(;;)
	{
		co2_get_data(co2_sensor);
		if(get_co2_threshold(&co2_th) < co2_sensor->val)
		{
			alarm_turn_on();
			rc_servo(100);
			add_to_payload(1,8,255,0);
			
			// START SERVO
			printf("CO2 Threshold surpassed : %d \n", co2_sensor->val);
		}
		else
		{
			alarm_turn_off();
			rc_servo(-100);
		}	
		add_to_payload(co2_get_average(co2_sensor), 0,1, 255);
		vTaskDelay(pdMS_TO_TICKS(4000UL)); // 500 ms
	}
}