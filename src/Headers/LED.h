#include <stdlib.h>
#include <stdio.h>

// LED PIN - 15 on J15 (Timers)

// Initializes the PIN where the LED is connected as OUTPUT
void LED_initialize();

// Sets the PIN where the LED is connected to 1
void LED_turn_on();

// Sets the PIN where the LED is connected to 0
void LED_turn_off();