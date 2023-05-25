#include "../Headers/LED.h"
#include <avr/io.h>

void LED_initialize(){
	DDRL |= (1 << DDL2);
}

void LED_turn_on(){
	PORTL |= (1 << PL2);
}
void LED_turn_off(){
	PORTL &= ~(1 << PL2);
}