// The display is layed out as follows:
// <wallet balance> sat(s)                            QRCODE
// -------------------------------------------------- QRCODE
// <amount> sat(s): comment1                          QRCODE
// <amount> sat(s): comment2 comment2 comment2
// comment2
// <amount> sat(s): comment3
// <fiatbalance> <currency> (<fiatprice> <currency)
//

#define balanceHeight roundEight(DISPLAY_HEIGHT/5)  // includes line underneath
#define fiatHeight    roundEight((DISPLAY_HEIGHT*4)/5)

// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <U8g2_for_Adafruit_GFX.h>

// select the display class and display driver class in the following file (new style):
#include "GxEPD2_display_selection_new_style.h"

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

long lastRefreshedVoltage = -UPDATE_VOLTAGE_PERIOD_MILLIS;  // this makes it update when first run
long lastUpdatedBalance = -UPDATE_BALANCE_PERIOD_MILLIS;  // this makes it update when first run
int lastBalance = -NOT_SPECIFIED;

int smallestFontHeight = 14;

int statusAreaVoltageHeight = -1; // this value is cached after it's calculated so it can be reused later to updated only the voltage

int blackBackgroundVerticalMargin=2;
int blackBackgroundHorizontalMargin=2;

String lines[10];
int nroflines = 0;

void setup_display() {
    display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
    display.setRotation(1); // display is used in landscape mode
    display.setPartialWindow(0, 0, display.width(), display.height());

    u8g2Fonts.begin(display); // connect u8g2 procedures to Adafruit GFX
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
}

int displayHeight() {
  return DISPLAY_HEIGHT;
}

int displayWidth() {
  return DISPLAY_WIDTH;
}

// size 0 = smallest font (8pt)
// size 1 = 10pt
// size 2 = 12pt
// size 3 = 14pt
// size 4 = 18pt
// size 5 = 24pt
void setFont(int fontSize) {
  if (fontSize < 0) {
    Serial.println("ERROR: font size " + String(fontSize) + " is not supported, setting min size");
    u8g2Fonts.setFont(u8g2_font_helvR08_tf);
  } else if (fontSize == 0) {
    u8g2Fonts.setFont(u8g2_font_helvR08_tf);
  } else if (fontSize == 1) {
    u8g2Fonts.setFont(u8g2_font_helvR10_tf);
  } else if (fontSize == 2) {
    u8g2Fonts.setFont(u8g2_font_helvR12_tf);
  } else if (fontSize == 3) {
    u8g2Fonts.setFont(u8g2_font_helvR14_tf);
  } else if (fontSize == 4) {
    u8g2Fonts.setFont(u8g2_font_helvR18_tf);
  } else if (fontSize == 5) {
    u8g2Fonts.setFont(u8g2_font_helvR24_tf);
  } else {
    Serial.println("ERROR: font size " + String(fontSize) + " is not supported, setting max size");
    u8g2Fonts.setFont(u8g2_font_helvR24_tf);
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
  //display.getTextBounds("$", 0, 0, &x1, &y1, &w, &h);
  w = u8g2Fonts.getUTF8Width("$");
  h = u8g2Fonts.getFontAscent()-u8g2Fonts.getFontDescent();
  Serial.println("Got big character bounds: " + String(x1) + "," + String(y1) + ","+ String(w) + "," + String(h) + " for text: $");
  uint16_t maxHeight = h * 1.5; // ensure it's really big, but smaller than 2 lines
  //Serial.println("maxHeight = " + String(maxHeight));
  h = 0;

  while (maxLength < text.length() && h < maxHeight && w < maxWidth) {
    String textToFit = text.substring(0, maxLength+2); // end is exclusive
    w = u8g2Fonts.getUTF8Width(textToFit.c_str());
    h = u8g2Fonts.getFontAscent()-u8g2Fonts.getFontDescent();
    Serial.println("Got text bounds: " + String(x1) + "," + String(y1) + ","+ String(w) + "," + String(h) + " for text: " + textToFit);
    maxLength++;
  }

  //Serial.println("Max text length that fits: " + String(maxLength) + " calculated in " + String(millis()-startTime) + "ms.");
  return maxLength;
}

// xPos,yPos is the top left of the line (in case no alignRight) or top right of the line (in case of alignRight)
// returns line height
int drawLine(String line, int xPos, int yPos, bool invert, bool alignRight) {
  int w = u8g2Fonts.getUTF8Width(line.c_str());
  int h = u8g2Fonts.getFontAscent()-u8g2Fonts.getFontDescent();
  Serial.println("Drawing text " + String(line) + " at (" + String(xPos) + "," + String(yPos) + ") with size "+ String(w) + "x"+ String(h));
  if (!alignRight) {
    Serial.println("u8g2Fonts.setCursor(" + String(xPos) + "," + String(yPos + h) + ")");
    u8g2Fonts.setCursor(xPos, yPos + h); // bottom of the line
  } else {
    Serial.println("u8g2Fonts.setCursor(" + String(xPos-w) + "," + String(yPos + h) + ")");
    u8g2Fonts.setCursor(xPos-w, yPos + h); // bottom of the line
  }
  if (!invert) {
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
  } else {
    Serial.println("Filling rectangle for inverted text from (" + String(xPos) + "," + String(yPos) + ") of size " + String(w) + "x" + String(h));
    display.fillRect(xPos-blackBackgroundHorizontalMargin, yPos, w+blackBackgroundHorizontalMargin*2, h+blackBackgroundVerticalMargin*2, GxEPD_BLACK);
    u8g2Fonts.setForegroundColor(GxEPD_WHITE);
    u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
  }
  Serial.println("Displaying line: " + line);
  u8g2Fonts.println(line);
  return h;
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
  bool debugDisplayFit = true;

  feed_watchdog(); // before this long-running and potentially hanging operation, it's a good time to feed the watchdog

  Serial.println("displayFit " + text + " of length: " + String(text.length()) + " from (" + String(startXbig) + "," + String(startYbig) + ") to (" + String(endXbig) + "," + String(endYbig) + ") with max fontSize " + String(fontSize));

  if (text.length() == 0) {
    Serial.println("Aborting displayFit due to zero length text.");
    return startYbig;
  }

  int16_t x1, y1;
  uint16_t w, h;
  int startX = startXbig;
  int startY = startYbig;
  int endX = endXbig;
  int endY = endYbig;
  int invertOffsetXbefore = blackBackgroundHorizontalMargin;
  int invertOffsetYbefore = 0;
  int invertOffsetXafter = blackBackgroundHorizontalMargin;
  int invertOffsetYafter = blackBackgroundVerticalMargin;
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

  int yPos;

  Serial.println("Setting partial window: (" + String(startXbig) + "," + String(startYbig) + " with size " + String(endXbig-startXbig+1) + "x" + String(endYbig-startYbig+1));
  display.setPartialWindow(startXbig, startYbig, endXbig-startXbig+1, endYbig-startYbig+1);
  while (fontSize > 0) {
    nroflines = 0;
    setFont(fontSize);

    yPos = startY;
    int textPos = 0;
    while (textPos < text.length()) {
      // Try to fit everything that still needs displaying:
      String textWithoutAlreadyPrintedPart = text.substring(textPos);
      int chars = fitMaxText(textWithoutAlreadyPrintedPart, endX);

      String textLine = text.substring(textPos, textPos+chars);
      if (debugDisplayFit) Serial.println("first line that fits: " + textLine);
      lines[nroflines] = textLine;
      nroflines++;

      h = u8g2Fonts.getFontAscent()-u8g2Fonts.getFontDescent();

      textPos += chars;
      yPos += h;
    }
    if (debugDisplayFit) Serial.println("After simulating the text, yPos = " + String(yPos) + " while endY = " + String(endY));

    // Check if the entire text fit:
    if (yPos <= endY) {
      if (debugDisplayFit) Serial.println("yPos (" + String(yPos) + ") <= endY (" + String(endY) + ") so fontSize " + String(fontSize) + " fits!");
      break; // exit the fontSize loop because it fits
    } else {
      if (debugDisplayFit) Serial.println("fontSize " + String(fontSize) + " did not fit so trying smaller...");
      fontSize--;
    }
  }

  // finally print the array
  display.firstPage();
  do {
    yPos = startY;
    for (int linenr=0;linenr<nroflines;linenr++) {
      if (!alignRight) {
        yPos += drawLine(lines[linenr],startX,yPos,invert,alignRight);
      } else {
        yPos += drawLine(lines[linenr],endX,yPos,invert,alignRight);
      }
    }
  } while (display.nextPage());
  if (debugDisplayFit) Serial.println("After writing the text, yPos = " + String(yPos) + " while endY = " + String(endY));

  feed_watchdog(); // after this long-running and potentially hanging operation, it's a good time to feed the watchdog
  if (debugDisplayFit) Serial.println("displayFit returning yPos = " + String(yPos) + " after runtime of " + String(millis() - startTime) + "ms."); // takes around 1700ms
  return yPos;
}

void showLogo(const unsigned char logo [], int sizeX, int sizeY, int posX, int posY) {
  display.drawImage(logo, posX, posY, sizeX, sizeY, false);
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

  // Display balance from 0 to balanceHeight
  display.setPartialWindow(0, 0, xBeforeLNURLp, balanceHeight);
  display.firstPage();
  do {
    u8g2Fonts.setCursor(0, balanceHeight-3); // bottom of the line
    setFont(4);
    u8g2Fonts.print(String(currentBalance) + " sats");
    display.fillRect(0, balanceHeight-1, xBeforeLNURLp-5, 1, GxEPD_BLACK);
  } while (display.nextPage());

  // Display payment amounts and comments
  int maxYforLNURLPayments = displayHeight();
  if (isConfigured(btcPriceCurrencyChar)) maxYforLNURLPayments = fiatHeight; // leave room for fiat values at the bottom (fontsize 2 = 18 + 2 extra for the black background)
  if (fetchPayments) fetchLNURLPayments(MAX_PAYMENTS);
  displayLNURLPayments(MAX_PAYMENTS, xBeforeLNURLp - 5, balanceHeight+1, maxYforLNURLPayments);

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
  int yPos = startY+4;
  for (int i=0;i<min(getNroflnurlPayments(),limit) && yPos+smallestFontHeight < maxY;i++) {
    Serial.println("Displaying payment: " + getLnurlPayment(i));
    yPos = displayFit(getLnurlPayment(i), 0, yPos, maxX, maxY, 3);
    yPos = roundEight(yPos);
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
  setFont(0);
  int lineHeight = u8g2Fonts.getFontAscent()-u8g2Fonts.getFontDescent(); // there's no fontDescent in these status lines 
  int qrPixels = displayWidth() - xBeforeLNURLp; // square
  Serial.println("qrPixels = " + String(qrPixels));

  display.setPartialWindow(xBeforeLNURLp, qrPixels, displayWidth()-xBeforeLNURLp, displayHeight()-qrPixels);
  display.firstPage();
  do {
    int startY = qrPixels;

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
    startY += drawLine(wifiString, displayWidth(), startY, false, true);
  
    String versionString = "v";
    if (isUpdateAvailable()) versionString = "UP!";
    versionString += getShortVersion();
    startY += drawLine(versionString, displayWidth(), startY, false, true);
  
    String displayString = getShortDisplayInfo();
    startY += drawLine(displayString, displayWidth(), startY, false, true);

    double voltage = getLastVoltage();
    if (showsleep) voltage = getBatteryVoltage(); // only refresh voltage before going to sleep
    String voltageString = "NOBAT";
    if (voltage > 0) voltageString = String(voltage, 2) + "V";
    startY += drawLine(voltageString, displayWidth(), startY, false, true);

    // Time is only shown before sleep
    if (showsleep) {
      String currentTime = getTimeFromNTP();
      drawLine(currentTime, displayWidth(), startY, false, true);    // 6 characters, width of 8
    }
  } while (display.nextPage());
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
    delay(5000);
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

  displayFit(toDisplay, 0, fiatHeight, maxX, displayHeight(), 2, true);
}


void showBootSlogan() {
  if (strncmp(showSloganAtBoot,"YES", 3) != 0) {
    Serial.println("Not showing slogan at boot because showSloganAtBoot is not 'YES'.");
    return;
  }

  int displayY = 2;
  int timeToWait = 0;

  if (isConfigured(bootSloganPrelude)) {
    displayY = displayFit(String(bootSloganPrelude), 0, 0, displayWidth(), balanceHeight, 3);
    timeToWait = 1000; // since the prelude is always the same, there's no need to wait a long time to allow reading it
  }

  String slogan = getRandomBootSlogan();
  Serial.println("Showing boot slogan: " + slogan);
  displayFit(slogan, 0, balanceHeight+8, displayWidth(), displayHeight(), 4); // leave multiple of 8px margin for font descent

  // Assuming a 7 year old averages one 4-letter word per second, that's 5 characters per second.
  timeToWait += strlen(slogan.c_str()) * 1000 / 5;
  // Limit to a maximum
  timeToWait = min(timeToWait, MAX_BOOTSLOGAN_SECONDS*1000);
  Serial.println("Waiting " + String(timeToWait) + "ms (of max. " + String(MAX_BOOTSLOGAN_SECONDS) + "s) to allow the bootslogan to be read...");
  delay(timeToWait);
}
