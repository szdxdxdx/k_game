#include <limits.h>

#include "k_game.h"

#include "../../core/k_SDL/k_SDL.h"

#include "../k_components_def.h"

struct k_collision_box_rectangle {
    uint8_t box_type;

    float *x;
    float *y;
    float offset_x1;
    float offset_y1;
    float offset_x2;
    float offset_y2;
};

struct k_collision_box_circle {
    uint8_t box_type;

    float *x;
    float *y;
    float offset_x;
    float offset_y;
    float r;
};

struct k_collision_box {
    uint8_t box_type;
    union {
        struct k_collision_box_rectangle rectangle;
        struct k_collision_box_circle    circle;
    };
};

struct k_collision_box_manager {

};

/* region [collision_check] */

struct k_collision_box *k_collision_check_rectangle(int layer, float x1, float y1, float x2, float y2) {

    return NULL;
}

struct k_collision_box *k_collision_check_circle(int layer, float x, float y, float r) {

    return NULL;
}

/* endregion */

/* region [collision_box_init] */

static void collision_box_draw(struct k_component *component) {

    struct k_collision_box *box = k_component_get_data(component);

    if (K_COLLISION_BOX_RECTANGLE == box->box_type) {
        struct k_collision_box_rectangle *rect = &box->rectangle;

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
        struct k_collision_box_circle *circle = &box->circle;

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

static int collision_box_init(struct k_component *component, void *params) {

    struct k_component_manager *manager = k_component_get_manager(component);
    if (NULL == manager)
        return -1;

    struct k_collision_box_config *box_config = params;

    switch (box_config->box_type) {
        case K_COLLISION_BOX_RECTANGLE: {
            struct k_collision_box_rectangle_config *config = &box_config->rectangle;
            struct k_collision_box_rectangle *rect = k_component_get_data(component);

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
            struct k_collision_box_circle_config *config = &box_config->circle;
            struct k_collision_box_circle *circle = k_component_get_data(component);

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

    k_component_add_draw_callback(component, collision_box_draw, INT_MAX - 2);
    return 0;
}

/* endregion */

/* region [collision_manager_init] */

static int collision_box_manager_init(struct k_component_manager *manager, void *params) {
    struct k_collision_box_manager *data = k_component_manager_get_data(manager);

    return 0;
}

/* endregion */

/* region [component_define] */

static struct k_component_type *c_box;

int k__component_def_collision_box(void) {

    struct k_component_manager_config manager_config = K_COMPONENT_MANAGER_CONFIG_INIT;
    manager_config.data_size = sizeof(struct k_collision_box_manager);
    manager_config.fn_init   = collision_box_manager_init;

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

struct k_collision_box *k_object_add_collision_box(struct k_object *object, struct k_collision_box_config *config) {

    return NULL;
}

void k_object_del_collision_box(struct k_collision_box *box) {

}

/* endregion */

/* region [room_add_collision_box_manager] */

int k_room_add_collision_box_manager(struct k_room *room) {
    return k_room_add_component_manager(room, c_box, NULL);;
}

/* endregion */
