#include "hih8120.h"
#include "../Headers/TempAndHum.h"
#include <stdlib.h>
#include "../Headers/Average.h"

typedef struct tempAndHum
{
	uint16_t humidity;
	uint16_t temperature;
	Average average_hum;
	Average average_temp;
}tempAndHum;

tempAndHum_t tempAndHum_create()
{
	tempAndHum_t _new_tempAndHum = (tempAndHum_t)calloc(1,sizeof(tempAndHum));
	if (NULL == _new_tempAndHum)
		return NULL;
	if (HIH8120_OK != hih8120_initialise())
		return NULL;
	_new_tempAndHum->humidity = 0;
	_new_tempAndHum->temperature = 0;
	_new_tempAndHum->average_hum = {0,0};
	_new_tempAndHum->average_temp = {0,0};
	return _new_tempAndHum;
}

void tempAndHum_destroy(tempAndHum_t self)
{
	if (NULL != self)
	free(self);
}

void update_averages(tempAndHum_t self){
	self->average_hum.measurements += 1;
	self->average_hum.current_average = (uint16_t)(float)((self->average_hum.current_average * (self->average_hum.measurements-1)) + self->humidity) / self->average_hum.measurements;
	self->average_temp.measurements += 1;
	self->average_temp.current_average = (uint16_t)(float)((self->average_temp.current_average * (self->average_temp.measurements-1)) + self->temperature) / self->average_temp.measurements;
}

void reset_averages(tempAndHum_t self){
	self->average_hum.measurements = 0;
	self->average_hum.current_average = 0;
	self->average_temp.measurements = 0;
	self->average_temp.current_average = 0;
}

bool measure_temp_hum(tempAndHum_t self)
{
	if ( HIH8120_OK != hih8120_wakeup())
		return false;
	vTaskDelay(60/portTICK_PERIOD_MS);
	if ( HIH8120_OK !=  hih8120_measure())
		return false;
	vTaskDelay(10/portTICK_PERIOD_MS);
	self->humidity = hih8120_getHumidityPercent_x10();
	self->temperature = hih8120_getTemperature_x10();
	update_averages(self);
	return true;
}

float get_humidity_float()
{
	return hih8120_getHumidity();
}
float get_temperature_float()
{
	return hih8120_getTemperature();
}
uint16_t get_humidity_int(tempAndHum_t self)
{
	return self->humidity;
}
uint16_t get_temperature_int(tempAndHum_t self)
{
	return self->temperature;
}

uint16_t get_average_temp(tempAndHum_t self){
	return self->average_temp.current_average;
}

uint16_t get_average_hum(tempAndHum_t self){
	return self->average_hum.current_average;
}