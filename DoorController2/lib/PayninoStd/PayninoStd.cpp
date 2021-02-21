#include <PayninoStd.h>
#include <Ticker.h>
#include <Wire.h>
#include "esp_wpa2.h"

Ticker wifiReconTimer;
bool isRebootingWiFi = false; //flag is set if wifi times out and reconect is issued. Prevent double reconnect regist
bool payninoStd_hasNetworkConnectivity = false;
bool payninoStd_hasEthConnectivity = false;
bool payninoStd_hasWifiConnectivity = false;

uint8_t PAYNINO_BOARD_TYPE;                            //Board Type

uint8_t PAYNINOPIN_SDA, PAYNINOPIN_SCL,              //I2C
PAYNINOPIN_RELAY, PAYNINOPIN_OUT1, PAYNINOPIN_OUT2,  //onBoard OUT's
PAYNINOPIN_IO4, PAYNINOPIN_IO2,                      //header IO's
PAYNINOPIN_ETH_PWR_EN, PAYNINOPIN_ETH_TX_EN,         //Eth
PAYNINOPIN_SERIAL2_RX, PAYNINOPIN_SERIAL2_TX ;       //serial2

OutputControl myRelay;
OutputControl myLed;
OutputControl myOut1;
OutputControl myOut2;
ExpanderControl myExpander; //needed for ETH board only but must be instantiated

char PAYNINOSTD_WIFI_MAC[18];       
char PAYNINOSTD_ETH_MAC[18];
char PAYNINOSTD_DEVICE_NAME[50];
char payninoStd_ipWifi[16];
char payninoStd_ipEth[16];

esp_wpa2_config_t _wpa2_config;

/////////////////////////////// PINS //////////////////////////////////

uint8_t _payninoStd_GetBoardType(){
    pinMode(PAYNINOPIN_BRD_DISCOVERY, INPUT_PULLDOWN);
    delay(5); //some time for the pulldown to be there
    if (digitalRead(PAYNINOPIN_BRD_DISCOVERY)==LOW){
        PAYNINO_BOARD_TYPE = PAYNINO_BOARD_TYPE_WIFI;
        return PAYNINO_BOARD_TYPE_WIFI;
    }
    PAYNINO_BOARD_TYPE = PAYNINO_BOARD_TYPE_ETH;
    return PAYNINO_BOARD_TYPE_ETH;
}

void _payninoStd_initPins(uint8_t board){
    if (board==0) board = PAYNINO_BOARD_TYPE;
    if (board==PAYNINO_BOARD_TYPE_WIFI){
        log_v("Init WiFi BRD");
        PAYNINOPIN_SDA = 21;
        PAYNINOPIN_SCL = 22;
        PAYNINOPIN_RELAY = 27;
        PAYNINOPIN_OUT1 = 25; 
        PAYNINOPIN_OUT2 = 26;
        PAYNINOPIN_IO2 = 2;
        PAYNINOPIN_IO4 = 4;
        PAYNINOPIN_SERIAL2_RX = 16;
        PAYNINOPIN_SERIAL2_TX = 17;

        pinMode(PAYNINOPIN_OUT1,  OUTPUT);
        digitalWrite(PAYNINOPIN_OUT1, LOW);

        pinMode(PAYNINOPIN_OUT2,  OUTPUT);
        digitalWrite(PAYNINOPIN_OUT2,LOW); 

        pinMode(PAYNINOPIN_RELAY, OUTPUT);
        digitalWrite(PAYNINOPIN_RELAY,PAYNINOSTD_RELAYSTATE_OFF);

        pinMode(PAYNINOPIN_IO2, INPUT);
        

    }else if(board==PAYNINO_BOARD_TYPE_ETH) {
        log_v("Init ETH BRD");
        PAYNINOPIN_SDA = 13;
        PAYNINOPIN_SCL = 16;
        PAYNINOPIN_RELAY = 2;   //IO expdr
        PAYNINOPIN_OUT1 = 0;    //IO expdr
        PAYNINOPIN_OUT2 = 1;    //IO expdr
        PAYNINOPIN_IO2 = 3;     //IO expdr
        PAYNINOPIN_IO4 = 5;
        PAYNINOPIN_ETH_PWR_EN = 12;
        PAYNINOPIN_ETH_TX_EN = 21;
        PAYNINOPIN_SERIAL2_RX = 36;
        PAYNINOPIN_SERIAL2_TX = 4;

        //turn OFF ETH at startup
        pinMode(PAYNINOPIN_ETH_TX_EN, OUTPUT);
        digitalWrite(PAYNINOPIN_ETH_TX_EN, LOW);
        pinMode(PAYNINOPIN_ETH_PWR_EN, OUTPUT);
        digitalWrite(PAYNINOPIN_ETH_PWR_EN, LOW);
    }

    //comon initialization
    pinMode(PAYNINOPIN_ONBOARDLED, OUTPUT);
    digitalWrite(PAYNINOPIN_ONBOARDLED,PAYNINOSTD_LEDSTATE_OFF);

    pinMode(PAYNINOPIN_IN1, INPUT);
    pinMode(PAYNINOPIN_IN2, INPUT);

    pinMode(PAYNINOPIN_SDA, INPUT);
    pinMode(PAYNINOPIN_SCL, INPUT);
    
    pinMode(PAYNINOPIN_IO4, INPUT);
}

bool _payninoStd_IsCfg_Enable(generalVoidFunction_t callback){
    bool configJumperOn = false;
    if(digitalRead(PAYNINOPIN_SDA)==LOW){
        configJumperOn=true;
        if (callback!=nullptr){
            callback();
        }
    }
    return configJumperOn;
}

bool payninoStd_Init(){
    _payninoStd_GetBoardType();
    _payninoStd_initPins();
    _payninoStd_getEthMacAddress(); 
    _payninoStd_getWifiMacAddress(); 
    sprintf(PAYNINOSTD_DEVICE_NAME,"%s-%s", DEVICE_NAME, PAYNINOSTD_WIFI_MAC);
    return _payninoStd_IsCfg_Enable();
}

bool payninoStd_InitOutputs(){
    if (!myLed.init(PAYNINOPIN_ONBOARDLED)) {
        log_e("LED init ERR");
        return false;
    }

    
    if (PAYNINO_BOARD_TYPE == PAYNINO_BOARD_TYPE_ETH){
        if (!Wire.begin(PAYNINOPIN_SDA, PAYNINOPIN_SCL)){
            log_e("I2C Init ERR");
            return false;
        }

        if (!myExpander.begin()){
            log_e("Expander Init ERR");
            return false;
        }

        if (  !(myRelay.init(PAYNINOPIN_RELAY, &myExpander) && myOut1.init(PAYNINOPIN_OUT1, &myExpander) && myOut2.init(PAYNINOPIN_OUT2, &myExpander))){
            log_e("OUT Init ERR");
            return false;
        }

    }else if(PAYNINO_BOARD_TYPE == PAYNINO_BOARD_TYPE_WIFI){
        if (  !( myRelay.init(PAYNINOPIN_RELAY) && myOut1.init(PAYNINOPIN_OUT1) && myOut2.init(PAYNINOPIN_OUT2) ) ){
            log_e("OUT Init ERR");
            return false;
        }
    }

    return true;
}

/////////////////////////////// NETWORKING //////////////////////////////////
bool _payninoStd_InitEth(const char*  ethConfigurationStrings[3] ){
    if (PAYNINO_BOARD_TYPE == PAYNINO_BOARD_TYPE_WIFI){
        log_e("ETH INIT ERR");
        return false;
    }
    ETH.begin(ETH_PHY_ADDR, PAYNINOPIN_ETH_PWR_EN, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_LAN8720, ETH_CLOCK_GPIO17_OUT ); //ETH needs to be intialized before setting cfg
    if (ethConfigurationStrings!=nullptr ){
        IPAddress ethConfigurationParameters[3]={}; // { ip , defaultGateway , subnetMask }
        IPAddress googleDns(8,8,8,8);
        bool isValidCred = true;
        for (uint8_t i=0;i<3;i++){
            if( !ethConfigurationParameters[i].fromString(ethConfigurationStrings[i]) ){
                isValidCred = false;
            }
        }
        if (isValidCred) ETH.config(ethConfigurationParameters[0],ethConfigurationParameters[1],ethConfigurationParameters[2], ethConfigurationParameters[1], googleDns);
        else log_w("Invalid Eth cfg");
    }
    return true;
}

bool _payninoStd_InitWifiClient( const char* ssid, const char* psw, const char ** wifiConfigurationStrings, const char* eapIdentity, const char* eapPassword, const char* eapAnonymousId){
    if (strlen(ssid)==0){
        log_w("Empty ssid");
        return false;
    }
    WiFi.disconnect(true);
    WiFi.mode(WIFI_MODE_STA);
    if (eapAnonymousId!=nullptr && strlen(eapAnonymousId)>0){
        esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)eapAnonymousId, strlen(eapAnonymousId));
        log_v("WPA2 Ano: %s", eapAnonymousId);
    }
    if (eapIdentity!=nullptr && strlen(eapIdentity)>0 && eapPassword!=nullptr && strlen(eapPassword)>0){
        esp_wifi_sta_wpa2_ent_set_username((uint8_t *)eapIdentity, strlen(eapIdentity));
        esp_wifi_sta_wpa2_ent_set_password((uint8_t *)eapPassword, strlen(eapPassword));
        _wpa2_config = WPA2_CONFIG_INIT_DEFAULT();
        esp_wifi_sta_wpa2_ent_enable(&_wpa2_config);  
        log_v("WPA2: %s | %s", eapIdentity, eapPassword);
    } 
    
    if (wifiConfigurationStrings!=nullptr){
        IPAddress wifiConfigurationParameters[3]={}; // { ip , defaultGateway , subnetMask }
        IPAddress googleDns(8,8,8,8);
        bool isValidWifiCred = true;
        for (uint8_t i=0;i<3;i++){
            if( !wifiConfigurationParameters[i].fromString(wifiConfigurationStrings[i]) ){
                isValidWifiCred = false;
            }
        }
        if (isValidWifiCred) WiFi.config(wifiConfigurationParameters[0],wifiConfigurationParameters[1],wifiConfigurationParameters[2], wifiConfigurationParameters[1], googleDns);
        else log_w("Invalid WiFi cfg");
    }
    WiFi.begin(ssid, psw);
    return true;

}

bool payninoStd_connectToNetwork(uint8_t networkMode, const char* ssid, const char* psw, const char* ip, const char* defaultGateway, const char* subnetMask, const char* eapIdentity, const char* eapPassword, const char* eapAnonymousId){
    if (PAYNINO_BOARD_TYPE==PAYNINO_BOARD_TYPE_WIFI && networkMode==PAYNINOSTD_NETMODE_ETH){
        log_e("ETH not supported");
        return false;
    }
    if ( (networkMode==PAYNINOSTD_NETMODE_WIFI || (networkMode==PAYNINOSTD_NETMODE_AUTO && PAYNINO_BOARD_TYPE==PAYNINO_BOARD_TYPE_WIFI) ) && ssid==(const char*)""){
        log_e("Empty SSID");
        return false;
    }

    //connect with the defined settings and set event driven function
    const char* netCfg[]={ip,defaultGateway,subnetMask};
    WiFi.onEvent(_payninoStd_networkEvent);
    if (networkMode!=PAYNINOSTD_NETMODE_WIFI && PAYNINO_BOARD_TYPE == PAYNINO_BOARD_TYPE_ETH){
        if(!_payninoStd_InitEth(netCfg)) return false;       
    }
    if (networkMode!=PAYNINOSTD_NETMODE_ETH){
        if(!_payninoStd_InitWifiClient(ssid, psw, netCfg, eapIdentity, eapPassword, eapAnonymousId) && PAYNINO_BOARD_TYPE==PAYNINO_BOARD_TYPE_WIFI) return false;     
    }
    
    return true;
}

void _payninoStd_networkEvent(WiFiEvent_t event, WiFiEventInfo_t info){
    log_v("ev: %d\n", event);
    switch (event) {
        case SYSTEM_EVENT_WIFI_READY: 
            log_d("WiFi ready");
            break;
        case SYSTEM_EVENT_SCAN_DONE:
            log_d("Completed AP scan");
            break;
        case SYSTEM_EVENT_STA_START:
            log_d("WiFi STA start");
            break;
        case SYSTEM_EVENT_STA_STOP:
            log_d("WiFi STA stop");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            log_d("WiFi STA connected");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            log_w("Disconnected STA WiFi");
            payninoStd_hasWifiConnectivity = false;
            log_d("Disconnect reason: %d\n", info.disconnected.reason);
            // if (!isRebootingWiFi && (info.disconnected.reason==WIFI_REASON_ASSOC_EXPIRE || info.disconnected.reason==WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT|| info.disconnected.reason==WIFI_REASON_NOT_AUTHED)){
            if (!payninoStd_hasEthConnectivity && ! isRebootingWiFi){
                log_w("Register Recon Timer");
                // log_w("ASSOC_EXPIRE detected");
                WiFi.mode(WIFI_OFF);
                wifiReconTimer.once_ms(3000,[](){
                    log_v("Recon Timer triggered");
                    if (!payninoStd_hasNetworkConnectivity){ // TODO test this with WiFi.reconnect() instead of block below
                        log_v("Will recon");
                        WiFi.mode(WIFI_MODE_STA);
                        WiFi.begin();
                    }
                    isRebootingWiFi = false;
                });
                isRebootingWiFi = true;
            }
            // }      
            break;
        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
            log_d("AP auth mode changed");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            payninoStd_hasWifiConnectivity = true;
            strUtils_ipAddress2String(WiFi.localIP()).toCharArray(payninoStd_ipWifi, 16);
            log_i("WiFi IP: %s  | SSID: %s", payninoStd_ipWifi, WiFi.SSID().c_str() );
            if (PAYNINO_BOARD_TYPE == PAYNINO_BOARD_TYPE_ETH){
                if (!payninoStd_hasEthConnectivity){
                    esp_eth_disable();
                }else{
                    WiFi.mode(WIFI_OFF);
                }
            }
            break;
        case SYSTEM_EVENT_STA_LOST_IP:
            log_d("Lost STA IP");
            payninoStd_hasWifiConnectivity = false;
            if (!isRebootingWiFi && !payninoStd_hasEthConnectivity){
                log_w("Register recon Timer");
                WiFi.mode(WIFI_OFF);
                wifiReconTimer.once_ms(3000,[](){
                    log_d("Recon Timer trig");
                    if (!payninoStd_hasNetworkConnectivity){
                        log_v("Will recon");
                        WiFi.mode(WIFI_MODE_STA);
                        WiFi.begin();
                    }
                    isRebootingWiFi = false;
                });
                isRebootingWiFi = true;
            
            } 
            break;
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
            log_d("WPS: succeeded enrollee");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
            log_d("WPS: failed enrollee");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
            log_d("WPS: timeout enrollee");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_PIN:
            log_d("WPS: code enrollee");
            break;
        case SYSTEM_EVENT_AP_START:
            log_d("WiFi AP start");
            break;
        case SYSTEM_EVENT_AP_STOP:
            log_d("WiFi AP stop");
            break;
        case SYSTEM_EVENT_AP_STACONNECTED:
            log_d("AP Client connected");
            break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            log_d("AP Client disconnected");
            break;
        case SYSTEM_EVENT_AP_STAIPASSIGNED:
            log_d("AP Client IP assign");
            break;
        case SYSTEM_EVENT_AP_PROBEREQRECVED:
            log_d("Rcv probe req");
            break;
        case SYSTEM_EVENT_GOT_IP6:
            log_d("IPv6 pref");
            break;
        case SYSTEM_EVENT_ETH_START:
            log_d("Eth start");
            break;
        case SYSTEM_EVENT_ETH_STOP:
            log_d("Eth stop");
            break;
        case SYSTEM_EVENT_ETH_CONNECTED:
            log_d("Eth connect");
            break;
        case SYSTEM_EVENT_ETH_DISCONNECTED:
            log_d("Eth disconnect");
            payninoStd_hasEthConnectivity = false;
            break;
        case SYSTEM_EVENT_ETH_GOT_IP:
            strUtils_ipAddress2String(ETH.localIP()).toCharArray(payninoStd_ipEth, 16);
            log_i("ETH IP: %s", payninoStd_ipEth);
            payninoStd_hasEthConnectivity = true;
            if (PAYNINO_BOARD_TYPE == PAYNINO_BOARD_TYPE_ETH) WiFi.mode(WIFI_OFF);
            break;
        default: break;
    }
    payninoStd_hasNetworkConnectivity = payninoStd_hasWifiConnectivity || payninoStd_hasEthConnectivity;
}

void _payninoStd_getWifiMacAddress() {
	uint8_t baseMac[6];
	esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
	sprintf(PAYNINOSTD_WIFI_MAC, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
}

void _payninoStd_getEthMacAddress() {
	uint8_t baseMac[6];
	esp_read_mac(baseMac, ESP_MAC_ETH);
	sprintf(PAYNINOSTD_ETH_MAC, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
}



bool payninoStd_InitConfigMode(const char* ssid, const char* psw, generalVoidFunction_t callback){
    char aux_ssid[45];
    if (ssid == NULL || strlen(ssid)==0){
        char subbuff[3];
        memcpy( subbuff, &PAYNINOSTD_WIFI_MAC[15], 2 );
        subbuff[2]='\0';
        sprintf(aux_ssid,"%s_%c%c%s" ,DEVICE_NAME, PAYNINOSTD_WIFI_MAC[12], PAYNINOSTD_WIFI_MAC[13], subbuff);

    }else if (strlen(ssid)<8) {
        log_e("ssid too small");
        return false;
    }

    if (!WiFi.mode(WIFI_AP)){
        log_e("AP Mode ERR");
        return false;
    }

    if (!WiFi.softAP(aux_ssid, psw)){
        log_e("AP Init ERR");
        return false;
    }
 
    log_v("AUX_SSID: %s", aux_ssid);
    if (callback!=nullptr) callback();
    return true;
}

/////////////////////////////// SDCARD //////////////////////////////////
// bool payninoStd_InitSdCard(){

// }

