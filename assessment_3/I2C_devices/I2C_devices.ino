#include <Wire.h>

void setup() {
    Serial.begin(115200);
    Wire.begin();
    Serial.println("Scanning I2C devices...");
    
    for (uint8_t address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) {
            Serial.print("Device found at 0x");
            Serial.println(address, HEX);
        }
    }
    
    Serial.println("Scan complete.");
}

void loop() {}
