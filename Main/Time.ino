String lastTime = "";

String getTimeFromNTP() {
  // Later on, this could validate the timezone even more,
  // and show a clear error if there's something wrong with it.
  if (!isConfigured(timezone)) return "";

  #ifdef DEBUG
  lastTime = "Wed 12:34";
  return "Wed 12:34";
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

  lastTime = dayOfWeekAsString + " " + time;
  return lastTime;
}

String getLastTime() {
  return lastTime;
}

String getDayOfWeekString(int dayOfWeek) {
  if (strncmp(localeSetting,"dk",2) == 0) {
     switch(dayOfWeek) {
        case 0:
          return "Søn";
        case 1:
          return "Man";
        case 2:
          return "Tir";
        case 3:
          return "Ons";
        case 4:
          return "Tor";
        case 5:
          return "Fre";
        case 6:
          return "Lør";
    }
  } else {
     switch(dayOfWeek) {
        case 0:
          return "Sun";
        case 1:
          return "Mon";
        case 2:
          return "Tue";
        case 3:
          return "Wed";
        case 4:
          return "Thu";
        case 5:
          return "Fri";
        case 6:
          return "Sat";
    }
  }
}
