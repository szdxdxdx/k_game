
#define K_LOG_TAG "yx:object:rival"
#include "k_log.h"

#include "k_game.h"

#include "sprite/yx_spr.h"

#include "object/fighter/rival/yx_obj_rival.h"

struct yx_obj_rival *yx_obj_rival_create(const struct yx_obj_rival_config *config) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_rival));

    struct yx_obj_rival *rival = k_object_get_data(object);
    rival->object = object;

    rival->x = config->x;
    rival->y = config->y;
    rival->vx_movement  = 0.0f;
    rival->vy_movement  = 0.0f;
    rival->vx_knockback = 0.0f;
    rival->vy_knockback = 0.0f;

    rival->spr_idle = yx_spr_llk_idle;
    rival->spr_run  = yx_spr_llk_run;

    if (0 != yx__obj_rival_on_create_add_movement(rival))
        goto err;
    if (0 != yx__obj_rival_on_create_add_collision(rival))
        goto err;
    if (0 != yx__obj_rival_on_create_add_draw(rival))
        goto err;

    return rival;

err:
    k_object_destroy(object);
    k_log_error("failed to create object: rival");
    return NULL;
}
