#include "../Headers/Sound.h"

typedef struct sound
{
	uint16_t _sound;
}sound;

sound_t sound_create(){
	sound_t _new_sound = (sound_t)calloc(1,sizeof(sound));
	if (NULL == _new_sound)
		return NULL;
	sen14262_initialise(); 
	return _new_sound;
}

bool get_sound_gate_state()
{
	return sen14262_gate();
}

void sound_destroy(sound_t self)
{
	if (NULL != self)
		free(self);
}

bool soundDetection(sound_t self)
{
	if(self == NULL)
		return false;
	self->_sound = sen14262_envelope();
	if (self->_sound > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void create_sound_task(sound_t* self)
{
	if(self == NULL)
		return;
	xTaskCreate(
	sound_task
	,  "Sound sensor task"
	,  configMINIMAL_STACK_SIZE
	,  self
	,  1  
	,  NULL );
}

void sound_task( void *pvParameters )
{
	sound_t sound_sensor = *((sound_t*)pvParameters);
	
	if(sound_sensor != NULL)
	{
		for(;;)
		{
			if(soundDetection(sound_sensor))
			{
				add_to_payload(1, 8,255, 2);
			}
			else
			{
				add_to_payload(0, 8,255, 2);
			}
			vTaskDelay(pdMS_TO_TICKS(4000UL)); // 10 ms
		}
	}
}
