#include "../Headers/Light.h"
#include "../Headers/Average.h"

typedef struct light
{
	uint16_t _tmp;
	float _lux;
	average_t average_light;
}light;

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
	_new_light->average_light.current_average = 0;
	_new_light->average_light.measurements = 0;
	return _new_light;
}
void light_destroy(light_t self)
{
	if (NULL != self)
	free(self);
}

// VALUES ARE WEIRD
void update_average_light(light_t self){
	self->average_light.measurements += 1;
	self->average_light.current_average = (uint16_t)(float)((self->average_light.current_average * (self->average_light.measurements-1)) + self->_lux) / self->average_light.measurements;
}

uint16_t get_average_light(light_t self){
	return self->average_light.current_average;
}

void reset_average_light(light_t self)
{
	self->average_light.measurements = 0;
	self->average_light.current_average = 0;
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
		self->_lux = _lux*100;
		update_average_light(self);
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
