#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Replace with your Wi-Fi credentials
const char* ssid = "IAmLordCruise";
const char* password = "IAmLordCruise";

ESP8266WebServer server(80);  // Create a web server on port 80

void handleRoot() {
    server.send(200, "text/plain", "ESP8266 is connected to the internet!"); 
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\nConnecting to Wi-Fi...");
    WiFi.begin(ssid, password);

    // Wait until connected
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());  // Print the local IP

    // Start Web Server
    server.on("/", handleRoot);  // Define response for root URL
    server.begin();
    Serial.println("Web server started!");
}

void loop() {
    server.handleClient();  // Handle web requests
}
