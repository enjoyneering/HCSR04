/***************************************************************************************************/
/*
  This is an Arduino sketch for HC-SR04, HC-SRF05, DYP-ME007, BLJ-ME007Y ultrasonic ranging sensor

  Operating voltage:    5v
  Operating current:    10..20mA
  Working range:        4cm..250cm
  Measuring angle:      15°
  Operating frequency:  40kHz
  Resolution:           0.3cm
  Maximum polling rate: 20Hz

  written by : enjoyneering79
  sourse code: https://github.com/enjoyneering/


  This LCD uses I2C bus to communicate, specials pins are required to interface
  Board:                                    SDA                    SCL                    Level
  Uno, Mini, Pro, ATmega168, ATmega328..... A4                     A5                     5v
  Mega2560................................. 20                     21                     5v
  Due, SAM3X8E............................. 20                     21                     3.3v
  Leonardo, Micro, ATmega32U4.............. 2                      3                      5v
  Digistump, Trinket, ATtiny85............. 0/physical pin no.5    2/physical pin no.7    5v
  Blue Pill, STM32F103xxxx boards.......... PB7                    PB6                    3.3v/5v
  ESP8266 ESP-01........................... GPIO0/D5               GPIO2/D3               3.3v/5v
  NodeMCU 1.0, WeMos D1 Mini............... GPIO4/D2               GPIO5/D1               3.3v/5v
  ESP32.................................... GPIO21/D21             GPIO22/D22             3.3v


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
#include <HCSR04.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //https://github.com/enjoyneering/LiquidCrystal_I2C

#define LCD_ROWS    4          //qnt. of lcd rows
#define LCD_COLUMNS 20         //qnt. of lcd columns

float distance = 0;

/*
HCSR04(trigger, echo, temperature, distance)

trigger     - trigger pin
echo        - echo pin
temperature - ambient temperature, in C
distance    - maximun measuring distance, in cm
*/
HCSR04            ultrasonicSensor(6, 7, 20, 300);
LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);


void setup()
{
  Serial.begin(115200);

  /* LCD connection check */  
  while (lcd.begin(LCD_COLUMNS, LCD_ROWS, LCD_5x8DOTS) != true) //20x4 display, 5x8 pixels size
  {
    Serial.println(F("PCF8574 is not connected or lcd pins declaration is wrong. Only pins numbers: 4,5,6,16,11,12,13,14 are legal."));
    delay(5000);
  }

  lcd.print(F("PCF8574 is OK")); //(F()) saves string to flash & keeps dynamic memory free
  delay(1000);

  lcd.clear();

  ultrasonicSensor.begin();      //set trigger as output & echo pin as input

  /* prints static text */
  lcd.setCursor(0, 0);
  lcd.print(F("D :"));

  lcd.setCursor(0, 1);
  lcd.print(F("Df:"));
}

void loop()
{
  distance = ultrasonicSensor.getDistance();

  lcd.setCursor(3, 0);
  if (distance != HCSR04_OUT_OF_RANGE)
  {
    lcd.print(distance, 1);
    lcd.print(F(" cm         "));
  }
  else
  {
    lcd.print(F("out of range"));
  }
  delay(50);                                             //wait 50msec or more, until echo from previous measurement disappears


  distance = ultrasonicSensor.getMedianFilterDistance(); //pass 3 measurements through median filter, better result on moving obstacles
  
  lcd.setCursor(3, 1);
  if (distance != HCSR04_OUT_OF_RANGE)
  {
    lcd.print(distance, 1);
    lcd.print(F(" cm         "));
  }
  else
  {
    lcd.print(F("out of range"));
  }

  delay(250);                                            //screen refresh rate
}
