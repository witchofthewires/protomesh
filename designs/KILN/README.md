# KILN

## Wiring
1. plug a standard JST cable into the I2C jack such that the red wire connects to IO25 and the black wire connects to GND.
2. plug that across series circuit: VCC, LED, 2kOhm resistor, GND.3.5" LCD Display, ESP32-32E 320x480, Resistance Touch.
3. plug a standard JST cable into the UART jack such that the red wire connects to 5V and the black wire connects to GND.
4. plug those into left power bus on breadboard. plug VCC and GND on MAX6675 into this bus.
5. plug 4-prong JST? cable into SPI jack. connect MISO to MAX6675 SO, SCK to SCK, CS to CS.

### Project Box Dimensions
- [ESP32R35T Schematic Drawing](https://www.lcdwiki.com/res/E32R35T/E32R35T_Size.pdf)
- [Crydom SSR Mechanical Specifications](https://www.sensata.com/sites/default/files/a/sensata-series-1-240-ac-panel-mount-ssr-datasheet.pdf)

screen 101.5x55.5x5.8
relay 57.3x44.5x22.6

157.59x61.56x38.1= 1/8 ft x (D * phi) x (D * phi^2)

38.1 - (22.6 + 5.8) = 9.7

157.59 - (101.5 + 44.5) = 11.59
## Dev
Use the Arduino IDE
### Board Configuration
This code was tested on the following boards:
- Yellow silkscreen "3.5" LCD Display, ESP32-32E 320x480, Resistance Touch." using board 'esp32' by Espressif Systems (tested on 3.0.3)

### Required Arduino libraries 
- lvgl by kisvegabor (tested on 9.3.0)
- MAX6675 library by Adafruit (tested on 1.1.2)