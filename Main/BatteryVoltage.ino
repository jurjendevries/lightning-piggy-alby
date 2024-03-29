// Keep a rolling average of the last 10 voltages
double averageVoltage = 0;
int averageVoltageN = 0;

// These values are remembered to know what to erase upon the next draw
int xPosOfHealthAndStatus = NOT_SPECIFIED;
int yPosOfHealthAndStatus = NOT_SPECIFIED;

double approxRollingAverage(double avg, double input) {
  averageVoltageN++;
  if (averageVoltageN>10) averageVoltageN=10;
  avg -= avg/averageVoltageN;
  avg += input/averageVoltageN;
  //Serial.print("approxRollingAverage = " + String(averageVoltage,4));
  return avg;
}

double getAverageBatteryVoltage() {
  return averageVoltage;
}

// Returns the VBAT value.
// If we're not on battery but USB powered, then it returns a negative VBAT value.
double getBatteryVoltage() {
    const int nrReads = 4;

    int totalDiff = 0;

    int prevValue = analogRead(35); delay(10);
    int totalLevel = prevValue;

    for (int multiread=0; multiread<nrReads; multiread++) {
      int value = analogRead(35);
      totalLevel += value;
      totalDiff += abs(value - prevValue);
      delay(100);
    }

    totalLevel = totalLevel / (nrReads+1); // one additional read is done for the prevValue
    totalDiff = totalDiff / nrReads;
    //Serial.println("Average battery level: " + String(totalLevel));
    //Serial.println("Average battery level diff: " + String(totalDiff));

    double voltage = (totalLevel * 1.72) / 1000;
    if (totalDiff > 32) {
      // The battery has a smoothing effect so lots of variance between levels means we're not on battery.
      voltage *= -1;
    }
    averageVoltage = approxRollingAverage(averageVoltage, voltage);
    //return 3.7; // for testing low battery situation
    return voltage;
}
