// DEBUG causes no wifi connection and only dummy data to be used,
// for faster startup and testing of display code etc.
//
//#define DEBUG
#include "Constants.h"

// These values get replaced with the user provided values by the Web Serial Installer for Lightning Piggy.
// But you can also replace them manually yourself here if you don't plan on using the Web Installer.

const char* ssid     = "REPLACETHISBYWIFISSID_REPLACETHISBYWIFISSID_REPLACETHISBYWIFISSID"; // wifi SSID here
const char* password = "REPLACETHISBYWIFIKEY_REPLACETHISBYWIFIKEY_REPLACETHISBYWIFIKEY"; // wifi password here
const char* lnbitsHost = "REPLACETHISBYLNBITSHOST_REPLACETHISBYLNBITSHOST_REPLACETHISBYLNBITSHOST"; // HOST NAME HERE E.G. legend.lnbits.com
const char* invoiceKey = "REPLACETHISBYLNBITSKEY_REPLACETHISBYLNBITSKEY_REPLACETHISBYLNBITSKEY"; // lnbits wallet invoice hey here

// Regular configuration values
const char* checkUpdateHost = "m.lightningpiggy.com";

const int sleepTimeMinutes = 60;

// If the fiat currency (btcPriceCurrencyChar) is not configured, then no fiat values are shown
const char* btcPriceCurrencyChar = "REPLACETHISBYFIATCURRENCY_REPLACETHISBYFIATCURRENCY_REPLACETHISBYFIATCURRENCY";

const char* timezone = "Europe/Copenhagen";

const int settingLanguage = LANGUAGE_EN_US;

const char* thousandsSeparator = "REPLACETHISBYTHOUSANDSSEPARATOR_REPLACETHISBYTHOUSANDSSEPARATOR_REPLACETHISBYTHOUSANDSSEPARATOR";
const char* defaultThousandsSeparator = ",";

const char* decimalSeparator = "REPLACETHISBYDECIMALSEPARATOR_REPLACETHISBYDECIMALSEPARATOR_REPLACETHISBYDECIMALSEPARATOR";
const char* defaultDecimalSeparator = ".";

// A bit of text to show before the boot slogan:
const char* bootSloganPrelude = "REPLACETHISBYBOOTSLOGANPRELUDE_REPLACETHISBYBOOTSLOGANPRELUDE_REPLACETHISBYBOOTSLOGANPRELUDE";

// Whether or not to show the boot slogan. Set to "YES" if you want it:
const char* showSloganAtBoot = "REPLACETHISBYSHOWBOOTSLOGAN_REPLACETHISBYSHOWBOOTSLOGAN_REPLACETHISBYSHOWBOOTSLOGAN";
