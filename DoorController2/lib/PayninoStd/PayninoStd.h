/**
 \file PayninoStd.h
 * Paynino Standard Library - Contains methods to use with both ETH and WiFi boards. 
 * Standard Network init method which assynchronously connects using the most suitable interface or the defined one (recommended in ETH board)
 * Sets pin numbers, default directions and init values
 * \bug Caveat: Config button (SDA) is shared with the I2C expander (PCA9536)
 * \author João Costa, CCODE
 * \date November 2020
*/

#ifndef PAYNINO_STD_H
#define PAYNINO_STD_H
#include <Arduino.h>
#include <ETH.h>
#include <WiFi.h>
#include <StrUtils.h>
#include <OutputControl.h>

typedef void (*generalVoidFunction_t)(void);
typedef void (*generalUintFunction_t)(uint16_t);
typedef void (* generalUlongFunction_t)(unsigned long);

#define PAYNINOSTD_LEDSIGNAGE_CONNECTINGTONETWORK myLed.blinkXTimesBlocking(2,5)        ///< When Connecting to network if blocking call is used
#define PAYNINOSTD_LEDSIGNAGE_ENTERLOOP myLed.blinkXTimesDifTimesBlocking(3,1000,200)   ///< When Init is done and dev will enter loop
#define PAYNINOSTD_LEDSIGNAGE_WIFIOK myLed.blinkXTimesBlocking(5, 2)                    ///< Conected to network via WiFi
#define PAYNINOSTD_LEDSIGNAGE_ETHOK myLed.blinkXTimesBlocking(6, 2)                     ///< Connected to network via Ethernet
#define PAYNINOSTD_FIRMWARE_FOOTPRINT Serial.printf("%sV%s-%d-%s\n\n",DEVICE_NAME, SW_VERSION, PAYNINO_BOARD_TYPE, __DATE__ " " __TIME__) ///< Print DeviceName,version, type, BoardType, Compilation Time

// Board Types
#define PAYNINO_BOARD_TYPE_WIFI 1
#define PAYNINO_BOARD_TYPE_ETH 2


//Pin Definitions Common to both board types
#define PAYNINOPIN_BRD_DISCOVERY 36           ///< Pin to use to distinguish boards

#define PAYNINOPIN_IN1             34         ///< COUNTER
#define PAYNINOPIN_IN2             35
#define PAYNINOPIN_ONBOARDLED      32
#define PAYNINOPIN_PIHATRST        33       ///< Pihat rst. Not initialized
#define PAYNINOPIN_CURRENTSENSE    39       ///< Not Initialized
#define PAYNINOPIN_SERIAL0_RX       3       ///< default HW Serial 0 RX
#define PAYNINOPIN_SERIAL0_TX       1       ///< default HW Serial 0 TX

//LED and relay States
#define PAYNINOSTD_LEDSTATE_OFF              LOW
#define PAYNINOSTD_LEDSTATE_ON               HIGH
#define PAYNINOSTD_RELAYSTATE_OFF            LOW
#define PAYNINOSTD_RELAYSTATE_ON             HIGH

//Networking modes
#define PAYNINOSTD_NETMODE_AUTO 0              ///< Discover suitable Interface
#define PAYNINOSTD_NETMODE_ETH 1               ///< Force ETH
#define PAYNINOSTD_NETMODE_WIFI 2              ///< Force WiFi
#define PAYNINOSTD_NETMODE_OFFLINE 3           ///< Force Offline


//Global singleton like variables common to all programs
extern uint8_t PAYNINO_BOARD_TYPE;                            ///< Board Type
extern char PAYNINOSTD_DEVICE_NAME[50];                       ///< DEVICE_NAME-WiFiMacAddress
extern bool payninoStd_hasNetworkConnectivity;                ///< One Network interface is initialized and has conectivity
extern bool payninoStd_hasEthConnectivity;                    ///< check if network interface in use is Ethernet
extern bool payninoStd_hasWifiConnectivity;                   ///< check if network interface in use is WiFi
extern char PAYNINOSTD_WIFI_MAC[18];                          ///< holds WiFi MAC after wifi init method has been called
extern char PAYNINOSTD_ETH_MAC[18];                           ///< holds ETH MAC after eth init method has been called
extern OutputControl myRelay;                                 ///< Global Relay instance
extern OutputControl myLed;                                   ///< Global LED instance
extern OutputControl myOut1;                                  ///< Global OUT1 instance
extern OutputControl myOut2;                                  ///< Global OUT2 instance
extern ExpanderControl myExpander;
extern char payninoStd_ipWifi[16];
extern char payninoStd_ipEth[16];

extern uint8_t PAYNINOPIN_SDA, PAYNINOPIN_SCL,                                      ///< I2C
PAYNINOPIN_RELAY, PAYNINOPIN_OUT1, PAYNINOPIN_OUT2,                                 ///< onBoard OUT's
PAYNINOPIN_IO4, PAYNINOPIN_IO2,                                                     ///< header IO's
PAYNINOPIN_ETH_PWR_EN, PAYNINOPIN_ETH_TX_EN,                                        ///< Eth
PAYNINOPIN_SERIAL2_RX, PAYNINOPIN_SERIAL2_TX,                                       ///< serial2
PAYNINOPIN_SPI_CS, PAYNINOPIN_SPI_MOSI, PAYNINOPIN_SPI_MISO, PAYNINOPIN_SPI_SCK;    ///< SPI (SD CARD)           




/**
 * \brief General Init method. Should be the first function to be called when board boots
 * Calls private methods to discover board type and init pins according to it, also setting pin variables
 * Inits Strings with Mac addresses and PAYNINO_DEVICE_NAME
 * Scans for side button (config) pressed.
 * \returns true if button is pressed. otherwise false
 */
bool payninoStd_Init();

/// \todo maybe set callbacks for Network Con and no con
/**
 * \brief Registers Network Connection. 
 * IF mode==WIFI ssid and password must be specified
 * If mode==AUTO, dev turns ON both interfaces and chooses the one which come alive first, turning OFF the other one
 * Possibility to specify fixed network credentials as String array {ipAddr, defaultgateway, subnetmask}
 * @param networkMode Specify network interface - PAYNINOSTD_NETMODE_WIFI | PAYNINOSTD_NETMODE_ETH | PAYNINOSTD_NETMODE_AUTO
 * @param ssid WiFI SSID. Needed if networkMode==PAYNINOSTD_NETMODE_WIFI
 * @param psw WiFI Password. Needed if networkMode==PAYNINOSTD_NETMODE_WIFI
 * @param ip Static IP address to be assign to the network interface - optional
 * @param defaultGateway Static gateway IP address to be assign to the network interface - optional
 * @param subnetMask Static subnet Mask to be assign to the network interface - optional
 * \returns true if connection registered succesfully otherwise false
 */
bool payninoStd_connectToNetwork(uint8_t networkMode=PAYNINOSTD_NETMODE_AUTO, const char* ssid=(char*)"", const char* psw=(char*)"", const char* ip=(char*)"", const char* defaultGateway=(char*)"", const char* subnetMask=(const char*)"", const char* eapIdentity=nullptr, const char* eapPassword=nullptr, const char* eapAnonymousId=nullptr);

/**
 * If Eth board, always call this method to initialize IO expander communication and respective outputs
 * Method not included in InitPins() because of shared pin between IO Expander and Config button (SDA)
 * Returns true if initialization was successfull. Otherwise false
 * \returns If WiFi board, this method will initialize OutputControl's and returns always true
 */
bool payninoStd_InitOutputs();

/**
 * Enters configuration mode as usually done in esp32 products
 * Starts AP, broadcasting open network with SSID=DevName-MacAddress
 * @param ssid AP SSID. if specified or empty, will be computed through define "DEVICE_NAME" in platformio.ini--> <DEVICE_NAME>-<MacAddress>
 * @param psw AP password. If not specified or empty, network will snot require authentication
 * @param callback void callback to be executed after AP initialization
 * \returns true if initialization successfull
 */
bool payninoStd_InitConfigMode(const char* ssid=NULL, const char* psw=NULL, generalVoidFunction_t callback=nullptr);

/**
 * \todo Prepare SD card for writting logs. Maybe do a class per file or so...
 * 
 */
// bool payninoStd_InitSdCard();

//////////////////////////////////////// PRIVATE //////////////////////////////////////

/**
 * Verification of configuration enable at start-up (side button pressed)
 * Must be called before initializing I2C communications
 * @param callback (optional) If config button pressed, callback will be called if != nullptr
 * \returns If jumper on configuration port is set or I2C SDA LOW (side button pressed), returns true, otherwise, false.
 */
bool _payninoStd_IsCfg_Enable(generalVoidFunction_t callback = nullptr);


/**
 * Discovers Board Type and returns it. 
 * WARNING - this function modifies direction of GPIO 36 to input
 * Fills PAYNINO_BOARD_TYPE Global Variable
 * \returns board type - see macros PAYNINO_BOARD_TYPE_...
 */
uint8_t _payninoStd_GetBoardType();


/**
 * Inits Pins according to board Type as they are eletrically prepared to
 * IF board = 0; uses payninoBoardType Gloabal Variable
 * Should be called after _payninoStdGetBoardType()
 * @param board (optional) specify which board in use. If 0 or not passed, func will see variable PAYNINO_BOARD_TYPE
 * \returns true if initialization success. Otherwise false
 */
void _payninoStd_initPins(uint8_t board = 0);


/**
 * Inits Ethernet adapter
 * Returns true if successfully initalized. False otherwise (probably trying to init ETH in WiFi only board)
 * @param ethConfigurationStrings (optional) c_string array with {ipAddr, defaulGateway, subnetMask }
 * \returns true if init success
 */ 
bool _payninoStd_InitEth( const char* ethConfigurationStrings[3]=nullptr);


/**
 * Inits WiFi adapter
 * Possibility to Pass String array with {ipAddr, defaulGateway, subnetMask }
 * \param ssid WiFi SSID to connect to
 * \param psw WiFi password to connect to. Leave empty if open WiFi
 * \param wifiConfigurationStrings (optional) c_string array with {ipAddr, defaulGateway, subnetMask }
 * \returns true if successfully initalized
*/ 
bool _payninoStd_InitWifiClient(const char* ssid, const char* psw, const char ** wifiConfigurationStrings=nullptr, const char* eapIdentity=nullptr, const char* eapPassword=nullptr, const char* eapAnonymousId=nullptr);

/**
 * Network Event driven function
 * Sets hasNetworkConnectivity variable for application to know if can proceed with communications
 * Called by underlying OS
 */
void _payninoStd_networkEvent(WiFiEvent_t event, WiFiEventInfo_t info);

/**
 * \brief Gets WiFi macc address and puts into PAYNINOSTD_WIFI_MAC 
 * After payninoStd_Init(), address is in global variable PAYNINOSTD_WIFI_MAC
 */
void _payninoStd_getWifiMacAddress();

/**
 * \brief Gets ETH mac address and puts into PAYNINOSTD_ETH_MAC 
 * After payninoStd_Init(), address is in global variable PAYNINOSTD_ETH_MAC
 */
void _payninoStd_getEthMacAddress();



#endif

