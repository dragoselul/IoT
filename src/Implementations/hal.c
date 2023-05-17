/*
 * hal.c
 *
 * Created: 16/05/2023 10.23.14
 *  Author: matas
 */ 

#include "hal.h"
#include <avr/io.h>



typedef struct motion {
	hcsr501_p hcsr501Inst;
} motion_t;

motion_t motion_create() {
	motion_t new_motion = (motion_t)calloc(1, sizeof(struct motion));
	if (NULL == new_motion)
	return NULL;
	
	new_motion->hcsr501Inst = NULL;
	new_motion->hcsr501Inst = hcsr501_create(&PORTE, PE5);
	if (NULL == new_motion->hcsr501Inst) {
		free(new_motion);
		return NULL;
	}
	
	return new_motion;
}

void motion_destroy(motion_t self) {
	hcsr501_destroy();
	if (NULL != self)
	free(self);
	
}

bool detecting(motion_t self) {
	if (hcsr501_isDetecting(self->hcsr501Inst)) {
		return true;
	}
	else {
		return false;
	}
}