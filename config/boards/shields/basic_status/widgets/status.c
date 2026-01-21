/*
 * Basic Status Screen Widget for nice!view display
 * Shows BT profile, battery level, and connection state
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_changed.h>
#include <zmk/endpoints.h>
#include <zmk/battery.h>
#include <zmk/ble.h>

#include "status.h"

static struct zmk_widget_status status_widget;

LV_FONT_DECLARE(lv_font_montserrat_28);

static void set_bt_profile(lv_obj_t *label, uint8_t profile) {
    char text[8];
    snprintf(text, sizeof(text), "BT %d", profile + 1);
    lv_label_set_text(label, text);
}

static void set_battery_status(lv_obj_t *label, uint8_t level) {
    char text[16];
    snprintf(text, sizeof(text), "BAT %d%%", level);
    lv_label_set_text(label, text);
}

static void set_connection_status(lv_obj_t *label, bool connected) {
    if (connected) {
        lv_label_set_text(label, "CONNECTED");
    } else {
        lv_label_set_text(label, "DISCONNECTED");
    }
}

int zmk_widget_status_init(struct zmk_widget_status *widget, lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(widget->obj, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_pad_all(widget->obj, 4, LV_PART_MAIN);
    lv_obj_set_flex_flow(widget->obj, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(widget->obj, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /* BT Profile - Large centered text */
    widget->bt_label = lv_label_create(widget->obj);
    lv_obj_set_style_text_font(widget->bt_label, &lv_font_montserrat_28, LV_PART_MAIN);
    lv_obj_set_style_text_color(widget->bt_label, lv_color_black(), LV_PART_MAIN);
    set_bt_profile(widget->bt_label, zmk_ble_active_profile_index());

    /* Battery Level */
    widget->battery_label = lv_label_create(widget->obj);
    lv_obj_set_style_text_color(widget->battery_label, lv_color_black(), LV_PART_MAIN);
    set_battery_status(widget->battery_label, zmk_battery_state_of_charge());

    /* Connection Status */
    widget->conn_label = lv_label_create(widget->obj);
    lv_obj_set_style_text_color(widget->conn_label, lv_color_black(), LV_PART_MAIN);
    set_connection_status(widget->conn_label, zmk_ble_active_profile_is_connected());

    return 0;
}

lv_obj_t *zmk_widget_status_obj(struct zmk_widget_status *widget) {
    return widget->obj;
}

static int battery_status_listener(const zmk_event_t *eh) {
    struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);
    if (ev) {
        set_battery_status(status_widget.battery_label, ev->state_of_charge);
    }
    return ZMK_EV_EVENT_BUBBLE;
}

static int ble_profile_listener(const zmk_event_t *eh) {
    struct zmk_ble_active_profile_changed *ev = as_zmk_ble_active_profile_changed(eh);
    if (ev) {
        set_bt_profile(status_widget.bt_label, ev->index);
        set_connection_status(status_widget.conn_label, zmk_ble_active_profile_is_connected());
    }
    return ZMK_EV_EVENT_BUBBLE;
}

static int endpoint_listener(const zmk_event_t *eh) {
    set_connection_status(status_widget.conn_label, zmk_ble_active_profile_is_connected());
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(widget_battery_status, battery_status_listener)
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);

ZMK_LISTENER(widget_ble_profile, ble_profile_listener)
ZMK_SUBSCRIPTION(widget_ble_profile, zmk_ble_active_profile_changed);

ZMK_LISTENER(widget_endpoint, endpoint_listener)
ZMK_SUBSCRIPTION(widget_endpoint, zmk_endpoint_changed);

/* Status screen creation - called by ZMK display subsystem */
lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);
    zmk_widget_status_init(&status_widget, screen);
    return screen;
}
