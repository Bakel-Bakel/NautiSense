#include <Wire.h>
#include "MPU6500_WE.h"

#define MPU6500_ADDR 0x68
#define BUZZER_PIN 9

MPU6500_WE myMPU6500(MPU6500_ADDR);

float initialRoll = 0.0;
float initialPitch = 0.0;
float yaw = 0.0;  // yaw must be integrated from gyroscope

void setup() {
    Serial.begin(115200);
    Wire.begin();

    if (!myMPU6500.init()) {
        Serial.println(F("MPU6500 error!"));
    } else {
        Serial.println(F("MPU6500 OK"));
    }

    // Calibration
    myMPU6500.autoOffsets();
    myMPU6500.enableGyrDLPF();
    myMPU6500.setGyrRange(MPU6500_GYRO_RANGE_250); // Lower noise

    xyzFloat g = myMPU6500.getGValues();
    initialRoll = atan2(g.y, g.z) * 180.0 / PI;
    initialPitch = atan2(-g.x, sqrt(g.y * g.y + g.z * g.z)) * 180.0 / PI;
}

void loop() {
    static unsigned long lastTime = millis();
    unsigned long now = millis();
    float dt = (now - lastTime) / 1000.0; // Convert ms to s
    lastTime = now;

    xyzFloat g = myMPU6500.getGValues();       // accelerometer
    xyzFloat gyr = myMPU6500.getGyrValues();   // gyroscope in °/s

    // Calculate roll and pitch using accelerometer
    float roll = atan2(g.y, g.z) * 180.0 / PI - initialRoll;
    float pitch = atan2(-g.x, sqrt(g.y * g.y + g.z * g.z)) * 180.0 / PI - initialPitch;

    // Calculate yaw by integrating gyroscope z-axis over time
    yaw += gyr.z * dt;

    // Normalize yaw to 0-360 degrees
    if (yaw < 0) yaw += 360;
    if (yaw > 360) yaw -= 360;

    // Print values
    Serial.print(roll, 2);
    Serial.print(",");
    Serial.print(pitch, 2);
    Serial.print(",");
    Serial.println(yaw, 2);

    delay(100);
}
