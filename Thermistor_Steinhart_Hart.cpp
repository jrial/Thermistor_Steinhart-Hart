/*
 * Thermistor library, using the Steinhart-Hart Equation:
 * https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
 *    Temperature in Kelvin = 1 / {A + B[ln(R)] + C[ln(R)]3}
 *
 * Default weights are for a 3590 NTC thermistor:
 * See http://www.rixratas.ee/jaga/jaga.php?fn=NTC_MF52AT_10K.jpg
 *    A = 0.001129148, B = 0.000234125, C = 8.76741E-08
 *
 * Schematic:
 *   [Ground] -- [Pulldown-Resistor] -- | -- [Thermistor] --[Vcc]
 *                                      |
 *                                  Analog Pin
 *
 */

#include "Thermistor_Steinhart_Hart.h"

Thermistor::Thermistor(int pin) {
  _tempPin = pin;
  setup();
}

float Thermistor::getTempK() {
  int readVal = analogRead(_tempPin);
  if (_debug) {
    printDebug(readVal);
  }
  if (readVal == 0) {
    // We should never read 0 due to the way the schematic is set up,
    // so if we do, return a negative Kelvin value so the application
    // knows something's wrong and can act approrpiately. Avoids
    // division by zero in resistance calculation.
    return -1.0;
  } else {
    long thermistor_res = _pulldownRes * ((4095.0 / readVal) - 1);
    // Caching the Log(resistance) of the thermistor to avoid recalculating
    float lr = log(thermistor_res);
    return (1 / (
      0.001129148 +
      0.000234125 * lr +
      0.0000000876741 * pow(lr, 3)
    ));
  }
}

float Thermistor::getTempC() {
  return getTempK() - 273.15;
}

float Thermistor::getTempF() {
  return 32 + 9 *(getTempK() - 273.15)  / 5;
}

void Thermistor::setup(float thermistorRes, float pulldownRes, float vcc) {
  _thermistorRes = thermistorRes;
  _pulldownRes = pulldownRes;
  _vcc = vcc;
  _debug = false;
}

void Thermistor::debug(bool state) {
  _debug = state;
  Serial.println("Thermistor debugging: " + (_debug) ? "ON" : "OFF");
}

void Thermistor::printDebug(int readVal) {
  Serial.print("ADC: ");
  Serial.println(readVal);

  Serial.print("VCC: ");
  Serial.println(_vcc, 2);

  Serial.print("Pulldown: ");
  Serial.print(_pulldownRes / 1000, 3);
  Serial.println("K");

  Serial.print("Voltage: ");
  Serial.println(((readVal * _vcc) / 4095.0), 3);

  Serial.print("Thermistor Resistance: ");
  if (readVal > 0) {
    Serial.print(_pulldownRes * ((4095.0 / readVal) - 1));
    Serial.println(" Ohms, ");
  } else {
    Serial.println("N/A!");
    Serial.print("Measuring ");
    Serial.print(readVal);
    Serial.print(" on pin ");
    Serial.print(_tempPin);
    Serial.print(". Check your connections!");
  }
}
