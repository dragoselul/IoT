/*
TODO When connecting to LoRaWAN, change com port in mh_z19_initialise() method
*/

#include "../Headers/CO2.h"
#include <mh_z19.h>
#include <stdlib.h>
#include <stdio.h>

uint16_t val;
mh_z19_returnCode_t rc;
char END = 3;

void mhz19_callback(uint16_t ppm){
	printf("[CO2 Sensor]: Value - %d", ppm);
	val = ppm;
}

/*
co2_t co2_create()
{
	co2_t _co2_sensor = (co2_t)calloc(1, sizeof(co2));
	if(_co2_sensor == NULL){
		printf("[CO2 Sensor]: Memory was not successfully allocated");
		return NULL;
	}
	mh_z19_initialise(ser_USART3);
	mh_z19_injectCallBack(mhz19_callback);
	_co2_sensor->_val = 0;
	return co2_sensor;
}*/

/*
void co2_destroy(co2_t self)
{
	if(self != NULL){
		free(self);
	}
}*/

void co2_initialize(){
	mh_z19_initialise(ser_USART3);
	mh_z19_injectCallBack(mhz19_callback);
}

void take_measuring(){
	logErrors(mh_z19_takeMeassuring());
}

void get_measuring(){
	logErrors(mh_z19_getCo2Ppm(&val));
}

uint16_t get_value(){
	printf("Value at get_value() -  %d%c",val,END);
	return val;
}

void logErrors(mh_z19_returnCode_t code){
	if(code == MHZ19_OK){
		printf("[CO2 Sensor]: MHZ19_OK%c",END);
	}if(code == MHZ19_NO_SERIAL){
		printf("[CO2 Sensor]: MHZ19_NO_SERIAL%c",END);
	}if(code == MHZ19_NO_MEASSURING_AVAILABLE){
	printf("[CO2 Sensor]: MHZ19_NO_MEASSURING_AVAILABLE%c",END);
	}if(code == MHZ19_PPM_MUST_BE_GT_999){
	printf("[CO2 Sensor]: MHZ19_PPM_MUST_BE_GT_999%c",END);
	}
}