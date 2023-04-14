/*
 * TempAndHumImpl.c
 *
 * Created: 4/12/2023 12:45:41 PM
 *  Author: drago
 */ 
#include "hih8120.h"
#include "TempAndHum.h"

typedef struct tempAndHum
{
	float humidityF;
	float temperatureF;
}tempAndHum;

tempAndHum_t tempAndHum_create()
{
	tempAndHum_t _new_tempAndHum = (tempAndHum_t)calloc(1,sizeof(tempAndHum));
	if (NULL == _new_tempAndHum)
		return NULL;
	if (HIH8120_OK != hih8120_initialise())
		return NULL;
	_new_tempAndHum->humidityF = 0.0;
	_new_tempAndHum->temperatureF = 0.0;
	return _new_tempAndHum;
}

void tempAndHum_destroy(tempAndHum_t self)
{
	if (NULL != self)
	free(self);
}

bool wakeup_sensor()
{
	if ( HIH8120_OK != hih8120_wakeup() )
	{
		return false;
	}
	return true;
}

bool measure_temp_hum()
{
	if ( HIH8120_OK !=  hih8120_measure() )
	{
		return false;
	}
	return true;
}

float get_humidity_float(tempAndHum_t self)
{
	self->humidityF = hih8120_getHumidity();
	return self->humidityF;
}
float get_temperature_float(tempAndHum_t self)
{
	self->temperatureF = hih8120_getTemperature();
	return self->temperatureF;
}
uint16_t get_humidity_bytes()
{
	return hih8120_getHumidityPercent_x10();
}
uint16_t get_temperature_bytes()
{
	return hih8120_getTemperature_x10();
}