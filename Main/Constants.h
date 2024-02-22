#ifndef CONSTANTS_H
#define CONSTANTS_H

extern const int NOT_SPECIFIED = -1; 

extern const int CURRENCY_USD = 1;
extern const int CURRENCY_DKK = 2;

extern const int LANGUAGE_EN_US = 1;
extern const int LANGUAGE_DA = 2;

const char * NOTCONFIGURED = "REPLACETHISBY";
const unsigned int NOTCONFIGURED_LENGTH = 13;

// Maximum time to show the bootslogan
extern const int MAX_BOOTSLOGAN_SECONDS = 15;

extern const int MAX_WATCHDOG_REBOOTS = 3;
extern const int SLEEP_HOURS_AFTER_MAX_WATCHDOG_REBOOTS = 18;

#endif
