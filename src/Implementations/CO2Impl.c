#include "../Headers/CO2.h"

mh_z19_returnCode_t rc = MHZ19_NO_MEASSURING_AVAILABLE;

co2_t co2_create(threshold_t* point){
	co2_t _new_co2 = (co2_t) calloc (1, sizeof(co2));
	if(NULL == _new_co2)
		return NULL;
	mh_z19_initialise(ser_USART3);
	_new_co2->val = 0;
	_new_co2->th_point = point;
	_new_co2->avg_co2 = 0.0;
	_new_co2->measurements = 0;
	servo_open_co2 = false;
	return _new_co2;
}

void co2_destroy(co2_t self){
	if (NULL != self){
		return;
	}
	free(self);
}

bool co2_get_data(co2_t self){
	if(mh_z19_takeMeassuring() == MHZ19_OK){
		vTaskDelay(pdMS_TO_TICKS(50UL));
		if(mh_z19_getCo2Ppm(&self->val) == MHZ19_OK){
			vTaskDelay(pdMS_TO_TICKS(10UL));
			co2_evaluate_threshold(self);
			co2_update_average(self);
			return true;
		}
	}
	return false;
}

void co2_evaluate_threshold(co2_t self){
	if(get_co2_threshold(self->th_point) < self->val && servo_open_humidity == false){
		alarm_turn_on();
		servo_open();
		servo_open_co2 = true;
		add_to_payload(1,8,255,0);
	}
	else{
		alarm_turn_off();
		servo_close();
	}
}

uint16_t co2_get_value(co2_t self){
	return self->val;
}

uint16_t co2_get_average(co2_t self){
	return (uint16_t)self->avg_co2;
}

void co2_reset_average(co2_t self){
	if(self == NULL){
		return;
	}
	self->avg_co2 = 0.0;
	self->measurements = 0;
}

void co2_update_average(co2_t self){
	if(self == NULL){
		return;
	}
	uint8_t co2Measurements = self->measurements;
	self->avg_co2 = (self->avg_co2 * self->measurements + self->val) / ++co2Measurements;
	self->measurements = co2Measurements;
}

void create_co2_task(co2_t* self){
	xTaskCreate(
	co2_task
	,  "CO2 Task"
	,  configMINIMAL_STACK_SIZE
	,  self
	,  1
	,  NULL );
}

void co2_task( void* pvParameters){
	co2_t co2_sensor = *((co2_t*)pvParameters);
	for(;;){
		co2_get_data(co2_sensor);
		add_to_payload(co2_get_average(co2_sensor), 0,1, 255);
		if(co2_reset == true)
		{
			co2_reset_average(co2_sensor);
			co2_reset = false;
		}
		vTaskDelay(pdMS_TO_TICKS(4000UL)); // 500 ms
	}
}