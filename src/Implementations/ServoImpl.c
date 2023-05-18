#include "../Headers/Servo.h"

// SERVO JC14 = 0, JC13 = 1
void rc_servo(uint16_t percentage){
	rc_servo_setPosition(1, percentage);
}