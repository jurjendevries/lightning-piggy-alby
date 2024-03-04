String lastTime = "";

String getTimeFromNTP() {
  // Later on, this could validate the timezone even more,
  // and show a clear error if there's something wrong with it.
  if (!isConfigured(timezone)) return "";

  #ifdef DEBUG
  lastTime = "W 23:39";
  return "W 23:39";
  #endif
  String timeData = getEndpointData(timeServer, String(timeServerPath) + String(timezone), false);

  DynamicJsonDocument doc(8192); 

  DeserializationError error = deserializeJson(doc, timeData);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return "na";
  }

  Serial.println("Extracting weekday and time from received data");

  String datetimeAsString = doc["datetime"];

  int dayOfWeek = doc["day_of_week"];
  String dayOfWeekAsString = getDayOfWeekString(dayOfWeek);
  String time = datetimeAsString.substring(datetimeAsString.indexOf("T") + 1, datetimeAsString.indexOf("T") + 6); // Extract only the time (hh:mm)

  lastTime = dayOfWeekAsString + time;
  return lastTime;
}

String getLastTime() {
  return lastTime;
}

String getDayOfWeekString(int dayOfWeek) {
  if (strncmp(localeSetting,"dk",2) == 0) {
     switch(dayOfWeek) {
        case 0:
          return "S";
        case 1:
          return "M";
        case 2:
          return "T";
        case 3:
          return "O";
        case 4:
          return "T";
        case 5:
          return "F";
        case 6:
          return "L";
    }
  } else {
     switch(dayOfWeek) {
        case 0:
          return "S";
        case 1:
          return "M";
        case 2:
          return "T";
        case 3:
          return "W";
        case 4:
          return "T";
        case 5:
          return "F";
        case 6:
          return "S";
    }
  }
}
