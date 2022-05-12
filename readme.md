# HDSP-21xx-driver

<div float="left">
    <img src="./assets/HDSP-21xx_driver.jpg" alt="drawing" height="130"/>
    <img src="./assets/display.jpg" alt="drawing" height="130"/>
</div>

A driver for LED character displays. The PCB is designed as generic frontpanel for several projects.

An example can be found in the uart test of the
[rpihal](https://github.com/oblaser/rpihal/blob/main/test/system/uart.cpp)
project.

### Compatible Models

- HDSP-2107
- HDSP-2112
- HDSP-2110
- HDSP-2111
- HDSP-2113
- HDSP-2504 \*
- HDSP-2502 \*
- HDSP-2500 \*
- HDSP-2501 \*
- HDSP-2503 \*

\* Due to their dimensions they are only software compatible and do not fit on the PCB.


## Programming the PIC

The PIC can be programmed with the Microchip IPE and the *.hex files in the releases.
