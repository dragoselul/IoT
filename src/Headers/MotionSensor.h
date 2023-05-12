#include "hcSr501.h"
#include <stdlib.h>
#include <avr/io.h>

typedef struct motion* motion_t;

motion_t motion_create();
void motion_destroy(motion_t self);
bool detecting(motion_t self);