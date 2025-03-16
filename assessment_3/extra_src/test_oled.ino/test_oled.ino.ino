#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128   // OLED width in pixels
#define SCREEN_HEIGHT 64   // OLED height in pixels
#define OLED_RESET    -1   // Reset pin (not used)
#define OLED_ADDR 0x3C     // Change to 0x3D if necessary

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Initialize the OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("OLED initialization failed!");
        while (1); // Halt execution if OLED fails
    }

    Serial.println("OLED initialized!");

    // Clear the display
    display.clearDisplay();

    // Display "Hello, World!"
    display.setTextSize(2);           // Set text size
    display.setTextColor(SSD1306_WHITE); // Set text color
    display.setCursor(10, 20);        // Set text position
    display.println("Hello!");
    display.display();                // Show text on screen
}

void loop() {
    // Nothing to do here, text stays on screen
}
