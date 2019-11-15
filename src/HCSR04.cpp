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

#include "HCSR04.h"


/**************************************************************************/
/*
    Constructor
*/
/**************************************************************************/
HCSR04::HCSR04(uint8_t triggerPin, uint8_t echoPin, int16_t temperature, uint16_t maxDistance)
{
  _triggerPin                 = triggerPin;
  _echoPin                    = echoPin;

  _oneCentimetreRoundTripTime = calcOneCentimetreRoundTripTime(calcSoundSpeed(temperature)); //in μs
  _timeOutMin                 = calcEchoTimeout(HCSR04_RANGE_MIN);                           //in μs
  _timeOutMax                 = calcEchoTimeout(maxDistance);                                //in μs

  if (_timeOutMax > HCSR04_OUT_OF_RANGE) _timeOutMax = HCSR04_OUT_OF_RANGE;                  //sensor returns 30000μs..38000μs echo pulse, if out of range
}


/**************************************************************************/
/*
    begin()

    Initializes Trigger & Echo pins
*/
/**************************************************************************/
void HCSR04::begin(void)
{
  pinMode(_triggerPin, OUTPUT);
  digitalWrite(_triggerPin, LOW);

  pinMode(_echoPin, INPUT);
}

/**************************************************************************/
/*
    getDistance()

    Returns distance, in cm

    NOTE:
    - speed of sound depends on ambient temperature
*/
/**************************************************************************/
float HCSR04::getDistance(void)
{
  return calcDistance(getEchoPulseLength());
}

/**************************************************************************/
/*
    getMedianFilterDistance()

    Returns median distance after 3 measurements, in cm

    NOTE:
    - see Normal/Gauss distribution for more details
*/
/**************************************************************************/
float HCSR04::getMedianFilterDistance(void)
{
  uint16_t data[3] = {0, 0, 0};
  uint16_t middle  = 0;
  
  for (int8_t i = 0; i < 3; i++)
  {
    data[i] = getEchoPulseLength();                      //save echo signal

    #ifndef HCSR04_ECHO_CANCELLATION
    delay(HCSR04_ECHO_DELAY);                            //wait until echo from previous measurement disappears
    #endif
  }

  if ((data[0] <= data[1]) && (data[0] <= data[2]))
  {
    middle = (data[1] <= data[2]) ? data[1] : data[2];
  }
  else if ((data[1] <= data[0]) && (data[1] <= data[2]))
  {
    middle = (data[0] <= data[2]) ? data[0] : data[2];
  }
  else
  {
    middle = (data[0] <= data[1]) ? data[0] : data[1];
  }

  return calcDistance(middle);                                  
}

/**************************************************************************/
/*
    setTemperature()

    Set air temperature to compensate change in speed of sound
*/
/**************************************************************************/
void HCSR04::setTemperature(int16_t temperature)
{
  _oneCentimetreRoundTripTime = calcOneCentimetreRoundTripTime(calcSoundSpeed(temperature));
}

/**************************************************************************/
/*
    calcSoundSpeed()

    Calculates speed of sound, in cm/s

    NOTE:
    - speed of sound depends on ambient temperature
      temp, °C   speed, m/sec
       35         351.88
       30         349.02
       25         346.13
       20         343.21
       15         340.27
       10         337.31
       5          334.32
       0          331.30
      −5          328.25
      −10         325.18
      −15         322.07
      −20         318.94
      −25         315.77
*/
/**************************************************************************/
uint16_t HCSR04::calcSoundSpeed(int16_t temperature)
{
  return (HCSR04_SOUND_SPEED_ZERO_C) + 60 * temperature;
}

/**************************************************************************/
/*
    calcOneCentimetreRoundTripTime()

    Calculates how many μs are required for ultrasound sound to make a
    round trip, 1cm forward + 1cm back = 2cm
*/
/**************************************************************************/
float HCSR04::calcOneCentimetreRoundTripTime(uint16_t soundSpeed)
{
  return (float)2000000 / soundSpeed; //in μs
}

/**************************************************************************/
/*
    calcEchoTimeout()

    Converts distance to time in μs, required for ultrasound sound to make
    a trip
*/
/**************************************************************************/
uint16_t HCSR04::calcEchoTimeout(uint16_t distance)
{
  if      (distance > HCSR04_RANGE_MAX) distance = HCSR04_RANGE_MAX;
  else if (distance < HCSR04_RANGE_MIN) distance = HCSR04_RANGE_MIN;

  return distance * (uint16_t)_oneCentimetreRoundTripTime;           //convert cm to μs
}

/**************************************************************************/
/*
    getEchoPulseLength()

    Returns interval of echo signal, in μs

    NOTE:
    - pulseIn() works on pulses from 2-3 microseconds to 3 minutes in length
    - pulseIn() must be called at least a few dozen microseconds before the
      start of the pulse
    - pulseIn() returns 0 if no complete pulse was received within timeout
    - next pulse can be transmitted after previous echo is faded away, 
      recommend echo delay ~50ms
    - after ~1.5cm sensor readings jump in the range 2.5cm - 3.5cm
*/
/**************************************************************************/
uint16_t HCSR04::getEchoPulseLength(void)
{
  uint16_t pLength = 0;                           //in μs

  #ifdef HCSR04_DISABLE_INTERRUPTS
  noInterrupts();                                 //disable all interrupts
  #endif

  /* start measurement */
  digitalWrite(_triggerPin, HIGH);
  delayMicroseconds(10);                          //length of triger pulse, 100μs maximum
  digitalWrite(_triggerPin, LOW);                 //300..500μs after trigger low, module during next 200μs sends 8 pulses at 40 kHz & measures echo

  pLength = pulseIn(_echoPin, HIGH, _timeOutMax); //must be called at least a few dozen μs before expected pulse, avarage tHOLLDOFF=700μs

  #ifdef HCSR04_DISABLE_INTERRUPTS
  interrupts();                                   //re-enable all interrupts
  #endif

  #ifdef HCSR04_ECHO_CANCELLATION
  delay(HCSR04_ECHO_DELAY);                       //wait until echo from previous measurement disappears
  #endif

  if ((pLength == 0) || (pLength <= _timeOutMin) || (pLength >= _timeOutMax)) return HCSR04_OUT_OF_RANGE;
                                                                              return pLength;
}

/**************************************************************************/
/*
    calcDistance()

    Convert μs to cm

    NOTE:
    - speed of sound depends on ambient temperature
*/
/**************************************************************************/
float HCSR04::calcDistance(uint16_t pulseLength)
{
  if (pulseLength != HCSR04_OUT_OF_RANGE) return (float)pulseLength / _oneCentimetreRoundTripTime;
                                          return HCSR04_OUT_OF_RANGE;
}
