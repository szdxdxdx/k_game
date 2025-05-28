
#define K_LOG_TAG "yx:object:ui_banner"
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_z_index.h"
#include "sprite/yx_spr.h"
#include "object/particle/yx_ui_banner.h"
#include "sound/yx_sound.h"

struct yx_ui_banner {
    struct k_sprite *spr_banner;
    float timer;
    float step_t0;
    float step_t1;
    float step_t2;
    float scale_0;
    float scale_1;
};

static void yx_ui_banner_on_draw(struct k_object *object) {
    struct yx_ui_banner *banner = k_object_get_data(object);

    float dt = k_time_get_step_delta();

    float scale;
    if (banner->timer < banner->step_t0) {
        banner->timer += dt;
        return;
    }
    else if (banner->timer < banner->step_t1) {
        banner->timer += dt;
        if (banner->timer < banner->step_t1) {
            scale = banner->scale_0 - (banner->scale_0 - banner->scale_1) / banner->step_t1 * banner->timer;
        } else {
            scale = banner->scale_1;
            k_camera_shake(20.0f, 0.85f);

            k_sound_sfx_play(yx_sfx_banner);
        }
    } else {
        banner->timer += dt;
        if (banner->step_t2 < banner->timer) {
            k_object_destroy(object);
            return;
        }
        scale = banner->scale_1;
    }

    float x = k_window_get_w() / 2;
    float y = k_window_get_h() / 2;
    struct k_canvas_draw_sprite_options options;
    options.angle = 0.0f;
    options.flip_x = 0;
    options.flip_y = 0;
    options.scaled_w = scale * (float)k_sprite_get_w(banner->spr_banner);
    options.scaled_h = scale * (float)k_sprite_get_h(banner->spr_banner);
    k_canvas_ui_draw_sprite(banner->spr_banner, 0, x, y, &options);
}

static struct yx_ui_banner *yx_ui_banner_create(struct k_sprite *spr_banner) {

    struct k_object *object = k_object_create(sizeof(struct yx_ui_banner));
    if (NULL == object)
        goto err;

    struct yx_ui_banner *banner = k_object_get_data(object);
    banner->spr_banner = spr_banner;
    banner->timer = 0.0f;
    banner->step_t0 = 1.4f;
    banner->step_t1 = 0.3f + banner->step_t0;
    banner->step_t2 = 3.5f + banner->step_t1;
    banner->scale_0 = 50.0f;
    banner->scale_1 = 2.8f;

    struct k_callback *callback = k_object_add_draw_callback(object, yx_ui_banner_on_draw, YX_CONFIG_Z_GROUP_FX + 1, YX_CONFIG_Z_LAYER_FX);
    if (NULL == callback)
        goto err;

    return banner;

err:
    k_log_error("failed to create object yx_ui_banner");
    k_object_destroy(object);
    return NULL;
}

void yx_ui_banner_show_YOU_WIN(void) {
    yx_ui_banner_create(yx_spr_banner_you_win);
}

void yx_ui_banner_show_YOU_DIED(void) {
    yx_ui_banner_create(yx_spr_banner_you_died);
}
