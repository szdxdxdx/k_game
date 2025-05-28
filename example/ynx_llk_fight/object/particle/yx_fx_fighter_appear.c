#include "k_game.h"

#include "config/yx_config_z_index.h"
#include "sprite/yx_spr.h"
#include "object/particle/yx_fx_fighter_appear.h"

static void yx__fx_fighter_appear_on_step(struct k_object *object) {
    struct yx_fx_fighter_appear *fx = k_object_get_data(object);

    float t1 = 0.5f;  /* 0 ~ t1 光标由大到小 */
    float t2 = 0.75f; /* t1 ~ t2 光标不动，超过 t2 后出现闪光特效 */
    float t3 = 1.0f;

    if (fx->timer < t1) {
        fx->timer += k_time_get_step_delta();
        if (fx->timer < t1) {
            float start_scale = 6.0f;
            float scale = start_scale - (start_scale - 1.0f) / t1 * fx->timer;
            k_sprite_renderer_scale(fx->spr_rdr, scale);
        } else {
            k_camera_shake(15.0f, 0.7f);
            k_sprite_renderer_scale(fx->spr_rdr, 1.0f);
        }
    }
    else if (fx->timer < t2) {
        fx->timer += k_time_get_step_delta();
        if (t2 < fx->timer) {
            k_sprite_renderer_set_speed(fx->spr_rdr, 1.0f);
            k_sprite_renderer_set_loop_count(fx->spr_rdr, 1);
            k_sprite_renderer_set_loop_callback(fx->spr_rdr, k_object_destroy);
        }
    }
    else {
        fx->timer += k_time_get_step_delta();
        if (t3 < fx->timer) {
            k_callback_del(fx->cb_step);
            fx->cb_step = NULL;

            if (fx->fn_callback != NULL) {
                fx->fn_callback(fx->data);
            }
        }
    }
}

struct yx_fx_fighter_appear *yx_fx_fighter_appear_create(float x, float y, void (*fn_callback)(struct k_object *data), struct k_object *data) {

    struct k_object *object = k_object_create(sizeof(struct yx_fx_fighter_appear));
    if (NULL == object)
        goto err;

    struct yx_fx_fighter_appear *fx = k_object_get_data(object);
    fx->object = object;
    fx->x = x;
    fx->y = y;
    fx->timer = 0.0f;
    fx->fn_callback = fn_callback;
    fx->data = data;

    {
        fx->cb_step = k_object_add_step_callback(object, yx__fx_fighter_appear_on_step);
        if (NULL == fx->cb_step)
            goto err;
    }

    {
        struct k_sprite_renderer_config spr_rdr_config;
        spr_rdr_config.x = &fx->x;
        spr_rdr_config.y = &fx->y;
        spr_rdr_config.sprite = yx_spr_fx_fighter_appear;
        spr_rdr_config.z_group = YX_CONFIG_Z_GROUP_FX;
        spr_rdr_config.z_layer = YX_CONFIG_Z_LAYER_FX;
        fx->spr_rdr = k_object_add_sprite_renderer(object, &spr_rdr_config);
        if (NULL == fx->spr_rdr)
            goto err;

        k_sprite_renderer_set_speed(fx->spr_rdr, 0.0f);
    }

    return fx;

err:
    k_object_destroy(object);
    return NULL;
}
