#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <pitches.h>
#include <NewPingESP8266.h>

// Wifi network credentials
const char* ssid = "DIGIFIBRA-eK9t";
const char* password = "qjaH49AUhvN9";

// MQTT broker configuration
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_topic_in = "devices/MUIoT-NAPIoTpaulz";
const char* mqtt_topic_out = "devices/MUIoT-NAPIoTpaulz-out";

// Pin where the buzzer is connected
const int buzzerPin = D8;
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6
}; // Array of notes
int duration = 500; // Duration of each note

// Ultrasonic sensor configuration
#define TRIGGER_PIN D1 // Ultrasonic sensor channel (trigger)
#define ECHO_PIN D2    // Ultrasonic sensor channel (echo)
#define MAX_DISTANCE 200 // Max distance in cm
NewPingESP8266 sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Initialize ultrasonic sensor

// Threshold distance to trigger buzzer
#define THRESHOLD_DISTANCE 5 // 5 cm: distance that will trigger buzzer

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

// Callback function that processes the received MQTT messages
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
    noTone(buzzerPin); // Turn off Buzzer
    Serial.println("Music OFF");
  } else if (message == "1") {
    tone(buzzerPin, 1000); // Turn on Buzzer
    Serial.println("Music ON");
  } else {
    Serial.println("Unknown command");
  }
}

// Function to reconnect to the MQTT broker if the connection is lost
void reconnect() {
  while (!client.connected()) {
    Serial.print("Trying to connect to MQTT broker...");
    if (client.connect("nodoMUIoT-NAPIoTpaulz")) {
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
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  delay(500);
  unsigned int distance = sonar.ping_cm();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance <= THRESHOLD_DISTANCE) {
    for (int thisNote = 0; thisNote < 8; thisNote++) {
      tone(buzzerPin, melody[thisNote], duration);
      delay(300);
    }
    noTone(buzzerPin);
  } else {
    noTone(buzzerPin);
  }

  char distanceStr[5];
  sprintf(distanceStr, "%u", distance);
  client.publish("devices/MUIoT-NAPIoTpaulz", distanceStr);
  Serial.print("Published distance: ");
  Serial.println(distanceStr);


}