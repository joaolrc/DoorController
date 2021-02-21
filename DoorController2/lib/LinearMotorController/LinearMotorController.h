/**
 \file LinearMotorController.h
 * General library for controlling DC motors with focus for Linear actuators. Possibility to add stop trigger via encoder, ISR or current sense
 * \author João Costa, Quinta da Grimpa
 * \date Jan 2021
*/
#include <Arduino.h>
#include <OutputControl.h>
#include <PayninoStd.h>

#ifndef LINEAR_MOTOR_CONTROLLER_H
#define LINEAR_MOTOR_CONTROLLER_H

#define LMC_DEFAULT_INCREMENTAL_SPEED_PERCENT 100
#define LMC_MOVDIR_FW 0
#define LMC_MOVDIR_RV 1

typedef void (*movementCb_t)(void);

typedef enum lmc_state_t{
    undefined = 0,
    stopped,
    mov
}lmc_state;



class LinearMotorController{

public:

    LinearMotorController( unsigned long stretchTimeoutMs = 0);

    void init(unsigned long stretchTimeoutMs=0 );

    bool setPosition(int8_t percentPosition, movementCb_t afterMovementCb=nullptr, uint8_t speedPercent= LMC_DEFAULT_INCREMENTAL_SPEED_PERCENT);

    bool open();

    bool close();

    void setZero();

    void setMovEndCb(generalUintFunction_t cb);

    bool moveToPos(uint8_t percentPosition );

    bool moveIncrement(int8_t percentIncrement =1);

    void poll();

    void stop();
    
    uint8_t getCurrPos();



private:

   uint8_t _pinForward;
   uint8_t _pinReverse;
   unsigned long _stretchTimeoutMs;
   generalUintFunction_t _onMovEndCb = nullptr;
   bool _isCalibrated = false;

   uint8_t _currentPercentPosition = 0;
   uint8_t _lastPercentIncrement;

   
   lmc_state_t _linearMotorState = lmc_state::undefined;
   lmc_state_t _linearMotorStatePrevious = lmc_state::undefined;

   OutputControl outputCtrlF;
   OutputControl outputCtrlR;
   unsigned long _movStMillis;
   unsigned long _currMovDuration;
   unsigned long _nextStopMillis = 0;

   void activateMovFw();

   void activateMovRv();

   unsigned long calcMovDuration(uint8_t percentIncrement);


};






#endif /* LINEAR_MOTOR_CONTROLLER_H */