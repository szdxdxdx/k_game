
#define K_LOG_TAG "yx:object:num_particle"
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_z_index.h"
#include "object/num_particle/yx_obj_num_particle.h"
#include "utils/yx_math.h"

static void yx__obj_num_particle_on_step(struct k_object *object) {
    struct yx_obj_num_particle *particle = k_object_get_data(object);
    particle->x += particle->vx;
    particle->y += particle->vy;
}

static void yx__obj_num_particle_on_draw(struct k_object *object) {
    struct yx_obj_num_particle *particle = k_object_get_data(object);
    k_canvas_set_draw_color(particle->color);
    k_canvas_room_printf(NULL, particle->x, particle->y, "%d", particle->num);
}

static void yx__obj_num_particle_on_alarm_destroy(struct k_object *object, int timeout_diff) {
    k_object_destroy(object);
}

void yx_obj_num_particle_create(float x, float y, int num, uint32_t color) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_num_particle));
    if (NULL == object)
        goto err;

    struct yx_obj_num_particle *particle = k_object_get_data(object);
    particle->color = color;
    particle->num = num;
    particle->x = x;
    particle->y = y;
    particle->vx = yx_rand(0.5f, 1.0f);
    particle->vy = yx_rand(0.5f, 1.0f);

    if (NULL == k_object_add_step_callback(object, yx__obj_num_particle_on_step))
        goto err;
    if (NULL == k_object_add_draw_callback(object, yx__obj_num_particle_on_draw, YX_CONFIG_Z_GROUP_TEXT, 0))
        goto err;
    if (NULL == k_object_add_alarm_callback(object, yx__obj_num_particle_on_alarm_destroy, 3000))
        goto err;

    return;

err:
    k_log_error("failed to create object: num particle");
    k_object_destroy(object);
}
