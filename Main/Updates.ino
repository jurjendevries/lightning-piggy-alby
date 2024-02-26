String currentVersion = "1.9.5";
String newVersion = ""; // used by the update checker

int lastChecked = NOT_SPECIFIED;

#ifdef LILYGO_T5_V266
  String hardwareBoard  = "LILYGOT5V266";
#elif defined LILYGO_T5_V213
  String hardwareBoard = "LILYGOT5V213";
#else
  String hardwareBoard = "UNKNOWNBOARD";
#endif

#ifdef _GxDEPG0266BN_H_
  String hardwareDisplay = "DEPG0266BN";
#elif defined _GxDEPG0213BN_H_
  String hardwareDisplay = "DEPG0213BN";
#elif defined _GxGDEM0213B74_H_
  String hardwareDisplay = "GDEM0213B74";
#else
  String hardwareDisplay = "UNKNOWNDISPLAY";
#endif

bool isUpdateAvailable() {
  return (newVersion != "" && newVersion != currentVersion);
}

void checkShowUpdateAvailable() {
  if (lastChecked == NOT_SPECIFIED || (millis()-lastChecked > CHECK_UPDATE_PERIOD_SECONDS*1000)) {
    newVersion = checkNewVersion();
    Serial.println("checkNewVersion returned: " + newVersion);
    if (newVersion != "") {
      lastChecked = millis();
      if (isUpdateAvailable()) {
        Serial.println("Update available!");
      } else {
        Serial.println("No update available.");
      }
    } else {
      Serial.println("checkNewVersion() returned '' so could not check for updates");
    }
  } else {
    Serial.println("Already checked for updates recently, not doing it again.");
  }
}

String getShortDisplayInfo() {
  String shortDisplayInfo = "";
  if (hardwareBoard == "LILYGOT5V266") {
    shortDisplayInfo = "2.66";
  } else if (hardwareBoard == "LILYGOT5V213") {
    shortDisplayInfo = "2.13";
  } else {
    shortDisplayInfo = "?.??";
  }
  if (hardwareDisplay == "DEPG0266BN" || hardwareDisplay == "DEPG0213BN") {
    shortDisplayInfo += "D";
  } else if (hardwareDisplay == "GDEM0213B74") {
    shortDisplayInfo += "G";
  } else {
    shortDisplayInfo += "?";
  }
  return shortDisplayInfo;
}

String getShortVersion() {
  return currentVersion;
}

String getFullVersion() {
  const char compiletime[] = __DATE__ " " __TIME__;
  String compileTime(compiletime);
  return currentVersion + "|" + hardwareBoard + "|" + hardwareDisplay + "|" + compileTime;
}

/*
 * Check if there's a new version available for the current version and hardware combination.
 *
 * The server decides whether or not the new version should be installed, which enables more advanced
 * things like incremental rollouts, or holding back an update if there are issues in the field.
 *
 * returns: the new version that should be installed
 */
String checkNewVersion() {
  Serial.print("Checking for updates: ");
  return getEndpointData(checkUpdateHost, "/", false);
}
