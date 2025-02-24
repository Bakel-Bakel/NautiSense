void setup() {
    Serial.begin(9600);  // Start serial communication
}

void loop() {
    Serial.println("Hello from Arduino!");  // Send test message
    delay(1000);  // Send every 1 second
}
