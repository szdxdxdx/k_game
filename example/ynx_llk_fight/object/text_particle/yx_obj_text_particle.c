
#define K_LOG_TAG "yx:object:text_particle"

#include <stdarg.h>
#include "k_log.h"

#include "k_printf.h"

#include "k_game.h"

#include "config/yx_config_z_index.h"
#include "object/text_particle/yx_obj_text_particle.h"
#include "utils/yx_math.h"

static void yx__obj_text_particle_on_step(struct k_object *object) {
    struct yx_obj_text_particle *particle = k_object_get_data(object);
    float dt = k_time_get_step_delta();
    particle->x += particle->vx * dt;
    particle->y += particle->vy * dt;
}

static void yx__obj_text_particle_on_draw(struct k_object *object) {
    struct yx_obj_text_particle *particle = k_object_get_data(object);
    k_canvas_set_draw_color(particle->color);
    k_canvas_room_printf(NULL, particle->x, particle->y, "%s", particle->text);
}

static void yx__obj_text_particle_on_alarm_destroy(struct k_object *object, int timeout_diff) {
    struct yx_obj_text_particle *particle = k_object_get_data(object);
    free(particle->text);
    k_object_destroy(object);
}

void yx_obj_text_particle_create(const struct yx_obj_text_particle_config *config, const char *fmt, ...) {

    char *text_copy = NULL;
    struct k_object *object = NULL;

    va_list args;
    va_start(args, fmt);
    int r = k_vasprintf(NULL, &text_copy, fmt, args);
    va_end(args);

    if (r <= 0)
        goto err;

    object = k_object_create(sizeof(struct yx_obj_text_particle));
    if (NULL == object)
        goto err;

    struct yx_obj_text_particle *particle = k_object_get_data(object);
    particle->color = config->color;
    particle->text = text_copy;
    particle->x = config->x;
    particle->y = config->y;
    particle->vx = config->vx;
    particle->vy = config->vy;

    if (NULL == k_object_add_step_callback(object, yx__obj_text_particle_on_step))
        goto err;
    if (NULL == k_object_add_draw_callback(object, yx__obj_text_particle_on_draw, YX_CONFIG_Z_GROUP_TEXT, 0))
        goto err;
    if (NULL == k_object_add_alarm_callback(object, yx__obj_text_particle_on_alarm_destroy, 1200))
        goto err;

    return;

err:
    k_log_error("failed to create object: text particle");
    k_object_destroy(object);
    free(text_copy);
}
