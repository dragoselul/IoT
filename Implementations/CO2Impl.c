#include "../Headers/CO2.h";
#include <mh_z19.h>

uint16_t co2val;
mh_z19_returnCode_t rc;

void callback(uint16_t ppm){
	co2val = ppm;
}

// CALL BEFORE VTaskStartScheduler()
void initialize(uint8_t com_port){
    mh_z19_initialise(ser_USART3);
    mh_z19_injectCallBack(callback);
}

bool takeMeasuring(){
    rc = mh_z19_takeMeassuring();
    if(rc != MHZ19_OK){
        return false;
    }
    return true;

}
bool getCO2Ppm(uint16_t * ppm){
    mh_z19_getCo2Ppm(ppm);
}

bool setAutoCalibration(bool on){
    rc = mh_z19_setAutoCalibration(on);
    if(rc != MHZ19_OK){
        return false;
    }
    return true;
}
bool calibrateZeroPoint(){
    rc = mh_z19_calibrateZeroPoint();
    if(rc != MHZ19_OK){
        return false;
    }
    return true;
}
bool calibrateSpanPoint(uint16_t ppm){
    rc = mh_z19_calibrateSpanPoint(ppm);
    if(rc != MHZ19_OK){
        return false;
    }
    return true;
}