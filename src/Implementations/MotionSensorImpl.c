#include "../Headers/MotionSensor.h"

typedef struct motion
{
	hcsr501_p hcsr501Inst;
}motion;

motion_t motion_create(hcsr501_p motion_sensor)
{
	motion_t _new_motion = (motion_t)calloc(1,sizeof(motion));
	if (NULL == _new_motion)
	return NULL;
	_new_motion->hcsr501Inst = NULL;
	_new_motion->hcsr501Inst = motion_sensor;
	if ( NULL == _new_motion->hcsr501Inst )
		return NULL;
	return _new_motion;
}

void motion_destroy(motion_t self)
{
	hcsr501_destroy(self->hcsr501Inst);
	if (NULL != self)
	free(self);
}

bool detecting(motion_t self)
{
	if ( hcsr501_isDetecting(self->hcsr501Inst) )
	{
		vTaskDelay(pdMS_TO_TICKS(10UL));
		return true;
	}
	else
	{
		vTaskDelay(pdMS_TO_TICKS(10UL));
		return false;
	}
}

void create_motion_task(motion_t* self)
{
	xTaskCreate(
	motion_task
	,  "Motion sensor task"  // A name just for humans
	,  configMINIMAL_STACK_SIZE  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  self
	,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

void motion_task(void *pvParameters)
{
	motion_t motion_sensor = *((motion_t*) pvParameters);
	if(motion_sensor != NULL)
	{
		for(;;)
		{
			if(detecting(motion_sensor))
			{
				add_to_payload(1, 8,255, 1);
			}
			else
			{
				add_to_payload(0, 8,255, 1);
			}
			vTaskDelay(pdMS_TO_TICKS(4000UL)); // 1000 ms
		}
	}
}