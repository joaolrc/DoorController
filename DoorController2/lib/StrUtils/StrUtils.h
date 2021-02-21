/** 
 * Module with useful functions to parse strings
 * \author João Costa $
 * \version $Revision: 0.1 $
 * \date $Date: 2020/10 $
 * Contact: jc@ccode.pt
 * Created on: Mon July 27 2020
*/

#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <Arduino.h>

# if !(defined(ARDUINO) && ARDUINO >= 100)
#include "WString.h"
#endif

#define base16char(i) ("0123456789ABCDEF"[i])

/** 
 * Returns true if str contains only numeric chars
*/
bool strUtils_isStrNumeric(String myStr);

/** 
 * @brief Returns true if str looks like a Valid Ip address
*/
bool strUtils_isValidIp(String ip);

/** 
 * Returns true if str looks like a Valid domain
*/
bool strUtils_isValidDomain(String domain);

/**
 * Returns number if char c occurences in string
*/ 
uint8_t strUtils_countChar(String str , char c='.' );

/**
 * @brief Returns IPAddress object passed, as string
 * 
 * @param ipAddress ip adress to convert
 * @return String 
 */
String strUtils_ipAddress2String(IPAddress ipAddress);

/**
 * @brief Checkes if a string is base64 encoded
 * 
 * @param base64String string to analyse
 * @param firstIndexToScan index where to start scanning (default to 0)
 * @return true if str is base64 encoded
 * @return false if str is not base64
 */
bool strUtils_isBase64(char * base64String, uint16_t firstIndexToScan = 0);

/**
 * @brief Checks if str is valid regID
 * 
 * @param regId input str
 * @return true if valid regID
 * @return false if invalid regID
 */
bool strUtils_isValidRegId(char* regId);


/**
 * @brief converts int64 to str
 * 
 * @param value to be converted
 * @param base DEC || HEX || OCT
 * @param prefix 
 * @param sign 
 * @return String returned
 */
String strUtils_int64String(uint64_t value, uint8_t base= DEC, bool prefix= false, bool sign= false);

/**
 * @brief converts int64 to string
 * 
 * @param value int64 value
 * @param base DEC || HEX || OCT
 * @param prefix 
 * @return String returned
 */
String strUtils_int64String(int64_t value, uint8_t base= DEC, bool prefix= false);

/**
 * @brief If endpoint starts with https:// , replace that to http://
 * 
 * @param endpoint reference to the string to modify
 */
void strUtils_enforceHttp(String &endpoint);

#endif
