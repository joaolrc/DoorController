#include <Arduino.h> 
#include <ESPAsyncWebServer.h>
#include <PayninoStd.h>
#include <DoorControllerWebConfig.h>
#include <LinearMotorController.h>
#include <StrUtils.h>
// #include <mqttFunctions.h>
#include <Ticker.h>


//global variables
bool enableConfig;                          ///< check this variable to see if dev is in config mode
bool isRebootNow = false;                   ///< any module can set this to true to restart the esp
bool pushServerIsValid;                     ///< At init, this is set to validate server inputed by user

bool isTimeToCleanupSockets = false;        ///< flag set by wsCleanTimer

Ticker wsCleanTimer;                    ///< Timer to clean unused sockets that were not properly closed by the clients
DoorControllerWebConfig web;            ///< Web Pages and NVS
AsyncWebSocket ws("/telemetry");        ///< websocket for webpage responsiveness
AsyncWebServer myServer(80);            ///< Server Instance
LinearMotorController myMotor;





/////////////////////////////////////////////////// SETUP //////////////////////////////////////////////////
void setup(){
    enableConfig = payninoStd_Init();
    Serial.begin(115200);
    delay(100);
    const char compile_date[] = __DATE__ " " __TIME__;
    Serial.printf("\n-------------\n[MAIN]Starting %sV%s -%d- Compile Time: %s\n\n",DEVICE_NAME, SW_VERSION, PAYNINO_BOARD_TYPE, compile_date);

    //if i try to init IO expander, will fail because SDA permanently LOW 
    if (PAYNINO_BOARD_TYPE==PAYNINO_BOARD_TYPE_ETH && enableConfig){
        myLed.init(PAYNINOPIN_ONBOARDLED,nullptr,false);
    }else{
        while (!payninoStd_InitOutputs()){
            log_e("OUTs INIT ERR");
            delay(2000);
        }
    }



    web.loadConfig();                     //Fetch Config from NVS  
    myMotor.init(web._motorOpenTime);
    /* Start Server */
    web.createServer(myServer, ws);


    if (enableConfig ){
        while (!payninoStd_InitConfigMode(NULL,NULL,[](){
            log_i("CFG Init OK");
            myLed.turnHigh();
        })){
            log_e("CFG Init ERR");
            delay(2000);
        }

    }else{
        /* Handle Network */
        log_d("Configuration OFF\n");
        while (!payninoStd_connectToNetwork(web._networkMode, web._ssid.c_str(), web._psw.c_str(), web._ipAddress.c_str(), web._defaultGateway.c_str(), web._subnetMask.c_str(),web._wpa2_identity.c_str(), web._wpa2_psw.c_str(), web._wpa2_anonymousId.c_str() )){
            log_e("Network ERR");
            delay(2000);
        }
        unsigned long conStartMillis = millis();
        while (!payninoStd_hasNetworkConnectivity){
            log_i("Waiting for Network");
            myLed.blinkXTimesBlocking(2,5);
            delay(200);
            if ( (millis()-conStartMillis>=10000)  && (web._wifiStatus!= DCWC_WIFISTATUS_NETWORK_UNREACHABLE) ){
                web.myEEPROM_dynamic.putInt(DCWC_EEPROM_ISWIFICLIENT, DCWC_WIFISTATUS_NETWORK_UNREACHABLE);
                web._wifiStatus = DCWC_WIFISTATUS_NETWORK_UNREACHABLE;
            }
        }

        if (web._wifiStatus != DCWC_WIFISTATUS_ONLINE) web.myEEPROM_dynamic.putInt(DCWC_EEPROM_ISWIFICLIENT, DCWC_WIFISTATUS_ONLINE);
        if (payninoStd_hasWifiConnectivity) myLed.blinkXTimesBlocking(5,2); //Could connect as WiFi client signage
        else if (payninoStd_hasEthConnectivity)  myLed.blinkXTimesBlocking(6,2); //Could connect as WiFi client signage




        log_i("Network Initialized Successfully");
       
        //validate pushServer
        if (strUtils_isValidIp(web._pushServer)|| strUtils_isValidDomain(web._pushServer)){ 
            pushServerIsValid=true;
            log_d("[MAIN] PushServer is valid\n");
        }else{
            pushServerIsValid=false;
            log_w("[MAIN] PushServer is NOT valid\n");
        }



        wsCleanTimer.attach_ms(1500,[](){
            log_d("Clean Socks");
            ws.cleanupClients();
        });

    }
}

/////////////////////////////////////////////////// LOOP //////////////////////////////////////////////////
void loop() {

    myMotor.poll();

    if (isRebootNow){
        log_i("[MAIN] Reset task attended! Restarting Now!\n");
        delay(200);
        ESP.restart();
    }


}

