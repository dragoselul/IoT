#include "../Headers/TempAndHum.h"

typedef struct tempAndHum
{
	uint16_t humidity;
	uint16_t temperature;
	average_t average_hum;
	average_t average_temp;
}tempAndHum;

int latch;

tempAndHum_t tempAndHum_create()
{
	tempAndHum_t _new_tempAndHum = (tempAndHum_t)calloc(1,sizeof(tempAndHum));
	if (NULL == _new_tempAndHum)
		return NULL;
	if (HIH8120_OK != hih8120_initialise())
		return NULL;
	_new_tempAndHum->humidity = 0;
	_new_tempAndHum->temperature = 0;
	_new_tempAndHum->average_hum = average_create();
	_new_tempAndHum->average_temp = average_create();
	latch = 0;
	return _new_tempAndHum;
}

void tempAndHum_destroy(tempAndHum_t self)
{
	if (NULL != self)
	free(self);
}

void update_averages(tempAndHum_t self){
	calculate_average(self->humidity, self->average_hum);
	calculate_average(self->temperature, self->average_temp);
}

void reset_averages(tempAndHum_t self){
	average_destroy(self->average_hum);
	average_destroy(self->average_temp);
	self->average_hum = average_create();
	self->average_temp = average_create();
	latch = 0;
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
	//Made a latch to ignore the first 3 values read because they are very weird and they spike up the average
	if(latch > 3)
		update_averages(self);
	else
		latch ++;
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
	return get_average(self->average_temp, false);
}

uint16_t get_average_hum(tempAndHum_t self){
	return get_average(self->average_hum, false);
}