**2.0.0**

Extended battery life:

- Infinite wifi retry, infinite HTTP response waiting time and other long battery-draining exceptions are adressed with a "smart" watchdog.
- The wake-up frequency now adapts based on a seven-point profile correlated with battery voltage.

User experience improvements:

- Realtime balance and payment updates also happen under battery power after manual wakeups, thanks to new awake/sleep status decision logic.
- Notification code enhancements that help the user to easily troubleshoot any possible errors.
- On-screen indication when the device is sleeping vs awake.
- New on-screen software update visibility.
- New startup screen logo by Bitko.

Optional new features:

- Fiat balance and Bitcoin price with configurable fiat currency (USD, EUR, DKK,...)
- Random bootup "slogan" with configurable prelude
- Show last updated day and time

Removed features:

- Disabled HAL sensor value as it wasn't useful

Many thanks to @lonerookie for his awesome pull request full of new features and improvements!


**1.8.1**
- Make QR code a bit bigger on big displays
- Workaround partial display refresh on GDEM displays
- Don't send unnecessary HTTP headers

**1.8.0**
- Ensure compatibility with both 2.13 and 2.66 inch ePaper DEPG

**1.7.6**
- Detect when battery/no battery is connected from few/lots of VBAT (battery voltage) fluctuations.
- Display "NOBAT" instead of battery voltage when no battery is connected.

**1.7.2**
- Cleanly disconnect wifi before hibernate

**1.7.1**
- Show "Connecting to SSIDNAME..." at startup

**1.7.0**
- Show wifi signal strength (%)
- Reduce max font size of payment comments for esthetics
- Use partial display updates to increase speed and eliminate flickering

**1.6.0**
- Fix sat/sats plural
- Draw a line under the total sats amount
- Fix overflow for amounts bigger than 999999 sats
- Improve LOW BATTERY warning and cleanups

**1.5.0**
- Add health and status values to bottom right (battery voltage etc)
- Make sure second payment always fits
- Scale text based on available space
- Fix issue with empty comment payments (reported by Bitcoin3us)

**1.4.3**
- Speed up by eliminating unneeded lnbits URL call
- Speed up chunked reader with buffer
- Rewrite chunked HTTP parser
- Add update checker

**1.3.0**
- Add support for 2.13 inch DEPG ePaper board
- Add shake-to-update
- Add unified screen with total sats balance, LNURLp QR code for receiving, and 2 latest payments
- Add logo's at startup
