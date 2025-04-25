#define K_LOG_TAG "llk:UI"
#include "k_log.h"

#include "k_game/core/k_canvas.h"
#include "k_game/core/k_image.h"

#include "../llk_ui_ext.h"

struct llk_ui_elem_image {
    struct llk_ui_elem super;

    struct k_image *src;
};

static int llk__ui_elem_image_init(struct llk_ui_elem *elem) {
    struct llk_ui_elem_image *image = (struct llk_ui_elem_image *)elem;

    image->src = NULL;
    return 0;
}

static int llk__ui_elem_image_set_attr_src(struct llk_ui_elem *elem, const char *val) {
    struct llk_ui_elem_image *image = (struct llk_ui_elem_image *)elem;

    struct k_image *src = k_image_find(val);
    if (NULL == src) {
        k_log_warn("Image not found, src: `%s`", val);
        return -1;
    }

    image->src = src;
    return 0;
}

static int llk__ui_elem_image_set_attr(struct llk_ui_elem *elem, const char *key, const char *val) {

    if (0 == strcmp(key, "src"))
        return llk__ui_elem_image_set_attr_src(elem, val);

    return llk__ui_elem_set_attr_default(elem, key, val);
}

static void llk__ui_elem_image_draw(struct llk_ui_elem *elem) {
    struct llk_ui_elem_image *image = (struct llk_ui_elem_image *)elem;

    if (NULL == image->src)
        return;

    struct k_canvas_draw_image_options options;
    options.scaled_w = elem->w.computed_val;
    options.scaled_h = elem->h.computed_val;
    options.angle    = 0.0f;
    options.pivot_x  = 0.0f;
    options.pivot_y  = 0.0f;
    options.flip_x   = 0;
    options.flip_y   = 0;
    k_canvas_ui_draw_image(image->src, NULL, elem->x, elem->y, &options);
}

struct llk_ui_elem_type llk__ui_elem_image = {
    .type_name         = "image",
    .elem_struct_size         = sizeof(struct llk_ui_elem_image),
    .fn_init           = llk__ui_elem_image_init,
    .fn_fini           = NULL,
    .fn_set_attr       = llk__ui_elem_image_set_attr,
    .fn_after_layout   = NULL,
    .fn_hit_test       = NULL,
    .fn_dispatch_event = NULL,
    .fn_draw           = llk__ui_elem_image_draw,
};
