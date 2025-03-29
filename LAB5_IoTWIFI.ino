#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "DIGIFIBRA-eK9t";
const char* password = "qjaH49AUhvN9";

// MQTT broker configuration
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;

// MQTT topics
const char* mqtt_topic_temp = "devices/NAPIoT-P2/temp";
const char* mqtt_topic_hum = "devices/NAPIoT-P2/hum";
const char* mqtt_topic_manual = "devices/NAPIoT-P2/manual-led"; 

// DHT11 sensor setup
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Blink LED pin
const int ledPin = D1;  

WiFiClient espClient;
PubSubClient client(espClient);

// Thresholds
const float tempThreshold = 27.0;
const float humThreshold = 60.0;

// Connect to WiFi
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

  Serial.println("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

// MQTT reconnect logic
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP-DHT11-Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read DHT11 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check for valid readings
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print and send sensor values
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" °C | Hum: ");
  Serial.print(humidity);
  Serial.println(" %");

  char tempStr[8];
  dtostrf(temperature, 4, 1, tempStr);
  client.publish(mqtt_topic_temp, tempStr);

  char humStr[8];
  dtostrf(humidity, 4, 1, humStr);
  client.publish(mqtt_topic_hum, humStr);

  // LED control based on thresholds
  if (temperature > tempThreshold || humidity > humThreshold) {
    digitalWrite(ledPin, HIGH);  // Turn LED ON
    Serial.println("⚠️  THRESHOLD EXCEEDED → LED ON");
  } else {
    digitalWrite(ledPin, LOW);   // Turn LED OFF
    Serial.println("✅  Normal values → LED OFF");
  }

  delay(5000);  // Send every 5 seconds
}
