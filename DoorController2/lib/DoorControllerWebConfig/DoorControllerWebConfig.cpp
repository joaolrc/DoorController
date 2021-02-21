#include "DoorControllerWebConfig.h"
#include "html_strings7.h"
#include <PayninoStd.h>
// #include <Ticker.h>
#include <StrUtils.h>

extern bool isRebootNow;
extern LinearMotorController myMotor;


DoorControllerWebConfig::DoorControllerWebConfig(){}

void DoorControllerWebConfig::loadConfig(){
    myEEPROM_dynamic.begin(DCWC_EEPROM_NAME_DYNAMIC_CFG, false);
    myEEPROM_static.begin(DCWC_EEPROM_NAME_STATIC_CFG,false);
    // myEEPROM_dynamic.clear();
    // myEEPROM_static.clear();
    //If first time using --> set default values
    bool isCfgDefault = false;
    //If first time using --> set default values
    if (!myEEPROM_dynamic.getBool(DCWC_EEPROM_NOTFIRSTUSE,false)){
        log_v("1st Use");
        setDefaultConfig();
        isCfgDefault= true;
    }else{
        log_v("NOT 1st use");
    }

    if (isSwDifferentVersion() && !isCfgDefault){
        log_v("Different SW");
        setDefaultConfig();
        isCfgDefault= true;
    }

    reloadVariables();      
}

void DoorControllerWebConfig::setDefaultConfig(){
    log_v("Set default cfg");
    //network and user variables
    myEEPROM_dynamic.putBool(DCWC_EEPROM_NOTFIRSTUSE,    true);
    // myEEPROM_static.putString(DCWC_EEPROM_REGID,          DCWC_EEPROM_DEFAULTSTRING);
    myEEPROM_dynamic.putInt(DCWC_EEPROM_ISWIFICLIENT,   DCWC_WIFISTATUS_OFFLINE);
    myEEPROM_dynamic.putString(DCWC_EEPROM_WIFI_SSID,   "");
    myEEPROM_dynamic.putString(DCWC_EEPROM_WIFI_PSW,    "");
    myEEPROM_dynamic.putString(DCWC_EEPROM_WIFI_IP,     "");
    myEEPROM_dynamic.putString(DCWC_EEPROM_WIFI_SUBNET, "");
    myEEPROM_dynamic.putString(DCWC_EEPROM_WIFI_GATEWAY,"");
    myEEPROM_dynamic.putString(DCWC_EEPROM_LOGIN_UNAME, "admin");
    myEEPROM_dynamic.putString(DCWC_EEPROM_LOGIN_PSW, "admin");
    //other config);
    myEEPROM_dynamic.putBool(DCWC_EEPROM_IS_HTTP, true);
    myEEPROM_dynamic.putString(DCWC_EEPROM_PUSH_SERVER, "");
    myEEPROM_dynamic.putString(DCWC_EEPROM_MQTT_TOPIC, "");
    myEEPROM_dynamic.putULong(DCWC_EEPROM_MQTT_PORT, DCWC_MQTT_DEFAULT_PORT);
    myEEPROM_dynamic.putBool(DCWC_EEPROM_MQTT_RETAIN, DCWC_MQTT_DEFAULT_RETAIN);
    myEEPROM_dynamic.putUShort(DCWC_EEPROM_MQTT_QOS, DCWC_MQTT_DEFAULT_QOS);
    myEEPROM_dynamic.putString(DCWC_EEPROM_MQTT_UNAME, "");
    myEEPROM_dynamic.putString(DCWC_EEPROM_MQTT_PSW, "");
    myEEPROM_dynamic.putString(DCWC_EEPROM_BEARER_TOKEN, "");
    myEEPROM_dynamic.putString(DCWC_EEPROM_MQTT_LAMP_TOPIC, "");
    myEEPROM_dynamic.putUChar(DCWC_EEPROM_NET_MODE, PAYNINOSTD_NETMODE_AUTO); 
    myEEPROM_dynamic.putString(DCWC_EEPROM_WPA2_PSW, "");
    myEEPROM_dynamic.putString(DCWC_EEPROM_WPA2_IDENTITY, "");
    myEEPROM_dynamic.putString(DCWC_EEPROM_WPA2_ANONYMOUSID, "");
}

bool DoorControllerWebConfig::isSwDifferentVersion(){
    String oldV = myEEPROM_static.getString(DCWC_EEPROM_SWVERSION);
    if (oldV!=SW_VERSION){
        log_v("Different SW: %s\n", oldV.c_str());
        myEEPROM_static.putString(DCWC_EEPROM_SWVERSION, SW_VERSION);
        return true;
    }
    return false;
}

//fetches all "global variables" from eeprom to flash for easy to use
void DoorControllerWebConfig::reloadVariables(){
    //Networking Variables
    _wifiStatus = myEEPROM_dynamic.getInt(DCWC_EEPROM_ISWIFICLIENT, DCWC_EEPROM_DEFAULTVALUE);
    _ssid = myEEPROM_dynamic.getString(DCWC_EEPROM_WIFI_SSID, "");
    _psw = myEEPROM_dynamic.getString(DCWC_EEPROM_WIFI_PSW, "");
    _ipAddress = myEEPROM_dynamic.getString(DCWC_EEPROM_WIFI_IP, "");
    _defaultGateway = myEEPROM_dynamic.getString(DCWC_EEPROM_WIFI_GATEWAY, "");
    _subnetMask = myEEPROM_dynamic.getString(DCWC_EEPROM_WIFI_SUBNET, "");
    //Portal Variables
    _loginUname = myEEPROM_dynamic.getString(DCWC_EEPROM_LOGIN_UNAME, "admin");
    _loginPsw = myEEPROM_dynamic.getString(DCWC_EEPROM_LOGIN_PSW, "admin");
    _isHttpPush = myEEPROM_dynamic.getBool(DCWC_EEPROM_IS_HTTP);
    _mqttTopic = myEEPROM_dynamic.getString(DCWC_EEPROM_MQTT_TOPIC);
    _mqttPort = myEEPROM_dynamic.getULong(DCWC_EEPROM_MQTT_PORT, DCWC_MQTT_DEFAULT_PORT);
    _mqttUname = myEEPROM_dynamic.getString(DCWC_EEPROM_MQTT_UNAME, "");
    _mqttPsw = myEEPROM_dynamic.getString(DCWC_EEPROM_MQTT_PSW, "");
    //other vars

    _motorOpenTime = myEEPROM_dynamic.getULong(DCWC_EEPROM_MOTOR_OPEN_TIME, 1000);
    _mqttRetain = myEEPROM_dynamic.getBool(DCWC_EEPROM_MQTT_RETAIN, DCWC_MQTT_DEFAULT_RETAIN);
    _mqttQos = myEEPROM_dynamic.getUShort(DCWC_EEPROM_MQTT_QOS, DCWC_MQTT_DEFAULT_QOS);
    _pushServer = myEEPROM_dynamic.getString(DCWC_EEPROM_PUSH_SERVER, "");
    _bearerToken = myEEPROM_dynamic.getString(DCWC_EEPROM_BEARER_TOKEN, "");
    _lampTopic = myEEPROM_dynamic.getString(DCWC_EEPROM_MQTT_LAMP_TOPIC, "");
    _networkMode = myEEPROM_dynamic.getUChar(DCWC_EEPROM_NET_MODE, PAYNINOSTD_NETMODE_AUTO);
    _wpa2_identity = myEEPROM_dynamic.getString(DCWC_EEPROM_WPA2_IDENTITY, "");
    _wpa2_psw = myEEPROM_dynamic.getString(DCWC_EEPROM_WPA2_PSW, "");
    _wpa2_anonymousId = myEEPROM_dynamic.getString(DCWC_EEPROM_WPA2_ANONYMOUSID, "");
    printVars();
}

void DoorControllerWebConfig::printVars(){
    log_d("_wifiStatus: %hhd",_wifiStatus);
    log_d("_ssid: %s",_ssid.c_str());
    log_d("_psw: %s",_psw.c_str());
    log_d("_regId: %s",_regId.c_str());
    log_d("_ipAddress: %s",_ipAddress.c_str());
    log_d("_defaultGateway: %s",_defaultGateway.c_str());
    log_d("_subnetMask: %s",_subnetMask.c_str());
    log_d("_wpa2_identity: %s", _wpa2_identity.c_str());
    log_d("_wpa2_psw: %s", _wpa2_psw.c_str());
    log_d("_wpa2_anonymousId: %s", _wpa2_anonymousId.c_str());
    log_d("_loginUname: %s", _loginUname.c_str());
    log_d("_loginPsw: %s", _loginPsw.c_str());
    log_d("_isHttpPush: %hhd", _isHttpPush);
    log_d("_mqttPort: %lu", _mqttPort);
    log_d("_mqttQos: %d", _mqttQos);
    log_d("_mqttTopic: %s", _mqttTopic.c_str());
    log_d("_pushServer: %s", _pushServer.c_str());
    log_d("_mqttRetain: %hhd", _mqttRetain);
    log_d("_mqttUname: %s", _mqttUname.c_str());
    log_d("_mqttPsw: %s", _mqttPsw.c_str());

    log_d("_motorOpenTime: %lu", _motorOpenTime );
    log_d("_bearerToken: %s", _bearerToken.c_str());
    log_d("_lampTopic: %s", _lampTopic.c_str());
    log_d("_networkMode: %d", _networkMode);
}


String DoorControllerWebConfig::updateApDash(String htmlPage){
    reloadVariables();
    if (_wifiStatus<=-2 && _wifiStatus!=-3){
        htmlPage.replace("%PLACEHOLDER_NETWORK_STATUS%","OFFLINE");
    }else if (_wifiStatus==DCWC_WIFISTATUS_NETWORK_PROVIDED){
        htmlPage.replace("%PLACEHOLDER_NETWORK_STATUS%","Network Provided: "+_ssid);
    }else if (_wifiStatus==DCWC_WIFISTATUS_NETWORK_UNREACHABLE){
        htmlPage.replace("%PLACEHOLDER_NETWORK_STATUS%","Could not connect to "+_ssid);
    }
    else if (_wifiStatus==DCWC_WIFISTATUS_ONLINE){
        htmlPage.replace("%PLACEHOLDER_NETWORK_STATUS%","ONLINE");
    }
    htmlPage.replace("%PLACEHOLDER_SWVERSION%", SW_VERSION);
    htmlPage.replace("%PLACEHOLDER_MAC%", PAYNINOSTD_WIFI_MAC);

    if (PAYNINO_BOARD_TYPE == PAYNINO_BOARD_TYPE_ETH){        
        htmlPage.replace("%PLACEHOLDER_MAC_ETH%", PAYNINOSTD_ETH_MAC);
        htmlPage.replace("%PLACEHOLDER_HIDDEN_ETH%", "");
    }else{
        htmlPage.replace("%PLACEHOLDER_HIDDEN_ETH%", "hidden");
    }
    
    if (_isSinglePathway){
        htmlPage.replace("%PLACEHOLDER_PATHWAY_ACTIVE%", "Single");
        htmlPage.replace("%PLACEHOLDER_PATHWAY_INACTIVE%", "Double");
    }else{
        htmlPage.replace("%PLACEHOLDER_PATHWAY_ACTIVE%", "Double");
        htmlPage.replace("%PLACEHOLDER_PATHWAY_INACTIVE%", "Single");
    }

    return htmlPage+styling;
}

String DoorControllerWebConfig::updateWifiPage(String htmlPage){
    htmlPage.replace("%PLACEHOLDER_SSID%", _ssid);    
    htmlPage.replace("%PLACEHOLDER_PSW%", _psw);    

    if (PAYNINO_BOARD_TYPE == PAYNINO_BOARD_TYPE_ETH){
        switch (_networkMode){
            case PAYNINOSTD_NETMODE_AUTO  :
                htmlPage.replace("%PLACEHOLDER_NETMODE_ACTIVE%", DCWC_NETWORK_KEYWORD_AUTO);
                htmlPage.replace("%PLACEHOLDER_NETMODE_INACTIVE%", DCWC_NETWORK_KEYWORD_ETH);
                htmlPage.replace("%PLACEHOLDER_NETMODE_INACTIVE2%", DCWC_NETWORK_KEYWORD_WIFI);
                break;
            case PAYNINOSTD_NETMODE_ETH:
                htmlPage.replace("%PLACEHOLDER_NETMODE_ACTIVE%", DCWC_NETWORK_KEYWORD_ETH);
                htmlPage.replace("%PLACEHOLDER_NETMODE_INACTIVE%", DCWC_NETWORK_KEYWORD_AUTO);
                htmlPage.replace("%PLACEHOLDER_NETMODE_INACTIVE2%", DCWC_NETWORK_KEYWORD_WIFI);
                break;
            case PAYNINOSTD_NETMODE_WIFI:
                htmlPage.replace("%PLACEHOLDER_NETMODE_ACTIVE%", DCWC_NETWORK_KEYWORD_WIFI);
                htmlPage.replace("%PLACEHOLDER_NETMODE_INACTIVE%", DCWC_NETWORK_KEYWORD_AUTO);
                htmlPage.replace("%PLACEHOLDER_NETMODE_INACTIVE2%", DCWC_NETWORK_KEYWORD_ETH);
                break;
        }
        htmlPage.replace("%PLACEHOLDER_HIDDEN_ETH%", "");
    }else{
        htmlPage.replace("%PLACEHOLDER_HIDDEN_ETH%", "hidden");
    }
    
    htmlPage.replace("%PLACEHOLDER_WPA2_ANONYMOUS_ID%", _wpa2_anonymousId);
    htmlPage.replace("%PLACEHOLDER_WPA2_PSW%", _wpa2_psw);
    htmlPage.replace("%PLACEHOLDER_WPA2_UNAME%", _wpa2_identity);

    htmlPage.replace("%PLACEHOLDER_SWVERSION%",SW_VERSION);
    htmlPage.replace("%PLACEHOLDER_IP%",_ipAddress);
    htmlPage.replace("%PLACEHOLDER_GATEWAY%",_defaultGateway);
    htmlPage.replace("%PLACEHOLDER_SUBNET%",_subnetMask);
    printVars();
    return htmlPage+styling;
}

String DoorControllerWebConfig::updateFirmwarePage(String htmlPage){
    htmlPage.replace("%PLACEHOLDER_SWVERSION%",SW_VERSION);
    printVars();
    return htmlPage+styling;
}


String DoorControllerWebConfig::updateCredentialsPage(String htmlPage){
    htmlPage.replace("%PLACEHOLDER_SWVERSION%", SW_VERSION);
    htmlPage.replace("%PLACEHOLDER_LOGIN_UNAME%", _loginUname);
    htmlPage.replace("%PLACEHOLDER_LOGIN_PSW%", _loginPsw);
    printVars();
    return htmlPage+styling;
}

String DoorControllerWebConfig::updatePushSettingsPage(String htmlPage){
    htmlPage.replace("%PLACEHOLDER_SWVERSION%", SW_VERSION);

    if (_isHttpPush){
        htmlPage.replace("%SERVER_LABEL%", "Server Endpoint");
        htmlPage.replace("%PLACEHOLDER_MQTT_TOPIC_HIDDEN%", "style=\"display: none;\"");
        htmlPage.replace("%PLACEHOLDER_MQTT_PORT_HIDDEN%", "style=\"display: none;\"");
        htmlPage.replace("%PLACEHOLDER_MQTT_QOS_HIDDEN%", "style=\"display: none;\"");
        htmlPage.replace("%PLACEHOLDER_MQTT_RETAIN_HIDDEN%", "style=\"display: none;\"");
        htmlPage.replace("%PLACEHOLDER_MQTT_UNAME_HIDDEN%", "style=\"display: none;\"");
        htmlPage.replace("%PLACEHOLDER_MQTT_PSW_HIDDEN%", "style=\"display: none;\"");

        htmlPage.replace("%PLACEHOLDER_PUSHPROTOCOL_ACTIVE%", "HTTP");
        htmlPage.replace("%PLACEHOLDER_PUSHPROTOCOL_INACTIVE%", "MQTT");

        htmlPage.replace("%PLACEHOLDER_BEARER_HIDDEN%", "");
        htmlPage.replace("%PLACEHOLDER_BEARER_TOKEN%", _bearerToken);
    }else{
        htmlPage.replace("%SERVER_LABEL%", "MQTT Broker Address");
        htmlPage.replace("%PLACEHOLDER_MQTT_TOPIC_HIDDEN%", "");
        htmlPage.replace("%PLACEHOLDER_MQTT_PORT_HIDDEN%", "");
        htmlPage.replace("%PLACEHOLDER_MQTT_QOS_HIDDEN%", "");
        htmlPage.replace("%PLACEHOLDER_MQTT_RETAIN_HIDDEN%", "");
        htmlPage.replace("%PLACEHOLDER_MQTT_UNAME_HIDDEN%", "");
        htmlPage.replace("%PLACEHOLDER_MQTT_PSW_HIDDEN%", "");

        htmlPage.replace("%PLACEHOLDER_PUSHPROTOCOL_ACTIVE%", "MQTT");
        htmlPage.replace("%PLACEHOLDER_PUSHPROTOCOL_INACTIVE%", "HTTP");

        htmlPage.replace("%PLACEHOLDER_BEARER_HIDDEN%", "style=\"display: none;\"");
    }


    htmlPage.replace("%PLACEHOLDER_MQTT_TOPIC%", _mqttTopic);
    htmlPage.replace("%PLACEHOLDER_SERVER%", _pushServer);
    htmlPage.replace("%PLACEHOLDER_MQTT_PORT%", String(_mqttPort) );
    htmlPage.replace("%PLACEHOLDER_MQTT_QOS%", String(_mqttQos) );
    htmlPage.replace("%PLACEHOLDER_MQTT_UNAME%", _mqttUname );
    htmlPage.replace("%PLACEHOLDER_MQTT_PSW%", _mqttPsw );
    htmlPage.replace("%PLACEHOLDER_MQTT_LAMPTOPIC%", _lampTopic );
    if (_mqttRetain){
        htmlPage.replace("%PLACEHOLDER_MQTT_RETAIN_ACTIVE%", "true" );
        htmlPage.replace("%PLACEHOLDER_MQTT_RETAIN_INACTIVE%", "false" );
    }else{
        htmlPage.replace("%PLACEHOLDER_MQTT_RETAIN_ACTIVE%", "false" );
        htmlPage.replace("%PLACEHOLDER_MQTT_RETAIN_INACTIVE%", "true" );
    }
    
    printVars();
    return htmlPage+styling;
}



// Check if header has cookie or if post body has user and pass
bool DoorControllerWebConfig::isAuthenticated(AsyncWebServerRequest *request){
    //Check if POST has user and pass 
    if (request->hasParam("user", true) && request->hasParam("pass", true)){
        if( request->getParam("user",true)->value()==_loginUname && request->getParam("pass",true)->value()==_loginPsw ){
            log_v("Auth cred");
            return true;
        }
    }
  //IF reached here, POST has no credentials. Let's see if has cookies
    if (request->hasHeader("Cookie")){
        String cookie = request->getHeader("Cookie")->value();
        if (cookie.indexOf("qibixxID=1") != -1) {
            log_v("Auth cookie");
            return true;
        }
    }
    log_v("Auth ERR");
    return false;
}


void DoorControllerWebConfig::factoryReset(){
    log_v("Factory rst");
    if(myEEPROM_dynamic.clear()){
       log_v("Clean OK");
    }else{
        log_e("Clean ERR");
    }
}

/////////////////////////////////////////////////////////////////// AP SERVER endpoints /////////////////////////////////////////////////////////
//Called when device in in AP mode (NOT WiFi client)
void DoorControllerWebConfig::createServer(AsyncWebServer &server, AsyncWebSocket &socket){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        log_v("cli root");
        AsyncWebServerResponse *resp = request->beginResponse(200, "text/html", APloginpage ); 
        resp->addHeader("Connection","close");
        request->send(resp); 
    });

    server.on("/dashboard", HTTP_ANY, [this](AsyncWebServerRequest *request) {
        if (isAuthenticated(request)){
            log_v("cli dash");
            AsyncWebServerResponse *resp = request->beginResponse(200, "text/html", updateApDash(AP_dash_html));
            resp->addHeader("Set-Cookie", "qibixxID=1");
            resp->addHeader("Connection","close");
            request->send(resp); 

        }else{
            AsyncWebServerResponse *resp = request->beginResponse(401, "text/html", "401 Unauthorized");
            resp->addHeader("Connection","close");
            request->send(resp); 
        }
    });

    server.on("/update", HTTP_GET, [&](AsyncWebServerRequest *request){
        if (isAuthenticated(request)){
            log_v("cli updt");
            AsyncWebServerResponse *resp = request->beginResponse(200, "text/html",updateFirmwarePage(AP_firmware));
            resp->addHeader("Connection","close");
            request->send(resp); 

        }else{
            AsyncWebServerResponse *resp = request->beginResponse(401, "text/html", "401 Unauthorized");
            resp->addHeader("Connection","close");
            request->send(resp); 
        }
    });

    server.on("/wifi", HTTP_GET, [&](AsyncWebServerRequest *request){
        AsyncWebServerResponse *resp;
        if (isAuthenticated(request)){
            log_v("cli wifi");
            resp = request->beginResponse(200, "text/html",updateWifiPage(AP_wificon));
        }else{
            resp = request->beginResponse(401, "text/html", "401 Unauthorized");
        }
        resp->addHeader("Connection","close");
        request->send(resp); 
    });

    server.on("/reset", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *resp;
        if (isAuthenticated(request)){
            log_v("cli reset");
            resp = request->beginResponse(200, "text/html", "Rebooting device" ); 
            isRebootNow = true;
        }else{
            resp = request->beginResponse(401, "text/html", "401 Unauthorized"); 
        }
        resp->addHeader("Connection","close");
        request->send(resp); 
    });

    server.on("/credentials", HTTP_GET, [&](AsyncWebServerRequest *request){
        AsyncWebServerResponse *resp;
        if (isAuthenticated(request)){
            log_v("cli credentials");
            resp = request->beginResponse(200, "text/html", updateCredentialsPage(AP_credentials));
        }else{
            resp = request->beginResponse(401, "text/html", "401 Unauthorized");
        }
        resp->addHeader("Connection","close");
        request->send(resp); 
    });

    server.on("/pushSettings", HTTP_GET, [&](AsyncWebServerRequest *request){
        AsyncWebServerResponse *resp;
        if (isAuthenticated(request)){
            log_v("cli pushSettings");
            resp = request->beginResponse(200, "text/html", updatePushSettingsPage(AP_pushSettings_html));
        }else{
            log_v("Unauthorized");
            resp = request->beginResponse(401, "text/html", "401 Unauthorized");
        }
        resp->addHeader("Connection","close");
        request->send(resp); 
    });

    server.on("/ChangeSettings", HTTP_GET, [&](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *resp;
        if (isAuthenticated(request)){
            bool isReloadVars = false;
            //Match regId with some regex like
            if (request->hasParam("regId")){
                String aux_regId = request->getParam("regId")->value();
                if( aux_regId!=_regId && !aux_regId.isEmpty()){  
                    myEEPROM_static.putString(DCWC_EEPROM_REGID, aux_regId);
                    isReloadVars = true;
                }
            }


            if (request->hasParam("motorOpenTime") && request->getParam("motorOpenTime")->value() != ""){
                char* endptr;
                unsigned long aux_motorOpenTime = strtoul(request->getParam("motorOpenTime")->value().c_str(), &endptr, 10);
                if (!*endptr && aux_motorOpenTime!= _motorOpenTime){ 
                    myEEPROM_dynamic.putULong(DCWC_EEPROM_MOTOR_OPEN_TIME, _motorOpenTime );
                    isReloadVars = true;
                }else{ // is part of the body --> append to body
                    log_w("ulong convert ERR");
                }
            }      

            if (isReloadVars){ 
                reloadVariables(); 
                resp = request->beginResponse(200, "text/html", "Changes submitted!");  
            }else{
                resp = request->beginResponse(200, "text/html", "Invalid Variables. Ignoring!" ); 
            }

        }else resp = request->beginResponse(401, "text/html", "401 Unauthorized");
        
        resp->addHeader("Connection","close");
        request->send(resp); 
    });

    server.on("/ChangePushSettings", HTTP_GET, [&](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *resp;
        if (isAuthenticated(request)){
            bool isReloadVars = false;
            //Check pushProtocol
            if (request->hasParam("pushProtocol")){ 
                String pushProto = request->getParam("pushProtocol")->value();
                if (pushProto == "MQTT" && _isHttpPush){
                    myEEPROM_dynamic.putBool(DCWC_EEPROM_IS_HTTP, false);
                    isReloadVars = true;
                }else if (pushProto == "HTTP" && !_isHttpPush){
                    myEEPROM_dynamic.putBool(DCWC_EEPROM_IS_HTTP, true);
                    isReloadVars = true;
                }
            }

            //check MQTT topic --> only relevant if isHttpProtocol == false
            if (request->hasParam("mqttTopic")){
                String aux_mqttTopic = request->getParam("mqttTopic")->value();
                if (aux_mqttTopic != _mqttTopic){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_MQTT_TOPIC, aux_mqttTopic);
                    isReloadVars = true;
                }
            }

            //check lamp topic --> only relevant if isHttpProtocol == false
            if (request->hasParam("lampTopic")){
                String aux_lampTopic = request->getParam("lampTopic")->value();
                if (aux_lampTopic != _lampTopic){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_MQTT_LAMP_TOPIC, aux_lampTopic);
                    isReloadVars = true;
                }
            }

            //check MQTT topic --> only relevant if isHttpProtocol == false
            if (request->hasParam("bearerToken")){
                String aux_bearer = request->getParam("bearerToken")->value();
                if (aux_bearer != _bearerToken){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_BEARER_TOKEN, aux_bearer);
                    isReloadVars = true;
                }
            }

            //check MQTT uname --> only relevant if isHttpProtocol == false
            if (request->hasParam("mqttUname")){
                String aux_mqttUname = request->getParam("mqttUname")->value();
                if (aux_mqttUname != _mqttUname){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_MQTT_UNAME, aux_mqttUname);
                    isReloadVars = true;
                }
            }

            //check MQTT PSW --> only relevant if isHttpProtocol == false
            if (request->hasParam("mqttPsw")){
                String aux_mqttPsw = request->getParam("mqttPsw")->value();
                if (aux_mqttPsw != _mqttPsw){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_MQTT_PSW, aux_mqttPsw);
                    isReloadVars = true;
                }
            }

            //check MQTT port
            if (request->hasParam("mqttPort") && request->getParam("mqttPort")->value() != ""){
                char* endptr;
                unsigned long aux_mqttPort = strtoul(request->getParam("mqttPort")->value().c_str(), &endptr, 10);
                if (!*endptr && aux_mqttPort!= _mqttPort){ //if string is numeric --> chunked response number
                    myEEPROM_dynamic.putULong(DCWC_EEPROM_MQTT_PORT, aux_mqttPort);
                    isReloadVars = true;

                }else log_w("ulong convert err");
            }

            if (request->hasParam("mqttRetain") && request->getParam("mqttRetain")->value() != ""){
                String aux_mqttRetain = request->getParam("mqttRetain")->value();
                if (aux_mqttRetain=="true" && !_mqttRetain){ 
                    myEEPROM_dynamic.putBool(DCWC_EEPROM_MQTT_RETAIN, true);
                    isReloadVars = true;
                }else if (aux_mqttRetain=="false" && _mqttRetain){
                    myEEPROM_dynamic.putBool(DCWC_EEPROM_MQTT_RETAIN, false);
                    isReloadVars = true;
                }
            }

            if (request->hasParam("mqttQos") && request->getParam("mqttQos")->value() != ""){
                char* endptr;
                uint8_t aux_mqttQos = strtoul(request->getParam("mqttQos")->value().c_str(), &endptr, 10);
                if (!*endptr && aux_mqttQos!= _mqttQos){ 
                    myEEPROM_dynamic.putUShort(DCWC_EEPROM_MQTT_QOS, aux_mqttQos);
                    isReloadVars = true;
                }else{ // is part of the body --> append to body
                    log_w("ulong convert err");
                }
            }

            //check 
            if (request->hasParam("server")){
                String aux_server = request->getParam("server")->value();
                if (aux_server != _pushServer){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_PUSH_SERVER, aux_server);
                    isReloadVars = true;
                }
            }

            if (isReloadVars){ 
                reloadVariables(); 
                resp = request->beginResponse(200, "text/html", "Changes submitted!" );  
            }else resp = request->beginResponse(200, "text/html", "Invalid Variables. Ignoring!" ); 
        }
        else{
            resp = request->beginResponse(401, "text/html", "401 Unauthorized");
        }
        resp->addHeader("Connection","close");
        request->send(resp); 
    });

    server.on("/ChangeCredentials", HTTP_GET, [&](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *resp;
        if (isAuthenticated(request)){
            bool isReloadVars = false;
            if (request->hasParam("uname") && request->hasParam("psw")){  //Check if POST has params
                String uname = request->getParam("uname")->value();
                String psw = request->getParam("psw")->value();
                if (uname != _loginUname && uname!=""){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_LOGIN_UNAME, uname);
                    isReloadVars = true;
                }

                if (psw != _loginPsw && psw!=""){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_LOGIN_PSW, psw);
                    isReloadVars =true;
                }

                if (isReloadVars){
                    reloadVariables();
                    resp = request->beginResponse(200, "text/html", "Changes submitted!" );
                }else resp = request->beginResponse(200, "text/html", "OK" );

            }else{
                resp = request->beginResponse(200, "text/html", "Query does not have necessary parameters!" );   
            }          
        }else{
            resp = request->beginResponse(401, "text/html", "401 Unauthorized");
        }
        resp->addHeader("Connection","close");
        request->send(resp); 
    });


    server.on("/WifiConnect", HTTP_GET, [&](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *resp;
        if (isAuthenticated(request)){
            bool isReloadVars = false;
            if ( request->hasParam("ssid") && request->hasParam("psw") ){  //Check if POST has params
                String aux_ssid = request->getParam("ssid")->value();
                if (aux_ssid!=_ssid){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_WIFI_SSID, aux_ssid);
                    isReloadVars =true;
                }

                String aux_psw = request->getParam("psw")->value();
                if (aux_psw!=_psw){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_WIFI_PSW, aux_psw);
                    isReloadVars = true;
                }

                myEEPROM_dynamic.putInt(DCWC_EEPROM_ISWIFICLIENT, -1);
            }

            if (request->hasParam("ip")){  //Check if POST has params
                String ip = request->getParam("ip")->value();
                if( (strUtils_isValidIp(ip) || ip.isEmpty()) && _ipAddress!= ip){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_WIFI_IP, ip);
                    isReloadVars = true;
                }
            }
            if (request->hasParam("gateway")){
                String gateway = request->getParam("gateway")->value();
                if( (strUtils_isValidIp(gateway) || gateway.isEmpty()) && gateway!= _defaultGateway){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_WIFI_GATEWAY, gateway);
                    isReloadVars = true;
                }
            }

            if (request->hasParam("subnet")){
                String subnet = request->getParam("subnet")->value();
                if( (strUtils_isValidIp(subnet) || subnet.isEmpty()) && _subnetMask!=subnet){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_WIFI_SUBNET, subnet);
                    isReloadVars = true;
                }
            }

            //erase wifi password just in case
            if (request->hasParam("wifiMode")){
                String aux_wifiMode = request->getParam("wifiMode")->value();
                if (aux_wifiMode == "wpa_e"){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_WIFI_PSW, "");
                    isReloadVars = true;
                } 
            }

            if (request->hasParam("wpa2Uname") && request->hasParam("wifiMode")){
                String aux_wpa2Uname = request->getParam("wpa2Uname")->value();
                String aux_wifiMode = request->getParam("wifiMode")->value();
                if (aux_wifiMode == "wpa_p") aux_wpa2Uname = "";
                if( aux_wpa2Uname != _wpa2_identity ){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_WPA2_IDENTITY, aux_wpa2Uname );
                    isReloadVars = true;
                }
            }


            if (request->hasParam("wpa2AnId") && request->hasParam("wifiMode") ){
                String aux_wpa2AnId= request->getParam("wpa2AnId")->value();
                String aux_wifiMode = request->getParam("wifiMode")->value();
                if (aux_wifiMode == "wpa_p") aux_wpa2AnId = "";
                if( aux_wpa2AnId != _wpa2_anonymousId ){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_WPA2_ANONYMOUSID, aux_wpa2AnId );
                    isReloadVars = true;
                }
            }

            if (request->hasParam("wpa2Psw") && request->hasParam("wifiMode") ){
                String aux_wpa2psw = request->getParam("wpa2Psw")->value();
                String aux_wifiMode = request->getParam("wifiMode")->value();
                if (aux_wifiMode == "wpa_p") aux_wpa2psw = "";
                if( aux_wpa2psw != _wpa2_psw ){
                    myEEPROM_dynamic.putString(DCWC_EEPROM_WPA2_PSW, aux_wpa2psw );
                    isReloadVars = true;
                }
            }

            if ( PAYNINO_BOARD_TYPE==PAYNINO_BOARD_TYPE_ETH && request->hasParam("netmode") && request->getParam("netmode")->value() != ""){
                log_i("Netmode Submitted");
                String aux_netmode = request->getParam("netmode")->value();
                if (aux_netmode==DCWC_NETWORK_KEYWORD_AUTO && _networkMode!=PAYNINOSTD_NETMODE_AUTO ){
                    myEEPROM_dynamic.putUChar(DCWC_EEPROM_NET_MODE, PAYNINOSTD_NETMODE_AUTO );
                    isReloadVars = true;
                }else if (aux_netmode==DCWC_NETWORK_KEYWORD_ETH && _networkMode!=PAYNINOSTD_NETMODE_ETH){
                    myEEPROM_dynamic.putUChar(DCWC_EEPROM_NET_MODE, PAYNINOSTD_NETMODE_ETH );
                    isReloadVars = true;
                }else if (aux_netmode==DCWC_NETWORK_KEYWORD_WIFI && _networkMode!=PAYNINOSTD_NETMODE_WIFI){
                    myEEPROM_dynamic.putUChar(DCWC_EEPROM_NET_MODE, PAYNINOSTD_NETMODE_WIFI );
                    isReloadVars = true;
                }else{ 
                    log_w("Netmode ERR");
                }
            }

            if (isReloadVars){
                resp = request->beginResponse(200, "text/html", "Changes submitted!" ); 
                reloadVariables();
            }else  resp = request->beginResponse(200, "text/html", "OK" ); 
            
            resp->addHeader("Connection","close");
            request->send(resp); 

        }else{
            resp = request->beginResponse(401, "text/html", "401 Unauthorized");
            resp->addHeader("Connection","close");
            request->send(resp); 
        }
    });


    server.on("/FactoryReset", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (isAuthenticated(request)){
            log_v("cli reset");
            AsyncWebServerResponse *resp = request->beginResponse(200, "text/html", "Restoring factory settings!" ); 
            resp->addHeader("Connection","close");
            request->send(resp); 
            factoryReset();
            isRebootNow = true;
        }
        else{
            AsyncWebServerResponse *resp = request->beginResponse(401, "text/html", "401 Unauthorized");
            resp->addHeader("Connection","close");
            request->send(resp); 
        }
    });

    /*handling uploading firmware file */
    server.on("/doUpdate", HTTP_POST, [](AsyncWebServerRequest *request) {},[&](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
     
        if (!index){
            log_i("Update");
            content_len = request->contentLength();
            // if filename includes spiffs, update the spiffs partition
            int cmd = (filename.indexOf("spiffs") > -1) ? U_SPIFFS : U_FLASH;
            if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd)) {
                Update.printError(Serial);
            }
        }
        if (Update.write(data, len) != len) {
            Update.printError(Serial);
        }
    
        if (final){
            AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "Please wait while the device reboots");
            response->addHeader("Refresh", "20");  
            response->addHeader("Location", "/");
            request->send(response);
            if (!Update.end(true)){
                Update.printError(Serial);
            } else{
                log_i("Update complete");
                Serial.flush();
                isRebootNow = true;
            }
        }
      
    });

    ///////////////////////////////////////////////////////////////////////
     //Start WebSocket code -> first msg sent by client must be login password!
    socket.onEvent([&] (AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
        switch (type){
            case WS_EVT_CONNECT:{                                           //client connects
                log_v("sock client con rcv");
                client->text(F("Telemetry Socket Enabled")); //TODO make timer to close socket if psw is not sent by client
                break;
            }

            case WS_EVT_DISCONNECT:{                                        //client disconnects
                log_v("cli discon");
                break;
            }

            case WS_EVT_DATA:{                                              //client sends data
                AwsFrameInfo * info = (AwsFrameInfo*)arg;
                String msg = "";
                if(info->final && info->index == 0 && info->len == len){    //Ignore messages that come in more that 1 frame/packet --> max 1428 bytes
                    log_v("[RetailLiteWebCfg][%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
                    if(info->opcode == WS_TEXT){
                        for(size_t i=0; i < info->len; i++) {
                            msg += (char) data[i];
                        }
                    } else {
                        char buff[3];
                        for(size_t i=0; i < info->len; i++) {
                            sprintf(buff, "%02x ", (uint8_t) data[i]);
                            msg += buff ;
                        }
                    }
                    // Serial.printf("%s\n",msg.c_str());

                    if(info->opcode == WS_TEXT){
                        log_d("Sock RCV: %s", msg);
                        if (msg.startsWith("pos:")){
                            uint8_t pos = msg.substring(4).toInt();
                            log_i("Pos Cmd rcv: %d", pos);
                            if (pos>=0 && pos<=100){
                                myMotor.moveToPos(pos) ? client->text("ok") : client->text("err");
                            }
                            
                        }else if (msg == "zero"){
                            log_i("Zero CMD");
                            myMotor.setZero();
                            client->text("ok");

                        }else if (msg.startsWith("inc:")){
                            uint8_t pos = msg.substring(4).toInt();
                            log_i("Pos Cmd rcv: %d", pos);
                            if (pos>=0 && pos<=100){
                                myMotor.moveIncrement(pos) ? client->text("ok") : client->text("err");
                            }
                        }
                        
                    }
                }
                break;
            }

            default:{
                log_e("Default ERR");
                break;
            }
        }
    });

    server.addHandler(&socket);
    //END of websocket code


    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        log_v("cli root");
        AsyncWebServerResponse *resp = request->beginResponse(200, "text/html", APloginpage);
        resp->addHeader("Connection","close");
        request->send(resp); 
    });

    server.on("/getCurrPos", HTTP_ANY, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *resp;
        if (isAuthenticated(request)){
            log_v("cli getCurrPos");
            String pos = String(myMotor.getCurrPos(), DEC);
            log_d("CurrPos is %s", pos.c_str());
            resp = request->beginResponse(200, "text/html", pos );
            resp->addHeader("Connection","close");
            request->send(resp);
        }else{
            resp = request->beginResponse(401, "text/html", "401 Unauthorized");
            resp->addHeader("Connection","close");
            request->send(resp); 
        }
    });

    // server.on("/setPos", HTTP_GET, [&](AsyncWebServerRequest *request) {
    //     AsyncWebServerResponse *resp;
    //     if (isAuthenticated(request)){
    //         log_v("cli setcount");
    //         if (request->hasParam("currPos")){
    //             log_i("asdasdasd");
    //         }else{
    //             resp = request->beginResponse(200, "text/html", "Invalid Request" );
    //         }
    //         resp->addHeader("Connection","close");
    //         request->send(resp);
    //     }else{
    //         resp = request->beginResponse(401, "text/html", "401 Unauthorized");
    //         resp->addHeader("Connection","close");
    //         request->send(resp); 
    //     }
    // });


    server.on("/getDevName", HTTP_ANY, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *resp;
        if (isAuthenticated(request)){
            log_v("cli getCurrCounts");
            resp = request->beginResponse(200, "text/html", PAYNINOSTD_DEVICE_NAME ); 
            resp->addHeader("Connection","close");
            request->send(resp);
        }else{
            resp = request->beginResponse(401, "text/html", "401 Unauthorized");
            resp->addHeader("Connection","close");
            request->send(resp); 
        }
    });




    server.on("/getSwVersion", HTTP_ANY, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *resp;
        if (isAuthenticated(request)){
            resp = request->beginResponse(200, "text/html", SW_VERSION ); 
            resp->addHeader("Connection","close");
            request->send(resp);
        }else{
            resp = request->beginResponse(401, "text/html", "401 Unauthorized");
            resp->addHeader("Connection","close");
            request->send(resp); 
        }
    });



    server.on("/control", HTTP_ANY, [this](AsyncWebServerRequest *request) {
         AsyncWebServerResponse *resp;
        if (isAuthenticated(request)){
            log_v("cli dashboard");
            resp = request->beginResponse_P(200, "text/html", WC_home2);
            resp->addHeader("Set-Cookie", "qibixxID=1");
            resp->addHeader("Connection","close");
            request->send(resp); 
        }else{
            resp = request->beginResponse(401, "text/html", "401 Unauthorized");
            resp->addHeader("Connection","close");
            request->send(resp); 
        }
    });
    server.begin();
}





