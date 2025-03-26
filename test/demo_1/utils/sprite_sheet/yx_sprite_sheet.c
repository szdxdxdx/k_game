#include <stdlib.h>
#include <string.h>

#include "k_read_file.h"
#include "k_json.h"

#include "./yx_sprite_sheet.h"

struct yx_sprite_sheet {
    struct k_image *image;
    float scale;
    struct k_json *j_config;
};

static struct k_sprite *sprite_sheet_extract(struct yx_sprite_sheet *sheet, const char *tag, float origin_x, float origin_y) {

    struct k_sprite_frame_config *frame_config = NULL;

    struct k_json *j_frame_tags = k_json_obj_get(k_json_obj_get(sheet->j_config, "meta"), "frameTags");

    /* 查找 tag */
    struct k_json *j_tag = NULL;
    size_t tags_num = k_json_arr_get_size(j_frame_tags);
    size_t i = 0;
    for (; i < tags_num; i++) {
        j_tag = k_json_arr_get(j_frame_tags, i);
        const char *name = k_json_str_get(k_json_obj_get(j_tag, "name"));
        if (0 == strcmp(name, tag))
            break;
    }

    /* 没有找到 tag */
    if (tags_num == i || NULL == j_tag)
        goto err;

    /* 获取各帧信息在精灵表中的位置 */
    int from = k_json_num_get_i(k_json_obj_get(j_tag, "from"));
    int to   = k_json_num_get_i(k_json_obj_get(j_tag, "to"));

    /* 获取精灵的帧数 */
    struct k_json *j_frames = k_json_obj_get(sheet->j_config, "frames");
    int frames_num = to - from + 1;
    int frame_idx = 0;

    /* 分配内存用于填写精灵帧信息 */
    frame_config = malloc(frames_num * sizeof(struct k_sprite_frame_config));
    if (NULL == frame_config)
        return NULL;

    /* 读取第 1 帧，确定精灵的宽高 */
    struct k_json *j_frame = k_json_obj_get(k_json_arr_get(j_frames, from), "frame");
    int w = k_json_num_get_i(k_json_obj_get(j_frame, "w"));
    int h = k_json_num_get_i(k_json_obj_get(j_frame, "h"));

    float scale = sheet->scale;

    /* 填写精灵信息 */
    struct k_sprite_config spr_config;
    spr_config.sprite_w   = (int)((float)w * scale);
    spr_config.sprite_h   = (int)((float)h * scale);
    spr_config.origin_x   = origin_x * scale;
    spr_config.origin_y   = origin_y * scale;
    spr_config.frames     = frame_config;
    spr_config.frames_num = frames_num;

    /* 读取各帧 */
    for (; from + frame_idx <= to; frame_idx++) {
        struct k_json *j_frame_i = k_json_arr_get(j_frames, from + frame_idx);
        j_frame = k_json_obj_get(j_frame_i, "frame");

        int x     = k_json_num_get_i(k_json_obj_get(j_frame, "x"));
        int y     = k_json_num_get_i(k_json_obj_get(j_frame, "y"));
        int delay = k_json_num_get_i(k_json_obj_get(j_frame_i, "duration"));

        frame_config[frame_idx].image    = sheet->image;
        frame_config[frame_idx].offset_x = (int)((float)x * scale);
        frame_config[frame_idx].offset_y = (int)((float)y * scale);
        frame_config[frame_idx].delay    = delay;
    }

    struct k_sprite *sprite = k_sprite_create(&spr_config);

    free(frame_config);
    return sprite;

err:
    free(frame_config);
    return NULL;
}

static int check_config(const struct yx_sprite_sheet_config *config) {

    if (NULL == config)
        return -1;
    if (NULL == config->image_filepath || '\0' == config->image_filepath[0])
        return -1;
    if (NULL == config->config_filepath || '\0' == config->config_filepath[0])
        return -1;
    if (config->scale <= 0.0f)
        return -1;
    if (NULL == config->sprites)
        return -1;

    return 0;
}

static int sprite_sheet_load(struct yx_sprite_sheet *sheet, const struct yx_sprite_sheet_config *config) {

    struct k_image *image = NULL;
    char *buf = NULL;
    struct k_json *j_config = NULL;

    image = k_image_load(config->image_filepath);
    if (NULL == image)
        goto err;

    if (1.0f != config->scale) {
        int scaled_w = (int)(config->scale * (float)k_image_get_width(image));
        int scaled_h = (int)(config->scale * (float)k_image_get_height(image));
        struct k_image *scaled_image = k_image_scale(image, scaled_w, scaled_h);
        if (NULL == scaled_image) {
            goto err;
        } else {
            k_image_release(image);
            image = scaled_image;
        }
    }

    buf = k_read_txt_file(config->config_filepath, NULL, 0, NULL);
    if (NULL == buf)
        goto err;

    j_config = k_json_parse(buf);
    if (NULL == j_config)
        goto err;

    sheet->image    = image;
    sheet->scale    = config->scale;
    sheet->j_config = j_config;

    return 0;

err:
    k_image_release(image);
    free(buf);
    k_json_free(j_config);

    return -1;
}

static int sprite_sheet_extract_all(struct yx_sprite_sheet *sheet, const struct yx_sprite_sheet_config *config) {

    struct yx_sprite_sheet_sprite_config *spr_config = config->sprites;
    while (1) {

        if (NULL == spr_config->get_sprite)
            break;

        const char *tag = spr_config->tag;
        float origin_x  = spr_config->origin_x;
        float origin_y  = spr_config->origin_y;
        struct k_sprite *sprite = sprite_sheet_extract(sheet, tag, origin_x, origin_y);

        if (NULL == sprite)
            goto err;

        *spr_config->get_sprite = sprite;
        spr_config++;
    }

    return 0;

err:
    spr_config--;
    while (1) {
        k_sprite_destroy(*(spr_config->get_sprite));
        *(spr_config->get_sprite) = NULL;

        if (spr_config == config->sprites)
            break;

        spr_config--;
    }

    return -1;
}

int yx_sprite_load_from_sheet(const struct yx_sprite_sheet_config *config) {

    if (0 != check_config(config))
        return -1;

    struct yx_sprite_sheet sheet;
    if (0 != sprite_sheet_load(&sheet, config))
        return -1;

    if (0 != sprite_sheet_extract_all(&sheet, config)) {
        k_image_release(sheet.image);
        k_json_free(sheet.j_config);
        return -1;
    }
    else {
        k_json_free(sheet.j_config);
        return 0;
    }
}
