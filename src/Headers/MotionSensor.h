#include "stdint.h"
#include <stdlib.h>
typedef struct motion* motion_t;

motion_t motion_create();
void motion_destroy(motion_t self);
bool detecting(motion_t self);