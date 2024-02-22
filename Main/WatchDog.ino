#include <esp_task_wdt.h>
#include "soc/rtc_wdt.h"

// When to restart (s)
#define WDT_TIMEOUT 20
#define WDT_TIMEOUT_LONG 80

// "noinit" DRAM is not initialized during software restarts so it's the perfect place to keep this counter
// without the need to resort to limited-write-cycle flash storage such as EEPROM, NVS, flash,...
// See https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/memory-types.html
__NOINIT_ATTR int watchdog_reboot_count;

int count_watchdog_kicks = 0;
int lastFeed = millis();

// Count number of times the watchdog triggers a reboot
void updateWatchdogRebootCount() {
  if (rtc_get_reset_reason(0) == 9) { // if reboot reason was watchdog reset (RTCWDT_SYS_RESET)
    watchdog_reboot_count++;
    Serial.println("This is the " + String(watchdog_reboot_count) + "th reboot caused by the watchdog.");
  } else {
    // if it was not a watchdog reset, then reset the watchdog_reboot_count to 0
    watchdog_reboot_count = 0;
  }
}

void setup_watchdog() {
  Serial.println("Configuring WDT Watch Dog Timer...");
  //esp_task_wdt_add(NULL); // only needed for task watchdog timer
  set_watchdog_time(WDT_TIMEOUT_LONG);

  updateWatchdogRebootCount();
}

void set_watchdog_time(int seconds) {
  Serial.println("Setting Watch Dog Timer to " + String(seconds) + " seconds");

  // For Task watchdog:
  // esp_task_wdt_init(seconds*1000, true);

  // for RTC watchdog:
  rtc_wdt_protect_off();      //Disable RTC WDT write protection
  rtc_wdt_set_stage(RTC_WDT_STAGE0, RTC_WDT_STAGE_ACTION_RESET_SYSTEM);
  rtc_wdt_set_time(RTC_WDT_STAGE0, seconds*1000);
  rtc_wdt_enable();           //Start the RTC WDT timer
  rtc_wdt_protect_on();       //Enable RTC WDT write protection
}

void short_watchdog_timeout() {
  set_watchdog_time(WDT_TIMEOUT);
}

void long_watchdog_timeout() {
  set_watchdog_time(WDT_TIMEOUT_LONG);
}

// Meant to be called from the loop function
void feed_watchdog() {
    Serial.println("Feeding watchdog...");
    rtc_wdt_feed(); // for rtc watchdog
    //esp_task_wdt_reset(); // for task watchdog
    lastFeed = millis();
    count_watchdog_kicks++;
}
