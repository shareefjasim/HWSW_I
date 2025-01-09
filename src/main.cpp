#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display width and height (typical 128x64)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C address for most SSD1306 modules is 0x3C.
#define OLED_I2C_ADDRESS 0x3C

// Create display object (software reset is optional)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{
  // Initialize Serial for debugging
  Serial.begin(115200);
  delay(1000);

  // Initialize I2C communication
  Wire.begin(21, 22); // SDA = GPIO21, SCL = GPIO22 on ESP32

  // Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS))
  {
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Clear the display buffer
  display.clearDisplay();

  // Draw a line at y=16, from x=0 to x=127, in white color
  display.drawLine(0, 26, 027, 26, WHITE);

  // Set text properties
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Hello World!");

  // Commit (show) the changes on the display
  display.display();

  Serial.println("OLED initialized. Displaying message...");
}

void loop()
{
  // You can print something to serial periodically
  Serial.println("Hello World, Loop is running...");
  delay(2000);
}
