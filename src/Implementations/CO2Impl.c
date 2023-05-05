/*
TODO When connecting to LoRaWAN, change com port in mh_z19_initialise() method
*/

#include "../Headers/CO2.h"
#include <mh_z19.h>
#include <stdlib.h>
#include <stdio.h>
#include "../Headers/Average.h"

uint16_t threshold = -1;
uint16_t val;
average_t average_co2;
	
bool threshold_surpassed(){
	return threshold != -1 && val > threshold;
}

uint16_t get_threshold(){
	return threshold;
}

void set_threshold(uint16_t newThreshold){
	threshold = newThreshold;
}
	
void updateAverage(uint16_t newVal){
	calculate_average(newVal, average_co2);
}

uint16_t get_average_co2(){
	return get_average(average_co2);
}

void reset_average_co2(){
	average_destroy(average_co2);
	average_co2 = average_create();
}


void mhz19_callback(uint16_t ppm){
	val = ppm;
	updateAverage(ppm);
}

void co2_initialize(){
	mh_z19_initialise(ser_USART3);
	mh_z19_injectCallBack(mhz19_callback);
	average_co2 = average_create();
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