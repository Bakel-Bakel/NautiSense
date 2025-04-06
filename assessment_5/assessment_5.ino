#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <MPU6500_WE.h>
#include <U8g2lib.h>

#define MPU6500_ADDR 0x68
#define BUZZER_PIN 9

// Wifi network credentials
const char* ssid = "DIGIFIBRA-eK9t";
const char* password = "qjaH49AUhvN9";

// MQTT broker configuration
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic_in = "devices/MUIoT-NAPIoT";
const char* mqtt_topic_out = "devices/MUIoT-NAPIoT-out";

// Initialize the IMU sensor and OLED
MPU6500_WE myMPU6500(MPU6500_ADDR);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Initial heel angle for calibration
float initialHeelAngle = 0.0;

WiFiClient espClient;
PubSubClient client(espClient);

// Function to connect to WiFi
void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    Serial.println("IP: ");
    Serial.println(WiFi.localIP());
}

// Callback function for handling received MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received [");
    Serial.print(topic);
    Serial.print("]: ");
  
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println("Message: " + message);

    if (message == "0") {
        digitalWrite(BUZZER_PIN, LOW); // Turn off buzzer
        Serial.println("Buzzer OFF");
    } else if (message == "1") {
        digitalWrite(BUZZER_PIN, HIGH); // Turn on buzzer
        Serial.println("Buzzer ON");
    } else {
        Serial.println("Unknown command");
    }
}

// Reconnect to MQTT broker if connection is lost
void reconnect() {
    while (!client.connected()) {
        Serial.print("Trying to connect to MQTT broker...");
        if (client.connect("nodoMUIoT-NAPIoT")) {
            Serial.println("Connected!");
            client.subscribe(mqtt_topic_out);
            Serial.println("Subscribed to the topic");
        } else {
            Serial.print("Connection error: ");
            Serial.print(client.state());
            Serial.println(", trying again in 5 seconds");
            delay(5000);
        }
    }
}

void setup() {
    pinMode(BUZZER_PIN, OUTPUT);
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(400000); // Increase I2C speed to reduce delays
  
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
  
    // Initialize the MPU6500 sensor
    if (!myMPU6500.init()) {
        Serial.println(F("MPU6500 error!"));
    } else {
        Serial.println(F("MPU6500 OK"));
    }
  
    u8g2.begin();  // OLED initialization
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
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    xyzFloat gValue = myMPU6500.getGValues();
    float currentHeelAngle = atan2(gValue.y, gValue.z) * 180.0 / PI;
    float relativeHeelAngle = currentHeelAngle - initialHeelAngle;

    // Update OLED display
    u8g2.clearBuffer();
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

    // Publish the current heel angle to MQTT broker
    char angleStr[8];
    sprintf(angleStr, "%.2f", currentHeelAngle);
    client.publish("devices/MUIoT-NAPIoT", angleStr);
    Serial.print("Published angle: ");
    Serial.println(angleStr);

    delay(500);
}

