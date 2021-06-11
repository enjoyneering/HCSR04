[![license-badge][]][license] ![version] [![stars][]][stargazers] ![hit-count] [![github-issues][]][issues]

# HС-SR04
This is an Arduino library for HC-SR04, HC-SRF05, DYP-ME007, BLJ-ME007Y, SEN136B5B, US-100 (PWM output mode), JSN-SR04T (PWM output mode) ultrasonic ranging sensor

- Operating voltage:    5v
- Operating current:    10..20mA
- Default range:        4cm..250cm
- Measuring angle:      15°
- Operating frequency:  40kHz
- Resolution:           0.3cm
- Maximum polling rate: 20Hz

Features:
- Read distance from 4cm to 250mm**
- Compensate speed of sound according to the ambient temperature
- Pass distance thought median filter
- Set maximum distance to speed up measurement

Tested on:

- Arduino AVR
- Arduino ESP8266
- Arduino STM32

**Library returns 38000 if OUT OF RANGE

[license-badge]: https://img.shields.io/badge/License-GPLv3-blue.svg
[license]:       https://choosealicense.com/licenses/gpl-3.0/
[version]:       https://img.shields.io/badge/Version-1.3.0-green.svg
[stars]:         https://img.shields.io/github/stars/enjoyneering/HCSR04.svg
[stargazers]:    https://github.com/enjoyneering/HCSR04/stargazers
[hit-count]:     https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2Fenjoyneering%2FHCSR04&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false
[github-issues]: https://img.shields.io/github/issues/enjoyneering/HCSR04.svg
[issues]:        https://github.com/enjoyneering/HCSR04/issues/
