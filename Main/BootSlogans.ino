// Array of Bitcoin slogans
String bitcoinSlogans[] = {
  "Bitcoin is nerds gold",
  "In Bitcoin we trust",
  "Be your own bank",
  "HODL",
  "Not your keys, not your coins.",
  "There are 100,000,000 sats in a bitcoin.",
  "Bitcoin was predicted by Henry Ford, Friedrich Hayek and Milton Friedman",
  "'The Times 03/Jan/2009 Chancellor on brink of second bailout for banks' -- Satoshi Nakamoto",
  "'If you don't believe me or don't get it, I don't have time to try to convince you, sorry.' -- Satoshi Nakamoto",
  "'The heat from your computer is not wasted if you need to heat your home.' -- Satoshi Nakamoto",
  "'Stay humble, stack sats.' -- Matt Odell",
  "'Bitcoin is a strange game where the only winning move is to play.' -- Bitstein",
  "'If privacy is outlawed, only outlaws heart will have privacy' -- Phil Zimmermann"
};


// Function to get a random Bitcoin slogan from the array
String getRandomBootSlogan() {
  int numOfSlogans = sizeof(bitcoinSlogans) / sizeof(bitcoinSlogans[0]); // Calculate the number of slogans in the array

  int randomIndex = random(0, numOfSlogans); // Generate a random index between 0 and numOfSlogans-1
  return bitcoinSlogans[randomIndex]; 
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
