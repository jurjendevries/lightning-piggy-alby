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
extern const int SLEEP_HOURS_AFTER_MAX_WATCHDOG_REBOOTS = 6;

#define AWAKE_SECONDS_AFTER_MANUAL_WAKEUP 4*60

#define CHECK_UPDATE_PERIOD_SECONDS 24*60*60

// In alphabetical order
const char * deWeekdays[] = { "S", "M", "D", "M", "D", "F", "S"};
const char * dkWeekdays[] = { "S", "M", "T", "O", "T", "F", "L"};
const char * esWeekdays[] = { "D", "L", "M", "M", "J", "V", "S"};
const char * enWeekdays[] = { "S", "M", "T", "W", "T", "F", "S"};
const char * nlWeekdays[] = { "Z", "M", "D", "W", "D", "V", "Z"};

String currentVersion = "2.1.2";

typedef enum {
    STR2INT_SUCCESS,
    STR2INT_OVERFLOW,
    STR2INT_UNDERFLOW,
    STR2INT_INCONVERTIBLE
} str2int_errno;

#endif
