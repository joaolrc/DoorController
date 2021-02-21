#include <DallasTemperature.h>

tempSensor_t myTempSensor;


bool tempSensor_t::initTempSensor(uint8_t oneWirePin, onValidTemp_t onTemp, byte resolution ){
    myTempSensor = {OneWire(oneWirePin), -3, {0,0,0,0,0,0,0,0}, DT_TEMP_INIT_VAL, dt_pollState::init_conv};
    myTempSensor.onNewT = onTemp;
    for (uint8_t i=0; i<DT_MOV_AVG_LEN; i++){
        myTempSensor.tempArr[i]= DT_TEMP_INIT_VAL;
    }
    uint8_t aCtr = 0;
    while (myTempSensor.type<-1){
        myTempSensor.type = _discoverTempSensor(myTempSensor.sensor, myTempSensor.addr);
        aCtr++;
        if (aCtr>=5){
            break;
        }
    }
    
    if (myTempSensor.type>=0){
        log_i("Found Temp Sensor");
        if (resolution!=0xFF) setResolution(resolution);
        return true;
    }else{
        log_e("Did Not found TempSensor");
        return false;
    }

}

int8_t _discoverTempSensor(OneWire &ds, byte addrBuff[8]){
    log_d("Scaning for OneWire Devices");

    if (!ds.search(addrBuff)){
        ds.reset_search();
        delay(250);
        return -10;
    }

    if (OneWire::crc8(addrBuff, 7) != addrBuff[7]){
        log_e("CRC is not valid");
        return -11;
    }

    int8_t retC;
    switch (addrBuff[0]){
        case 0x10:
            log_v("  Chip = DS18S20"); // or old DS1820
            retC = 1;
            break;
        case 0x28:
            log_v("  Chip = DS18B20");
            retC = 0;
            break;
        case 0x22:
            log_v("  Chip = DS1822");
            retC= 0;
            break;
        default:
            log_e("Device is not a DS18x20 family device.");
            retC =  -1;
            break;
    }
    ds.reset();
    return retC;
}

float tempSensor_t::getTemperature(){
    myTempSensor.lastTemp = _getTemperature(myTempSensor.sensor, myTempSensor.addr, myTempSensor.type);
    return myTempSensor.lastTemp;
}

float _getTemperature(OneWire &ds, byte* addr, int8_t type_s, bool isCelsius){
    byte data[12];
    if (!ds.reset()){
        log_e("Sensor Disconected from 1W Bus");
        return DT_TEMP_INIT_VAL;
    }
    ds.select(addr);
    ds.write(0x44, 0);

    while (ds.read()!=0xFF) yield();
    
    if (!ds.reset()){
        log_e("Sensor Disconnected");
        return DT_TEMP_INIT_VAL;
    }
    ds.select(addr);    
    ds.write(0xBE);         // Read Scratchpad
    
    for ( uint8_t i = 0; i < 9; i++) {           // we need 9 bytes
        data[i] = ds.read();
   
    }
    log_v("RCV: %x", data);
    log_d("CRC= 0x%x", OneWire::crc8(data, 8));
   

    int16_t raw = (data[1] << 8) | data[0];
    if (type_s) {
        raw = raw << 3; // 9 bit resolution default
        if (data[7] == 0x10) {
        // "count remain" gives full 12 bit resolution
        raw = (raw & 0xFFF0) + 12 - data[6];
        }
    }else{
        byte cfg = (data[4] & 0x60);
        //zero undef bits
        if (cfg == 0x00) raw = raw & ~7;  // 9 bit res
        else if (cfg == 0x20) raw = raw & ~3; // 10 bit res
        else if (cfg == 0x40) raw = raw & ~1; // 11 bit res
        // default is 12 bit
    }
    
    myTempSensor.lastTemp = (float)raw / 16.0;

    if (isCelsius){
        return myTempSensor.lastTemp ;
    } else{
        return float (myTempSensor.lastTemp  * 1.8 + 32.0);
    }
}



float getLastTemp(){
    return myTempSensor.lastTemp;
}

uint8_t tempSensor_t::poll(){
    unsigned long aux_time_now = millis();
    switch(pollState){
        case dt_pollState::init_conv:{
            if (!sensor.reset()){
                log_e("Sensor Disconected");
            }else{
                sensor.select(addr);
                sensor.write(DT_INITIATE_CONVERSION, 0);
                _lastStateChangeMs = aux_time_now;
                pollState++;
            }
            break;
        }

        case dt_pollState::readTemp:{
            if (sensor.read()!=0xff){       //Conversion not ready yet, try next poll
                if (aux_time_now-_lastStateChangeMs>=DT_TEMP_REQ_TIMEOUT_MS) pollState = dt_pollState::init_conv;
                break;
            }
            
            if (!sensor.reset()){
                log_e("Sensor Disconected");
                pollState=dt_pollState::init_conv;
                break;
            }
      
            log_v("ConvT: %lu\n", aux_time_now-_lastStateChangeMs);
            sensor.select(addr);
            sensor.write(DT_READ_SCRATCHPAD);
            byte data[9];
            log_v("Will get data");
            sensor.read_bytes(data, 9);
            // if CRC not match, req temp again
            if (OneWire::crc8(data, 8)!=data[8]){
                log_w("CRC ERR");
                pollState = dt_pollState::init_conv;
                break;
            }
            
            log_v("CRC OK");

            if (type) { //9 bit res
                log_d("DS18S20");
                int16_t dataRaw = (((int16_t) data[0] << 11) | (((int16_t) data[1]) << 3));
                lastTemp = (float)(dataRaw-0.25+( (int16_t)(data[7]-data[6]) / data[7]))/128.0f;
                
            }else{
                log_v("DS18B20");
                log_v("TH: %d  | TL: %d\n", data[2], data[3]);
                int16_t dataRaw = (data[1] << 8) + data[0];     //LSB 1st
                byte resolution = (data[4] & 0x60);  //get resolution (bits 7 and 8)
                log_v("Using res= %x", resolution);
                // at lower res, the low bits are undefined, so let's zero them
                if (resolution == 0x00) dataRaw = dataRaw & ~7;  // ~7=000   9 bit resolution, 93.75 ms
                else if (resolution == 0x20) dataRaw = dataRaw & ~3; //~3=00   10 bit res, 187.5 ms
                else if (resolution == 0x40) dataRaw = dataRaw & ~1; // 11 bit res, 375 ms
                lastTemp = (float)dataRaw/16.0f;
            }

            //call cb
            log_d("New Temp: %.2f\n", lastTemp );
            if (onNewT!=NULL) {
                onNewT(lastTemp);
            }
            _lastStateChangeMs = aux_time_now;
            sensor.depower();
            pollState=dt_pollState::clean;

            break;
        }

        case dt_pollState::clean:{
            if ( aux_time_now - _lastStateChangeMs>= DT_MIN_TIME_BETWEEN_MEASURES_MS){
                _lastStateChangeMs = aux_time_now;
                pollState = dt_pollState::init_conv;
            }
            
            break;
        }

        default:{
            log_e("Should Not be reached");
            pollState = dt_pollState::init_conv;
            break;
        }
    }

    return pollState;
}


bool tempSensor_t::setResolution(byte res){
    if (type!=0){
        log_w("Sensor res is not mutable");
        return false;
    }
    if (res!=DT_RESOLUTION_9BIT && res!=DT_RESOLUTION_10BIT && res!=DT_RESOLUTION_11BIT && res!=DT_RESOLUTION_12BIT){
        log_w("Invalid sensor res");
        return false;
    }

    log_v("Will set res %x", res);
    sensor.reset();
    sensor.select(addr);
    sensor.write(DT_WRITE_SCRATCHPAD);
    sensor.write(0x64); //100ªC
    sensor.write(0x00); //0ªC
    sensor.write(res);
    return true;
}