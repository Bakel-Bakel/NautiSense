#include <MPU6500_WE.h> // MPU6500 IMU library
#include <Wire.h>       // I2C communication library
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define MPU6500_ADDR 0x68   // I2C address of MPU6500
#define OLED_ADDR 0x3C      // Common I2C address for OLED (check with scanner)
#define BUZZER_PIN 9        // Buzzer is connected to pin 9
#define SCREEN_WIDTH 128    // OLED display width
#define SCREEN_HEIGHT 64    // OLED display height

MPU6500_WE myMPU6500 = MPU6500_WE(MPU6500_ADDR); // Create IMU object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // OLED object

float initialHeelAngle = 0.0;  // Initial heel angle reference

void setup() {
    Serial.begin(115200);
    Wire.begin();
    pinMode(BUZZER_PIN, OUTPUT);

    // Initialize IMU
    if (!myMPU6500.init()) {
        Serial.println("MPU6500 does not respond");
    } else {
        Serial.println("MPU6500 is connected");
    }

    Serial.println("Position your MPU6500 flat for calibration...");
    delay(1000);
    myMPU6500.autoOffsets();
    Serial.println("Calibration complete!");

    myMPU6500.enableGyrDLPF();
    myMPU6500.setGyrDLPF(MPU6500_DLPF_6);
    myMPU6500.setSampleRateDivider(5);
    myMPU6500.setGyrRange(MPU6500_GYRO_RANGE_250);
    myMPU6500.setAccRange(MPU6500_ACC_RANGE_2G);
    myMPU6500.enableAccDLPF(true);
    myMPU6500.setAccDLPF(MPU6500_DLPF_6);

    xyzFloat gValue = myMPU6500.getGValues();
    initialHeelAngle = atan2(gValue.y, gValue.z) * 180.0 / PI;

    Serial.print("Initial Heel Angle: ");
    Serial.println(initialHeelAngle);
delay(100);
    // Initialize OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("OLED initialization failed!");
        while (1);
    }
    Serial.println("OLED initialized!");

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);
    display.println("OLED Ready");
    display.display();

    delay(200);
}

void loop() {
    xyzFloat gValue = myMPU6500.getGValues();
    xyzFloat gyr = myMPU6500.getGyrValues();
    float temp = myMPU6500.getTemperature();
    float resultantG = myMPU6500.getResultantG(gValue);
    xyzFloat angles = myMPU6500.getAngles();  // Read pitch, roll, yaw

    float currentHeelAngle = atan2(gValue.y, gValue.z) * 180.0 / PI;
    float relativeHeelAngle = currentHeelAngle - initialHeelAngle;

    Serial.println("Acceleration in g (x,y,z):");
    Serial.print(gValue.x); Serial.print("   ");
    Serial.print(gValue.y); Serial.print("   ");
    Serial.println(gValue.z);
    Serial.print("Resultant g: ");
    Serial.println(resultantG);
    Serial.println("Gyroscope data in degrees/s: ");
    Serial.print(gyr.x); Serial.print("   ");
    Serial.print(gyr.y); Serial.print("   ");
    Serial.println(gyr.z);
    Serial.println(myMPU6500.getPitch());
    Serial.println(myMPU6500.getRoll());
    Serial.print("Temperature in °C: ");
    Serial.println(temp);
    Serial.print("Current Heel Angle: ");
    Serial.print(currentHeelAngle);
    Serial.println(" degrees");

    Serial.print("Relative Heel Angle: ");
    Serial.print(relativeHeelAngle);
    Serial.println(" degrees");

    Serial.print("Angle x = ");
    Serial.print(angles.x);
    Serial.print("  |  Angle y = ");
    Serial.print(angles.y);
    Serial.print("  |  Angle z = ");
    Serial.println(angles.z);

    if (angles.y > 30 || angles.y < -30) {
        digitalWrite(BUZZER_PIN, HIGH);
        Serial.println("WARNING: Heel angle out of safe range! BUZZER ON!");
    } else {
        digitalWrite(BUZZER_PIN, LOW);
    }

    Serial.println("********************************************");

    // Display Data on OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print("Heel Angle: ");
    display.print(currentHeelAngle);
    display.println(" deg");
    display.print("Pitch: ");
    display.println(myMPU6500.getPitch());
    display.print("Roll: ");
    display.println(myMPU6500.getRoll());
    display.print("Temp: ");
    display.print(temp);
    display.println(" C");
    display.display();

    delay(1000);
}
