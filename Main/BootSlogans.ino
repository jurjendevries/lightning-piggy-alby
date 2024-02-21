// Array of Bitcoin slogans
String bitcoinSlogans[] = {
  "Bitcoin is nerds gold",
  "In Bitcoin we trust",
  "Be your own bank",
  "HODL"
};


// Function to get a random Bitcoin slogan from the array
String getRandomBootSlogan() {
  int numOfSlogans = sizeof(bitcoinSlogans) / sizeof(bitcoinSlogans[0]); // Calculate the number of slogans in the array

  int randomIndex = random(0, numOfSlogans); // Generate a random index between 0 and numOfSlogans-1
  return bitcoinSlogans[randomIndex]; 
}

function showBootSlogan() {
   if (settingLanguage == LANGUAGE_EN_US) {
      displayFit("Behold, today's pearl of wisdom", 0, 20, displayWidth(), 40, 1);
      displayFit("from Dad is...:", 0, 40, displayWidth(), 60, 1);
    }
    else if (settingLanguage == LANGUAGE_DA){
      displayFit("Klar! Her kommer dagens perle af", 0, 20, displayWidth(), 40, 1);
      displayFit("visdom fra far...:", 0, 40, displayWidth(), 60, 1);
    }

    String slogan = getRandomBootSlogan();
    Serial.println("slogan " + slogan);
    displayFit(slogan, 0, 65, displayWidth(), 160, 4);

    delay(3000);
}
