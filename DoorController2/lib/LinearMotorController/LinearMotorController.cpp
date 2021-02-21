#include <LinearMotorController.h>


#define LMC_PIN_FW myOut1
#define LMC_PIN_RV myOut2


LinearMotorController::LinearMotorController(unsigned long stretchTimeoutMs ){
    if (stretchTimeoutMs) _stretchTimeoutMs = stretchTimeoutMs;
}

void LinearMotorController::init(unsigned long stretchTimeoutMs ){
    if (stretchTimeoutMs) _stretchTimeoutMs = stretchTimeoutMs;
    assert(_stretchTimeoutMs);
    log_d("Init OK. Waiting for calibration");

}

bool LinearMotorController::moveToPos(uint8_t percentPosition){
    if (!_isCalibrated){
        log_e("Calibration Required");
        return false;
    }

    if (_linearMotorState == lmc_state::mov){
        log_e("Ongoing Mov");
        return false;
    }


    if (percentPosition>100) percentPosition = 100;
    else if (percentPosition<0) percentPosition = 0;

    if (percentPosition == _currentPercentPosition){
        log_e("Current Position ERR");
        return false;
    }

    _currMovDuration = calcMovDuration(abs((int8_t)_currentPercentPosition - (int8_t)percentPosition ));
    log_d("Will move to %d% | duration %u ", percentPosition, _currMovDuration);
    _linearMotorState = lmc_state::mov;
    _movStMillis = millis();
    _nextStopMillis = _movStMillis+_currMovDuration;
    percentPosition > _currentPercentPosition ? activateMovFw() : activateMovRv();

    return true;
}



bool LinearMotorController::moveIncrement(int8_t percentIncrement){
    if (_linearMotorState == lmc_state::mov){
        log_e("Ongoing Mov ERR");
        return false;
    }

    if (!_isCalibrated){
        int16_t endPos = (int16_t)_currentPercentPosition+(int16_t)percentIncrement;
        
        if ( (_linearMotorState!=lmc_state::undefined) &&  (endPos > 100 || endPos<0) ){
            log_e("Invalid End Pos %d", endPos);
            return false;
        }
    }

    _currMovDuration = calcMovDuration(abs(percentIncrement));
    _linearMotorState = lmc_state::mov;
    _movStMillis = millis();
    _nextStopMillis = _movStMillis+_currMovDuration;
    _lastPercentIncrement = percentIncrement;
    percentIncrement>0 ? activateMovFw() : activateMovRv();
    return true;
}



void LinearMotorController::setZero(){
    _currentPercentPosition = 0;
    _linearMotorState = lmc_state::stopped;
    _isCalibrated = true;
}



void LinearMotorController::poll(){

    if (_linearMotorState!= _linearMotorStatePrevious){
        log_i("State Change: %d", _linearMotorState);
        _linearMotorStatePrevious  = _linearMotorState;
    }

    switch (_linearMotorState){
    case lmc_state::undefined:
        /* wait for calibration */
        break;

    case lmc_state::stopped:
        /* wait for mov */
        break;
        
    case lmc_state::mov:
        if ( millis()-_movStMillis>=_nextStopMillis){
            stop();
            _isCalibrated ?  _linearMotorState = lmc_state::stopped : _linearMotorState = lmc_state::undefined;
            if (_onMovEndCb) _onMovEndCb(_currentPercentPosition);
        }
        break;
    }

}


unsigned long LinearMotorController::calcMovDuration(uint8_t percentIncrement){
    return percentIncrement*_stretchTimeoutMs/100;
}


void LinearMotorController::stop(){
    log_d("STOP");
    LMC_PIN_FW.turnLow();
    LMC_PIN_RV.turnLow();
    if (!_isCalibrated){
        return;
    }

    unsigned long aux_timeNow = millis();
    int8_t increment  = ((aux_timeNow-_movStMillis)/_stretchTimeoutMs);
    _lastPercentIncrement>0 ? _currentPercentPosition += increment : _currentPercentPosition -= increment;
    log_i("Current Position is %d", _currentPercentPosition);
    _nextStopMillis = 0;
    
}


void LinearMotorController::setMovEndCb(generalUintFunction_t cb){
    _onMovEndCb = cb;
}


void LinearMotorController::activateMovFw(){
    log_d("Mov FW");
    LMC_PIN_RV.turnLow();
    LMC_PIN_FW.turnHigh();
}


void LinearMotorController::activateMovRv(){
    log_d("Mov FW");
    LMC_PIN_FW.turnLow();
    LMC_PIN_RV.turnHigh();
}

bool LinearMotorController::open(){
    return moveToPos(100);
}

bool LinearMotorController::close(){
    return moveToPos(0);
}


uint8_t LinearMotorController::getCurrPos(){
    return _currentPercentPosition;
}