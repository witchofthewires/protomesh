/*
 * ui.cpp - Application UI for the LvglClaudeCodeStub example.
 *
 * A clean starting point for building an LVGL application on the ESP32-32E 4" LCD
 * (320x480 portrait resistive touchscreen).  Replace the body of ui_init()
 * with your own widgets.
 *
 * This stub doubles as a bring-up test:
 *   - "Hello, world!" follows your finger, centered on the touch point, so you
 *     can confirm touch coordinates line up with the display.
 *   - A rectangle cycles RED -> GREEN -> BLUE once per second so you can verify
 *     the panel's color order (BGR vs RGB) and inversion are correct.
 *
 * The display, touch input, and LVGL itself are already initialized by the
 * sketch (see lv_setup.hpp) before ui_init() is called.
 */
#include <cstdlib>
#include "ui.h"

static lv_obj_t *preheat_label, *soak_label, *reflow_label, *preheat_temp_label, *soak_temp_label, *reflow_temp_label, *preheat_time_label, *soak_time_label, *reflow_time_label;
static lv_obj_t *confirm_button;
static lv_obj_t *confirm_text_label;
static lv_obj_t *boot_label;

// Move the "Hello, world!" label so it's centered on wherever the screen is
// being touched.  Fires continuously while pressed, so it tracks a drag too.
static void follow_touch_cb(lv_event_t *e) {
    lv_indev_t *indev = lv_indev_active();
    if (!indev) return;

    lv_point_t p;
    lv_indev_get_point(indev, &p);
/*
    lv_coord_t w = lv_obj_get_width(hello_label);
    lv_coord_t h = lv_obj_get_height(hello_label);
    lv_obj_set_pos(hello_label, p.x - w / 2, p.y - h / 2);
    */
}

// Cycle the test rectangle through pure red, green, and blue.
static void color_cycle_cb(lv_timer_t *t) {
    static const uint32_t colors[3] = { 0xFF0000, 0x00FF00, 0x0000FF };
    static const char    *names[3]  = { "RED", "GREEN", "BLUE" };
    static int i = 0;

    lv_obj_set_style_bg_color(confirm_button, lv_color_hex(colors[i]), 0);
    lv_label_set_text(confirm_text_label, names[i]);
    i = (i + 1) % 3;
}

void write_text(lv_obj_t *screen, lv_obj_t *label, char *text, int x, int y, lv_align_t justification) {
    label = lv_label_create(screen);
    lv_label_set_text(label, text);
    lv_obj_align(label, justification, x, y);
}

void draw_button(lv_obj_t *screen, lv_obj_t *button) {
    lv_obj_set_size(button, 200, 90);
    lv_obj_align(button, LV_ALIGN_TOP_MID, 0, 170);
    lv_obj_set_style_bg_opa(button, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(button, 0, 0);
    lv_obj_remove_flag(button, LV_OBJ_FLAG_SCROLLABLE);
    // Don't let the rectangle eat touches meant for the "follow" behavior.
    lv_obj_remove_flag(button, LV_OBJ_FLAG_CLICKABLE);
}

void draw_boot_screen(void) {
    lv_obj_t *screen = lv_screen_active();
    write_text(screen, boot_label, "KILN: DIY Solder Reflow Oven\nMade with <3 by the Protomesh Collective", 0, 70, LV_ALIGN_TOP_MID);
}

void draw_setup_menu(void) {

    lv_obj_t *screen = lv_obj_create(NULL);
    lv_screen_load(screen);

    write_text(screen, preheat_label, "preheat", -50, 20, LV_ALIGN_TOP_MID);
    write_text(screen, soak_label, "soak", 0, 20, LV_ALIGN_TOP_MID);
    write_text(screen, reflow_label, "reflow", 50, 20, LV_ALIGN_TOP_MID);
    write_text(screen, preheat_temp_label, "300C", -50, 70, LV_ALIGN_TOP_MID);
    write_text(screen, soak_temp_label, "350C", 0, 70, LV_ALIGN_TOP_MID);
    write_text(screen, reflow_temp_label, "400C", 50, 70, LV_ALIGN_TOP_MID);
    write_text(screen, preheat_time_label, "1m", -50, 120, LV_ALIGN_TOP_MID);
    write_text(screen, soak_time_label, "2m", 0, 120, LV_ALIGN_TOP_MID);
    
    confirm_button = lv_obj_create(screen);
    draw_button(screen, confirm_button);

    confirm_text_label = lv_label_create(confirm_button);
    lv_obj_set_style_text_color(confirm_text_label, lv_color_white(), 0);    
    lv_obj_set_style_bg_color(confirm_button, lv_color_hex(0xFF0000), 0);
    lv_obj_center(confirm_text_label);
    lv_label_set_text(confirm_text_label, "CONFIRM");
}

/*
void ui_init(void) {
    // The screen LVGL created for us.  Everything we draw is a child of this.
    lv_obj_t *screen = lv_screen_active();
    draw_setup_menu(screen);

    lv_display_t *disp = lv_display_get_default();
    lv_coord_t scr_w = lv_display_get_horizontal_resolution(disp);
    lv_coord_t scr_h = lv_display_get_vertical_resolution(disp);
    lv_obj_set_pos(hello_label,
                   (scr_w - lv_obj_get_width(hello_label))  / 2,
                   (scr_h - lv_obj_get_height(hello_label)) / 2);

    // Track touches on the whole screen.  The screen must be clickable to
    // receive press events from the touch indev; disable scrolling so dragging
    // a finger doesn't rubber-band/scroll the screen contents.
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(screen, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(screen, follow_touch_cb, LV_EVENT_PRESSING, NULL);

    // Run the color cycle once per second.
    lv_timer_create(color_cycle_cb, 1000, NULL);

    // From here, add your own screens, widgets, and event handlers.
    // See https://docs.lvgl.io/ for the LVGL widget reference.
    
}
*/
