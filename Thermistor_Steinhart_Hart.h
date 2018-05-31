#ifndef Thermistor_Steinhart_Hart_h
#define Thermistor_Steinhart_Hart_h
#include <Esp.h>
#include <math.h>

class Thermistor {
  public:
    Thermistor(int pin);
    // ~Thermistor();
    void setup(
      // thermistor nominal resistance
      float thermistorRes = 10000,
      // balance/pad resistor value, set this to the measured resistance of
      // your pad resistor
      float pulldownRes = 10000,
      // only used for display purposes, if used set to the measured Vcc.
      float vcc = 5.0
    );
    float getTempK();
    float getTempC();
    float getTempF();
    void debug(bool state);

  private:
    void printDebug(int readVal);
    int _tempPin;
    float _thermistorRes;
    float _pulldownRes;
    float _vcc;
    bool _debug;
};

#endif
