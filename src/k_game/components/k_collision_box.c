#include <stdint.h>
#include <stdio.h>
#include <limits.h>

#include "SDL.h"

#include "k_game_component.h"
#include "k_game_components/k_collision_box.h"

#include "./k_components_def.h"

#include "../game/k_game_context.h"
#include "../k_SDL/k_SDL.h"

/* region [component_data] */

union k_collision_box_config {
    enum k_collision_box_type box_type;
    struct k_collision_box_rectangle_config rectangle;
    struct k_collision_box_circle_config    circle;
};

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

union k_collision_box {
    uint8_t box_type;
    struct k_collision_box_rectangle rectangle;
    struct k_collision_box_circle    circle;
};
/* endregion */

/* region [collision_box_manager] */

struct k_collision_box_manager {

};

static struct k_collision_box_manager collision_box_manager;

/* endregion */

/* region [component_callback] */

static void collision_box_draw(struct k_component *component) {

    union k_collision_box *box = k_component_get_data(component);

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
    // if (NULL == manager)
    //     return -1;

    union k_collision_box_config *box_config = params;

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

/* region [define_component_type] */

int k__component_def_collision_box(void) {

    struct k_component_manager_config manager_config;
    /* TODO */

    struct k_component_entity_config component_config = K_COMPONENT_ENTITY_CONFIG_INIT;
    component_config.data_size = sizeof(union k_collision_box);
    component_config.fn_init   = collision_box_init;

    struct k_component_type *type = k_component_define(&manager_config, &component_config);
    if (NULL == type)
        return -1;

    k_component_type_set_name(type, "k/C-box");
    return 0;
}

/* endregion */

/* region [collision_check] */

void k_collision_check_rectangle(float x1, float y1, float x2, float y2) {

}

/* endregion */
