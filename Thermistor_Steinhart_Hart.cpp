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

// Public methods

Thermistor::Thermistor(int tempPin, int togglePin) {
  _tempPin = tempPin;
  _togglePin = togglePin;
  if (_togglePin != NOT_A_PIN) {
    digitalWrite(_togglePin, LOW);
  }
  // Set default parameters for 3590 NTC thermistor
  setThermistorParams();
  setSchematicParams();
}

float Thermistor::getTempK() {
  if (_togglePin != NOT_A_PIN) {
    digitalWrite(_togglePin, HIGH);
  }
  int readVal = analogRead(_tempPin);
  if (_debug) {
    printDebug(readVal);
  }
  if (_togglePin != NOT_A_PIN) {
    digitalWrite(_togglePin, LOW);
  }
  return getTempK(getThermistorRes(readVal));
}

float Thermistor::getTempC() {
  return getTempK() - 273.15;
}

float Thermistor::getTempF() {
  return 32 + 9 *(getTempK() - 273.15)  / 5;
}

void Thermistor::debug(bool state) {
  _debug = state;
  Serial.println("Thermistor debugging: " + (_debug) ? "ON" : "OFF");
}

void Thermistor::setThermistorParams(float a, float b, float c) {
  _param_a = a;
  _param_b = b;
  _param_c = c;
}

void Thermistor::setSchematicParams(float pulldownRes, float thermistorRes, float vcc) {
  _thermistorRes = thermistorRes;
  _pulldownRes = pulldownRes;
  _vcc = vcc;
  _debug = false;
}


// Private methods

void Thermistor::printDebug(int readVal) {

  Serial.print("Thermistor Nominal Value: ");
  Serial.print(_thermistorRes / 1000, 3);
  Serial.println("k");

  Serial.print("Pulldown Resistor: ");
  Serial.print(_pulldownRes / 1000, 3);
  Serial.println("k");

  Serial.print("VCC: ");
  Serial.println(_vcc, 2);

  Serial.print("ADC: ");
  Serial.println(readVal);

  Serial.print("Voltage: ");
  Serial.println(((readVal * _vcc) / 4095.0), 3);

  Serial.print("Thermistor Resistance: ");
  if (readVal > 0) {
    long thermistor_res = getThermistorRes(readVal);
    Serial.print(thermistor_res);
    Serial.println(" Ohms, ");

    long temp_k = getTempK(thermistor_res);
    Serial.print("Which coincides with: ");
    Serial.print(temp_k);
    Serial.print(" Kelvin, or ");
    Serial.print(temp_k - 273.15);
    Serial.println(" degree Celcius");
  } else {
    Serial.println("N/A!");
    Serial.print("Measuring ");
    Serial.print(readVal);
    Serial.print(" on pin ");
    Serial.print(_tempPin);
    Serial.print(". Check your connections!");
  }
}

float Thermistor::getThermistorRes(int readVal) {
  if (readVal == 0) {
    // We should never read 0 due to the way the schematic is set up,
    // so if we do, return a negative Kelvin value so the application
    // knows something's wrong and can act approrpiately. Avoids
    // division by zero.
    return -1.0;
  } else {
    return _pulldownRes * ((4095.0 / readVal) - 1);
  }
}

float Thermistor::getTempK(float thermistorRes) {
  // Caching the Log(resistance) of the thermistor to avoid recalculating
  float lr = log(thermistorRes);
  return (1 / (_param_a + _param_b * lr + _param_c * pow(lr, 3)));
}
