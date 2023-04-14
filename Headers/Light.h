/*
 * Light.h
 *
 * Created: 4/13/2023 1:19:34 PM
 *  Author: drago
 */ 
typedef struct light* light_t;

light_t light_create();
void light_destroy(light_t);
bool power_up_sensor();
bool power_down_sensor();
uint16_t get_light_data();