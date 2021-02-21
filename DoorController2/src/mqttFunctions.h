#ifndef MQTT_FUNCTIONS_H
#define MQTT_FUNCTION_H

#include <AsyncMqttClient.h>
#include <PayninoStd.h>

//objects
AsyncMqttClient myMqttClient;
extern RetailLiteWebConfig web;
extern AsyncWebSocket ws;
//MQTT variables
char MQTT_LAST_WILL_MSG[75];                    ///< Last Will msg in case device dies or goes offline
bool isRetrySendMqttPush = false;               ///< if is to retry to send last msg, set to true
uint16_t lastPacketId;                          ///< last packet sent
//externs
extern bool isRebootNow;                        ///< flag to set to reboot if necessary
extern unsigned long globalPeopleCount;
extern unsigned long totalExits;
extern unsigned long totalEntries;

             

void mqttConnect(); //function header to allow calling from sendMqttPush
void sendMqttPush();


void signalPushSuccess(bool isSuccess){
    if (isSuccess){
        myLed.insertInQueue(OP_FUNCTION_CODE::BLINKXTIMES,2,1); //times, frequency
    }else{
        myLed.insertInQueue(OP_FUNCTION_CODE::BLINKXTIMES,5,4);
    }
}


//on connect --> subsribe to topics
void onMqttConnect(bool sessionPresent) {
    log_i("[MQTT] Connected to MQTT");
    log_d("[MQTT] Session present: %hhd\n", sessionPresent);
    uint16_t packetIdSub = myMqttClient.subscribe(web._mqttTopic.c_str(), web._mqttQos);
    log_d("[MQTT] Subscribing topic %s  --> QoS %d, packetId: %d\n",web._mqttTopic.c_str(), web._mqttQos, packetIdSub);
    if (web._lampTopic!=""){
        uint16_t packetLampIdSub = myMqttClient.subscribe(web._lampTopic.c_str(), web._mqttQos);
        log_i("[MQTT] Subscribing LAMP topic %s  --> QoS %d, packetId: %d\n",web._lampTopic.c_str(), web._mqttQos, packetLampIdSub);
    } 
    if (isRetrySendMqttPush){
        sendMqttPush();
        isRetrySendMqttPush = false;
    }
}

//if disconnect --> restart slave. Will enter infinite loop on boot searching for master
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    log_w("[MQTT] Disconnected from MQTT broker");
}

//on subscribe
void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
    log_d("[MQTT] Subscribe acknowledged.");
    log_v("[MQTT] packetId: %d\n", packetId); 
    log_v("[MQTT] qos: %d\n", qos);

}

//on unsubscribe (wont be used probably)
void onMqttUnsubscribe(uint16_t packetId) {
    log_d("[MQTT] Unsubscribe acknowledged.");
    log_d("[MQTT] packetId: %d", packetId);
}

//on msg received in topics
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    log_i("[MQTT] Publish received on topic: %s\n", topic);
    // log_d("[MQTT] qos: %d \n", properties.qos);
    // log_d("[MQTT] dup: %hhd\n", properties.dup );
    // log_d("[MQTT] retain: %hhd\n", properties.retain);
    if (strcmp(topic, web._lampTopic.c_str())==0){

        if ( strncmp(payload, (char*)"green", 5)==0 ) {
            if (myRelay.isHigh()){
                Serial.print("Toggling Relay to GREEN");
                myRelay.turnLow();
            } 
        }else if( strncmp(payload, (char*)"red", 3)==0){
                if (!myRelay.isHigh()){
                Serial.print("Toggling Relay to RED");
                myRelay.turnHigh();
            } 
        }

        if (ws.availableForWriteAll() && ws.enabled()){
            web.updateMsgToSendSocket();
            ws.textAll(web.msgToSendToSocketClients.c_str());
        }
    }
}

//on publish on subscribed topics --> private method DO NOT CALL
void onMqttPublish(uint16_t packetId) {
    log_i("[MQTT] Publish acknowledged.");
    log_v("[MQTT] packetId: %d\n", packetId);
    //check if it was packet sent by itself
    if (packetId == lastPacketId){
        signalPushSuccess(true);
    }
}

//config MQTT client --> first one to be called
template <typename T> 
void configureMqtt(AsyncMqttClient &mqttClient, T MqttHost, uint16_t MqttPort = web._mqttPort){
    log_i("[MQTT] Configuring MQTT");
    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onSubscribe(onMqttSubscribe);
    mqttClient.onUnsubscribe(onMqttUnsubscribe);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.onPublish(onMqttPublish);
    //If uname provided, set uname and psw for mqtt client
    if (web._mqttUname!="") mqttClient.setCredentials(web._mqttUname.c_str(), web._mqttPsw.c_str()); //remove
    snprintf(MQTT_LAST_WILL_MSG, 75, "{\r\n\t\"Device\":\"%s\",\r\n\t\"Msg\":\"%s\"\r\n}", PAYNINOSTD_DEVICE_NAME, "Device Offline" );
    mqttClient.setClientId(PAYNINOSTD_DEVICE_NAME);
    mqttClient.setServer(MqttHost, MqttPort);
    myMqttClient.setWill(web._mqttTopic.c_str(), web._mqttQos, web._mqttRetain, MQTT_LAST_WILL_MSG );
    log_v("[MQTT] Finished setting events");
}


//get Dev mac address
String getMacAddress() {
	uint8_t baseMac[6];
	// Get MAC address for WiFi station
	esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
	char baseMacChr[18] = {0};
	sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
    log_i("MAC is: %s\n", baseMacChr);
	return String(baseMacChr);
}

//format message to push HTTP or MQTT
String formatPushMsg(){
    return (String)"{\r\n\t\"Device\":\""+String(PAYNINOSTD_DEVICE_NAME)+"\",\r\n\t\"GlobalCount\":"+String(globalPeopleCount)+",\r\n\t\"TotalEntries\":"+String(totalEntries)+",\r\n\t\"TotalExits\":"+String(totalExits)+"\r\n}";
}

/**
 * @brief Send count to MQTT BROKER
 * If disconnected, tries reconnecting
 * 
 */
void sendMqttPush(){
    if (myMqttClient.connected()){
        String strToPublish=formatPushMsg();
        log_i("[MQTT] Will publish:\n%s\n", strToPublish.c_str());
        uint16_t packetIdPub = myMqttClient.publish(web._mqttTopic.c_str(), web._mqttQos, web._mqttRetain, strToPublish.c_str());
        log_v("[MQTT]Qos: %d | retain: %hhd | packetId Pub is: %d\n", web._mqttQos, web._mqttRetain, packetIdPub);
        lastPacketId = packetIdPub;

    }else{
        log_e("[MQTT] MQTT is not connected!!");
        signalPushSuccess(false);
        mqttConnect();
        isRetrySendMqttPush = true;
    }
}

/**
 * @brief Connect to MQTT broker
 * 
 */
void mqttConnect(){
    log_i("[MQTT] Connecting to MQTT broker");
    myMqttClient.connect();

}



#endif