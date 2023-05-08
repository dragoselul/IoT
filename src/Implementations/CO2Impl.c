#include "../Headers/CO2.h"

typedef struct co2
{
	uint16_t val;
	average_t average_co2;		
	uint16_t threshold;
} co2;

uint16_t val;

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
	_new_co2->threshold = -1;
	_new_co2->average_co2 = average_create();
	
	return _new_co2;
}
void co2_destroy(co2_t self){
	if (NULL != self)
	free(self);
}
void co2_measure()
{
	mh_z19_takeMeassuring();
}
void co2_get_data(co2_t self){
	self->val = val;
}

uint16_t co2_get_value(co2_t self){
	co2_update_average(self);
	return self->val;
}
uint16_t co2_get_average(co2_t self){
	return get_average(self->average_co2);
}
void co2_reset_average(co2_t self){
	average_destroy(self->average_co2);
	self->average_co2 = average_create();
}
void co2_update_average(co2_t self){
	calculate_average(val, self->average_co2);
}

bool co2_threshold_surpassed(co2_t self){
	return threshold_surpassed(self->threshold, self->val);
} 
uint16_t co2_get_threshold(co2_t self){
	return get_threshold(self->threshold);
}
void co2_set_threshold(co2_t self, uint16_t newThreshold){
	set_threshold(self, newThreshold);
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