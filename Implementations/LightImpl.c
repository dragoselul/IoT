/*
 * LightImpl.c
 *
 * Created: 4/13/2023 2:04:26 PM
 *  Author: drago
 */ 
#include "../Headers/Light.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct light
{
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
			printf("Light tmp: %d", _tmp);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Visible overflow - change gain and integration time\n");
		}
		
		if ( TSL2591_OK == (rc = tsl2591_getLux(&_lux)) )
		{
			printf("Light lux: %f", _lux);
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
	_tmp = 0;
	_lux = 0.00;
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
	if ( TSL2591_OK == tsl2591_disable())
	{
		return true;
	}
	return false;
}

void get_light_data(light_t self)
{
	if ( TSL2591_OK == tsl2591_fetchData() ) 
	{
	
	}
	else 
	{
		_tmp = 0;
		_lux = 0.00;	
	}
}

uint16_t get_tmp(light_t self)
{
	return _tmp;
}
float get_lux(light_t self)
{
	return _lux;
}
