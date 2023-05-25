#include "../Headers/Alarm.h"
#include <avr/io.h>

void alarm_initialize(){
	DDRL |= (1 << DDL0);
}


void alarm_turn_on(){
	PORTL |= (1 << PL0);
}


void alarm_turn_off(){
	PORTL &= ~(1 << PL0);
}