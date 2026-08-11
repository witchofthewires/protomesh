/*******************************************************************************
 * built from https://github.com/chipguyhere/ESP32-32E_4inch_display
 * for the ESP32-32E 4.0" touch LCD (ST7796 320x480 + XPT2046 touch,
 * display and touch sharing one SPI bus).
 *
 * Orientation: lv_setup.begin(rotation) accepts 0 = portrait (320x480, the
 * ST7796's native orientation), 1 = landscape (480x320), 2 = portrait
 * flipped, 3 = landscape flipped (degrees 90/180/270 work as aliases for
 * 1/2/3).  This sketch defaults to portrait; change the argument below to
 * rotate.  Display and touch rotate together.
 *
 * Compatible with LVGL 9; the bundled lv_conf.h is based on LVGL 9.3.  If your
 * installed LVGL differs, the API is generally close enough to adapt.
 *
 * Requires the following libraries:
 *   - chipguy_ESP32_32E_4inch_display  (display + touch driver, this library)
 *   - lvgl 9
 *
 * Arduino IDE Board Settings:
 *   - Board: "ESP32 Dev Module" (in Tools > Board > esp32).
 *   - The defaults work; the sketch fits the default partition scheme.
 *
 ******************************************************************************/

// The bundled lv_conf.h is based on LVGL 9.3 (LVGL 9 compatible).
// It's important for the config and library versions to be compatible.
#include "lv_conf.h"
#include "lvgl.h"
#include "lv_setup.hpp"
#include "max6675.h"

// Fonts
// The Montserrat font is built into LVGL in multiple sizes, each size takes up memory.
// We enable the 14 point font as a default font.
#define LV_FONT_MONTSERRAT_14 1
// need other sizes?  You'll know if you see compiler errors after trying to
// use them.  Just edit lv_conf.h where FONTs are turned off (0) and turn them on (1)

// The UI for this example lives in ui.h / ui.cpp in this sketch folder.
// It is a hand-written "Hello, world!" screen meant as a starting point.
// Build your own application by editing ui_init() in ui.cpp.
#include "ui.h"

//int SCK = 18;
int SO = 19;
int CS = 21; 
int RELAY_SIGNAL = 25;
int RELAY_STATE = LOW;
MAX6675 thermocouple(SCK, CS, SO);

uint8_t STATE_BOOT = 0;
uint8_t STATE_HALT = 255;

uint8_t advance_state(uint8_t cur_state) {
    switch (cur_state) {
        case STATE_BOOT:
        case STATE_HALT:
            loop();
    }
}

void relayWrite(int pin, int state) {
    digitalWrite(pin, state);
    Serial.printf("[RELAY] Pin %d set to %d\n", pin, state);
}

void setup() {

    int baud = 115200;
    Serial.begin(baud);
    Serial.printf("\n[ESP32] Serial initialized at %d baud\n", baud);

    Serial.printf("[SPI] MOSI pin %d, MISO pin %d, SCK pin %d, CS pin %d\n", MOSI, MISO, SCK, CS);

    // initialize relay pins
    //pinMode(RELAY_GND, OUTPUT);
    //digitalWrite(RELAY_GND, LOW);
    pinMode(RELAY_SIGNAL, OUTPUT);
    relayWrite(RELAY_SIGNAL, RELAY_STATE);

    // Initialize display, touch, and LVGL.  Defaults to portrait; pass a
    // rotation to change it (0/2 = portrait, 1/3 = landscape; see header).
    lv_setup.begin(1);
    Serial.printf("[ESP32] LVGL initialized with %dx%d touchscreen\n",
                  display.width(), display.height());

    // Resistive touch varies between panels.  If touch lands in the wrong
    // place, gather raw values with lv_setup.touch().readRaw() and feed the
    // corners into lv_setup.touch().setCalibration(xMin, xMax, yMin, yMax).

    // Start the application's own setup
    ui_init();
    Serial.printf("[ESP32] UI initialized\n");
}


void loop() {
    // Give loop control to LVGL objects created by the application
    for (int i=0; i<200; i++) {
      lv_timer_handler();
      delay(5);
    }
    
    RELAY_STATE ^= HIGH;
    relayWrite(RELAY_SIGNAL, RELAY_STATE);
    Serial.printf("[MAX6675] Temperature: %f deg C | %f deg F\n", thermocouple.readCelsius(), thermocouple.readFahrenheit());
}
