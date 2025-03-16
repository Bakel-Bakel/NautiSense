#include <MPU6500_WE.h> // MPU6500 IMU library
#include <Wire.h>       // I2C communication library
#include <math.h>       // Math library for calculations

#define MPU6500_ADDR 0x68   // I2C address of MPU6500
#define BUZZER_PIN 9        // Buzzer is connected to pin 9

MPU6500_WE myMPU6500 = MPU6500_WE(MPU6500_ADDR); //Create the IMU object
float initialHeelAngle = 0.0;  // Initial heel angle reference

void setup() {

    Serial.begin(115200);  // Initialize Serial communication for debugging
    Wire.begin();          // Start I2C communication

    pinMode(BUZZER_PIN, OUTPUT); // Set buzzer pin as OUTPUT

    // Check if the MPU6500 sensor is detected and initialized properly
    if (!myMPU6500.init()) {
        Serial.println("MPU6500 does not respond"); // Print error message if not detected
    } else {
        Serial.println("MPU6500 is connected"); // Print success message if detected
    }

    // Inform the user to place the IMU flat for calibration
    Serial.println("Position your MPU6500 flat and don't move it - calibrating...");
    delay(1000); // Wait for 1 second before starting calibration

    // Automatically determine and set offsets for accurate readings
    myMPU6500.autoOffsets();
    Serial.println("Done!"); // Notify that calibration is complete

    // Enable the Digital Low Pass Filter (DLPF) for the gyroscope to reduce noise
    myMPU6500.enableGyrDLPF();

    // Set the DLPF level for the gyroscope to 6 for optimal noise reduction
    myMPU6500.setGyrDLPF(MPU6500_DLPF_6);

    // Set the sample rate divider to 5 (reducing data collection frequency for stability)
    myMPU6500.setSampleRateDivider(5);

    // Set the gyroscope range to ±250 degrees per second (highest precision)
    myMPU6500.setGyrRange(MPU6500_GYRO_RANGE_250);

    // Set the accelerometer range to ±2g (for high accuracy in small movements)
    myMPU6500.setAccRange(MPU6500_ACC_RANGE_2G);

    // Enable the Digital Low Pass Filter (DLPF) for the accelerometer
    myMPU6500.enableAccDLPF(true);

    // Set the DLPF level for the accelerometer to 6 for reduced noise
    myMPU6500.setAccDLPF(MPU6500_DLPF_6);

    // Read the initial accelerometer values to establish a reference angle
    xyzFloat gValue = myMPU6500.getGValues();

    // Compute the initial heel angle around the x-axis using arctangent
    initialHeelAngle = atan2(gValue.y, gValue.z) * 180.0 / PI;

    // Print the initial heel angle for reference
    Serial.print("Initial Heel Angle (X-axis): ");
    Serial.println(initialHeelAngle);

    // Small delay before entering the main loop
    delay(200);
}

void loop() {
    // Read accelerometer, gyroscope, and temperature data
    xyzFloat gValue = myMPU6500.getGValues();
    xyzFloat gyr = myMPU6500.getGyrValues();
    float temp = myMPU6500.getTemperature();
    float resultantG = myMPU6500.getResultantG(gValue);
    xyzFloat angles = myMPU6500.getAngles();  // Read pitch, roll, and yaw

    // Compute the current heel angle (tilt around x-axis)
    float currentHeelAngle = atan2(gValue.y, gValue.z) * 180.0 / PI;
    float relativeHeelAngle = currentHeelAngle - initialHeelAngle;

    // Print acceleration values
    Serial.println("Acceleration in g (x,y,z):");
    Serial.print(gValue.x);
    Serial.print("   ");
    Serial.print(gValue.y);
    Serial.print("   ");
    Serial.println(gValue.z);
    Serial.print("Resultant g: ");
    Serial.println(resultantG);

    // Print gyroscope data
    Serial.println("Gyroscope data in degrees/s: ");
    Serial.print(gyr.x);
    Serial.print("   ");
    Serial.print(gyr.y);
    Serial.print("   ");
    Serial.println(gyr.z);

    // Print pitch and roll angles
    Serial.println(myMPU6500.getPitch());
    Serial.println(myMPU6500.getRoll());

    // Print temperature
    Serial.print("Temperature in °C: ");
    Serial.println(temp);

    // Print heel angle
    Serial.print("Current Heel Angle: ");
    Serial.print(currentHeelAngle);
    Serial.println(" degrees");

    Serial.print("Relative Heel Angle: ");
    Serial.print(relativeHeelAngle);
    Serial.println(" degrees");

    // Print angles from IMU
    Serial.print("Angle x = ");
    Serial.print(angles.x);
    Serial.print("  |  Angle y = ");
    Serial.print(angles.y);
    Serial.print("  |  Angle z = ");
    Serial.println(angles.z);

    // Buzzer Logic: Beep if the heel angle (angles.y) exceeds ±30 degrees
    if (angles.y > 30 || angles.y < -30) {
        digitalWrite(BUZZER_PIN, HIGH); // Turn ON buzzer
        Serial.println("WARNING: Heel angle out of safe range! BUZZER ON!");
    } else {
        digitalWrite(BUZZER_PIN, LOW);  // Turn OFF buzzer
    }

    Serial.println("********************************************");

    delay(1000); // Delay 1 second before next measurement
}
