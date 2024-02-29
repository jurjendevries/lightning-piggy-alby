// Code for the Lightning Piggy running on the TTGO LilyGo 2.13 and 2.66 inch ePaper hardware.
// See README.md for tips and tricks.

// For 2.13 inch LilyGo ePaper:
#define LILYGO_T5_V213

// For 2.66 inch LilyGo ePaper (end of life), uncomment the LILYGO_T5_V266 line below and comment out the LILYGO_T5_V213 line above.
//#define LILYGO_T5_V266

#include <boards.h>
#include <string.h>

#include "logos.h"
#include "config.h"

// Display stuff:
#include <GxEPD.h>
#ifdef LILYGO_T5_V266
  #include <GxDEPG0266BN/GxDEPG0266BN.h>    // 2.66" black/white ePaper display by DKE GROUP
#elif defined LILYGO_T5_V213
  #include <GxDEPG0213BN/GxDEPG0213BN.h>    // 2.13" black/white ePaper display by DKE GROUP
//#include <GxGDEM0213B74/GxGDEM0213B74.h>  // 2.13" black/white ePaper display by GoodDisplay with 2 additional gray levels
#endif
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

// Fonts after display:
#include "Fonts/LatoMedium8pt.h"
#include "Fonts/LatoMedium12pt.h"
#include "Fonts/LatoMedium18pt.h"
#include "Fonts/LatoMedium20pt.h"
#include "Fonts/LatoMedium26pt.h"

#include "Constants.h"

// Global variables for display
GxIO_Class io(SPI,  EPD_CS, EPD_DC,  EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);

int xBeforeLNURLp; // global variable gets updated in setup() function after drawing the LNURLp QR code

void setup() {
    Serial.begin(115200);
    Serial.println("Starting Lightning Piggy " + getFullVersion());

    // turn on the green LED-IO12 on the PCB, to show the board is on
    // it will turn off when the board hibernates
    pinMode(12, OUTPUT);
    digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)

    print_reset_reasons();
    print_wakeup_reason();

    setup_display();

    // allow some time to show low battery warning
    if (displayVoltageWarning()) {
       delay(5000);
    }

    showBootSlogan();

    // erase the screen 
    display.fillScreen(GxEPD_WHITE);
    updateWindow(0, 0, displayWidth(), displayHeight());

    // logo indicates board is starting
    showLogo(epd_bitmap_Lightning_Piggy, 250, 100, (displayWidth() - 250) / 2, 0);

    setup_watchdog();

    displayFit("Wifi: " + String(ssid), 0, displayHeight()-15, displayWidth(), displayHeight(), 1);
    #ifndef DEBUG
    connectWifi();
    short_watchdog_timeout(); // after the long wifi connection stage, the next operations shouldn't take long
    #endif

    feed_watchdog(); // Feed the watchdog regularly, otherwise it will "bark" (= reboot the device)
    displayFit("Fetching " + String(lnbitsHost), 0, displayHeight()-15, displayWidth(), displayHeight(), 1);
    String lnurlp = getLNURLp();

    display.fillScreen(GxEPD_WHITE);  // erase the setup screen
    updateWindow(0, 0, displayWidth(), displayHeight());
    if (lnurlp == "null") {
      xBeforeLNURLp = displayWidth();
      Serial.println("Warning, could not find lnurlp link for this wallet, did you create one?");
      Serial.println("You can do so by activating the LNURLp extension in LNBits, clicking on the extension, and clicking 'NEW PAY LINK'");
      Serial.println("You probably don't want to go for 'fixed amount', but rather for any amount.");
    } else {
        xBeforeLNURLp = showLNURLpQR(lnurlp); // xBeforeLNURLp = 192 on 250px wide display
    }

    // showLogo(epd_bitmap_Bitcoin, 40, 40, displayWidth()-1 - ((displayWidth()-xBeforeLNURLp+40)/2), (displayWidth()-xBeforeLNURLp)+1);
}

void loop() {
    displayHealthAndStatus(false);

    feed_watchdog(); // Feed the watchdog regularly, otherwise it will "bark" (= reboot the device)
    int balance = getWalletBalance();

    // build the new screen:
    int yAfterBalance  = 0;

    if (balance != NOT_SPECIFIED) {
      String walletBalanceText = String(balance) + " sats";
      // height on 122px display should be 20px so (height - 2) / 6
      // width on 250px display should be 192px so width * 3 / 4
      yAfterBalance = displayFit(walletBalanceText,0,0,xBeforeLNURLp,(displayHeight()/7)+1,5);
    }
    else {
       displayBoldMessage("Get wallet error", 22);
    }

    feed_watchdog(); // Feed the watchdog regularly, otherwise it will "bark" (= reboot the device)
    int maxYforLNURLPayments = displayHeight()-1;
    if (isConfigured(btcPriceCurrencyChar)) maxYforLNURLPayments -= 20; // leave room for fiat values at the bottom (fontsize 2 = 18 + 2 extra for the black background)
    showLNURLPayments(2, xBeforeLNURLp - 10, yAfterBalance, maxYforLNURLPayments);

    feed_watchdog(); // Feed the watchdog regularly, otherwise it will "bark" (= reboot the device)
    showFiatValues(balance);

    displayTime(false);

    displayVoltageWarning();

    feed_watchdog(); // Feed the watchdog regularly, otherwise it will "bark" (= reboot the device)
    if (wifiConnected()) checkShowUpdateAvailable();

    watchdogWasntTriggered();
    hibernateDependingOnBattery();
}
