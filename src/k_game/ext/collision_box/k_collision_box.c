#include <limits.h>

#include "k_list.h"
#include "k_int_map.h"

#include "k_game.h"

#include "../../core/k_SDL/k_SDL.h"

#include "../k_components_def.h"

/* region [struct def] */

/* region [collision_box] */

enum {
    K_COLLISION_BOX_RECTANGLE,
    K_COLLISION_BOX_CIRCLE,
};

union k_collision_box_config {
    uint8_t box_type;
    struct k_collision_rectangle_config rectangle;
    struct k_collision_circle_config    circle;
};

struct k_collision_rectangle {
    uint8_t box_type;

    float *x;
    float *y;
    float offset_x1;
    float offset_y1;
    float offset_x2;
    float offset_y2;
};

struct k_collision_circle {
    uint8_t box_type;

    float *x;
    float *y;
    float offset_x;
    float offset_y;
    float r;
};

struct k_collision_box {

    struct k_list_node list_node;

    struct k_component *component;

    union {
        uint8_t box_type;
        struct k_collision_rectangle rectangle;
        struct k_collision_circle    circle;
    };
};

/* endregion */

/* region [collision_manager] */

struct k_collision_group {
    struct k_int_map_node map_node;

    struct k_list box_list;
};

struct k_collision_manager {

    struct k_int_map group_map;
};

/* endregion */

/* endregion */

/* region [collision_manager] */

int collision_manager_add(struct k_collision_manager *manager, struct k_collision_box *box, int group_idx) {

    struct k_collision_group *group;

    struct k_int_map_node *map_node = k_int_map_get(&manager->group_map, group_idx);
    if (NULL != map_node) {
        group = container_of(map_node, struct k_collision_group, map_node);
    }
    else {
        group = k_malloc(sizeof(struct k_collision_group));
        if (NULL == group)
            return -1;

        k_int_map_add_directly(&manager->group_map, group_idx, &group->map_node);

        k_list_init(&group->box_list);
    }

    k_list_add_tail(&group->box_list, &box->list_node);
    return 0;
}

/* endregion */

/* region [debug] */

static void collision_debug_draw(struct k_component *component) {

    struct k_collision_box *box = k_component_get_data(component);

    if (K_COLLISION_BOX_RECTANGLE == box->box_type) {
        struct k_collision_rectangle *rect = &box->rectangle;

        float x1 = *rect->x + rect->offset_x1;
        float y1 = *rect->y + rect->offset_y1;
        float x2 = *rect->x + rect->offset_x2;
        float y2 = *rect->y + rect->offset_y2;

        SDL_FRect draw_rect = {
            .x = x1 < x2 ? x1 : x2,
            .y = y1 < y2 ? y1 : y2,
            .w = x1 < x2 ? x2 - x1 : x1 - x2,
            .h = y1 < y2 ? y2 - y1 : y1 - y2,
        };
        SDL_SetRenderDrawColor(k__window.renderer, 255, 0, 0, 255);
        SDL_RenderDrawRectF(k__window.renderer, &draw_rect);
    }

    else if (K_COLLISION_BOX_CIRCLE == box->box_type) {
        struct k_collision_circle *circle = &box->circle;

        float center_x = *circle->x + circle->offset_x;
        float center_y = *circle->y + circle->offset_y;

        SDL_SetRenderDrawColor(k__window.renderer, 255, 0, 0, 255);
        /* SDL 没有绘制圆的 API，下述代码由 AI 给出
         * 【中点圆算法】通过模拟圆的对称性，只计算 1/8 的圆弧，然后通过对称性绘制整个圆
         */
        {
            int x = (int)circle->r;
            int y = 0;
            int err = 0; /* <- 表示当前点到理想圆路径的误差 */
            int centerX = (int)center_x;
            int centerY = (int)center_y;
            while (x >= y) {
                SDL_RenderDrawPoint(k__window.renderer, centerX + x, centerY + y);
                SDL_RenderDrawPoint(k__window.renderer, centerX + y, centerY + x);
                SDL_RenderDrawPoint(k__window.renderer, centerX - y, centerY + x);
                SDL_RenderDrawPoint(k__window.renderer, centerX - x, centerY + y);
                SDL_RenderDrawPoint(k__window.renderer, centerX - x, centerY - y);
                SDL_RenderDrawPoint(k__window.renderer, centerX - y, centerY - x);
                SDL_RenderDrawPoint(k__window.renderer, centerX + y, centerY - x);
                SDL_RenderDrawPoint(k__window.renderer, centerX + x, centerY - y);
                if (err <= 0) {
                    y += 1;
                    err += 2 * y + 1;
                } else {
                    x -= 1;
                    err -= 2 * x + 1;
                }
            }
        }
    }
}

/* endregion */

/* region [collision_box_init] */

static int collision_box_init(struct k_component *component, void *params) {

    struct k_collision_manager *manager = k_component_get_manager_data(component);
    if (NULL == manager)
        return -1;

    struct k_collision_box *box = k_component_get_data(component);
    union k_collision_box_config *box_config = params;

    switch (box_config->box_type) {
        case K_COLLISION_BOX_RECTANGLE: {
            struct k_collision_rectangle_config *config = &box_config->rectangle;
            struct k_collision_rectangle *rect = &box->rectangle;

            rect->box_type  = config->box_type;
            rect->x         = config->x;
            rect->y         = config->y;
            rect->offset_x1 = config->offset_x1;
            rect->offset_y1 = config->offset_y1;
            rect->offset_x2 = config->offset_x2;
            rect->offset_y2 = config->offset_y2;
            break;
        }

        case K_COLLISION_BOX_CIRCLE: {
            struct k_collision_circle_config *config = &box_config->circle;
            struct k_collision_circle *circle = &box->circle;

            circle->box_type = config->box_type;
            circle->x        = config->x;
            circle->y        = config->y;
            circle->offset_x = config->offset_x;
            circle->offset_y = config->offset_y;
            circle->r        = config->r;
            break;
        }

        default: return -1;
    }

    int tmp_group = 3; /* <- tmp */
    if (0 != collision_manager_add(manager, box, tmp_group))
        return -1;

    k_component_add_draw_callback(component, collision_debug_draw, INT_MAX - 2);
    return 0;
}

/* endregion */

/* region [collision_manager_init] */

static int collision_manager_init(struct k_component_manager *component_manager, void *params) {
    struct k_collision_manager *manager = k_component_manager_get_data(component_manager);

    size_t buckets_num = 32;
    struct k_hash_list *buckets = k_malloc(sizeof(struct k_hash_list) * buckets_num);
    if (buckets == NULL)
        return -1;

    k_int_map_init(&manager->group_map, buckets, buckets_num);
    return 0;
}

/* endregion */

/* region [component_define] */

static struct k_component_type *c_box;

int k__component_def_collision_box(void) {

    struct k_component_manager_config manager_config = K_COMPONENT_MANAGER_CONFIG_INIT;
    manager_config.data_size = sizeof(struct k_collision_manager);
    manager_config.fn_init   = collision_manager_init;

    struct k_component_entity_config entity_config = K_COMPONENT_ENTITY_CONFIG_INIT;
    entity_config.data_size = sizeof(struct k_collision_box);
    entity_config.fn_init   = collision_box_init;

    c_box = k_component_define(&manager_config, &entity_config);
    if (NULL == c_box)
        return -1;

    return 0;
}

/* endregion */

/* region [object_add_collision_box] */

struct k_collision_box *k_object_add_collision_rectangle(struct k_object *object, struct k_collision_rectangle_config *config) {

    if (NULL == object || NULL == config)
        return NULL;

    config->box_type = K_COLLISION_BOX_RECTANGLE;

    struct k_component *component = k_object_add_component(object, c_box, config);
    if (NULL == component)
        return NULL;

    return k_component_get_data(component);
}

void k_object_del_collision_box(struct k_collision_box *box) {

    if (NULL != box)
        k_object_del_component(box->component);
}

/* endregion */

/* region [room_add_collision_manager] */

int k_room_add_collision_manager(struct k_room *room) {
    return k_room_add_component_manager(room, c_box, NULL);;
}

/* endregion */

/* region [collision_check] */

struct k_collision_box *k_collision_check_rectangle(int group_idx, float x1, float y1, float x2, float y2) {

    struct k_room *room = k_get_current_room();
    struct k_component_manager *component_manager = k_room_get_component_manager(room, c_box);
    if (NULL == component_manager)
        return NULL;

    struct k_collision_manager *manager = k_component_manager_get_data(component_manager);
    struct k_int_map_node *map_node = k_int_map_get(&manager->group_map, group_idx);
    if (NULL == map_node)
        return NULL;

    struct k_collision_group *group = container_of(map_node, struct k_collision_group, map_node);

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, list_node);

        switch (box->box_type) {
            case K_COLLISION_BOX_RECTANGLE: {
                struct k_collision_rectangle *rect = &box->rectangle;

                float x3 = *(rect->x) + rect->offset_x1;
                float y3 = *(rect->y) + rect->offset_y1;
                float x4 = *(rect->x) + rect->offset_x2;
                float y4 = *(rect->y) + rect->offset_y2;
                if (   ((x1 < x4) != (x2 < x3) && (y1 < y4) != (y2 < y3))
                    || ((x1 < x3) != (x2 < x4) && (y1 < y3) != (y2 < y4))
                ) {
                    return box;
                }

                break;
            }
            case K_COLLISION_BOX_CIRCLE: {
                break;
            }
        }
    }

    return NULL;
}

struct k_collision_box *k_collision_check_circle(int group_idx, float x, float y, float r) {

    return NULL;
}

/* endregion */
