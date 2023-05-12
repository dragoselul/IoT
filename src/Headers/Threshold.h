#include "stdint-gcc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct threshold* threshold_t;

threshold_t threshold_create();
void threshold_destroy(threshold_t);
bool threshold_surpassed(threshold_t, uint16_t);
uint16_t get_threshold(threshold_t);
void set_threshold(threshold_t, uint16_t);
