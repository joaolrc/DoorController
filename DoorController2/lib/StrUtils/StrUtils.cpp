#include <StrUtils.h>

bool strUtils_isValidIp(String ip){
    if (ip=="" || strUtils_countChar(ip)!=3){
        return false;
    }
    uint8_t dotsIdx[4];
    dotsIdx[0] = ip.indexOf(".");
    dotsIdx[1] = dotsIdx[0]+ip.substring(dotsIdx[0]+1).indexOf(".")+1;
    dotsIdx[2] = ip.lastIndexOf(".");
    String ipSegment;

    for (uint8_t i=0; i<4; i++){
        // Serial.printf("Dots IDX %i--> %i\n", i, dotsIdx[i]);
        if (i==0) ipSegment = ip.substring(0,dotsIdx[0]);
        else if (i!=3) ipSegment = ip.substring(dotsIdx[i-1]+1,dotsIdx[i]);
        else if (i==3) ipSegment = ip.substring(dotsIdx[i-1]+1);

        // Serial.printf("--Segment %i --> %s\n", i, ipSegment.c_str());

        if (!strUtils_isStrNumeric(ipSegment)){
            return false;
        }

        if (ipSegment.toInt()>255 || ipSegment.toInt()<0){
            return false;
        }
    }

    return true;
}

bool strUtils_isValidDomain(String domain){
    if (!(strUtils_countChar(domain)>0)){
        return false;
    }
    if (domain.length()<3){
        return false;
    }

    return true;
}


bool strUtils_isStrNumeric(String myStr){
    for (uint16_t i = 0; i< myStr.length(); i++){
        if ( !isDigit(myStr[i]) && myStr[i]!='.' ){
            return false;
        }
    }
    return true;
}

uint8_t strUtils_countChar(String str , char c){
    uint8_t count=0;
    for (uint8_t i=0; i<str.length(); i++){
        count += (str[i] == c);
    }
    return count;
}

String strUtils_ipAddress2String(IPAddress ipAddress){
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ;
}


bool strUtils_isBase64(char * base64String, uint16_t firstIndexToScan) {
    for ( uint8_t i = firstIndexToScan; i < strlen(base64String); i++ ){
        if ( !(isalnum(base64String[i]) || (base64String[i] == '+') || (base64String[i] == '/')) ){
            log_v("not b64: %c", base64String[i]);
            return false;
        }
    }
    return true;
}

bool strUtils_isValidRegId(char* regId){
    return !((strcmp(regId, "") == 0) || regId==NULL || !strUtils_isBase64(regId,1) || strlen(regId)!=12);
}


// Unsigned integer function, it does the bulk of the work, returns a String
// Based on work done by Rob Tillaart on the print9 function from:
// https://forum.arduino.cc/index.php?topic=143584.msg1519824#msg1519824
String strUtils_int64String(uint64_t value, uint8_t base, bool prefix, bool sign) {
    // keep base within "reasonable limits
    if (base < 2)
        base = 2;
    else if (base > 16)
        base = 16;

    // start at position 64 (65th byte) and work backwards
    uint8_t i = 64;
    // 66 buffer for 64 characters (binary) + B prefix + \0
    char buffer[66] = {0};

    if (value == 0)
        buffer[i--] = '0';
    else {
        uint8_t base_multiplied = 3;
        uint16_t multiplier = base * base * base;

        if (base < 16) {
            multiplier *= base;
            base_multiplied++;
        }
        if (base < 8) {
            multiplier *= base;
            base_multiplied++;
        }

        // Five 64 bit devisions max
        while (value > multiplier) {
            uint64_t q = value / multiplier;
            // get remainder without doing another division with %
            uint16_t r = value - q * multiplier;

            for (uint8_t j = 0; j < base_multiplied; j++) {
                uint16_t rq = r / base;
                buffer[i--] = base16char(r - rq * base);
                r = rq;
            }

            value = q;
        }

        uint16_t remaining = value;
        while (remaining > 0) {
            uint16_t q = remaining / base;
            buffer[i--] = base16char(remaining - q * base);
            remaining = q;
        }
    }

    // Signed numbers only make sense for decimal numbers
    if (base == DEC && sign)
        buffer[i--] = '-';
    else if (prefix) {
        if (base == HEX) {
            // 0x prefix
            buffer[i--] = 'x';
            buffer[i--] = '0';
        }
        else if (base == OCT) buffer[i--] = '0';
        
        else if (base == BIN) buffer[i--] = 'B';
    }

    // return String starting at position i + 1
    return String(&buffer[i + 1]);
}

// Signed integer function, returns a String
String strUtils_int64String(int64_t value, uint8_t base, bool prefix) {
    // Signed numbers only make sense for decimal numbers
    bool sign = base == DEC && value < 0;
    // if signed, make it positive
    uint64_t uvalue = sign ? -value : value;

    // call the unsigned function to format the number
    return strUtils_int64String(uvalue, base, prefix, sign);
}

void strUtils_enforceHttp(String &endpoint){
    if ( !endpoint.isEmpty() && endpoint.substring(0,5).equalsIgnoreCase("https")) endpoint.replace("https://", "http://");
}