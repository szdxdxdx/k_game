#include <stdlib.h>
#include <string.h>

#include "k_read_file.h"

#include "./yx_sprite_sheet.h"

int yx_sprite_sheet_init(struct yx_sprite_sheet *sheet, const char *image_filepath, float scale, const char *config_filepath) {

    if (scale <= 0)
        return -1;

    struct k_image *image = k_image_load(image_filepath);
    if (NULL == image)
        return -1;

    if (1.0f != scale) {
        float scaled_w = (float)k_image_get_width(image) * scale;
        float scaled_h = (float)k_image_get_height(image) * scale;
        struct k_image *scaled_image = k_image_scale(image, (int)scaled_w, (int)scaled_h);
        if (NULL == scaled_image) {
            k_image_release(image);
            return -1;
        }

        image = scaled_image;
    }

    char *buf = k_read_txt_file(config_filepath, NULL, 0, NULL);
    if (NULL == buf) {
        k_image_release(image);
        return -1;
    }

    struct k_json *config = k_json_parse(buf);
    if (NULL == config) {
        k_image_release(image);
        free(buf);
        return -1;
    }

    sheet->scale = scale;
    sheet->image = image;
    sheet->config = config;
    return 0;
}

void yx_sprite_sheet_fini(struct yx_sprite_sheet *sheet) {
    k_json_free(sheet->config);
}

struct k_sprite *yx_sprite_load_from_sheet(struct yx_sprite_sheet *sheet, const char *tag_name, float origin_x, float origin_y) {

    struct k_sprite_frame_config *frame_config = NULL;

    struct k_json *j_frame_tags = k_json_obj_get(k_json_obj_get(sheet->config, "meta"), "frameTags");

    /* 查找 tag */
    struct k_json *j_tag = NULL;
    size_t tags_num = k_json_arr_get_size(j_frame_tags);
    size_t i = 0;
    for (; i < tags_num; i++) {
        j_tag = k_json_arr_get(j_frame_tags, i);
        const char *name = k_json_str_get(k_json_obj_get(j_tag, "name"));
        if (0 == strcmp(name, tag_name))
            break;
    }

    /* 没有找到 tag */
    if (tags_num == i || NULL == j_tag)
        goto err;

    /* 获取各帧信息在精灵表中的位置 */
    int from = k_json_num_get_i(k_json_obj_get(j_tag, "from"));
    int to   = k_json_num_get_i(k_json_obj_get(j_tag, "to"));

    /* 获取精灵的帧数 */
    struct k_json *j_frames = k_json_obj_get(sheet->config, "frames");
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
