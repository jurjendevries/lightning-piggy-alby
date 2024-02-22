#include <ArduinoJson.h>
#include "Constants.h"

float getBitcoinPrice() {
  String btcPriceCurrency = String(btcPriceCurrencyChar);
  Serial.println("Getting Bitcoin price...");

  #ifdef DEBUG
  Serial.println("Mocking getBitcoinPrice:"); return 30000.2;
  #endif

  // Get the data
  String path = "/v1/bpi/currentprice/" + btcPriceCurrency + ".json";
  String priceData = getEndpointData("api.coindesk.com", path, false);
  DynamicJsonDocument doc(8192); // the size of the list of links is unknown so don't skimp here

  DeserializationError error = deserializeJson(doc, priceData);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return NOT_SPECIFIED;
  }

  Serial.println("Extracting bitcoin price from received data");

  float btcPrice = doc["bpi"][btcPriceCurrency]["rate_float"];

  if (btcPrice == 0.0) {
    Serial.println("BTC Price not found, returning NOT_SPECIFIED");
    return (float)NOT_SPECIFIED;
  }

  Serial.println("BTC Price: " + String(btcPrice, 2));
  return btcPrice;
}

// This shows something like:
// 123.23$ (51234.1$)
// 123.23KR (512021)
// 123.23E (51234.1E)
void showFiatValues(int balance) {
  if (!isConfigured(btcPriceCurrencyChar)) {
    Serial.println("Not showing fiat values because no fiat currency is configured.");
    return;
  }

  float btcPrice = getBitcoinPrice();

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
  if (strlen(currentBtcPriceToShow.c_str()) <= 6) {
     currentBtcPriceToShow += getCurrentCurrencyCode();
  }
  toDisplay += "(" + currentBtcPriceToShow + ")";

  displayBoldMessage(toDisplay, displayHeight() - 4);
}
