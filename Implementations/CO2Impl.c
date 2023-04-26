/*
TODO When connecting to LoRaWAN, change com port in mh_z19_initialise() method
*/

#include "../Headers/CO2.h"
#include <mh_z19.h>
#include <stdlib.h>
#include <stdio.h>

uint16_t val;
Average average = {0, 0};
	
void updateAverage(uint16_t newVal){
	average.measurements += 1;
	average.current_average = (uint16_t)(float)((average.current_average * (average.measurements-1)) + newVal) / average.measurements;
}

uint16_t get_measurements(){
	return average.measurements;
}

uint16_t get_average(){
	return average.current_average;
}

void resetAverage(){
	average.measurements = 0;
	average.current_average = 0;
}


void mhz19_callback(uint16_t ppm){
	val = ppm;
	updateAverage(ppm);
}

void co2_initialize(){
	mh_z19_initialise(ser_USART3);
	mh_z19_injectCallBack(mhz19_callback);
}

void take_measuring(){
	//log_errors(mh_z19_takeMeassuring());
	mh_z19_takeMeassuring();
}

uint16_t get_value(){
	return val;
}

void log_errors(mh_z19_returnCode_t code){
	if(code == MHZ19_OK){
		printf("[CO2 Sensor]: MHZ19_OK\n");
	}if(code == MHZ19_NO_SERIAL){
		printf("[CO2 Sensor]: MHZ19_NO_SERIAL\n");
	}if(code == MHZ19_NO_MEASSURING_AVAILABLE){
	printf("[CO2 Sensor]: MHZ19_NO_MEASSURING_AVAILABLE\n");
	}if(code == MHZ19_PPM_MUST_BE_GT_999){
	printf("[CO2 Sensor]: MHZ19_PPM_MUST_BE_GT_999\n");
	}
}