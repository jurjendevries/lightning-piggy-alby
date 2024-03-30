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

#include <ArduinoJson.h>
#include <WebSocketsClient.h> // Needs to be here, otherwise compilation error...

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

    #ifndef DEBUG
    connectWifi();
    short_watchdog_timeout(); // after the long wifi connection stage, the next operations shouldn't take long
    displayWifiStrengthBottom();
    #endif
    displayFetching();

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

    displayStatus(xBeforeLNURLp, false);
    displayBalanceAndPaymentsPeriodically(xBeforeLNURLp);

    if (wifiConnected()) checkShowUpdateAvailable();

    watchdogWasntTriggered();
    hibernateDependingOnBattery();

    // try to fetch walletID from lnurlp/list if we don't have it yet:
    if (!isConfigured(walletID) && getWalletIDfromLNURLp().length() == 0) getLNURLp(true);

    // connect the websocket if we have a walletID
    if (isConfigured(walletID) || getWalletIDfromLNURLp().length() > 0) connectWebsocket();
}

void loop() {
  feed_watchdog(); // Feed the watchdog regularly, otherwise it will "bark" (= reboot the device)
  /* This receives random data while using a USB cable without the USB-to-serial in between:
  while (Serial.available() > 0) {
    char aChar = Serial.read();
    if (aChar == 's') {
      hibernate(10);
    } else if (aChar == 'w') {
      displayWifiStrengthBottom();
    } else if (aChar == 'd') {
      displayStatus(xBeforeLNURLp, false);
    }
  }*/
  if (isConfigured(walletID) || getWalletIDfromLNURLp().length() > 0) {
    websocket_loop();
    displayRefreshedVoltagePeriodically(); // only refresh voltage so the user can see it going down with time. the rest can stay.
  } else {
    displayStatus(xBeforeLNURLp, false);  // takes ~2000ms, which is too much to do with the websocket
    displayBalanceAndPaymentsPeriodically(xBeforeLNURLp);
    // if some time has passed, then check balance and if it changed, then update payments
  }
  if (!hibernateDependingOnBattery()) delay(200);
}
