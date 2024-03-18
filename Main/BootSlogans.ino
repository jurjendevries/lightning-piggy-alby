// Array of Bitcoin slogans
// This one was removed because it's a bit confusing for kids without context:
// "'The Times 03/Jan/2009 Chancellor on brink of second bailout for banks' -- Satoshi Nakamoto",
String bitcoinSlogans[] = {
  "Bitcoin is nerds gold.",
  "In Bitcoin we trust.",
  "Be your own bank.",
  "HODL",
  "Not your keys, not your coins.",
  "There are 100,000,000 sats in a Bitcoin.",
  "Bitcoin was predicted by Henry Ford, Friedrich Hayek and Milton Friedman",
  "Do your own research.",
  "Think for yourself.",
  "Don't delegate your thinking to someone else.",
  "Pursue truth relentlessly.",
  "Love yourself and love others.",
  "Bitcoin is Right.",
  "Be just, be fair and be kind.",
  "If you're afraid of something, study it.",
  "Everybody has a hungry heart.",
  "All humans want the same fundamental things for the same reasons.",
  "No matter how much you have, you will be equally happy.",
  "Having more will not make you happy.",
  "Practice compassion; put yourself in someone else's shoes.",
  "Practice attention; feel without judgement. Experience with curiosity.",
  "Practice gratitude; always find things to be thankful for.",
  "Actions speak louder than words.",
  "What you do determines who you are.",
  "You have a right to property.",
  "Your body is your property.",
  "You are the only one in charge of you.",
  "Your life is yours and yours alone.",
  "Only you can live your life.",
  "Cypherpunks write code.",
  "'If you don't believe me or don't get it, I don't have time to try to convince you, sorry.' -- Satoshi Nakamoto",
  "'The heat from your computer is not wasted if you need to heat your home.' -- Satoshi Nakamoto",
  "'How does everyone feel about the B symbol with the two lines through the outside?' - Satoshi Nakamoto",
  "'The credential that establishes someone as real is the ability to supply CPU power.' - Satoshi Nakamoto",
  "'For greater privacy, it's best to use bitcoin addresses only once.' - Satoshi Nakamoto",
  "'I am not Dorian Nakamoto.' - Satoshi Nakamoto",
  "'I'm sure that in 20 years there will either be very large transaction volume or no volume.' - Satoshi Nakamoto",
  "'Lost coins only make everyone else's coins worth slightly more. Think of it as a donation to everyone.' - Satoshi Nakamoto",
  "'Stay humble, stack sats.' - Matt Odell",
  "'Bitcoin is a strange game where the only winning move is to play.' -- Bitstein",
  "'If privacy is outlawed, only outlaws will have privacy.' -- Phil Zimmermann"
};

String bitcoinSlogans_nl[] = {
  "Bitcoin is goud voor nerds.",
  "We vertrouwen op Bitcoin.",
  "Wees je eigen bank.",
  "HODL",
  "Niet je sleutels, niet je munten.",
  "Er zitten 100 000 000 sats in een Bitcoin.",
  "Bitcoin werd voorspeld door Henry Ford, Friedrich Hayek en Milton Friedman.",
  "Doe je eigen onderzoek.",
  "Denk zelf.",
  "Delegeer je denken niet aan een ander.",
  "Blijf de waarheid opzoeken.",
  "Hou van jezelf en van anderen.",
  "Bitcoin is Juist.",
  "Wees rechtvaardig, eerlijk, en lief.",
  "Als je ergens bang van bent, bestudeer het dan.",
  "Iedereen heeft een hongerig hart.",
  "Alle mensen willen funadmenteel hetzelfde om dezelfde redenen.",
  "Hoe veel je ook hebt, je zal even gelukkig zijn.",
  "Meer hebben zal je niet gelukkiger maken.",
  "Beoefen medeleven; stel jezelf in andermans schoenen.",
  "Beoefen aandacht; voel zonder oordeel. Ervaar met nieuwsgierigheid.",
  "Beoefen dankbaarheid; benoem waar je dankbaar voor bent.",
  "Acties spreken luider dan woorden.",
  "Wat je doet bepaalt wie je bent.",
  "Je hebt recht op eigendom.",
  "Je lichaam is jouw eigendom.",
  "Enkel jij bent de baas van jou.",
  "Je leven is van alleen van jou.",
  "Enkel jij kan je leven leiden.",
  "Cypherpunks schrijven code.",
  "'Als je me niet gelooft of het niet begrijpt, heb ik geen tijd om je te proberen te overtuigen, sorry.' -- Satoshi Nakamoto",
  "'De warmte van je computer wordt niet verspild als je je huis moet verwarmen.' -- Satoshi Nakamoto",
  "'Wat vinden jullie van het B symbool met 2 lijnen door de buitenkant?' - Satoshi Nakamoto",
  "'Wat bewijst dat iemand echt is, is dat hij rekenkracht kan leveren.' - Satoshi Nakamoto",
  "'Voor meer privacy is het best om Bitcoin adressen slechts 1 keer te gebruiken.' - Satoshi Nakamoto",
  "'Ik ben niet Dorian Nakamoto.' - Satoshi Nakamoto",
  "'Ik ben zeker dat er binnen 20 jaar ofwel heel veel transacties zullen zijn, ofwel geen enkele.' - Satoshi Nakamoto",
  "'Verloren munten maken gewoon de munten van de anderen iets meer waard. Je kan het beschouwen als een gift aan iedereen.' - Satoshi Nakamoto",
  "'Blijf bescheiden, stapel sats.' - Matt Odell",
  "'Bitcoin is een vreemd spel waarbij het enige wat je kan doen om te winnen, is te spelen.' -- Bitstein",
  "'Als privacy illegaal wordt, zullen alleen illegalen privacy hebben.' -- Phil Zimmermann"
};

// Function to get a random Bitcoin slogan from the array
String getRandomBootSlogan() {
  if (isConfigured(localeSetting)) {
    if (strncmp(localeSetting,"nl",2) == 0) {
      return getRandomStringFromArray(bitcoinSlogans_nl, sizeof(bitcoinSlogans_nl)/sizeof(bitcoinSlogans_nl[0]));
    }
  }

  // Default to Engish
  return getRandomStringFromArray(bitcoinSlogans, sizeof(bitcoinSlogans)/sizeof(bitcoinSlogans[0]));
}

String getRandomStringFromArray(String bitcoinSlogans[], int numOfSlogans) {
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
