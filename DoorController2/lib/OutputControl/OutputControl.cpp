#include <OutputControl.h>

OutputControl::OutputControl(){}


bool OutputControl::init(int pin, ExpanderControl * ioExpander, bool startLow, bool isReversePolarity){
    _myPin.pin = pin;
    _myPin.isRevPolarity = isReversePolarity;
    _myPin.isHigh = false;
    _isExpander = false;

    if (ioExpander!=nullptr){
        // log_v("Will init expdr pin: %d", pin);
        _myExpander = ioExpander; 
        _isExpander = true;
    }
    

    if (_isExpander){
        if (startLow){
            if (_myPin.isRevPolarity) _myExpander->digitalWrite(_myPin.pin , HIGH);
            else _myExpander->digitalWrite(_myPin.pin , LOW);
        
        }else{
            if (_myPin.isRevPolarity) _myExpander->digitalWrite(_myPin.pin , LOW);
            else _myExpander->digitalWrite(_myPin.pin , HIGH);
        }
        _myExpander->pinMode(_myPin.pin, OUTPUT);


    }else{
        if (startLow){
            if (_myPin.isRevPolarity) digitalWrite(_myPin.pin , HIGH);
            else digitalWrite(_myPin.pin , LOW);
        
        }else{
            if (_myPin.isRevPolarity) digitalWrite(_myPin.pin , LOW);
            else digitalWrite(_myPin.pin , HIGH);
        }
        pinMode(_myPin.pin, OUTPUT);
    }

    return true;
}



void OutputControl::switchState(){
    debugPrint(F("[OutputControl] Switch State "), 2);

    if (_myPin.isHigh){ //--> switch to LOW
        debugPrint(F("to LOW"),2);        
        if (_myPin.isRevPolarity) {
            if (!_isExpander) digitalWrite(_myPin.pin, HIGH);
            else _myExpander->digitalWrite(_myPin.pin, HIGH);
            
        }
        else {
            if (!_isExpander) digitalWrite(_myPin.pin, LOW);
            else _myExpander->digitalWrite(_myPin.pin, LOW);
        }

        _myPin.isHigh = false;

    } else {  //--> switch to HIGH
        debugPrint(F("to HIGH"), 2);
        if (_myPin.isRevPolarity) {
            if (!_isExpander) digitalWrite(_myPin.pin, LOW);
            else _myExpander->digitalWrite(_myPin.pin, LOW);

        }else {
            if (!_isExpander) digitalWrite(_myPin.pin, HIGH);
            else _myExpander->digitalWrite(_myPin.pin, HIGH);
        }

        _myPin.isHigh = true;
    }
}



int8_t OutputControl::getFunctionQueueSize(){
    return _functionQueueCounter;
}



bool OutputControl::turnHigh(unsigned long delayToLow){
    _isBlinking = false;
    if (_myPin.isHigh) return false;
    debugPrint(F("[OutputControl] HIGH"), 2);
    if (_myPin.isRevPolarity) {
        if (!_isExpander) digitalWrite(_myPin.pin,LOW);
        else _myExpander->digitalWrite(_myPin.pin, LOW);
    }
    else {
        if (!_isExpander) digitalWrite(_myPin.pin,HIGH);
        else _myExpander->digitalWrite(_myPin.pin, HIGH);
    }

    _myPin.isHigh = true;

    if (delayToLow > 0){
        _delayToSwitch = delayToLow;
        _lastSwitchingTime = millis();
    }

    return true;
}



bool OutputControl::turnLow(unsigned long delayToHigh){
    _isBlinking = false;
    if (!_myPin.isHigh) return false;
    debugPrint(F("[OutputControl] LOW"),2);
    if (_myPin.isRevPolarity) {
        if (!_isExpander) digitalWrite(_myPin.pin,HIGH);
        else _myExpander->digitalWrite(_myPin.pin, HIGH);
    }
    else {
        if (!_isExpander) digitalWrite(_myPin.pin,LOW);
        else _myExpander->digitalWrite(_myPin.pin, LOW);
    }

    _myPin.isHigh = false;

    if (delayToHigh > 0){
        _delayToSwitch = delayToHigh;
        _lastSwitchingTime = millis();
    }

    return true;
}



void OutputControl::resetFunctionQueue(){
    _functionQueueCounter = 0;
}


void OutputControl::poll() {
    //take care of delays if not blinking
    unsigned long aux_time_now = millis();
    if (!_isBlinking){
        if ( (aux_time_now - _lastSwitchingTime >= _delayToSwitch) && (_delayToSwitch!=0) ){
            switchState();
            _delayToSwitch = 0;
        } else{
            //take care of delay a bit between functions to understand each one
            if (_functionQueueCounter!=0){
                if (_isDelay){
                    debugPrint(F("[OutputControl] Delaying1"),2);
                    _delayStartTime = aux_time_now;
                    _isDelay = false;
                }else if(aux_time_now-_delayStartTime >= OP_FUNCTION_QUEUE_DELAY_MS){
                    callNextFunctionInQueue();
                    _isDelay = true;
                }
            }
        }

    }else{
        if (_timesToBlink == 0){
           debugPrint(F("[OutputControl] Stop blink"),2);
            _isBlinking = false;
            if (_functionQueueCounter!=0){
                if (_isDelay){
                    debugPrint(F("[OutputControl] Delaying2"),2);
                    _delayStartTime = aux_time_now;
                    _isDelay = false;
                }else if(aux_time_now-_delayStartTime >= OP_FUNCTION_QUEUE_DELAY_MS){
                    callNextFunctionInQueue();
                    _isDelay = true;
                }
            }
        }
        else if ( (millis() - _lastSwitchingTime >= _millisBetweenBlinks) ){
            char buffer[100];
            snprintf(buffer,100,"[OutputControl] Blink. Times Left: %d\n", _timesToBlink);
            debugPrint(buffer,2);
            switchState();
            if (_timesToBlink != -1) _timesToBlink--; //otherwise blink forever until turnON() or turnOff() are called
            _lastSwitchingTime = _lastSwitchingTime + _millisBetweenBlinks;
        }
    }
}


bool OutputControl::isHigh(){
    return _myPin.isHigh;
}

bool OutputControl::isBlinking(){
    return _isBlinking;
}


void OutputControl::blinkXTimes(int16_t times, float frequencyHz, bool isLastStateLow){
    char buffer[100];
    snprintf(buffer, 100, "[OutputControl] blink times:%d | freq %.2f\n",times,frequencyHz);
    debugPrint(buffer,1);

    _delayToSwitch = 0;                     //unregister switch callback
    _isBlinking = true;                     //register blink action

    if (_myPin.isHigh) switchState();       //always start LOW

    //blink forever
    if (times == -1){
        debugPrint("[OutputControl] blink forever\n", 1);
        _timesToBlink = -1;

    }else{                           
        if (isLastStateLow){
            _timesToBlink = (2*times);      //1 blink = HIGH + LOW.
        }else{
            _timesToBlink = (2*times) + 1;  //1 blink = HIGH + LOW (+ last time HIGH)
        }
    }

    _millisBetweenBlinks = 0.5*(1/frequencyHz)*1000;
    
    snprintf(buffer,100,"[OutputControl] Millis between blinks: %.2f\n",_millisBetweenBlinks);
    debugPrint(buffer,2);
    _lastSwitchingTime = millis();
}


void OutputControl::blinkXTimesBlocking(int16_t times, float frequencyHz, bool isLastStateLow){
    char buffer[100];
    snprintf(buffer,100,"[OutputControl] Will blink block times:%d | freq %.2f\n",times,frequencyHz);
    debugPrint(buffer,1);

    _delayToSwitch = 0;                     //unregister switch callback
    _isBlinking = true;                     //register blink action

    if (_myPin.isHigh) switchState();       //always start LOW
                           
    if (isLastStateLow){
        _timesToBlink = (2*times);          //1 blink = HIGH + LOW.
    }else{
        _timesToBlink = (2*times) + 1;      //1 blink = HIGH + LOW (+ last time HIGH)
    }

    _millisBetweenBlinks = 0.5*(1/frequencyHz)*1000;

    for (uint16_t i =0 ; i<_timesToBlink; i++){
        switchState();
        delay(_millisBetweenBlinks);
    }

    _isBlinking = false;  
}

void OutputControl::blinkXTimesDifTimesBlocking(int16_t times, uint16_t millisOn, uint16_t millisOff, bool isLastStateLow){
    char buffer[100];
    snprintf(buffer,100,"[OutputControl] Block Blink times:%d TimeON:%d | TimeOFF:%d\n", times, millisOn, millisOff);
    debugPrint(buffer,1);

    _delayToSwitch = 0;                     //unregister switch callback
    _isBlinking = true;                     //register blink action

    if (_myPin.isHigh) switchState();       //always start LOW
                           
    _timesToBlink = (times);          //1 blink = HIGH + LOW.

    for (uint16_t i =0 ; i<_timesToBlink; i++){
        switchState();
        delay(millisOn);
        switchState();
        delay(millisOff);
    }

    if(!isLastStateLow){
        switchState();
    }

    _isBlinking = false;  
}


void OutputControl::callNextFunctionInQueue(){
    if (_functionQueueCounter==0){
        return;
    }
    char buffer[150];
    snprintf(buffer,150,"[OutputControl] Calling queue: %d | arg1: %d | arg2: %d\n\n\n",(int) _functionQueue[0], _functionQueueFirstArg[0], _functionQueueSecArg[0]);
    debugPrint(buffer,1);

    switch(_functionQueue[0]){
        case OP_FUNCTION_CODE::TURNHIGH:{     //1
            debugPrint("[OutputControl] TURNHIGH\n",3);
            if (_functionQueueFirstArg[0] == -1) turnHigh();
            else turnHigh(_functionQueueFirstArg[0]);
            break;
        }

        case OP_FUNCTION_CODE::TURNLOW:{      //0
            debugPrint("[OutputControl] TURNLOW\n",3);
            if (_functionQueueFirstArg[0] == -1) turnLow();
            else turnLow(_functionQueueFirstArg[0]);
            break;
        }

        case OP_FUNCTION_CODE::SWITCHSTATE:{  //2
            debugPrint("[OutputControl] SWITCHSTATE\n",3);
            switchState();
            break;
        }

        case OP_FUNCTION_CODE::BLINKXTIMES:{  //3
            debugPrint("[OutputControl] BLINKXTIMES\n",3);
            if (_functionQueueFirstArg[0] == -1 && _functionQueueSecArg[0]==-1 )  blinkXTimes();
            else if (_functionQueueFirstArg[0] != -1 && _functionQueueSecArg[0]==-1) blinkXTimes(_functionQueueFirstArg[0]);
            else blinkXTimes(_functionQueueFirstArg[0], _functionQueueSecArg[0]);
            break;
        }
    }

    //shift array elements to left dropping first one
    for (int i=0; i<_functionQueueCounter; i++){
        _functionQueueFirstArg[i] = _functionQueueFirstArg[i+1];
        _functionQueueSecArg[i] = _functionQueueSecArg[i+1];
        _functionQueue[i] = _functionQueue[i+1];
    }
    _functionQueueCounter--;
}

void OutputControl::insertInQueue(OP_FUNCTION_CODE functionCode, int16_t firstArg, int16_t secArg){
    if( _functionQueueCounter>=  OP_FUNCTIONQUEUEMAXLEN ){
        debugPrint(F("[OutputControl] Queue ERR\n"),1);
        return;
    }
    char buffer[100];
    snprintf(buffer,100,"[OutputControl] Insert Queue: %d - 1stArg=%d  | 2ndArg=%d\n", (int)functionCode, (int)firstArg, (int) secArg);
    debugPrint(buffer,1);

    _functionQueue[_functionQueueCounter] = functionCode;
    _functionQueueFirstArg[_functionQueueCounter] = firstArg;
    _functionQueueSecArg[_functionQueueCounter] = secArg;
    _functionQueueCounter++;
}

void OutputControl::debugPrint(String stringToPrint, uint8_t verboseLvl){
    #ifdef OP_VERBOSE_LVL
    if (OP_VERBOSE_LVL>=verboseLvl) Serial.print(stringToPrint);
    #endif
}