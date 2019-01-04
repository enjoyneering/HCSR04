[![ci-badge][]][ci] [![license-badge][]][license]

# HR-SR04
This is an Arduino library for HC-SR04, HC-SRF05, DYP-ME007, BLJ-ME007Y, SEN136B5B ultrasonic ranging sensor

- Operating voltage:    5v
- Operating current:    10..20mA
- Default range:        4cm..250cm
- Measuring angle:      15°
- Operating frequency:  40kHz
- Resolution:           0.3cm
- Maximum polling rate: 20Hz

Features:
- Read distance from 4cm to 250mm
- Compensate speed of sound according to the ambient temperature
- Pass distance thought median filter
- Set maximum distance to speed up measurement

Tested on:

- Arduino AVR
- Arduino ESP8266
- Arduino STM32

[ci]:                 https://travis-ci.org/dabbotorg/radios
[ci-badge]:           https://img.shields.io/travis/dabbotorg/radios.svg?style=flat-square
[license]:            https://choosealicense.com/licenses/gpl-3.0/
[license-badgehttps]: https://img.shields.io/aur/license/yaourt.svg
