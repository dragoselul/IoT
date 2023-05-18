#include "../Headers/Servo.h"

// SERVO JC14 = 0, JC13 = 1
void open_door(){
	rc_servo_setPosition(1, 100);
}

void close_door(){
	rc_servo_setPosition(1, -100);
}