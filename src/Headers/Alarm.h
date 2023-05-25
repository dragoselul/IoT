#include <stdlib.h>
#include <stdio.h>

// ALARM PIN - 11 on J15 (Timers)

// Initializes the PIN where the alarm is connected as OUTPUT
void alarm_initialize();

// Sets the PIN where the alarm is connected to 1
void alarm_turn_on();

// Sets the PIN where the alarm is connected to 0
void alarm_turn_off();