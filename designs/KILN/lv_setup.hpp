/*
 * lv_setup.hpp - Connects LVGL to the ESP32-32E 4" display and touch.
 * Include this file after lvgl.h in your sketch.
 *
 * The ST7796 is driven in LVGL PARTIAL render mode: LVGL renders into a small
 * in-RAM buffer and hands us regions to blit over SPI.  Touch comes from the
 * XPT2046 resistive controller, which shares the display's SPI bus — so before
 * each touch read we drain any in-flight display DMA (waitFlush) to free the
 * bus.
 */

#pragma once

#include <chipguy_ESP32_32E_4inch_display.h>
#include <chipguy_ESP32_32E_4inch_touch.h>

static chipguy_ESP32_32E_4inch_display display;

// Height (in rows) of the LVGL partial-render buffer.  Bigger = fewer flushes
// (smoother) at the cost of RAM.  320*40*2 = ~25 KB per buffer (portrait).
#define LV_SETUP_BUFFER_ROWS 40

class lv_setup_class {
public:
    // Initializes display, touch input, and LVGL.
    // rotation: 0 = portrait (320x480), 1 = landscape (480x320),
    //           2 = portrait flipped, 3 = landscape flipped.  Degrees
    //           (0/90/180/270) are accepted as aliases for 0/1/2/3.
    void begin(uint16_t rotation = 0) {
        if (!display.begin(rotation)) {
            Serial.println("Display init failed!");
            while (1) delay(100);
        }
        Serial.println("Display initialized");

        // Touch shares the display's SPI bus, so it must come up AFTER the
        // display's begin() has initialized that bus.
        if (!_touch.begin()) {
            Serial.println("Touch init failed!");
            while (1) delay(100);
        }
        _touch.setRotation(rotation);

        lv_init();

        static lv_display_t *disp = lv_display_create(display.width(), display.height());
        lv_display_set_flush_cb(disp, _disp_flush);
        lv_display_set_user_data(disp, this);
        // Force RGB565 so our flush byte-swap (2 bytes/pixel) is always correct,
        // independent of how LVGL's own lv_conf.h color depth resolved.
        lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);

        // Two DMA-capable partial-render buffers (RGB565 = 2 bytes/pixel).
        // The second buffer is what lets the driver DMA one region to the panel
        // while LVGL renders the next region into the other — see flushRegion().
        size_t buf_bytes = (size_t)display.width() * LV_SETUP_BUFFER_ROWS * 2;
        lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc(buf_bytes, MALLOC_CAP_DMA);
        lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc(buf_bytes, MALLOC_CAP_DMA);
        if (!buf1 || !buf2) {
            Serial.println("LVGL buffer allocation failed!");
            while (1) delay(100);
        }
        lv_display_set_buffers(disp, buf1, buf2, buf_bytes,
                               LV_DISPLAY_RENDER_MODE_PARTIAL);

        static lv_indev_t *indev = lv_indev_create();
        lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
        lv_indev_set_read_cb(indev, _touchpad_read);
        lv_indev_set_user_data(indev, this);

        lv_tick_set_cb(_tick_get);
    }

    // Access the touch driver to tune calibration, e.g. from setup():
    //   lv_setup.touch().setCalibration(xMin, xMax, yMin, yMax);
    chipguy_ESP32_32E_4inch_touch &touch() { return _touch; }

private:
    chipguy_ESP32_32E_4inch_touch _touch;

    static uint32_t _tick_get(void) { return millis(); }

    static void _disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *pixelmap) {
        // flushRegion byte-swaps in place, waits for the previous region's DMA,
        // then QUEUES this region's DMA and returns before it finishes.  Calling
        // flush_ready now lets LVGL render the next region into the other buffer
        // while this one transmits; the driver serializes the bus internally.
        display.flushRegion(area->x1, area->y1, area->x2, area->y2,
                            (uint16_t *)pixelmap);
        lv_display_flush_ready(disp);
    }

    static void _touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
        auto *self = (lv_setup_class *)lv_indev_get_user_data(indev);
        // Touch and display share one SPI bus.  Drain any in-flight display DMA
        // so the bus is free before we talk to the XPT2046.
        display.waitFlush();
        uint16_t x, y;
        if (self->_touch.read(x, y)) {
            data->point.x = x;
            data->point.y = y;
            data->state = LV_INDEV_STATE_PRESSED;
        } else {
            data->state = LV_INDEV_STATE_RELEASED;
        }
    }
};

static lv_setup_class lv_setup;
