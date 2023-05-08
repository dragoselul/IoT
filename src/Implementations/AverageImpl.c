#include "../Headers/Average.h"

typedef struct average
{
	double current_average;
	uint16_t measurements;
} average;


average_t average_create()
{
	average_t _new_average = (average_t)calloc(1,sizeof(average));
	if (NULL == _new_average)
		return NULL;
	_new_average->current_average = 0.0;
	_new_average->measurements = 0;
	return _new_average;
}
void average_destroy(average_t self)
{
	if (NULL != self)
		free(self);
}
void calculate_average(uint16_t data, average_t self)
{
	self->current_average = (self->current_average*self->measurements + data)/(self->measurements+1);
	//self->current_average = self->current_average + (data - self->current_average) / (self->measurements + 1);
	self->measurements++;
}

uint16_t get_average(average_t self)
{
	double scaled_average = self->current_average;
	return (uint16_t)round(scaled_average);
}
uint16_t get_measurements(average_t self)
{
	return self->measurements;
}