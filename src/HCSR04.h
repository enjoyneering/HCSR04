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


  Board:                                     Level
  Uno, Mini, Pro, ATmega168, ATmega328.....  5v
  Mega, Mega2560, ATmega1280, ATmega2560...  5v
  Due, SAM3X8E.............................  3.3v
  Leonardo, ProMicro, ATmega32U4...........  5v
  Blue Pill, STM32F103xxxx boards..........  3v
  NodeMCU 1.0, WeMos D1 Mini...............  3v/5v
  ESP32....................................  3v

  Frameworks & Libraries:
  ATtiny Core           - https://github.com/SpenceKonde/ATTinyCore
  ESP32 Core            - https://github.com/espressif/arduino-esp32
  ESP8266 Core          - https://github.com/esp8266/Arduino
  STM32 Core            - https://github.com/rogerclarkmelbourne/Arduino_STM32

  GNU GPL license, all text above must be included in any redistribution, see link below for details:
  - https://www.gnu.org/licenses/licenses.html
*/
/***************************************************************************************************/

#ifndef HCSR04_h
#define HCSR04_h

/* 
The arduino toolchain includes library headers before it includes your sketch.
Unfortunately, you cannot #define something in a sketch & get it in the library.
*/
//#define HCSR04_DISABLE_INTERRUPTS //uncomment to disable interrupts during measurement
//#define HCSR04_ECHO_CANCELLATION  //uncoment to disable echo cancellation after measurement

#if defined(ARDUINO) && ((ARDUINO) >= 100) //arduino core v1.0 or later
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#if defined(__AVR__)
#include <avr/pgmspace.h>                  //for Arduino AVR PROGMEM support
#elif defined(ESP8266)
#include <pgmspace.h>                      //for Arduino ESP8266 PROGMEM support
#elif defined(_VARIANT_ARDUINO_STM32_)
#include <avr/pgmspace.h>                  //for Arduino STM32 PROGMEM support
#endif

#define HCSR04_RANGE_MIN          4        //in cm, after 1.5cm readings jump in the range 2.5cm - 3.5cm & to avoid it use min distance > 3.5cm
#define HCSR04_RANGE_MAX          500      //in cm
#define HCSR04_ECHO_DELAY         50       //delay before next measurement, 30ms..60ms

#define HCSR04_SOUND_SPEED_ZERO_C 33130    //in cm/s, speed of sound @ 0°C
#define HCSR04_OUT_OF_RANGE       38000    //sensor returns 30000μs..38000μs echo pulse, if out of range


class HCSR04
{
 public:
  HCSR04(uint8_t triggerPin, uint8_t echoPin, int16_t temperature = 20, uint16_t maxDistance = 300);

  void     begin(void);
  float    getDistance(void);
  float    getMedianFilterDistance(void);
  void     setTemperature(int16_t temperature);

 private:
  uint8_t  _triggerPin;
  uint8_t  _echoPin;
  float    _oneCentimetreRoundTripTime;
  uint8_t  _timeOutMin;
  uint16_t _timeOutMax;

         uint16_t calcSoundSpeed(int16_t temperature);
         float    calcOneCentimetreRoundTripTime(uint16_t soundSpeed);
         uint16_t calcEchoTimeout(uint16_t distance);
  inline uint16_t getEchoPulseLength(void);
  inline float    calcDistance(uint16_t pulseLength);
};

#endif
