#include "../Headers/TempAndHum.h"

typedef struct tempAndHum
{
	uint16_t humidity;
	int16_t temperature;
	float avg_humidity;
	float avg_temperature;
	uint8_t measurements;
	threshold_t threshold_hum;
	threshold_t threshold_temp;
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
	_new_tempAndHum->avg_temperature = 0.0;
	_new_tempAndHum->avg_humidity = 0.0;
	_new_tempAndHum->measurements = 0;
	_new_tempAndHum->threshold_hum = threshold_create();
	_new_tempAndHum->threshold_temp = threshold_create();
	return _new_tempAndHum;
}

void tempAndHum_destroy(tempAndHum_t* self)
{
	hih8120_destroy();
	if (NULL != self)
	free(*self);
}

void update_averages(tempAndHum_t self){
	if(self == NULL)
		return;
	if(self->measurements <= 4)
	{
		self->avg_temperature = self->temperature;
		self->avg_humidity = self->humidity;
		return;
	}
	self->avg_temperature = self->avg_temperature + (self->temperature - self->avg_temperature) / (self->measurements + 1);
	self->avg_humidity = self->avg_humidity + (self->humidity - self->avg_humidity) / (self->measurements + 1);
}

void reset_averages(tempAndHum_t self){
	if(&self == NULL)
		return;
	self->avg_temperature = 0.0;
	self->avg_humidity = 0.0;
	self->measurements = 0;
}

bool measure_temp_hum(tempAndHum_t self)
{
	if(NULL == self)
		return false;
	if ( HIH8120_OK != hih8120_wakeup())
		return false;
	vTaskDelay(pdMS_TO_TICKS(60UL));
	if ( HIH8120_OK !=  hih8120_measure())
		return false;
	vTaskDelay(pdMS_TO_TICKS(10UL));
	self->humidity = hih8120_getHumidityPercent_x10();
	self->temperature = hih8120_getTemperature_x10();
	update_averages(self);
	self->measurements++;
	return true;
}

int16_t get_average_temp(tempAndHum_t self)
{
	if(self == NULL)
		return 0;
	return (int16_t)self->avg_temperature;
}

uint16_t get_average_hum(tempAndHum_t self)
{
	if(self == NULL)
		return 0;
	return (uint16_t)self->avg_humidity;
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
	tempAndHum_t temp_hum = *((tempAndHum_t*)(pvParameters));
	for(;;)
	{
		if(measure_temp_hum(temp_hum))
		{
			//comment the payloads if you want to test them.
			add_to_payload(get_average_temp(temp_hum),2,3,NULL);
			add_to_payload(get_average_hum(temp_hum),4,5,NULL);
		}
		xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(4000UL));
	}
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