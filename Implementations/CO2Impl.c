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
	printf("[CO2 Sensor]: Value - %d\n", ppm);
	val = ppm;
}

void co2_initialize(){
	mh_z19_initialise(ser_USART3);
	mh_z19_injectCallBack(mhz19_callback);
}

void take_measuring(){
	log_errors(mh_z19_takeMeassuring());
}

uint16_t get_value(){
	printf("Value at get_value() -  %d\n",val,END);
	return val;
}

void log_errors(mh_z19_returnCode_t code){
	if(code == MHZ19_OK){
		printf("[CO2 Sensor]: MHZ19_OK\n",END);
	}if(code == MHZ19_NO_SERIAL){
		printf("[CO2 Sensor]: MHZ19_NO_SERIAL\n",END);
	}if(code == MHZ19_NO_MEASSURING_AVAILABLE){
	printf("[CO2 Sensor]: MHZ19_NO_MEASSURING_AVAILABLE\n",END);
	}if(code == MHZ19_PPM_MUST_BE_GT_999){
	printf("[CO2 Sensor]: MHZ19_PPM_MUST_BE_GT_999\n",END);
	}
}