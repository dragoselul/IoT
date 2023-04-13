/*
 * TempAndHum.h
 *
 * Created: 4/12/2023 12:29:07 PM
 *  Author: dragos
 */ 
typedef struct tempAndHum* tempAndHum_t;

tempAndHum_t tempAndHum_create();
void tempAndHum_destroy(tempAndHum_t);
bool wakeup_sensor();
bool measure_temp_hum();
float get_humidity_float(tempAndHum_t);
float get_temperature_float(tempAndHum_t);
uint16_t get_humidity_bytes();
uint16_t get_temperature_bytes();