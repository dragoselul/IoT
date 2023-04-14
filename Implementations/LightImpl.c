/*
 * LightImpl.c
 *
 * Created: 4/13/2023 2:04:26 PM
 *  Author: drago
 */ 
#include "tsl2591.h"
#include "Light.h"

typedef struct light
{
	uint16_t _tmp;
	float _lux;
}light;

void tsl2591Callback(tsl2591_returnCode_t rc, light_t self)
{
	uint16_t _tmp;
	float _lux;
	switch (rc)
	{
		case TSL2591_DATA_READY:
		if ( TSL2591_OK == (rc = tsl259_getVisibleRaw(&_tmp)) )
		{
			self->_tmp = _tmp;
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Visible overflow - change gain and integration time\n");
		}
		
		if ( TSL2591_OK == (rc = tsl2591_getLux(&_lux)) )
		{
			self->_lux = _lux;
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
	_new_light->_lux = 0.0;
	_new_light->_tmp = 0;
	return _new_light;
}
void light_destroy(light_t self)
{
	if (NULL != self)
	free(self);
}
bool power_up_sensor()
{
	if ( TSL2591_OK == tsl2591_enable() )
	{
		return true;
	}
	return false;
}
bool power_down_sensor()
{
	if ( TSL2591_OK == tsl2591_destroy())
	{
		return true;
	}
	return false;
}

light_t get_light_data(light_t self)
{
	if ( TSL2591_OK != tsl2591_fetchData() )
	{
		return NULL;
	}
	else
	{
		tsl2591Callback(TSL2591_DATA_READY, self);
		return self
	}
}
