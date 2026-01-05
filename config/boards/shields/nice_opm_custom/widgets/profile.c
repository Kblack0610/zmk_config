#include <zephyr/kernel.h>
#include "profile.h"

/* Declare the new BT status icons */
LV_IMG_DECLARE(bt_status_empty);
LV_IMG_DECLARE(bt_status_bonded);
LV_IMG_DECLARE(bt_status_connected);

/* Icon dimensions and layout */
#define BT_ICON_SIZE 10
#define BT_ICON_SPACING 2
#define BT_ICON_STRIDE (BT_ICON_SIZE + BT_ICON_SPACING)

/* Starting position for the icon row */
#define BT_ICONS_START_X 5
#define BT_ICONS_Y (145 + BUFFER_OFFSET_BOTTOM)

/* Active profile highlight border thickness */
#define ACTIVE_BORDER_WIDTH 1
#define ACTIVE_BORDER_PADDING 1

/**
 * Draw a single profile status icon at the specified position
 */
static void draw_profile_icon(lv_obj_t *canvas, int profile_idx, int x_pos,
                              const struct status_state *state) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    const lv_img_dsc_t *icon;

    /* Choose icon based on profile status */
    if (!state->profile_bonded[profile_idx]) {
        /* Profile is empty/unbonded - hollow circle */
        icon = &bt_status_empty;
    } else if (!state->profile_connected[profile_idx]) {
        /* Profile is bonded but disconnected - circle with dot */
        icon = &bt_status_bonded;
    } else {
        /* Profile is connected - filled circle */
        icon = &bt_status_connected;
    }

    lv_canvas_draw_img(canvas, x_pos, BT_ICONS_Y, icon, &img_dsc);
}

/**
 * Draw a highlight border around the active profile icon
 */
static void draw_active_highlight(lv_obj_t *canvas, int x_pos) {
    lv_draw_rect_dsc_t rect_dsc;
    init_rect_dsc(&rect_dsc, LVGL_FOREGROUND);
    rect_dsc.bg_opa = LV_OPA_TRANSP;
    rect_dsc.border_width = ACTIVE_BORDER_WIDTH;
    rect_dsc.border_color = LVGL_FOREGROUND;
    rect_dsc.border_opa = LV_OPA_COVER;

    /* Draw border rectangle around the icon */
    int border_x = x_pos - ACTIVE_BORDER_PADDING;
    int border_y = BT_ICONS_Y - ACTIVE_BORDER_PADDING;
    int border_size = BT_ICON_SIZE + (ACTIVE_BORDER_PADDING * 2);

    lv_canvas_draw_rect(canvas, border_x, border_y, border_size, border_size, &rect_dsc);
}

/**
 * Main function to draw the enhanced BT profile status
 */
void draw_profile_status(lv_obj_t *canvas, const struct status_state *state) {
    /* Draw all 5 profile status icons */
    for (int i = 0; i < BT_PROFILE_COUNT; i++) {
        int x_pos = BT_ICONS_START_X + (i * BT_ICON_STRIDE);

        /* Draw the status icon */
        draw_profile_icon(canvas, i, x_pos, state);

        /* Draw highlight border around active profile */
        if (i == state->active_profile_index) {
            draw_active_highlight(canvas, x_pos);
        }
    }
}
