#include <stdlib.h>
#include "k_utils.h"
#include "k_json.h"

#include "./_internal.h"

struct k_sprite *yx_spr_ynx_idle = NULL;
struct k_sprite *yx_spr_ynx_run  = NULL;

static struct k_sprite *load_sprite_strip(const char *filepath, int frames_num, int delay) {

    int scale = 2;

    struct k_image *img = k_image_load(filepath);
    int scaled_image_w = scale * k_image_get_width(img);
    int scaled_image_h = scale * k_image_get_height(img);
    struct k_image *img_player = k_image_scale(img, scaled_image_w, scaled_image_h);
    k_image_release(img);

    int sprite_w = k_image_get_width(img_player) / frames_num;
    int sprite_h = k_image_get_height(img_player);

    float origin_x = (float)scale * 15;
    float origin_y = (float)scale * 23;

    /* 素材中的动画都不超过 15 帧，此处不需要动态申请内存 */
    struct k_sprite_frame_config frames_config[] = {
            { img_player,  0 * sprite_w, 0, delay },
            { img_player,  1 * sprite_w, 0, delay },
            { img_player,  2 * sprite_w, 0, delay },
            { img_player,  3 * sprite_w, 0, delay },
            { img_player,  4 * sprite_w, 0, delay },
            { img_player,  5 * sprite_w, 0, delay },
            { img_player,  6 * sprite_w, 0, delay },
            { img_player,  7 * sprite_w, 0, delay },
            { img_player,  8 * sprite_w, 0, delay },
            { img_player,  9 * sprite_w, 0, delay },
            { img_player, 10 * sprite_w, 0, delay },
            { img_player, 11 * sprite_w, 0, delay },
            { img_player, 12 * sprite_w, 0, delay },
            { img_player, 13 * sprite_w, 0, delay },
            { img_player, 14 * sprite_w, 0, delay },
            { img_player, 15 * sprite_w, 0, delay },
    };

    struct k_sprite_config config = {
        .sprite_w   = sprite_w,
        .sprite_h   = sprite_h,
        .origin_x   = origin_x,
        .origin_y   = origin_y,
        .frames     = frames_config,
        .frames_num = frames_num
    };

    return k_sprite_create(&config);
}

int yx_load_spr_ynx_(void) {

    yx_spr_ynx_idle = load_sprite_strip("./demo_1/sprite/ynx/idle.png", 2, 64 * 3);
    yx_spr_ynx_run  = load_sprite_strip("./demo_1/sprite/ynx/run.png",  6, 80);
    return 0;
}

int yx_load_spr_ynx(void) {

    char buf_[64];
    char *buf = k_read_txt_file("./demo_1/sprite/ynx/ynx.json", buf_, sizeof(buf_), NULL);
    if (NULL == buf)
        return -1;

    struct k_json *json = k_json_parse(buf);

    struct k_json *frames = k_json_obj_get(json, "frames");

    {
        struct k_json *meta = k_json_obj_get(json, "meta");
        struct k_json *frame_tags = k_json_obj_get(meta, "frameTags");

        size_t tags_num = k_json_arr_get_size(frame_tags);
        size_t i = 0;
        for (; i < tags_num; i++) {
            struct k_json *tag = k_json_arr_get(frame_tags, i);

            struct k_json *name = k_json_obj_get(tag, "name");
            const char *name_s = k_json_str_get(name);

            printf("%s\n", name_s);
        }
    }

    if (buf != buf_)
        free(buf);

    return 0;
}
