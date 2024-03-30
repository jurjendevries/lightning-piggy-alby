Lightninig Piggy
====

Bitcoin Lightning piggybank using LNBits (for LNURLp) running on TTGO LilyGo ePaper hardware with ESP32 microcontroller.

See https://www.lightningpiggy.com/ and https://makers.bolt.fun/project/lightningpiggy

Main Source code
===========

The source code in Main/Main.ino works on both the TTGO LilyGo T5 ePaper 2.13 inch DEPG0213BN display and the (discontinued and more expensive) TTGO LilyGo T5 ePaper 2.66 inch DEPG0266BN display boards.

**Tested with:**

- Arduino IDE version 1.8.13
- ESP32 Board Support version 2.0.14
- Preferences -> Compiler warnings: Default
- Tools -> Board -> ESP32 Arduino -> ESP32 Dev Module
- Tools -> Upload Speed: 921600
- Tools -> CPU Frequency: 240Mhz
- Tools -> Flash Frequency: 80Mhz
- Tools -> Flash Mode: QIO
- Tools -> Flash Size: 4MB (32Mb)
- Tools -> Partition Scheme: Default 4MB with spiffs (1.2MB APP, 1.5MB SPIFFS)
- Tools -> Core Debug Level: Warn
- Tools -> PSRAM: Disabled
- Tools -> Port: /dev/ttyACM0

Make sure the Arduino IDE has permissions to access the serial port:

`sudo chmod -f 777 /dev/ttyACM* /dev/ttyUSB*`

**On the lnbits webpage:**

- activate the LNURLp extension
- click on the LNURLp extension
- click "NEW PAY LINK"
- untick "fixed amount"
- set minimum amount: 1
- set maximum amount: 100000000
- set currency to "satoshis"
- click "Advanced options"
- set "Comment maximum characters" to 128
- set "Webhook URL" to https://p.lightningpiggy.com/ (optional, for anonymous usage metrics)
- set a "Success message", like: Thanks for sending sats to my piggy

**Known issues:**
- There's an issue with the 2.13 inch GDEM0213B74 display, somehow the display becomes blank after updateWindow() calls.

How to release
==============
- Update version number in Constants.h
- Update CHANGELOG.md
- Make sure config.h has no changes (cp Main/config.h /tmp; git checkout Main/config.h)
- In Main.ino, make sure you have:
#define LILYGO_T5_V213
//#define LILYGO_T5_V266
- Compile the project
- Copy /tmp/build_*/Main.ino.bin to lightningpiggy.github.io/firmware/ttgo_lilygo_2.13_inch_epaper_latest/Main.ino.bin
- Update the version number in lightningpiggy.github.io/manifests/manifest_ttgo_lilygo_2.13_inch_epaper_latest.json
- In Main.ino, make sure you have:
//#define LILYGO_T5_V213
#define LILYGO_T5_V266
- Compile the project
- Copy /tmp/build_*/Main.ino.bin to lightningpiggy.github.io/firmware/ttgo_lilygo_2.66_inch_epaper_latest/Main.ino.bin
- Update the version number in lightningpiggy.github.io/manifests/manifest_ttgo_lilygo_2.66_inch_epaper_latest.json
- Revert back to default code with custom config (cp /tmp/config.h Main/config.h; git checkout Main/Main.ino)
- Test the webinstaller
