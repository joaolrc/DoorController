/*Module to Interface Dalas series of temperature sensors. Currently only sensor is supported simultaneously:
-  DS18S20  - Not Tested
-  DS18B20  - Tested
-  DS1822   - Not tested

João Costa, Oct 2020 Qibixx Ag
*/


#ifndef DALLAS_TEMPERATURE_H
#define DALLAS_TEMPERATURE_H

#include <Arduino.h>
#include <OneWire.h>
#include <CountinoPins.h>
#include <functional>

#define DT_TEMP_INIT_VAL -300
#define DT_MOV_AVG_LEN  10
#define DT_MIN_TIME_BETWEEN_MEASURES_MS 1000  //when using poll(), define here a delay between measurements to not overheat sensor
#define DT_TEMP_REQ_TIMEOUT_MS  2000
 
#define DT_INITIATE_CONVERSION 0x44
#define DT_READ_SCRATCHPAD  0xBE
#define DT_WRITE_SCRATCHPAD 0x4E

#define DT_RESOLUTION_9BIT 0x1F
#define DT_RESOLUTION_10BIT 0x3F
#define DT_RESOLUTION_11BIT 0x5F
#define DT_RESOLUTION_12BIT 0x7F

enum dt_pollState { 
    init_conv = 0, 
    readTemp,
    clean
};

typedef void (*onValidTemp_t)(float);

// typedef std::function<void(float)> onValidTemp_t;  // callback type if activity sensed

struct tempSensor_t{
    OneWire sensor;             //one wire bus instance (maybe use pointer to enable multiple sensors)
    int8_t type;                //sensor type
    byte addr[8];               //sensor addr
    float lastTemp;             //last Temp measured
    uint8_t pollState;     
    float tempArr[DT_MOV_AVG_LEN];     //mov avg temp array
    /*Fills tempsensor struct, calls discoverTempSensor() and saves type and addr of sensor found to myTempSensor struct. returns true if any sensor found on OneWire bus */
    bool initTempSensor(uint8_t oneWirePin = COUNTINOPIN_SCL, onValidTemp_t onTemp=NULL, byte resolution=0xFF);
    /*Read Temp from sensor, returns it as float and stores it in lastTemp */
    float getTemperature();
    /*Polls sensor for temperature, saves it in array and gets avg*/
    uint8_t poll();
    unsigned long _lastStateChangeMs;
    // onValidTemp_t *onNewT;
    onValidTemp_t onNewT;
    bool setResolution(byte res);
};

extern tempSensor_t myTempSensor;    // = {OneWire(COUNTINOPIN_SCL), -1, {0,0,0,0,0,0,0,0}, DT_TEMP_INIT_VAL};

/*discover sensors on the OneWire Bus. Returns sensor type or -1 if did not match any. Pass addrBuff to be filled with sensor address*/
int8_t _discoverTempSensor(OneWire &ds, byte addrBuff[8]);

//read sensor and return
float _getTemperature(OneWire &sensor, byte* addr, int8_t type_s, bool isCelsius = true); 

// get last valid measure
float getLastTemp();



#endif