/* 
Simple library for controlling an Output without blocking the code execution.
Possibility to handle Reverse Polarity Pin (eg. to use with PNP transistor) - Not tested
WARNING - all time related events will NOT work on SETUP and rely on OutputControl::poll() to be executed, therefore, some blocking methods are provided to handle such situations.
Possibility to insert functions in queue (FIFO) which will be called in sequence with delay of OP_FUNCTION_QUEUE_DELAY_MS defined below. 
If you start filling the queue, functions start executing. To gain control again, one must reset it with resetFunctionQueue()

When queue has functions and you call eg BlinkXTimes directly, it will execute imidiately, stopping any function that may be executing and discarding them. Then the queue will resume. 
If any function was stopped for the blink to execute, it will not resume when blink is over. 
João Costa - São João da Azenha, 1 Feb 2020
*/

#ifndef OUTPUT_CONTROL_H
#define OUTPUT_CONTROL_H
#include <Arduino.h>
#include <functional>
#include <ExpanderControl.h>
#define IO_EXPDR_PIN_RELAY 2
#define IO_EXPDR_PIN_OUT1 0
#define IO_EXPDR_PIN_OUT2 1
#define IO_EXPDR_PIN_IO2 3


#define OP_FUNCTIONQUEUEMAXLEN 10
#define OP_FUNCTION_QUEUE_DELAY_MS 500
// #define OP_VERBOSE_LVL 0

//Queue Function Codes - When insertInQueue() --> this are the codes
enum class OP_FUNCTION_CODE{
    TURNLOW = 0,
    TURNHIGH,
    SWITCHSTATE,
    BLINKXTIMES,
};


typedef struct{
        uint8_t pin;
        bool isRevPolarity;
        bool isHigh;
    }GeneralPin_t;

class OutputControl
{
public:
    
    /**
     * Default Constructor. no code executed here
    */
    OutputControl(); 
   
    /**
     * Defines pin to use, sets direction and start value
     * @param pin Pin to be used by OutputControl instance
     * @param ioExpander If pin is connected to IO Expander (ETH board), pass address of the ExpanderControl instance. Otherwise pass nullptr
     * @param startLow True if start value is low for this pin
     * @param isReversePoilarity If pin has PNP mosfet between Board IO and chip IO,m set this to true and use it as if no mosfet is there
    */
    bool init(int pin, ExpanderControl * ioExpander=  nullptr,  bool startLow = true, bool isReversePolarity = false);

    /**
     * switches current state of GPIO. Action will take effect in  next poll()
    */
    void switchState();

    /**
     * Turns output HIGH. 
     * @param delayToLow Possibility to pass a timeout to switch back to LOW. returns false if gpio was already LOW
     * Actions will only take effect in next polls()
    */
    bool turnHigh(unsigned long delayToLow = 0);
    
    /**
     * Turns output LOW. 
     * @param delayToLow Possibility to pass a timeout to switch back to LOW. returns false if gpio was already LOW
     * Actions will only take effect in next polls()
    */
    bool turnLow(unsigned long delayToHigh = 0);

    /**
     * State machine like. Need to be called each loop() in order to make the requested actions
    */
    void poll();

    /**
     *returns true if GPIO is HIGH (LOW if isRevPolarity) 
    */
    bool isHigh();

    /**
     * returns true if pin is in blinking process
    */ 
    bool isBlinking();
    
    /**
     * Blink number of times without blocking code execution
     * @param times number of ON times
     * @param frequencyHz blink frequency
     * @param isLastSateLow if true, blink will end in LOW state. If false, end in HIGH state
    */
    void blinkXTimes(int16_t times = 5, float frequencyHz = 1, bool isLastStateLow = true);

    /**
     * Similar method to the one above but blocks code execution but does not rely on poll() for changes to take effect
     * If signage in setup, call this method. 
     * WARNING - Blocks code execution
     * @param times number of ON times
     * @param frequencyHz blink frequency
     * @param isLastSateLow if true, blink will end in LOW state. If false, end in HIGH state
    */
    void blinkXTimesBlocking(int16_t times = 5, float frequencyHz = 1, bool isLastStateLow = true);

    /**
     * Similar method to the one above but to blink with different ON and OFF times
     * Usefull to make odd signage
     * WARNING- function blocks code execution
     * @param times number of ON times
     * @param millisOn milliseconds ON
     * @param millisOff milliseconds OFF
     * @param isLastSateLow if true, blink will end in LOW state. If false, end in HIGH state
    */
    void blinkXTimesDifTimesBlocking(int16_t times, uint16_t millisOn, uint16_t millisOff, bool isLastStateLow = true);

    /**
     * Register LED function to be called when last one is over
     * @param functionCode  Which function to be executed. See OP_FUNCTION_CODE enum class
     * @param firstArg First argument that the called function takes - optional
     * @param secArg Second argument that the called function takes - optional
     * WARNING - function queue may have only up to 10 elements to be executed in polls()
    */
    void insertInQueue(OP_FUNCTION_CODE functionCode, int16_t firstArg = -1, int16_t secArg = -1);

    /**
     * If need to retake full controll of the output, and stop using queue, call this before directly using another function
    */
    void resetFunctionQueue();

    int8_t getFunctionQueueSize();

    



private:
    GeneralPin_t _myPin;
    ExpanderControl * _myExpander;
    bool _isExpander = false;

    unsigned long _delayToSwitch = 0;
    unsigned long _lastSwitchingTime = 0;
    unsigned long _delayStartTime;

    bool _isDelay = true;

    //needed for blinkXTimes()
    float _millisBetweenBlinks;
    //true if blinking 
    bool _isBlinking = false;
    //times left to blink if blinkXtimes was called
    int16_t _timesToBlink;
    OP_FUNCTION_CODE  _functionQueue[OP_FUNCTIONQUEUEMAXLEN];
    int16_t _functionQueueFirstArg[OP_FUNCTIONQUEUEMAXLEN];
    int16_t _functionQueueSecArg[OP_FUNCTIONQUEUEMAXLEN];
    int8_t _functionQueueCounter = 0;



    //called on poll() 
    void callNextFunctionInQueue();
    //debug output
    void debugPrint(String stringToPrint, uint8_t verboseLvl);

    




    

};




#endif

