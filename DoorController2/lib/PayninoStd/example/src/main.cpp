#include <Arduino.h>
#include <./../../cc_esp32_comp/PayninoStd/PayninoStd.h>
// #include <./../../cc_esp32_comp/StrUtils/StrUtils.h>

#define REPTIME_MS 5000

char * ssid = (char*)"TP-Link_3DFA";
char * pass = (char*) "74904006";
unsigned long lastRepTime = 0;

unsigned long timeNow = 0;

void setup(){
    Serial.begin(115200);
    bool enCfg = payninoStd_Init();
    Serial.printf("Board Type is: %d  | enableCfg: %hhd\n", PAYNINO_BOARD_TYPE, enCfg);
    log_i("Network result: %hhd\n", payninoStd_connectToNetwork(PAYNINOSTD_NETMODE_AUTO, ssid, pass));//, (char*)"192.168.1.220", (char*)"192.168.1.1" , (char*) "255.255.255.0" ));
    log_i("WiFiMac: %s   |   EthMac: %s\n", PAYNINOSTD_WIFI_MAC, PAYNINOSTD_ETH_MAC);
    delay(1000);
    if (!payninoStd_InitOutputs()){
        log_d("Error initializing Outputs");
    }else{
        log_d("Outputs Initialized Successfully");
    }

    //If you need network for next actions, you can block code execution like this
    while(!payninoStd_hasNetworkConnectivity){
        delay(200);
        log_i("Waiting for Network Connectivity");
    }

    log_i("Network connection established. Will Block making some blinks");

    //SD Card Test





    //OUTPUTS test

    myRelay.blinkXTimesDifTimesBlocking(5, 500, 200);
    myLed.blinkXTimesDifTimesBlocking(5, 300, 100);
    log_i("\n\n\nOUT1\n\n\n");
    myOut1.blinkXTimesBlocking(20,1);
    log_i("\n\n\nOUT2\n\n\n");
    myOut2.blinkXTimesBlocking(20,1);

    log_i("Will switch state in 1000 ms");
    delay(1000);

    myRelay.switchState();
    myLed.switchState();
    myOut1.switchState();
    myOut2.switchState();
    delay(2000);


    // During normal operation is better to call this, instead of calling function directly for LED signs. This way, all signages will be completed before others kick in
    // Calling directly will act as placing the function in the index 0 of the queue, mantaining all the other elements
    log_v("Will insert in queue then loop...");
    myLed.insertInQueue(OP_FUNCTION_CODE::BLINKXTIMES, 5, 10 );
    myRelay.insertInQueue(OP_FUNCTION_CODE::BLINKXTIMES, 5, 10 );

    myLed.insertInQueue(OP_FUNCTION_CODE::BLINKXTIMES, 10, 5 );
    myRelay.insertInQueue(OP_FUNCTION_CODE::BLINKXTIMES, 10, 5 );

    myLed.insertInQueue(OP_FUNCTION_CODE::BLINKXTIMES, 15, 2 );
    myRelay.insertInQueue(OP_FUNCTION_CODE::BLINKXTIMES, 15, 2 );


    // myRelay.resetFunctionQueue();  //to abort previous registers uncomment this
    myRelay.blinkXTimes(5,1);   
    
}

void loop(){
    timeNow = millis();
    // Despite blocking in Init, newtwork can fail duriong runtime and you can check it like this. Dont worry, payninoStd should will do its job, reconnecting to WiFi/ETH
    if (timeNow - lastRepTime> REPTIME_MS){
        Serial.printf("Connectivity: %hhd\n", payninoStd_hasNetworkConnectivity);
        Serial.printf("WiFi Interface: %hhd\n", payninoStd_hasWifiConnectivity);
        Serial.printf("ETH Interface: %hhd\n", payninoStd_hasEthConnectivity);
        Serial.printf("Relay Functions left to trigger: %d\n", myRelay.getFunctionQueueSize());
        lastRepTime = timeNow;
        
    }

    myRelay.poll();
    myLed.poll();
    myOut1.poll();
    myOut2.poll();

   
}
