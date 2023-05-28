#include "../Headers/Servo.h"

void servo_initialize(){
	rc_servo_initialise();
	servo_close();
}

void servo_open(){
	rc_servo_setPosition(1, 100);
}

void servo_close(){
	rc_servo_setPosition(1, -100);
}