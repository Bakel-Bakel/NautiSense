#include <Wire.h>
#include <MPU6500_WE.h>
#define MPU6500_ADDR 0x68
#define BUZZER_PIN 9

MPU6500_WE myMPU6500(MPU6500_ADDR);
float initialHeelAngle = 0.0;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    if (!myMPU6500.init()) {
        Serial.println(F("MPU6500 error!"));
    } else {
        Serial.println(F("MPU6500 OK"));
    }


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

    Serial.println(relativeHeelAngle);



     // Check if heel angle exceeds 30 degrees
    if (abs(relativeHeelAngle) > 30) {
        
        Serial.println(F("Warning: Heel Angle Exceeded 30!"));
    } else {
        
    }

    delay(500);
}