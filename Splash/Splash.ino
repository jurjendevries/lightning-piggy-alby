// Code for the Lightning Piggy Splash screen running on the TTGO LilyGo 2.13 and 2.66 inch ePaper hardware.
//
// Tested with:
// - Arduino IDE version 1.8.13
// - ESP32 Board Support version 2.0.14
// - Preferences -> Compiler warnings: Default
// - Tools -> Board -> ESP32 Arduino -> ESP32 Dev Module
// - Tools -> Upload Speed: 921600
// - Tools -> CPU Frequency: 240Mhz
// - Tools -> Flash Frequency: 80Mhz
// - Tools -> Flash Mode: QIO
// - Tools -> Flash Size: 4MB (32Mb)
// - Tools -> Partition Scheme: Default 4MB with spiffs (1.2MB APP, 1.5MB SPIFFS)
// - Tools -> Core Debug Level: Warn
// - Tools -> PSRAM: Disabled
// - Tools -> Port: /dev/ttyACM0
//
// See README.md for more tips and tricks.

#include "logos.h"
#include "config.h"
#include "Constants.h"

void setup() {
    Serial.begin(115200);
    Serial.println("Starting Lightning Piggy Splash ...");

    // turn on the green LED-IO12 on the LilyGo 2.66 inch, to show the board is on
    // it will turn off when the board hibernates
    pinMode(12, OUTPUT);
    digitalWrite(12, HIGH); // turn the LED on (HIGH is the voltage level)

    Serial.println("Setup the display ...");
    setup_display();

    Serial.println("Show the logo ...");
    showLogo(epd_bitmap_Lightning_Piggy, 104, 250, displayHeight() - 104, (displayWidth() - 250) / 2); // width and height are swapped because display rotation

    Serial.println("Show next steps to setup ...");
    displaySetup();

    // Put the device into deep sleep mode
    Serial.println("Entering deep sleep mode ...");
    esp_deep_sleep_start();
}

void loop() {
}
