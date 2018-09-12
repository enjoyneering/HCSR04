/***************************************************************************************************/
/*
  This is an Arduino library for HC-SR04, HC-SRF05, DYP-ME007, BLJ-ME007Y ultrasonic ranging sensor

  Operating voltage:    5v
  Operating current:    10..20mA
  Working range:        4cm..250cm
  Measuring angle:      15°
  Operating frequency:  40kHz
  Resolution:           0.3cm
  Maximum polling rate: 20Hz

  written by : enjoyneering79
  sourse code: https://github.com/enjoyneering/


  Frameworks & Libraries:
  ATtiny Core           - https://github.com/SpenceKonde/ATTinyCore
  ESP32 Core            - https://github.com/espressif/arduino-esp32
  ESP8266 Core          - https://github.com/esp8266/Arduino
  ESP8266 I2C lib fixed - https://github.com/enjoyneering/ESP8266-I2C-Driver
  STM32 Core            - https://github.com/rogerclarkmelbourne/Arduino_STM32

  GNU GPL license, all text above must be included in any redistribution, see link below for details:
  - https://www.gnu.org/licenses/licenses.html
*/
/***************************************************************************************************/

/* 
The arduino tool chain includes library headers before it includes your sketch.
So you cannot #define something in a sketch and have it picked up in the library.
*/

#ifndef HCSR04_h
#define HCSR04_h

#if defined(ARDUINO) && ARDUINO >= 100 //arduino core v1.0 or later
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#if defined(__AVR__)
#include <avr/pgmspace.h>              //use for PROGMEM Arduino AVR
#elif defined(ESP8266)
#include <pgmspace.h>                  //use for PROGMEM Arduino ESP8266
#elif defined(_VARIANT_ARDUINO_STM32_)
#include <avr/pgmspace.h>              //use for PROGMEM Arduino STM32
#endif

#define HCSR04_SOUND_SPEED_ZERO_C 33130 //in cm/s, speed of sound @ 0°C
#define HCSR04_RANGE_MIN          4     //in cm, after ~1.5cm sensor readings jump in the range ~2.5cm - 3.5cm, to be safe use min distace > 3.5cm
#define HCSR04_RANGE_MAX          500   //in cm

#define HCSR04_OUT_OF_RANGE       38    //sensor returns 38msec echo pulse if out of range

class HCSR04
{
 public:
  HCSR04(uint8_t triggerPin, uint8_t echoPin, int16_t temperature = 20, uint16_t maxDistance = 300);

  void     begin(void);
  float    getDistance(void);
  float    getMedianFilterDistance(void);

 private:
  uint8_t  _triggerPin;
  uint8_t  _echoPin;
  float    _oneCentimetreRoundTripTime;
  uint8_t  _timeOutMin;
  uint16_t _timeOutMax;

  uint16_t calcSoundSpeed(int16_t temperature);
  float    calcOneCentimetreRoundTripTime(uint16_t soundSpeed);
  uint16_t calcEchoTimeout(uint16_t distance);
  uint16_t getEchoPulseLength(void);
};

#endif
