// Returns true if the value is configured, otherwise false.
bool isConfigured(const char * configName) {
  if (strncmp(configName, NOTCONFIGURED, NOTCONFIGURED_LENGTH) == 0) {
    return false;
  } else {
    return true;
  }
}

String formatFloatWithSeparator(float number) 
{
  if (number < 1000) {
    return String(number, 0);
  }

  int numberAsInt = (int)number; 
  float thousands = number / 1000.0f;
  int remainder = numberAsInt % 1000;

  return String(thousands, 0) + getCurrentCurrencyThousandsSeparator() + String(remainder);
}

String getCurrentCurrencyCode() {
  if (strncmp(btcPriceCurrencyChar, "USD", 3) == 0) {
      return "$";
  } else if (strncmp(btcPriceCurrencyChar, "DKK", 3) == 0) {
      return "kr";
  } else if (strncmp(btcPriceCurrencyChar, "EUR", 3) == 0) {
      return "E";
  } else {
    return "NA";
  }
}

char getCurrentCurrencyDecimalSeparator() {
   if (isConfigured(decimalSeparator)) {
    return decimalSeparator[0];
  } else {
    return defaultDecimalSeparator[0];
  }
}

char getCurrentCurrencyThousandsSeparator() {
  if (isConfigured(thousandsSeparator)) {
    return thousandsSeparator[0];
  } else {
    return defaultThousandsSeparator[0];
  }
}

// Replace dot with CurrencyDecimalSeparator
String floatToString(float number, int decimals) {
  char buffer[15]; 

  // Minimum width of 0 means it takes whatever width is required
  dtostrf(number, 0, decimals, buffer);

  for (int i = 0; i < strlen(buffer); i++) {
    if (buffer[i] == '.') {
      buffer[i] = getCurrentCurrencyDecimalSeparator();
      break; 
    }
  }

  return String(buffer); 
}

String ipToString(IPAddress ip) { // IP v4 only
  String ips;
  ips.reserve(16);
  ips = ip[0];  ips += '.';
  ips += ip[1]; ips += '.';
  ips += ip[2]; ips += '.';
  ips += ip[3];
  return ips;
}
