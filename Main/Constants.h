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

#define MAX_PAYMENTS 6 // even the 2.66 inch display can only fit 6

extern const int MAX_WATCHDOG_REBOOTS = 3;
extern const int SLEEP_HOURS_AFTER_MAX_WATCHDOG_REBOOTS = 6;

#define AWAKE_SECONDS_AFTER_MANUAL_WAKEUP 3*60

#define CHECK_UPDATE_PERIOD_SECONDS 24*60*60

#define UPDATE_BALANCE_PERIOD_MILLIS 1000 * 60 * 10 // fallback to updating balance every 10 minutes if the instant websocket method is unavailable

#define UPDATE_VOLTAGE_PERIOD_MILLIS 1000 * 60 * 5 // update voltage display every 5 minutes

#define HIBERNATE_CHECK_PERIOD_MILLIS 1000 * 30 // hibernate check every 30 seconds

// In alphabetical order
const char * deWeekdays[] = { "S", "M", "D", "M", "D", "F", "S"};
const char * dkWeekdays[] = { "S", "M", "T", "O", "T", "F", "L"};
const char * esWeekdays[] = { "D", "L", "M", "M", "J", "V", "S"};
const char * enWeekdays[] = { "S", "M", "T", "W", "T", "F", "S"};
const char * nlWeekdays[] = { "Z", "M", "D", "W", "D", "V", "Z"};

String currentVersion = "2.2.3";

typedef enum {
    STR2INT_SUCCESS,
    STR2INT_OVERFLOW,
    STR2INT_UNDERFLOW,
    STR2INT_INCONVERTIBLE
} str2int_errno;

String websocketApiUrl = "/api/v1/ws/";

#endif
