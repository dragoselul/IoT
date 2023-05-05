#include "stdint-gcc.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct average* average_t;

average_t average_create();
void average_destroy(average_t);
void calculate_average(uint16_t, average_t);
uint16_t get_average(average_t);
