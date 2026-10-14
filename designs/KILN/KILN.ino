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
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_40 1

// The UI for this example lives in ui.h / ui.cpp in this sketch folder.
// It is a hand-written "Hello, world!" screen meant as a starting point.
// Build your own application by editing ui_init() in ui.cpp.
#include "ui.h"
extern uint8_t is_button_pressed;
extern int32_t preheat_time, preheat_temp, soak_time, soak_temp, reflow_time, reflow_temp, cool_time, cool_temp; 

/***********************************************
 * from https://github.com/aBoyCanDream/Solder-Reflow-Oven/blob/main/PlatformIO/src/main.cpp
 ***********************************************/
// Define Variables we'll be connecting to
double pid_in, pid_out, pid_sp;

#include <PID_v1.h> // Need to add to your library. Search for "PID" by Brett Beauregard
// Specify the links and initial tuning parameters
// Awesome explanation of PID and how to tune the values below if you're overshooting or undershooting temp can be found here: https://www.youtube.com/watch?v=hRnofMxEf3Q&pp=ygULZGlnaWtleSBwaWQ%3D
double Kp=2, Ki=5, Kd=1;
PID myPID(&pid_in, &pid_out, &pid_sp, Kp, Ki, Kd, DIRECT);
/********************************************** */
unsigned long time_since_reflow_started;
unsigned long time_temp_check = 2000;
unsigned long last_time_temp_check = 0;
bool preheating = false, soaking = false, reflowing = false, cooling = false, baking = false, new_state = false;

//int SCK = 18;
int SO = 19;
int CS = 21; 
int RELAY_PIN = 25;
int RELAY_STATE = LOW;
MAX6675 thermocouple(SCK, CS, SO);

const uint8_t STATE_BOOT = 0;
const uint8_t STATE_SETUP_MENU_BUILD = 1;
const uint8_t STATE_SETUP_MENU_RENDER = 2;
const uint8_t STATE_FIRE = 3;
const uint8_t STATE_BAKE = 100;
const uint8_t STATE_HALT = 255;
uint8_t state = STATE_BOOT;

void execute_and_advance_state(void) {
    Serial.printf("[SYSTEM] State: %d\tButton: %d\n", 
                  state, 
                  is_button_pressed);
    delay(2000);
    switch (state) {
        case STATE_BOOT:
            execute_boot_state();
            state = STATE_SETUP_MENU_BUILD;
            break;
        case STATE_SETUP_MENU_BUILD:
            draw_setup_menu();
            Serial.printf("[SYSTEM] Setup menu drawn!\n");
            state = STATE_SETUP_MENU_RENDER;
            break;
        case STATE_SETUP_MENU_RENDER:
            execute_setup_menu_render_state();
            if (is_button_pressed) {state = STATE_FIRE; is_button_pressed=0;}
            break;
        case STATE_FIRE:
            read_spinboxes();
            Serial.printf("[FIRE] Preheat %dC for %ds; soak %dC for %ds; reflow %dC for %ds\n", 
                          preheat_temp, preheat_time / 1000, soak_temp, soak_time / 1000 , reflow_temp, reflow_time / 1000);
            state = STATE_BAKE;
            break;
        case STATE_BAKE:
            bake();
            state = STATE_SETUP_MENU_RENDER;
            break;
        case STATE_HALT:
            execute_halt_state();
            break;
        default:
            execute_error_state();
            break;
    }
}

void relayWrite(int pin, int state) {
    digitalWrite(pin, state);
    Serial.printf("[RELAY] Pin %d set to %d\n", pin, state);
}

void execute_boot_state() {
    draw_boot_screen();
    Serial.printf("[SYSTEM] KILN: DIY Solder Reflow Oven\n");
    Serial.printf("[SYSTEM] Made with <3 by the Protomesh Collective\n");
    delay(2000);
}

void execute_error_state() {
    Serial.printf("[SYSTEM] Error! Aborting execution.\n");
    Serial.printf("[SYSTEM] Recommend power cycle.\n");
}

void execute_setup_menu_render_state() {
    Serial.printf("[MAX6675] Temperature: %f deg C | %f deg F\n", thermocouple.readCelsius(), thermocouple.readFahrenheit());
}

void execute_halt_state() { execute_setup_menu_render_state(); }

void setup() {

    int baud = 115200;
    Serial.begin(baud);
    Serial.printf("\n[ESP32] Serial initialized at %d baud\n", baud);

    // Initialize display, touch, and LVGL.  Defaults to portrait; pass a
    // rotation to change it (0/2 = portrait, 1/3 = landscape; see header).
    lv_setup.begin(1);
    Serial.printf("[ESP32] LVGL initialized with %dx%d touchscreen\n",
                  display.width(), display.height());

    // Start the application's own setup
    Serial.printf("[SYSTEM] Starting finite state machine\n");
    execute_and_advance_state();

    Serial.printf("[SPI] MOSI pin %d, MISO pin %d, SCK pin %d, CS pin %d\n", MOSI, MISO, SCK, CS);

    /* from aBoyCanDream */
    pid_sp = cool_temp;
    myPID.SetOutputLimits(0, 1); // tell the PID to range between 0 and the full window size
    myPID.SetMode(AUTOMATIC); // turn the PID on
    Serial.printf("[PID] Initialized cybernetic control loop (Kp=%.3f, Ki=%.3f, Kd=%.3f)\n", Kp, Ki, Kd);

    pinMode(RELAY_PIN, OUTPUT);
    relayWrite(RELAY_PIN, RELAY_STATE);
    Serial.printf("[PID] Initialized solid-state relay (pin %d) to %d\n", RELAY_PIN, RELAY_STATE);

  // pinMode(BACKLIGHT_PIN, OUTPUT);
  // digitalWrite(BACKLIGHT_PIN, HIGH); // We'll just have the backlight on all the time. If you want it to sleep, you can toggle this on/off
    /*********************/

    // Resistive touch varies between panels.  If touch lands in the wrong
    // place, gather raw values with lv_setup.touch().readRaw() and feed the
    // corners into lv_setup.touch().setCalibration(xMin, xMax, yMin, yMax).
}

void loop() {
    // Give loop control to LVGL objects created by the application
    for (int i=0; i<200; i++) {
      lv_timer_handler();
      delay(5);
    }
    
    execute_and_advance_state();
}

void bake() {
    uint32_t total_time = preheat_time + soak_time + reflow_time + cool_time;
    Serial.printf("[OVEN] Initiating bake\n");
    unsigned long reflowStarted = millis();
    baking = true;
    while (baking) {
        time_since_reflow_started = millis() - reflowStarted;
        if (time_since_reflow_started - last_time_temp_check >= time_temp_check) {
            last_time_temp_check = time_since_reflow_started;
            pid_in = thermocouple.readCelsius();
            printState();
        }

        //Serial.print("\tSetpoint:"); Serial.print(pid_sp);
        //Serial.print("\tInput:"); Serial.print(pid_in);
        myPID.Compute();
        if (pid_out < 0.5) {
          digitalWrite(RELAY_PIN,LOW);
        }
        if (pid_out > 0.5) {
          digitalWrite(RELAY_PIN,HIGH);
        }
        //Serial.print("\tOutput:"); Serial.println(pid_out);

        if(time_since_reflow_started > total_time) {
            Serial.println("exit at (time_since_reflow_started > total_time)");
            baking = false;
        } else if (time_since_reflow_started > (preheat_time + soak_time + reflow_time)) { // preheat and soak and reflow are complete. Start cooldown
            if (!cooling) {
                new_state = true;
                preheating = false, soaking = false, reflowing = false, cooling = true;
            }
            pid_sp = cool_temp;
        } else if (time_since_reflow_started > (preheat_time + soak_time)) { // preheat and soak are complete. Start reflow
            if (!reflowing) {
                new_state = true;
                preheating = false, soaking = false, reflowing = true, cooling = false;
            }
            pid_sp = reflow_temp;
        } else if (time_since_reflow_started > preheat_time) { // preheat is complete. Start soak
            if (!soaking) {
                new_state = true;
                preheating = false, soaking = true, reflowing = false, cooling = false;
            }
            pid_sp = soak_temp;
        } else { // cycle is starting. Start preheat
            if (!preheating) {
                new_state = true;
                preheating = true, soaking = false, reflowing = false, cooling = false;
            }
            pid_sp = preheat_temp;
        }
    }
}

void printState(){
    Serial.printf("Current time: %d\t", time_since_reflow_started);
    Serial.printf("Current temp: %.2f\t", pid_in);
    //tft.fillRoundRect(4*gridSize+2, 3*gridSize+2, 2*gridSize-4, gridSize-4, 10, TFT_BLACK);
    //writeText(4,3,2,1,1,TFT_WHITE, TFT_BLACK, time, 10, true);
    String tempReading = String(pid_in) + " C";
    //writeText(4,3,2,1,3,TFT_WHITE, TFT_BLACK, tempReading, 10, true);
    String currentState;
    if(preheating){
        currentState = "Preheating";
    }
    if(soaking){
        currentState = "Soaking";
    }
    if(reflowing){
        currentState = "Reflowing";
    }
    if(cooling){
        currentState = "Cool Down";
    }
    Serial.println(currentState);

    if(new_state){
        //tft.setFont(&FreeMonoBold12pt7b);
        //tft.setFreeFont(FSB12);
        new_state = false;
        //tft.fillRoundRect(1*gridSize+25, 0*gridSize+2, 3*gridSize-4, gridSize-20, 10, TFT_BLACK);
        //writeText(2,0,2,1, 4, TFT_WHITE, TFT_BLACK, currentState);
    }
}
