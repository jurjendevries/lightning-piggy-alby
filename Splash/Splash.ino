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
#include "driver/rtc_io.h"  // for rtc_gpio_pullup_dis and rtc_gpio_pulldown_en
#include <rom/rtc.h>        // for rtc_get_reset_reason

#define BUTTON_PIN_BITMASK 0x100000000 // 2^32 means GPIO32

RTC_DATA_ATTR int wakeup_count;
bool tiltTriggered = false; // Flag to indicate if the tilt switch has been triggered

void print_reset_reason(int reason) {
  switch (reason) {
    case 1 : Serial.println("POWERON_RESET"); break;
    case 3 : Serial.println("SW_RESET"); break;
    case 4 : Serial.println("OWDT_RESET"); break;
    case 5 : Serial.println("DEEPSLEEP_RESET"); break;
    case 6 : Serial.println("SDIO_RESET"); break;
    case 7 : Serial.println("TG0WDT_SYS_RESET"); break;
    case 8 : Serial.println("TG1WDT_SYS_RESET"); break;
    case 9 : Serial.println("RTCWDT_SYS_RESET"); break;
    case 10 : Serial.println("INTRUSION_RESET"); break;
    case 11 : Serial.println("TGWDT_CPU_RESET"); break;
    case 12 : Serial.println("SW_CPU_RESET"); break;
    case 13 : Serial.println("RTCWDT_CPU_RESET"); break;
    case 14 : Serial.println("EXT_CPU_RESET"); break;
    case 15 : Serial.println("RTCWDT_BROWN_OUT_RESET"); break;
    case 16 : Serial.println("RTCWDT_RTC_RESET"); break;
    default : Serial.println("NO_MEAN");
  }
}

String print_wakeup_reason() {
  Serial.println("Wakeup from sleep count: " + String(wakeup_count));
  wakeup_count++;

  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using EXT0 RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using EXT1 RTC_CNTL"); print_GPIO_wake_up(); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default: Serial.printf("Wakeup was not caused by deep sleep, wakeup reason: %d\n", wakeup_reason); break;
  }

  return "This is the " + String(wakeup_count) + "th wakeup in a row, for reason (%d): " + String(wakeup_reason);
}

void print_GPIO_wake_up() {
  uint64_t GPIO_reason = esp_sleep_get_ext1_wakeup_status();
  Serial.print("GPIO that triggered the wake up: GPIO ");
  Serial.println((log(GPIO_reason)) / log(2), 0);
}

void refresh_display() {
  Serial.println("Refreshing display...");
  setup_display();
  showLogo(epd_bitmap_Lightning_Piggy, 104, 250, displayHeight() - 104, (displayWidth() - 250) / 2); // Width and height are swapped because display rotation
  displaySetup();
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Ensure serial connection is established

  // Print reset reasons
  Serial.println("CPU0 reset reason:");
  print_reset_reason(rtc_get_reset_reason(0));

  Serial.println("CPU1 reset reason:");
  print_reset_reason(rtc_get_reset_reason(1));

  // Print wakeup reason
  print_wakeup_reason();

  // Refresh display on wakeup
  refresh_display();

  Serial.println("Starting Lightning Piggy Splash ...");

  // Turn on the green LED-IO12 on the LilyGo 2.66 inch, to show the board is on
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH); // Turn the LED on (HIGH is the voltage level)

  // Enable GPIO32 (vibration sensor pin) wakeup
  pinMode(32, INPUT); // Ensure GPIO32 is set as input
  rtc_gpio_pullup_dis(GPIO_NUM_32);  // Disable pullup
  rtc_gpio_pulldown_en(GPIO_NUM_32); // Enable pulldown
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);

  // Put the device into deep sleep mode
  Serial.println("Entering deep sleep mode ...");
  delay(2000);  // Add a delay before entering deep sleep to see the output
  esp_deep_sleep_start();
}

void loop() {
  /*
  // Check if the tilt switch has been triggered
  if (digitalRead(32) == HIGH) {
    if (!tiltTriggered) {
      Serial.println("Tilt switch triggered!");
      refresh_display();
      tiltTriggered = true;
    }
  } else {
    tiltTriggered = false; // Reset the flag if the switch is not triggered
  }

  delay(100); // Small delay to debounce the switch and avoid rapid triggering
  */
}
