#include "stdint.h"
#include "mh_z19.h"

uint16_t val;

void mhz19_callback(uint16_t ppm);
void co2_initialize();
//co2_t co2_create();
//void co2_destroy(co2_t self);
void take_measuring();
void get_measuring();
uint16_t get_value();
void logErrors(mh_z19_returnCode_t code);
