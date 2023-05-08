#include "../Headers/Light.h"

typedef struct light
{
	uint16_t _tmp;
	float _lux;
	average_t average_light;
	threshold_t threshold;
}light;

int latch;
uint16_t _tmp;
float _lux;

void tsl2591Callback(tsl2591_returnCode_t rc)
{
	switch (rc)
	{
		case TSL2591_DATA_READY:
		if ( TSL2591_OK == (rc = tsl259_getVisibleRaw(&_tmp)) )
		{
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Visible overflow - change gain and integration time\n");
		}
		
		if ( TSL2591_OK == (rc = tsl2591_getLux(&_lux)) )
		{
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Lux overflow - change gain and integration time\n");
		}
		break;
		
		case TSL2591_OK:
		// Last command performed successful
		break;
		
		case TSL2591_DEV_ID_READY:
		// Dev ID now fetched
		break;
		
		default:
		break;
	}
}

light_t light_create()
{
	light_t _new_light = (light_t)calloc(1,sizeof(light));
	if (NULL == _new_light)
		return NULL;
	if (TSL2591_OK != tsl2591_initialise(tsl2591Callback))
		return NULL;
	_new_light->_tmp = 0;
	_new_light->_lux = 0;
	_new_light->threshold = threshold_create();
	latch = 0;
	_new_light->average_light = average_create();
	return _new_light;
}
void light_destroy(light_t self)
{
	if (NULL != self)
	free(self);
}

void update_average_light(light_t self){
	float aux_lux = self->_lux;
	uint16_t lux = (uint16_t)aux_lux;
	calculate_average(lux, self->average_light);
}

uint16_t get_average_light(light_t self){
	return get_average(self->average_light);
}

void reset_average_light(light_t self)
{
	average_destroy(self->average_light);
	self->average_light = average_create();
}

bool get_light_data(light_t self)
{
	if ( TSL2591_OK != tsl2591_enable() )
		return false;
	vTaskDelay(60/portTICK_PERIOD_MS);
	if ( TSL2591_OK == tsl2591_fetchData() ) 
	{
		vTaskDelay(10/portTICK_PERIOD_MS);
		self->_tmp = _tmp;
		self->_lux = _lux*100.0;
		if(latch > 3)
			update_average_light(self);
		else
			latch++;	
	}
	if ( TSL2591_OK != tsl2591_disable())
		return false;
	return true;
}

uint16_t get_tmp(light_t self)
{
	return self->_tmp;
}
float get_lux(light_t self)
{
	return self->_lux;
}

bool light_threshold_surpassed(light_t self){
	return threshold_surpassed(self->threshold, self->_lux);
}

uint16_t light_get_threshold(light_t self){
	return get_threshold(self->threshold);
}

void light_set_threshold(light_t self, uint16_t val){
	set_threshold(self->threshold, val);
}
