#include "../Headers/MotionSensor.h"

typedef struct motion
{
	hcsr501_p hcsr501Inst;
} motion;

motion_t motion_create(hcsr501_p motion_sensor){
	motion_t _new_motion = (motion_t)calloc(1,sizeof(motion));
	if (NULL == _new_motion){
        return NULL;
	}
    _new_motion->hcsr501Inst = motion_sensor;
    if (NULL == _new_motion->hcsr501Inst){
        return NULL;
	}
	return _new_motion;
}

void motion_destroy(motion_t* self){
	if (self != NULL && *self != NULL){
		hcsr501_destroy((*self)->hcsr501Inst);
		free(*self);
	}
}

bool detecting(motion_t self){
	if(self == NULL){
		return false;
	}
	if ( hcsr501_isDetecting(self->hcsr501Inst)){
		vTaskDelay(pdMS_TO_TICKS(10UL));
		return true;
	}
	else{
		vTaskDelay(pdMS_TO_TICKS(10UL));
		return false;
	}
}

void create_motion_task(motion_t* self){
	if(self == NULL){
		return;
	}
	
	xTaskCreate(
	motion_task // function the task will execute
	,  "Motion sensor task"
	,  configMINIMAL_STACK_SIZE
	,  self
	,  1  // Priority (3 being the highest)
	,  NULL );
}

void motion_task(void *pvParameters){
	motion_t motion_sensor = *((motion_t*) pvParameters);
	if(motion_sensor != NULL){
		for(;;){
			if(detecting(motion_sensor)){
				// Send "motion detected" to payload
				add_to_payload(1, 8,255, 1);
			}
			else{
				// Send "motion not detected" to payload
				add_to_payload(0, 8,255, 1);
			}
			vTaskDelay(pdMS_TO_TICKS(4000UL)); // 4000 ms
		}
	}
}