#include "../Headers/TempAndHum.h"

typedef struct tempAndHum
{
	uint16_t humidity;
	int16_t temperature;
	average_t average_hum;
	average_t average_temp;
	threshold_t threshold_hum;
	threshold_t threshold_temp;
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
	_new_tempAndHum->threshold_hum = threshold_create();
	_new_tempAndHum->threshold_temp = threshold_create();
	latch = 0;
	return _new_tempAndHum;
}

void tempAndHum_destroy(tempAndHum_t self)
{
	hih8120_destroy();
	if (NULL != self)
	free(self);
}

void update_averages(tempAndHum_t self){
	if(self == NULL)
		return;
	calculate_average(self->humidity, self->average_hum);
	calculate_average(self->temperature, self->average_temp);
}

void reset_averages(tempAndHum_t self){
	if(self==NULL)
		return;
	average_destroy(self->average_hum);
	average_destroy(self->average_temp);
	self->average_hum = average_create();
	self->average_temp = average_create();
	latch = 0;
}

bool measure_temp_hum(tempAndHum_t self)
{
	if(self == NULL)
		return false;
	if ( HIH8120_OK != hih8120_wakeup())
		return false;
	vTaskDelay(pdMS_TO_TICKS(60));
	if ( HIH8120_OK !=  hih8120_measure())
		return false;
	vTaskDelay(pdMS_TO_TICKS(10));
	self->humidity = hih8120_getHumidityPercent_x10();
	self->temperature = hih8120_getTemperature_x10();
	//Made a latch to ignore the first 3 values read because they are very weird and they spike up the average
	if(latch > 3)
		update_averages(self);
	else
		latch ++;
	return true;
}

void create_temp_hum_task(tempAndHum_t *self)
{
	if(self == NULL)
		return;
	xTaskCreate
	(
		temp_hum_task,
		"Temperature and Humidity task",
		configMINIMAL_STACK_SIZE,
		self,
		1,
		NULL
	);
}

void temp_hum_task( void *pvParameters )
{
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		if(measure_temp_hum(*((tempAndHum_t*)(pvParameters))))
		{
			//adding to payload
		}
		xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(4000));
	}
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
	if(self == NULL)
		return 0;
	return self->humidity;
}
int16_t get_temperature_int(tempAndHum_t self)
{
	if(self == NULL)
		return 0;
	return self->temperature;
}

int16_t get_average_temp(tempAndHum_t self){
	if(self == NULL)
		return 0;
	return get_average(self->average_temp);
}

uint16_t get_average_hum(tempAndHum_t self){
	if(self == NULL)
		return 0;
	return get_average(self->average_hum);
}

bool hum_threshold_surpassed(tempAndHum_t self){
	return threshold_surpassed(self->threshold_hum, self->humidity);
}
uint16_t hum_get_threshold(tempAndHum_t self){
	return get_threshold(self->threshold_hum);
}
void hum_set_threshold(tempAndHum_t self, uint16_t val){
	set_threshold(self->threshold_hum, val);
}

bool temp_threshold_surpassed(tempAndHum_t self){
	return threshold_surpassed(self->threshold_temp, self->temperature);
}
uint16_t temp_get_threshold(tempAndHum_t self){
	return get_threshold(self->threshold_temp);
}
void temp_set_threshold(tempAndHum_t self, uint16_t val){
	set_threshold(self->threshold_temp, val);
}