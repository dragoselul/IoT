#include "../Headers/Light.h"

typedef struct light
{
	uint16_t _tmp;
	float _lux;
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
	return _new_light;
}

void light_destroy(light_t self)
{
	if (NULL != self)
	free(self);
}

bool get_light_data(light_t self)
{
	if ( TSL2591_OK != tsl2591_enable() )
		return false;
	vTaskDelay(pdMS_TO_TICKS(60UL));
	if ( TSL2591_OK == tsl2591_fetchData() ) 
	{
		vTaskDelay(pdMS_TO_TICKS(10UL));
		self->_tmp = _tmp;
		self->_lux = _lux*100.0;
	}
	if ( TSL2591_OK != tsl2591_disable())
		return false;
	return true;
}

uint16_t get_tmp(light_t self)
{
	return self->_tmp;
}
uint16_t get_lux(light_t self)
{
	return (uint16_t)self->_lux;
}

void create_light_task(light_t* self)
{
	xTaskCreate(
	light_task
	,  "Light Task"
	,  configMINIMAL_STACK_SIZE
	,  self
	,  1 
	,  NULL );
}
void light_task( void* pvParameters)
{
	TickType_t xLastWakeTime = xTaskGetTickCount();
	light_t light_sensor = *((light_t*)(pvParameters));
	if(light_sensor != NULL)
	{
		for(;;)
		{
			if(get_light_data(light_sensor))
			{
				add_to_payload(get_lux(light_sensor), 6,7, NULL);
				vTaskDelay(pdMS_TO_TICKS(4000UL)); // 4000 ms
			}
		}
	}
}


/*
bool light_threshold_surpassed(light_t self){
	return threshold_surpassed(self->threshold, self->_lux);
}

uint16_t light_get_threshold(light_t self){
	return get_threshold(self->threshold);
}

void light_set_threshold(light_t self, uint16_t val){
	set_threshold(self->threshold, val);
}*/
