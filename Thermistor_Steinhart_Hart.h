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

#ifndef Thermistor_Steinhart_Hart_h
#define Thermistor_Steinhart_Hart_h
#include <math.h>

class Thermistor {
  public:
    Thermistor(int pin);
    // ~Thermistor();
    void setup(
      // thermistor nominal resistance
      float thermistorRes = 10000,
      // pulldown resistor value, set this to the measured resistance of
      // your pulldown resistor
      float pulldownRes = 10000,
      // only used for display purposes, if used set to the measured Vcc.
      float vcc = 5.0
    );
    float getTempK();
    float getTempC();
    float getTempF();
    void debug(bool state);
    void setParams(float a, float b, float c);

  private:
    void printDebug(int readVal);
    int _tempPin;
    float _thermistorRes;
    float _pulldownRes;
    float _vcc;
    bool _debug;
    // The A, B and C parameters in the Steinhart-Hart equation
    float _param_a;
    float _param_b;
    float _param_c;
};

#endif
