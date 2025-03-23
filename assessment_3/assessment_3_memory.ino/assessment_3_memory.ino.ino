#include <Wire.h>
#include <MPU6500_WE.h>
#include <U8g2lib.h>
#define MPU6500_ADDR 0x68
#define BUZZER_PIN 9

MPU6500_WE myMPU6500(MPU6500_ADDR);
// Use U8g2 (memory-efficient OLED library)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
float initialHeelAngle = 0.0;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(400000); // Increase I2C speed to reduce delays

    pinMode(BUZZER_PIN, OUTPUT);

    if (!myMPU6500.init()) {
        Serial.println(F("MPU6500 error!"));
    } else {
        Serial.println(F("MPU6500 OK"));
    }
    u8g2.begin();// OLED Initialization (U8g2 is more memory-efficient)
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.clearBuffer();
    u8g2.drawStr(10, 15, "OLED Ready!");
    u8g2.sendBuffer();

    // IMU Calibration
    myMPU6500.autoOffsets();
    xyzFloat gValue = myMPU6500.getGValues();
    initialHeelAngle = atan2(gValue.y, gValue.z) * 180.0 / PI;
    Serial.print(F("Initial Heel Angle: "));
    Serial.println(initialHeelAngle);
}

void loop() {
    xyzFloat gValue = myMPU6500.getGValues();
    float currentHeelAngle = atan2(gValue.y, gValue.z) * 180.0 / PI;
    float relativeHeelAngle = currentHeelAngle - initialHeelAngle;
    u8g2.clearBuffer();// Update OLED Display
    u8g2.setCursor(0, 10);
    u8g2.print(F("Heel Angle: "));
    u8g2.print(currentHeelAngle);
    u8g2.sendBuffer();

     // Check if heel angle exceeds 30 degrees
    if (abs(relativeHeelAngle) > 30) {
        digitalWrite(BUZZER_PIN, HIGH); // Turn on buzzer
        Serial.println(F("Warning: Heel Angle Exceeded 30!"));
    } else {
        digitalWrite(BUZZER_PIN, LOW); // Turn off buzzer
    }

    delay(500);
}
