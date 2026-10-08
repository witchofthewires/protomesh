/*
 * ui.cpp - Application UI
 *
 * The display, touch input, and LVGL itself are already initialized by the
 * sketch (see lv_setup.hpp) before ui_init() is called.
 */
#include <cstdlib>
#include "ui.h"

static lv_obj_t *preheat_label, *soak_label, *reflow_label, *time_label, *temp_label;
static lv_obj_t *preheat_temp_input, *preheat_time_input, *soak_temp_input, *soak_time_input, *reflow_temp_input, *reflow_time_input;
static lv_obj_t *confirm_button;
static lv_obj_t *confirm_text_label;
static lv_obj_t *boot_label;

uint8_t is_button_pressed = 0;
uint16_t preheat_time = 300;
uint16_t preheat_temp = 300;
uint16_t soak_time = 300;
uint16_t soak_temp = 300;
uint16_t reflow_time = 300;
uint16_t reflow_temp = 300;

static void touch_cb(lv_event_t *e);
void write_text(lv_obj_t *screen, lv_obj_t *label, char *text, int x, int y, lv_align_t justification);
void draw_button(lv_obj_t *screen, lv_obj_t *button);
static void lv_spinbox_increment_event_cb(lv_event_t *e);
static void lv_spinbox_decrement_event_cb(lv_event_t *e);
void create_spinbox(lv_obj_t *spinbox, float min, float max, uint8_t num_digits, uint8_t num_dec_points, uint16_t width, uint16_t x, uint16_t y);
void draw_boot_screen();
void draw_setup_menu(); 

static void touch_cb(lv_event_t *e) {
    is_button_pressed ^= 1;
}

void write_text(lv_obj_t *screen, lv_obj_t *label, char *text, int x, int y, lv_align_t justification) {
    label = lv_label_create(screen);
    lv_label_set_text(label, text);
    lv_obj_align(label, justification, x, y);
}

void draw_button(lv_obj_t *screen, lv_obj_t *button) {
    lv_obj_set_size(button, 200, 90);
    lv_obj_align(button, LV_ALIGN_TOP_MID, 0, 220);
    lv_obj_set_style_bg_opa(button, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(button, 0, 0);
    //lv_obj_remove_flag(button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(button, touch_cb, LV_EVENT_CLICKED, NULL);
}

static void lv_spinbox_increment_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *spinbox = (lv_obj_t*) lv_event_get_user_data(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(spinbox);
    }
}

static void lv_spinbox_decrement_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *spinbox = (lv_obj_t*) lv_event_get_user_data(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(spinbox);
    }
}

/**
 * @title Spinbox with plus and minus buttons
 * @brief Step a fixed-point spinbox with two side buttons that repeat on hold.
 *
 * A centered spinbox is set to $NUM_DIGITS digits with a decimal point at position $NUM_DIGITS - $NUM_DEC_POINTS
 * and a range of `$MIN..$MAX`. `lv_spinbox_step_prev` shifts the active
 * digit. Two square buttons sized to the spinbox height sit on either side
 * using `LV_SYMBOL_PLUS` and `LV_SYMBOL_MINUS` as background images. Their
 * `LV_EVENT_ALL` handlers call `lv_spinbox_increment` or `lv_spinbox_decrement`
 * on `LV_EVENT_SHORT_CLICKED` and `LV_EVENT_LONG_PRESSED_REPEAT`.
 */
void create_spinbox(lv_obj_t *spinbox, float min, float max, uint8_t num_digits, uint8_t num_dec_points, uint16_t width, uint16_t x, uint16_t y)
{
    spinbox = lv_spinbox_create(lv_screen_active());
    lv_spinbox_set_range(spinbox, min, max);
    lv_spinbox_set_digit_format(spinbox, num_digits, num_digits - num_dec_points);
    lv_spinbox_step_prev(spinbox);
    lv_obj_set_width(spinbox, width);
    lv_obj_center(spinbox);
    lv_obj_align(spinbox, LV_ALIGN_TOP_LEFT, x, y);

    uint16_t h = lv_obj_get_height(spinbox);

    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_image_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_increment_event_cb, LV_EVENT_ALL, (lv_obj_t*) spinbox);

    btn = lv_button_create(lv_screen_active());
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_image_src(btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_decrement_event_cb, LV_EVENT_ALL, (lv_obj_t*) spinbox);
}

void draw_boot_screen(void) {
    lv_obj_t *screen = lv_screen_active();
    write_text(screen, boot_label, "KILN: DIY Solder Reflow Oven\nMade with <3 by the Protomesh Collective", 0, 70, LV_ALIGN_TOP_MID);
}

void draw_setup_menu(void) {

    lv_obj_t *screen = lv_obj_create(NULL);
    lv_screen_load(screen);

    write_text(screen, preheat_label, "preheat", -200, 70, LV_ALIGN_TOP_MID);
    write_text(screen, soak_label, "soak", -200, 120, LV_ALIGN_TOP_MID);
    write_text(screen, reflow_label, "reflow", -200, 170, LV_ALIGN_TOP_MID);

    write_text(screen, temp_label, "temp (degF)", -60, 20, LV_ALIGN_TOP_MID);
    create_spinbox(preheat_temp_input, 0, 500, 3, 0, 50, 150, 70);
    create_spinbox(soak_temp_input, 0, 500, 3, 0, 50, 150, 120);
    create_spinbox(reflow_temp_input, 0, 500, 3, 0, 50, 150, 170);

    write_text(screen, time_label, "time (min)", 120, 20, LV_ALIGN_TOP_MID);
    create_spinbox(preheat_time_input, 0, 10, 1, 0, 50, 350, 70);
    create_spinbox(soak_time_input, 0, 10, 1, 0, 50, 350, 120);
    create_spinbox(reflow_time_input, 0, 10, 1, 0, 50, 350, 170);

    confirm_button = lv_obj_create(screen);
    draw_button(screen, confirm_button);
    confirm_text_label = lv_label_create(confirm_button);
    lv_obj_set_style_text_color(confirm_text_label, lv_color_white(), 0);    
    lv_obj_set_style_bg_color(confirm_button, lv_color_hex(0xFF0000), 0);
    lv_obj_center(confirm_text_label);
    lv_label_set_text(confirm_text_label, "CONFIRM");
}

void read_spinboxes(void) {
    preheat_time = lv_spinbox_get_value(preheat_time_input);
    preheat_temp = lv_spinbox_get_value(preheat_temp_input);
    soak_time = lv_spinbox_get_value(soak_time_input);
    soak_temp = lv_spinbox_get_value(soak_temp_input);
    reflow_time = lv_spinbox_get_value(reflow_time_input);
    reflow_temp = lv_spinbox_get_value(reflow_temp_input);
}