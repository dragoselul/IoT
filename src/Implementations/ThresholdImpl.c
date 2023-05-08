#include "../Headers/Threshold.h"

typedef struct threshold
{
	uint16_t val;	
} threshold;

threshold_t threshold_create(){
	threshold_t _new_threshold = (threshold_t) calloc(1,sizeof(threshold));
	if(NULL == _new_threshold){
		return NULL;
	}
	_new_threshold -> val = -1;
	return _new_threshold;
}
void threshold_destroy(threshold_t self){
	if(NULL != self){
		free(self);
	}
}
bool threshold_surpassed(threshold_t self, uint16_t val){
	if(self->val != -1 && self->val < val)
		return true;
	return false;
}
uint16_t get_threshold(threshold_t self){
	return self->val;
}

void set_threshold(threshold_t self, uint16_t val){
	self->val = val;
}
