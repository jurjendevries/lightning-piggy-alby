
// These values are remembered to know what to erase upon the next draw
int xPosOfHealthAndStatus = NOT_SPECIFIED;
int yPosOfHealthAndStatus = NOT_SPECIFIED;

// Returns the VBAT value.
// If we're not on battery but USB powered, then it returns a negative VBAT value.
double getBatteryVoltage() {
    const int nrReads = 4;

    int totalDiff = 0;

    int prevValue = analogRead(35); delay(10);
    int totalLevel = prevValue;

    for (int multiread=0; multiread<nrReads; multiread++) {
      int value = analogRead(35);
      totalLevel += value;
      totalDiff += abs(value - prevValue);
      delay(100);
    }

    totalLevel = totalLevel / (nrReads+1); // one additional read is done for the prevValue
    totalDiff = totalDiff / nrReads;
    Serial.println("Average battery level: " + String(totalLevel));
    Serial.println("Average battery level diff: " + String(totalDiff));

    double voltage = (totalLevel * 1.72) / 1000;
    if (totalDiff > 32) {
      // The battery has a smoothing effect so lots of variance between levels means we're not on battery.
      voltage *= -1;
    }
    //return 3.7; // for testing low battery situation
    return voltage;
}

// This function also displays the LOW BATTERY warning:
void displayHealthAndStatus(bool showsleep) {
    setFont(1);
    int16_t x1, y1;
    uint16_t w, h;
    int verticalSpace = 2; // space between each item
    int yStart = displayHeight() - 16; // leave room for 1 row of small text, the "last updated"
    int yPos = yStart;
    int xOffset = 1;
    int minX = displayWidth(); // track min X to know which area of display to update

    if (xPosOfHealthAndStatus != NOT_SPECIFIED || yPosOfHealthAndStatus != NOT_SPECIFIED) {
      Serial.println("Clearing health and status by drawing white rectangle: " + String(xPosOfHealthAndStatus) + "," + String(yPosOfHealthAndStatus) + " to " + String(displayWidth()-1) + "," + String(yStart));
      display.fillRect(xPosOfHealthAndStatus, yPosOfHealthAndStatus, displayWidth()-1, yStart, GxEPD_WHITE);
    } else{
      Serial.println("Not clearing health and status because it has never been drawn before.");
    }

    /* Temperature sensor is missing and workaround shows too high or needs calibration...
    String tempString = String(readTemp1(false), 1); // one digit after comma
    tempString += "C";
    const char *tempChar = tempString.c_str();
    display.getTextBounds((char*)tempChar, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(displayWidth()-w-xOffset,yPos);
    display.print((char*)tempChar);
    yPos = yPos - h - 1;
    */

    double voltage = getBatteryVoltage();
    String voltageString = "NOBAT";
    if (voltage > 0) {
      voltageString = String(voltage, 2) + "V";
    }
    const char *voltageChar = voltageString.c_str();
    display.getTextBounds((char*)voltageChar, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(displayWidth()-w-xOffset,yPos);
    minX = min(displayWidth()-w-xOffset,minX);
    display.print((char*)voltageChar);
    yPos = yPos - h - verticalSpace;


    String displayString = getShortDisplayInfo();
    const char *displayChar = displayString.c_str();
    display.getTextBounds((char*)displayChar, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(displayWidth()-w-xOffset,yPos);
    minX = min(displayWidth()-w-xOffset,minX);
    display.print((char*)displayChar);
    yPos = yPos - h - verticalSpace;

    String versionString = "v" + getShortVersion();
    const char *versionChar = versionString.c_str();
    display.getTextBounds((char*)versionChar, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(displayWidth()-w-xOffset,yPos);
    minX = min(displayWidth()-w-xOffset,minX);
    display.print((char*)versionChar);
    yPos = yPos - h - verticalSpace;

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
    const char *wifiChar = wifiString.c_str();
    display.getTextBounds((char*)wifiChar, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(displayWidth()-w-xOffset,yPos);
    minX = min(displayWidth()-w-xOffset,minX);
    display.print((char*)wifiChar);
    yPos = yPos - h - verticalSpace;

    Serial.println("health and status updateWindow minX,yPos = " + String(minX) + "," + String(yPos) + " with width " + String(displayWidth()-minX) + " and height " + String(yStart-yPos)); // minX,yPos = 192,67
    updateWindow(minX, yPos, displayWidth()-minX, yStart-yPos);
    xPosOfHealthAndStatus = minX;
    yPosOfHealthAndStatus = yPos;
}

// returns true if voltage is low, false otherwise
bool displayVoltageWarning() {
    double voltage = getBatteryVoltage();
    // Print big fat warning on top of everything if low battery
    if (voltage > 0 && voltage < 3.8) {
      String lowBatString = " ! LOW BATTERY (" + String(voltage) + "V) ! ";
      displayBoldMessage(lowBatString, displayHeight()-12);
      return true;
    } else {
      return false;
    }
}
