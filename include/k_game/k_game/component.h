#ifndef K_GAME_COMPONENT_H
#define K_GAME_COMPONENT_H

#include <stddef.h>

struct k_object;

/**
 * \brief 组件类型
 *
 * TODO docs
 */
struct k_component_type;

/**
 * \brief 组件
 *
 * TODO docs
 */
struct k_component;

/* region [define_component_type] */

struct k_component_type_config {

    const char *type_name;

    size_t data_size;

    int (*fn_create)(struct k_component *component, void *params);

    void (*fn_destroy)(struct k_component *component);
};

struct k_component_type *k_define_component_type(const struct k_component_type_config *config);

/* endregion */

/* region [component_get] */

const char *k_component_type_get_name(struct k_component_type *component_type);

void *k_component_get_data(struct k_component *component);

struct k_component_type *k_get_component_type_by_name(const char *type_name);

/* endregion */

/**
 * \brief 组件回调
 *
 * TODO docs
 */
struct k_component_callback;

/* region [component_callback] */

struct k_component_callback *k_component_add_alarm_callback(struct k_component *component, void (*fn_callback)(struct k_component *component, int timeout_diff), int delay_ms);

struct k_component_callback *k_component_add_step_callback(struct k_component *component, void (*fn_callback)(struct k_component *component));

struct k_component_callback *k_component_add_draw_callback(struct k_component *component, void (*fn_callback)(struct k_component *component), int z_index);

void k_component_del_callback(struct k_component_callback *callback);

/* endregion */

/* region [object_add_component] */

struct k_component *k_object_add_component(struct k_object *object, struct k_component_type *component_type, void *params);

void k_object_del_component(struct k_component *component);

struct k_component *k_object_get_component(struct k_object *object, struct k_component_type *component_type);

struct k_object *k_component_get_object(struct k_component *component);

// int k_get_objects_with_component(size_t component_type_id, struct k_object ***get_objects, size_t *n);

/* endregion */

#endif
