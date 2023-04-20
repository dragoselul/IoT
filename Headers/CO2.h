#include "stdint.h";
#include "mh_z19.h";

typedef struct CO2;

void initialize(uint8_t com_port);
bool takeMeasuring();
bool getCO2Ppm();
bool setAutoCalibration(bool on);
bool calibrateZeroPoint();
bool calibrateSpanPoint();