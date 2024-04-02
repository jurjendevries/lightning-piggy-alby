long lastRefreshedVoltage = -UPDATE_VOLTAGE_PERIOD_MILLIS;  // this makes it update when first run
long lastUpdatedBalance = -UPDATE_BALANCE_PERIOD_MILLIS;  // this makes it update when first run
int lastBalance = -NOT_SPECIFIED;

int smallestFontHeight = 14;

int statusAreaVoltageHeight = -1; // this value is cached after it's calculated so it can be reused later to updated only the voltage

void setup_display() {
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.init();

    // partial update to full screen to preset for partial update of box window (this avoids strange background effects)
    // this needs to be done before setRotation, otherwise still faint/missing pixels, even with using_rotation = true
    display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);

    display.setTextColor(GxEPD_BLACK);
    display.setRotation(1); // display is used in landscape mode
    display.update(); // clear the screen from any old data that might faintly be there
}

int displayHeight() {
  // lilygo 2.66 is 152px, lilygo 2.13 is 122px (DEPG?) or 128px (GDEM?)
  //return 128; // for testing the 2.13's lower resolution on the 2.66's high res display
  return GxEPD_WIDTH; // width and height are swapped because display is rotated
}

int displayWidth() {
  // lilygo 2.66 is 296px, lilygo 2.13 is 250px
  //return 250; // for testing the 2.13's lower resolution on the 2.66's high res display
  return GxEPD_HEIGHT; // width and height are swapped because display is rotated
}

void updateWindow(int x, int y, int w, int h) {
  display.updateWindow(x, y, w, h, true);
}

// size 0 = smallest font (8pt)
// size 1 = 12pt
// size 2 = 18pt
// size 3 = 20pt
// size 4 = 26pt
void setFont(int fontSize) {
  if (fontSize < 0) {
    Serial.println("ERROR: font size " + String(fontSize) + " is not supported, setting min size");
    display.setFont(&Lato_Medium_8);
  } else if (fontSize == 0) {
    display.setFont(&Lato_Medium_8);
  } else if (fontSize == 1) {
    display.setFont(&Lato_Medium_12);
  } else if (fontSize == 2) {
    display.setFont(&Lato_Medium_18);
  } else if (fontSize == 3) {
    display.setFont(&Lato_Medium_20);
  } else if (fontSize == 4) {
    display.setFont(&Lato_Medium_26);
  } else {
    Serial.println("ERROR: font size " + String(fontSize) + " is not supported, setting max size");
    display.setFont(&Lato_Medium_26);
  }
}

void displayBorder() {
  horizontalLine();
  verticalLine();
}

void horizontalLine() {
    // Line for showing end of display
    for (int16_t x = 0; x<displayWidth()+1; x++) {
      display.drawPixel(x,displayHeight()+1,0);
    }
}
void verticalLine() {
    // Line for showing end of display
    for (int16_t y = 0; y<displayHeight()+1; y++) {
      display.drawPixel(displayWidth()+1,y,0);
    }
}


// find the max length that fits the width
int fitMaxText(String text, int maxWidth) {
  //long startTime = millis();
  int maxLength = 0;
  int16_t x1, y1;
  uint16_t w, h;

  // first get height of one big character
  display.getTextBounds("$", 0, 0, &x1, &y1, &w, &h);
  //Serial.println("Got big character bounds: " + String(x1) + "," + String(y1) + ","+ String(w) + "," + String(h) + " for text: $");
  uint16_t maxHeight = h * 1.5; // ensure it's really big, but smaller than 2 lines
  //Serial.println("maxHeight = " + String(maxHeight));
  h = 0;

  while (maxLength < text.length() && h < maxHeight && w < maxWidth) {
    String textToFit = text.substring(0, maxLength+2); // end is exclusive
    display.getTextBounds(textToFit, 0, 0, &x1, &y1, &w, &h);
    //Serial.println("Got text bounds: " + String(x1) + "," + String(y1) + ","+ String(w) + "," + String(h) + " for text: " + textToFit);
    maxLength++;
  }

  //Serial.println("Max text length that fits: " + String(maxLength) + " calculated in " + String(millis()-startTime) + "ms.");
  return maxLength;
}

int displayFit(String text, int startX, int startY, int endX, int endY, int fontSize) {
    return displayFit(text, startX, startY, endX, endY, fontSize, false, false);
}

int displayFit(String text, int startX, int startY, int endX, int endY, int fontSize, bool invert) {
    return displayFit(text, startX, startY, endX, endY, fontSize, invert, false);
}

// Try to fit a String into a rectangle, including the borders.
// bool bold == true means black background, white text
// returns: the y position after fitting the text
int displayFit(String text, int startXbig, int startYbig, int endXbig, int endYbig, int fontSize, bool invert, bool alignRight) {
  long startTime = millis();
  bool debugDisplayFit = false;

  feed_watchdog(); // before this long-running and potentially hanging operation, it's a good time to feed the watchdog

  text.replace("~","-");
  Serial.println("displayFit " + text + " length: " + String(text.length()));

  if (text.length() == 0) {
    Serial.println("Aborting displayFit due to zero length text.");
    return startYbig;
  }

  int startX = startXbig;
  int startY = startYbig;
  int endX = endXbig;
  int endY = endYbig;
  int invertOffsetXbefore = 2;
  int invertOffsetYbefore = 0;
  int invertOffsetXafter = 2;
  int invertOffsetYafter = 2;
  if (invert) {
    // black rectangle is slightly bigger than the text; from (-2,-1) inclusive until (+2,+2) inclusive
    startX = startXbig + invertOffsetXbefore;
    startY = startYbig + invertOffsetYbefore;
    endX = endXbig - invertOffsetXafter;
    endY = endYbig - invertOffsetYafter;
  }

  // Don't go past the end of the display and remember pixels start from zero, so [0,max-1]
  endX = min(displayWidth()-1,endX);
  endY = min(displayHeight()-1,endY);

  int spaceBetweenLines = 1;
  int yPos;

  while (fontSize > 0) {
    setFont(fontSize);

    display.fillRect(startXbig, startYbig, endXbig-startXbig, endYbig-startYbig, GxEPD_WHITE);

    yPos = startY;
    int textPos = 0;
    while (textPos < text.length()) {
      // Try to fit everything that still needs displaying:
      String textWithoutAlreadyPrintedPart = text.substring(textPos);
      int chars = fitMaxText(textWithoutAlreadyPrintedPart, endX);

      // Print the text that fits:
      String textLine = text.substring(textPos, textPos+chars);
      if (debugDisplayFit) Serial.println("first line that fits: " + textLine);

      int16_t x1, y1;
      uint16_t w, h;
      display.getTextBounds(textLine, 0, 0, &x1, &y1, &w, &h);
      if (debugDisplayFit) Serial.println("getTextBounds of textLine: " + String(x1) + "," + String(y1) + ","+ String(w) + ","+ String(h));
      if (!alignRight) {
        display.setCursor(startX, yPos + h); // bottom of the line
      } else {
        display.setCursor(endX-w, yPos + h); // bottom of the line
      }
      if (!invert) {
        display.setTextColor(GxEPD_BLACK);
      } else {
        display.fillRect(startX-invertOffsetXbefore, yPos-invertOffsetYbefore,w+invertOffsetXbefore+invertOffsetXafter, h+invertOffsetYbefore+invertOffsetYafter, GxEPD_BLACK);
        display.setTextColor(GxEPD_WHITE);
      }
      display.print(textLine);

      textPos += chars;
      yPos += h + spaceBetweenLines;
    }
    yPos -= spaceBetweenLines; // remove the last space between lines
    if (debugDisplayFit) Serial.println("After writing the text, yPos = " + String(yPos) + " while endY = " + String(endY));

    // Check if the entire text fit:
    if (yPos <= endY) {
      if (debugDisplayFit) Serial.println("yPos (" + String(yPos) + ") <= endY (" + String(endY) + ") so fontSize " + String(fontSize) + " fits!");
      break; // exit the fontSize loop because it fits
    } else {
      if (debugDisplayFit) Serial.println("fontSize " + String(fontSize) + " did not fit so trying smaller...");
      fontSize--;
    }
  }

  updateWindow(startX, startY, endX-startX+1, endY-startY+1); // takes around 1400ms
  feed_watchdog(); // after this long-running and potentially hanging operation, it's a good time to feed the watchdog
  if (debugDisplayFit) Serial.println("displayFit returning yPos = " + String(yPos) + " after runtime of " + String(millis() - startTime) + "ms."); // takes around 1700ms
  return yPos;
}

void displayTime(bool useLast) {
  String currentTime = getLastTime();
  if (!useLast) {
    currentTime = getTimeFromNTP();
  }
  displayFit(currentTime, displayWidth()-6*8, displayHeight()-14, displayWidth(), displayHeight(), 1);    // 6 characters, width of 8
}

void showLogo(const unsigned char logo [], int sizeX, int sizeY, int posX, int posY) {
  display.drawBitmap(logo, posX, posY, sizeX, sizeY, GxEPD_WHITE);
  updateWindow(posX, posY, sizeX, sizeY);
}

// returns whether it updated the display
bool displayBalanceAndPaymentsPeriodically(int xBeforeLNURLp) {
  long nowUpdatedBalanceMillis = millis();
  // if there is a lastBalance and it was recently refreshed, then don't update balance
  if (lastBalance != -NOT_SPECIFIED && (nowUpdatedBalanceMillis - lastUpdatedBalance) < UPDATE_BALANCE_PERIOD_MILLIS) {
    return false;
  } else {
    lastUpdatedBalance = nowUpdatedBalanceMillis; // even if the below operations fail, this still counts as an update, because we don't want to retry immediately if something fails
  }

  int currentBalance = getWalletBalance();
  if (currentBalance == NOT_SPECIFIED) {
    displayFit("Get wallet error", 0, 0, xBeforeLNURLp, 22, 2, true);
    return false;
  } else if (currentBalance == lastBalance) {
    return false; // no change (unless someone deposited and withdrew the same amount) so no need to fetch payments and fiat values
  }

  updateBalanceAndPayments(xBeforeLNURLp, currentBalance, true);
  return true;
}

// fetchPayments forcing option is there to populate the walletID for the websocket (in case it's not configured)
void updateBalanceAndPayments(int xBeforeLNURLp, int currentBalance, bool fetchPayments) {
  lastBalance = currentBalance;

  // Display balance
  // height on 122px display should be 20px so (height - 2) / 6
  // width on 250px display should be 192px so width * 3 / 4
  int yAfterBalance = displayFit(String(currentBalance) + " sats",0,0,xBeforeLNURLp,(displayHeight()/7)+1,5);

  // Display payment amounts and comments
  int maxYforLNURLPayments = displayHeight()-1;
  if (isConfigured(btcPriceCurrencyChar)) maxYforLNURLPayments -= 20; // leave room for fiat values at the bottom (fontsize 2 = 18 + 2 extra for the black background)
  if (fetchPayments) fetchLNURLPayments(MAX_PAYMENTS);
  displayLNURLPayments(MAX_PAYMENTS, xBeforeLNURLp - 10, yAfterBalance, maxYforLNURLPayments);

  // Display fiat values
  showFiatValues(currentBalance, xBeforeLNURLp);
}

/**
 * @brief Get recent LNURL Payments
 *
 * @param limit
 */
void displayLNURLPayments(int limit, int maxX, int startY, int maxY) {
  int smallestFontHeight = 8;
  // Draw a line under the total sats amount
  // Draws at 0,22 with size 179,1 on 250x122px display
  display.fillRect(0, startY+3, maxX-3, 1, GxEPD_BLACK);
  updateWindow(0, startY+3, maxX-3, 1);
  startY+=4;
  uint16_t yPos = startY;
  for (int i=0;i<min(getNroflnurlPayments(),limit) && yPos+smallestFontHeight < maxY;i++) {
    Serial.println("Displaying payment: " + getLnurlPayment(i));
    yPos = displayFit(getLnurlPayment(i), 0, yPos, maxX, maxY, 3);
    yPos += 2; // leave some margin between the comments
  }
}

void displayWifiConnecting() {
  displayFit("Wifi: " + String(ssid), 0, displayHeight()-smallestFontHeight, displayWidth(), displayHeight(), 1);
}

void displayWifiStrengthBottom() {
  displayWifiStrength(displayHeight()-14);
}

void displayWifiStrength(int y) {
  int wifiStrengthPercent = strengthPercent(getStrength(5));
  String wifiString = "Wifi:" + String(wifiStrengthPercent) + "%";
  displayFit(wifiString, displayWidth()-8*7, y, displayWidth(), y+smallestFontHeight, 1, false, true);
}

void displayFetching() {
  displayFit("Fetching " + String(lnbitsHost), 0, displayHeight()-smallestFontHeight, displayWidth()-8*7, displayHeight(), 1); // leave room for 8 characters of wifi strength bottom right
}

// returns the y value after showing all the status info
void displayStatus(int xBeforeLNURLp, bool showsleep) {
  int vMargin = 2;
  int startY = displayWidth() - xBeforeLNURLp + vMargin; // x == y because the QR code is square

  // wifi strength or zzzz
  String wifiString = "..zzzZZZZ";
  if (!showsleep) {
    wifiString = "Wifi:";
    if (wifiConnected()) {
      int wifiStrengthPercent = strengthPercent(getStrength(5));
      wifiString += String(wifiStrengthPercent) + "%";
    } else {
      wifiString += "off";
    }
  }
  Serial.println("Displaying wifi string: " + wifiString);
  startY = vMargin + displayFit(wifiString, xBeforeLNURLp, startY, displayWidth(), startY+smallestFontHeight-vMargin, 1, false, true);

  String versionString = "v";
  if (isUpdateAvailable()) versionString = "UP!";
  versionString += getShortVersion();
  startY = vMargin + displayFit(versionString, xBeforeLNURLp, startY, displayWidth(), startY+smallestFontHeight-vMargin, 1, false, true);

  String displayString = getShortDisplayInfo();
  startY = vMargin + displayFit(displayString, xBeforeLNURLp, startY, displayWidth(), startY+smallestFontHeight-vMargin, 1, false, true);

  statusAreaVoltageHeight = startY;
  displayRefreshedVoltagePeriodically();
}

// Uses the cached displayRefreshedVoltage
// This can be called without doing the entire refresh, which is too slow for reliable websocket handling.
void displayRefreshedVoltagePeriodically() {
  long nowRefreshedVoltageMillis = millis();
  // if there is a lastBalance and it was recently refreshed, then don't update balance
  if ((nowRefreshedVoltageMillis - lastRefreshedVoltage) < UPDATE_VOLTAGE_PERIOD_MILLIS) {
    return;
  } else {
    lastRefreshedVoltage = nowRefreshedVoltageMillis; // even if the below operations fail, this still counts as an update, because we don't want to retry immediately if something fails
  }

  double voltage = getBatteryVoltage();
  String voltageString = "NOBAT";
  if (voltage > 0) {
    voltageString = String(voltage, 2) + "V";
  }
  displayFit(voltageString, xBeforeLNURLp, statusAreaVoltageHeight, displayWidth(), statusAreaVoltageHeight+smallestFontHeight, 1, false, true);
}

// returns true if voltage is low, false otherwise
bool displayVoltageWarning() {
    double voltage = getBatteryVoltage();
    // Print big fat warning on top of everything if low battery
    if (voltage > 0 && voltage < 3.8) {
      String lowBatString = " ! LOW BATTERY (" + String(voltage) + "V) ! ";
      displayFit(lowBatString, 0, displayHeight()-12-18, displayWidth(), displayHeight()-12,2, true);
      return true;
    } else {
      return false;
    }
}


// This shows something like:
// 123.23$ (51234.1$)
// 123.23KR (512021)
// 123.23E (51234.1E)
void showFiatValues(int balance, int maxX) {
  if (!isConfigured(btcPriceCurrencyChar)) {
    Serial.println("Not showing fiat values because no fiat currency is configured.");
    return;
  }

  #ifdef DEBUG
  float btcPrice = 60456;
  #else
  float btcPrice = getBitcoinPrice();
  #endif

  if (btcPrice == NOT_SPECIFIED) {
    Serial.println("Not showing fiat values because couldn't find Bitcoin price.");
    return;
  }

  String toDisplay = "";

  // Try to add the fiat balance
  if (balance == NOT_SPECIFIED) {
    Serial.println("Not showing fiat balance because couldn't find Bitcoin balance.");
  } else {
    float balanceValue = btcPrice / 100000000 * balance;
    toDisplay += floatToString(balanceValue, 2) + getCurrentCurrencyCode() + " ";
    Serial.println("balanceValue: " + toDisplay + " ");
  }

  // Add the Bitcoin price
  String currentBtcPriceToShow = formatFloatWithSeparator(btcPrice);
  // Only add currency code if the price is not too long, to save screen space
  if (currentBtcPriceToShow.length() <= 6) currentBtcPriceToShow += getCurrentCurrencyCode();
  toDisplay += "(" + currentBtcPriceToShow + ")";

  //displayBoldMessage(toDisplay, displayHeight() - 4); // bold text adds 1 pixel before + 2 pixels after + 1 pixel because displayHeight() starts counting at 0
  displayFit(toDisplay, 0, displayHeight() - 20, maxX, displayHeight(), 2, true);
}


void showBootSlogan() {
  if (strncmp(showSloganAtBoot,"YES", 3) != 0) {
    Serial.println("Not showing slogan at boot because showSloganAtBoot is not 'YES'.");
    return;
  }

  int displayY = 2;
  int timeToWait = 0;

  if (isConfigured(bootSloganPrelude)) {
    displayY = displayFit(String(bootSloganPrelude), 0, displayY, displayWidth(), displayHeight()/5, 3);
    timeToWait = 1000; // since the prelude is always the same, there's no need to wait a long time to allow reading it
  }

  String slogan = getRandomBootSlogan();
  Serial.println("Showing boot slogan: " + slogan);
  displayFit(slogan, 0, displayY+5, displayWidth(), displayHeight(), 4);

  // Assuming a 7 year old averages one 4-letter word per second, that's 5 characters per second.
  timeToWait += strlen(slogan.c_str()) * 1000 / 5;
  // Limit to a maximum
  timeToWait = min(timeToWait, MAX_BOOTSLOGAN_SECONDS*1000);
  Serial.println("Waiting " + String(timeToWait) + "ms (of max. " + String(MAX_BOOTSLOGAN_SECONDS) + "s) to allow the bootslogan to be read...");
  delay(timeToWait);
}
