/*
 * Basic Status Screen Widget for nice!view display
 * Shows BT profile, battery level, and connection state
 */

#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>

struct zmk_widget_status {
    lv_obj_t *obj;
    lv_obj_t *bt_label;
    lv_obj_t *battery_label;
    lv_obj_t *conn_label;
};

int zmk_widget_status_init(struct zmk_widget_status *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_status_obj(struct zmk_widget_status *widget);
