#include "../Headers/LED.h"

void LED_turn_on(){
	PORTL |= (1 << PL2);
}
void LED_turn_off(){
	PORTL &= ~(1 << PL2);
}