#include <esp_task_wdt.h>

// When to restart (s)
#define WDT_TIMEOUT 20
#define WDT_TIMEOUT_LONG 80

// "Data in RTC memory is initialised whenever the SoC restarts, except when waking from deep sleep.
// When waking from deep sleep, the values which were present before going to sleep are kept."
// if the device wakes up from sleep, wakeup_count is preserved and incremented.
// if the device wakes up from something else, like a watchdog trigger, the wakeup_count is reset
RTC_DATA_ATTR int wakeup_count = 0;

int count_watchdog_kicks = 0;
int lastFeed = millis();

void setup_watchdog() {
  Serial.println("Configuring WDT Watch Dog Timer...");
  esp_task_wdt_init(WDT_TIMEOUT_LONG, true); // enable panic so ESP32 restarts after this many seconds
  esp_task_wdt_add(NULL); //add current thread to WDT watch

  //Increment wakeup number and print it
  wakeup_count++;
  Serial.println("Wakeup count: " + String(wakeup_count));
}

void short_watchdog_timeout() {
  Serial.println("Restoring WDT Watch Dog Timer to WDT_TIMEOUT... ");
  esp_task_wdt_init(WDT_TIMEOUT, true); // enable panic so ESP32 restarts after this many seconds
}

void long_watchdog_timeout() {
  Serial.println("Configuring WDT Watch Dog Timer to WDT_TIMEOUT_LONG, should be restored after the upload...");
  esp_task_wdt_init(WDT_TIMEOUT_LONG, true); // enable panic so ESP32 restarts after this many seconds
}

// Meant to be called from the loop function
void feed_watchdog() {
    Serial.println("Feeding watchdog...");
    esp_task_wdt_reset();
    lastFeed = millis();
    count_watchdog_kicks++;
}
