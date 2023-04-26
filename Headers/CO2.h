#include "stdint.h"
#include "mh_z19.h"

typedef struct {
	uint16_t current_average;
	uint16_t measurements;
} Average;

Average average;

uint16_t get_average();
uint16_t get_measurements();
uint16_t reset_average();
void co2_initialize();
void take_measuring();
uint16_t get_value();
