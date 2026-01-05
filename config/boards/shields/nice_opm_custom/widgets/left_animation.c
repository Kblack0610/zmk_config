#include <stdlib.h>
#include <zephyr/kernel.h>
#include "left_animation.h"

LV_IMG_DECLARE(left_image);
LV_IMG_DECLARE(left_image_2);
LV_IMG_DECLARE(left_image_3);
LV_IMG_DECLARE(left_image_4);
LV_IMG_DECLARE(left_image_5);

const lv_img_dsc_t *anim_imgs[] = {
    &left_image, &left_image_2, &left_image_3, &left_image_4, &left_image_5,
};

/*
 * Animation scaling: 256 = 100%, 128 = 50%, 64 = 25%
 * Adjust ANIM_ZOOM to resize the One Punch Man animation
 */
#define ANIM_ZOOM 128  /* 50% of original size */

/* Positioning adjustments for scaled animation */
#define ANIM_X_OFFSET 56  /* Adjusted for smaller image */
#define ANIM_Y_OFFSET 10  /* Slight vertical offset to center */

void draw_left_animation(lv_obj_t *canvas) {
#if IS_ENABLED(CONFIG_NICE_LEFT_ANIMATION)
    lv_obj_t *art = lv_animimg_create(canvas);
    lv_obj_center(art);

    lv_animimg_set_src(art, (const void **)anim_imgs, 5);
    lv_animimg_set_duration(art, CONFIG_NICE_LEFT_ANIMATION_MS);
    lv_animimg_set_repeat_count(art, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(art);

    /* Apply zoom scaling to shrink the animation */
    lv_img_set_zoom(art, ANIM_ZOOM);
#else
    lv_obj_t *art = lv_img_create(canvas);

    lv_img_set_src(art, &left_image_5);

    /* Apply zoom scaling to shrink the static image */
    lv_img_set_zoom(art, ANIM_ZOOM);
#endif

    lv_obj_align(art, LV_ALIGN_TOP_LEFT, ANIM_X_OFFSET, ANIM_Y_OFFSET);
}
