/*
 * hal.h
 *
 * Created: 16/05/2023 10.24.02
 *  Author: matas
 */ 

#pragma once
#include <stdint.h>
#include "hcSr501.h"
#include "../Headers/MotionSensor.h"
#include <stdlib.h>

#include <ATMEGA_FreeRTOS.h>

motion_t motion_create();
