
// Returns true if the value is configured, otherwise false.
bool isConfigured(const char * configName) {
  if ((strncmp(configName, NOTCONFIGURED, NOTCONFIGURED_LENGTH) == 0) || (strlen(configName) == 0)) {
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

// Should the currency symbol be prepended (= placed before the amount) or not?
bool prependCurrencySymbol() {
  if ((strncmp(btcPriceCurrencyChar,"USD",3) == 0) || (strncmp(btcPriceCurrencyChar,"EUR",3) == 0) || (strncmp(btcPriceCurrencyChar,"GBP",3) == 0)) {
      return true;
  }
  return false;
}

String getCurrentCurrencyCode() {
  if (strncmp(btcPriceCurrencyChar, "USD", 3) == 0) {
      return "$";
  } else if (strncmp(btcPriceCurrencyChar, "DKK", 3) == 0) {
      return "kr";
  } else if (strncmp(btcPriceCurrencyChar, "EUR", 3) == 0) {
      return "€";
  } else if (strncmp(btcPriceCurrencyChar, "CHF", 3) == 0) {
    return "Fr";
  } else if (strncmp(btcPriceCurrencyChar, "GBP", 3) == 0) {
    return "£";
  } else {
    return "";
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

/* Convert string s to int out.
 *
 * @param[out] out The converted int. Cannot be NULL.
 *
 * @param[in] s Input string to be converted.
 *
 *     The format is the same as strtol,
 *     except that the following are inconvertible:
 *
 *     - empty string
 *     - leading whitespace
 *     - any trailing characters that are not part of the number
 *
 *     Cannot be NULL.
 *
 * @param[in] base Base to interpret string in. Same range as strtol (2 to 36).
 *
 * @return Indicates if the operation succeeded, or why it failed.
 */
str2int_errno str2int(int *out, char *s, int base) {
    char *end;
    if (s[0] == '\0' || isspace(s[0]))
        return STR2INT_INCONVERTIBLE;
    errno = 0;
    long l = strtol(s, &end, base);
    /* Both checks are needed because INT_MAX == LONG_MAX is possible. */
    if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX))
        return STR2INT_OVERFLOW;
    if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN))
        return STR2INT_UNDERFLOW;
    if (*end != '\0')
        return STR2INT_INCONVERTIBLE;
    *out = l;
    return STR2INT_SUCCESS;
}


String stringArrayToString(String stringArray[], int nrOfItems) {
  String returnValue = "String Array with " + String(nrOfItems) + " items:\n";
  for (int i=0;i<nrOfItems;i++) {
    returnValue += "item " + String(i) + ":" + stringArray[i] + "\n";
  }
  return returnValue;
}

String paymentJsonToString(JsonObject areaElems) {
  String paymentDetail = "";
  //Serial.println("paymentJsonToString");
  if(areaElems["extra"] && !areaElems["pending"] && areaElems["extra"]["tag"]) {
    // Only do lnurlp payments
    const char* tag = areaElems["extra"]["tag"];
    //Serial.println("tag = " + String(tag));
    if(strncmp(tag,"lnurlp",6) == 0) {

      // Payment always has an amount
      long long amount = areaElems["amount"]; // long long to support amounts above 999999000 millisats
      long amountSmaller = amount / 1000; // millisats to sats

      String paymentWalletID = areaElems["wallet_id"];
      setFoundWalletID(paymentWalletID);

      String paymentAmount(amountSmaller);
      String units = "sats";
      if (amountSmaller < 2) units = "sat";
      paymentDetail = paymentAmount + " " + units;

      // Payment has an optional comment
      if (areaElems["extra"]["comment"]) {
        //Serial.println("Getting comment...");
        const char* comment = areaElems["extra"]["comment"];
        if (!comment && areaElems["extra"]["comment"][0]) { // comments can also be a list
          //Serial.println("Getting comment from list...");
          comment = areaElems["extra"]["comment"][0];
        }
        String paymentComment(comment);
        paymentDetail += ": " + paymentComment;
      } else {
        Serial.println("Payment has no comment.");
        paymentDetail += "!"; // no comment so "99999999 sats!" (= almost 1 BTC) will fit on one line in big font
      }
    } else {
      Serial.println("Ignoring non lnurlp payment.");
    }
  } else {
    Serial.println("Ignoring regular payment without extra info for lnurlp.");
  }
  return paymentDetail;
}

String getRandomElementFromArray(String inputArray[], int numOfSlogans) {
  int randomIndex = random(0, numOfSlogans); // Generate a random index between 0 and numOfSlogans-1
  return inputArray[randomIndex]; 
}
