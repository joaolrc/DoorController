#ifndef DOOR_CONTROLLER_WEB_CFG_H
#define DOOR_CONTROLLER_WEB_CFG_H

#include <Arduino.h>
#include <FS.h>
#include <Update.h>
#include <ESPAsyncWebServer.h>
// #include <functional>
#include <Preferences.h>
// #include <CaptiveRequestHandler.h>
#include <DNSServer.h>
#include <LinearMotorController.h>

// #define DCWC_VERBOSE_LVL 3     //1 to 3 || Comment to supress debug
#define DCWC_ULONGMAXLEN 30
#define DCWC_EEPROM_DEFAULTVALUE 9696
#define DCWC_MQTT_DEFAULT_PORT 1883
#define DCWC_MQTT_DEFAULT_QOS 2
#define DCWC_MQTT_DEFAULT_RETAIN false
#define DCWC_EEPROM_DEFAULTSTRING "----------"250
// #define DCWC_EEPROM_DEFAULT_MIN_TIME_BETWEEN_COUNTS_MS 100
//EEPROM Namespaces
#define DCWC_EEPROM_NAME_DYNAMIC_CFG "A"
#define DCWC_EEPROM_NAME_STATIC_CFG "B"    //static variables (not compiler static) like SWVERSION and regId
//static Configurations
#define DCWC_EEPROM_SWVERSION "BA"
#define DCWC_EEPROM_REGID "BB"
//Dynamic Configurations
//User Variables
#define DCWC_EEPROM_NOTFIRSTUSE "AA"   //starts as 0 and changes to 1 after first initialization -> used for setting defaults if no config provided
#define DCWC_EEPROM_ISWIFICLIENT "AB"  // if -2 -> WiFi not specified. If -1 -> specified but never conected. If 0 -> connected
//Network Variables
#define DCWC_EEPROM_WIFI_SSID "AC"
#define DCWC_EEPROM_WIFI_PSW "AD"
#define DCWC_EEPROM_WIFI_IP "AE" 
#define DCWC_EEPROM_WIFI_GATEWAY "AF"
#define DCWC_EEPROM_WIFI_SUBNET "AG"
//Other config
#define DCWC_EEPROM_MOTOR_OPEN_TIME "AH"   //5
#define DCWC_EEPROM_IS_HTTP "AI"            //true
#define DCWC_EEPROM_PUSH_SERVER "AJ"
#define DCWC_EEPROM_MQTT_TOPIC "AK"
#define DCWC_EEPROM_MQTT_PORT "AL"          //1883
#define DCWC_EEPROM_MQTT_QOS "AM"           //2
#define DCWC_EEPROM_MQTT_RETAIN "AN"        //Degfaults to false
#define DCWC_EEPROM_MQTT_UNAME    "AS"
#define DCWC_EEPROM_MQTT_PSW    "AT"
#define DCWC_EEPROM_KEEP_ALIVE   "AV"
#define DCWC_EEPROM_BEARER_TOKEN   "AX"
#define DCWC_EEPROM_MQTT_LAMP_TOPIC   "AZ"
#define DCWC_EEPROM_WPA2_IDENTITY "AAA"
#define DCWC_EEPROM_WPA2_PSW "AAB"
#define DCWC_EEPROM_WPA2_ANONYMOUSID "AAC"


#define DCWC_EEPROM_NET_MODE "AU"
// #define DCWC_NETMODE_AUTO 0
// #define DCWC_NETMODE_ETH 1
// #define DCWC_NETMODE_WIFI 2
#define DCWC_NETWORK_KEYWORD_WIFI "WiFi"
#define DCWC_NETWORK_KEYWORD_ETH "Ethernet"
#define DCWC_NETWORK_KEYWORD_AUTO "Auto"


//Credentials
#define DCWC_EEPROM_LOGIN_UNAME "AP"
#define DCWC_EEPROM_LOGIN_PSW   "AQ"
//Wifi connection status
#define DCWC_WIFISTATUS_ONLINE 0
#define DCWC_WIFISTATUS_NETWORK_PROVIDED -1
#define DCWC_WIFISTATUS_OFFLINE -2
#define DCWC_WIFISTATUS_NETWORK_UNREACHABLE -3



class DoorControllerWebConfig{
public:
    /**
     * @brief Construct a new Retail Lite Web Config object
     * No code executed here
     */
    DoorControllerWebConfig();

     /**
     * @brief Define endpoints, events and launch Operation Server.
     * Some network interface should be initialized before calling this method
     * @param server Object of type AsyncWebServer from EspAsyncWebServer lib
     */
    void createServer(AsyncWebServer &server, AsyncWebSocket &socket);
    /**
     * @brief Check if SWversion is different and reset to defaults if true. Also calls reloadVariables()
     * 
     */
    void loadConfig();
    /** \todo enhance performance on tis
     * @brief Update JSON payload to send to browser socket
     * 
     */
    void updateMsgToSendSocket();



    Preferences myEEPROM_dynamic;                       ///< NVS dedicated to memory changeable by the user. Usefull for Reset to Defaults
    Preferences myEEPROM_static;                        ///< NVS dedicated for static (input once) configuration (ex. RegID)
    DNSServer dnsServer;

    uint8_t _networkMode;  ///< 0=auto, 1=ETH, 2=WIFI - check PayninoStd for information about network modes

    // NETWORKING
    int _wifiStatus;
    String _psw;
    String _ssid;
    String _regId;                        //unused but maybe needed for later
    String _ipAddress;
    String _subnetMask;
    String _defaultGateway;  
    String _wpa2_identity;
    String _wpa2_psw;
    String _wpa2_anonymousId;

    //PUSH SETTINGS
    bool _isSinglePathway;
    bool _isHttpPush;
    String _pushServer;                 //http/mqtt server/broker
    unsigned long _mqttPort;
    String _mqttTopic;
    bool _mqttRetain;
    uint8_t _mqttQos;
    String _mqttUname;          
    String _mqttPsw;                                                                ///< mqtt psw
    
    String _bearerToken;                                                            ///< HTTP authorization header
    unsigned long _motorOpenTime;  
    String _lampTopic;                                                              ///< mqtt topic to control relay/signage
    

private:
    size_t content_len;                                     ///< Used with firmware update

    //credentials and networking
    String _loginPsw;               ///< Password for WebUI login
    String _loginUname;             ///< Username for WebUI login



    /** \todo add security
     * @brief Check if HTTP request is authenticated
     * 
     * @param request Object of type AsyncWebServer
     * @return true if request is authenticated
     * @return false if request is not authenticated
     */
    bool isAuthenticated(AsyncWebServerRequest *request);
    
    //Template engine for AP dashboard
    String updateApDash(String AP_dash_html);

    //Template engine for Wifi dashboard
    String updateWifiPage(String AP_wificon);

    /**
     * @brief updat page template engine
     * 
     * @param AP_firmware HTML to be processed
     * @return String modified page
     */
    String updateFirmwarePage(String AP_firmware);

    /**
     * @brief credentials page template engine
     * 
     * @param AP_credentials 
     * @return String modified page
     */
    String updateCredentialsPage(String AP_credentials);

    /**
     * @brief pushSettings page templating engine
     * 
     * @param AP_pushSettings_html HTML to be processed
     * @return String modified page
     */
    String updatePushSettingsPage(String AP_pushSettings_html);

    /**
     * @brief Simple debug print all variables
     * 
     */
    void printVars();

    /**
     * @brief Clear EEPROM/Preferences
     */
    void factoryReset();

    /**
     * @brief Check if uploaded SWversion is different from the one in NVS
     * Usefull for seting defaults of the new application in case anyv old config can break it
     * @return true if New Version diferent than Old Version
     * @return false Otherwise
     */
    bool isSwDifferentVersion();

    /**
     * @brief Reload RAM variables from NVS storage
     */
    void reloadVariables();


    /**
     * @brief Set the Default Configuration
     * Wipes all NVS, setting it to the default one,s EXCEPT REGID
     */
    void setDefaultConfig();

};

#endif
