#include "../Headers/Alarm.h"

alarm_turn_on(){
	PORTL |= (1 << PL0);
}

alarm_turn_off(){
	PORTL &= ~(1 << PL0);
}