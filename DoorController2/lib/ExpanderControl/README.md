## ExpanderControl

Class to interface PCA9536 via I2C bus

Supports methods such as digitalWrite, pinMode and other arduino familiar stuff. Exmaple usage below.

```cpp
#include <Arduino.h>  //Dont fuck with include order
#include <./../../../cc_esp32_comp/OutputControl/OutputControl.h>
#include <ExpanderControl.h>


#define I2C_SDA 13
#define I2C_SCL 16
bool isRebootNow = false; //any module can set this to true to restart the esp

OutputControl myLed(32);
ExpanderControl myExpander;


/////////////////////////////////////////////////// SETUP //////////////////////////////////////////////////
void setup() {
    Serial.begin(115200);
    log_d("Serial Port opened successfully!\n");
    const char compile_date[] = __DATE__ " " __TIME__;
    Wire.begin(I2C_SDA, I2C_SCL);
      // Initialize the PCA9536 with a begin function
    if (myExpander.begin() == false){
        Serial.println("PCA9536 not detected. Please check wiring. Freezing...");
        while (1);
    }

    myExpander.pinMode(2,OUTPUT);
    delay(20);
    myExpander.digitalWrite(2,HIGH);
    Serial.print("HIGH\n");
    delay(1000);
    myExpander.digitalWrite(2,LOW);
    Serial.print("LOW\n");

    Serial.print("[MAIN] Will enter loop\n");
    myLed.blinkXTimesDifTimesBlocking(3,1000,200);
}

/////////////////////////////////////////////////// LOOP //////////////////////////////////////////////////
void loop() {

    myLed.poll();

}


```
