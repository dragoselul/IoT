#include "../Headers/Threshold.h"

typedef struct threshold
{
	int16_t temperature_threshold;
	uint16_t humidity_threshold;
	uint16_t light_threshold;
	uint16_t co2_threshold;
	bool automatic_lights;
} threshold;

threshold_t threshold_create(){
	threshold_t _new_threshold = (threshold_t) calloc(1,sizeof(threshold));
	if(NULL == _new_threshold){
		return NULL;
	}
	_new_threshold -> temperature_threshold = 1000;
	_new_threshold -> humidity_threshold = 1000;
	_new_threshold -> light_threshold = 180;
	_new_threshold -> co2_threshold = 1000;
	return _new_threshold;
}
void threshold_destroy(threshold_t self){
	if(NULL != self){
		free(self);
	}
}

int16_t get_temperature_threshold(threshold_t* thresholds)
{
	return (*thresholds)->temperature_threshold;
}
uint16_t get_humidity_threshold(threshold_t* thresholds)
{
	return (*thresholds)->humidity_threshold;
}
uint16_t get_co2_threshold(threshold_t* thresholds)
{
	return (*thresholds)->co2_threshold;
}
uint16_t get_light_threshold(threshold_t* thresholds)
{
	return (*thresholds)->light_threshold;
}
bool get_automatic_lights(threshold_t* thresholds){
	return (*thresholds)->automatic_lights;
}

void set_temperature_threshold(threshold_t* thresholds, int16_t new_threshold)
{
	(*thresholds)->temperature_threshold = new_threshold;
	//printf("temperature -> %d | %d\n", (*thresholds)->temperature_threshold, new_threshold);
}
void set_humidity_threshold(threshold_t* thresholds, uint16_t new_threshold)
{
	(*thresholds)->humidity_threshold = new_threshold;
	//printf("humidity -> %d | %d\n", (*thresholds)->humidity_threshold, new_threshold);
}
void set_co2_threshold(threshold_t* thresholds, uint16_t new_threshold)
{
	(*thresholds)->co2_threshold = new_threshold;
	//printf("co2 -> %d | %d\n", (*thresholds)->co2_threshold, new_threshold);
}
void set_light_threshold(threshold_t* thresholds, uint16_t new_threshold)
{
	(*thresholds)->light_threshold = new_threshold;
	//printf("light -> %d | %d\n", (*thresholds)->light_threshold, new_threshold);
}

void set_automatic_lights(threshold_t* thresholds, bool turnOn){
	(*thresholds)->automatic_lights = turnOn;
}