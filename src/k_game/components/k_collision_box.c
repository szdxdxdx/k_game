#include <stdint.h>
#include <stdio.h>
#include <limits.h>

#include "SDL.h"

#include "k_game_component.h"
#include "k_game_components/k_collision_box.h"

#include "./k_components_def.h"

#include "../game/k_game_context.h"
#include "../k_SDL/k_SDL.h"

/* region [box_type_def] */

struct k_collision_box_rectangle {
    uint8_t box_type;

    float *x, *y;
    float offset_x1, offset_y1;
    float offset_x2, offset_y2;
};

struct k_collision_box_circle {
    uint8_t box_type;

    float *x, *y;
    float offset_x, offset_y;
    float r;
};

union k_collision_box {
    uint8_t box_type;
    struct k_collision_box_rectangle rectangle;
    struct k_collision_box_circle    circle;
};

union k_collision_box_config {
    enum k_collision_box_type box_type;
    struct k_collision_box_rectangle_config rectangle;
    struct k_collision_box_circle_config    circle;
};

/* endregion */

void box_debug_draw(struct k_component *component) {

    union k_collision_box *box = k_component_get_data(component);

    if (K_COLLISION_BOX_RECTANGLE == box->box_type) {
        struct k_collision_box_rectangle *rect = &box->rectangle;

        float x1 = *rect->x + rect->offset_x1;
        float y1 = *rect->y + rect->offset_y1;
        float x2 = *rect->x + rect->offset_x2;
        float y2 = *rect->y + rect->offset_y2;

        SDL_FRect draw_rect = {
            .x = (x1 < x2 ? x1 : x2),
            .y = (y1 < y2 ? y1 : y2),
            .w = (x2 > x1 ? x2 - x1 : x1 - x2),
            .h = (y2 > y1 ? y2 - y1 : y1 - y2),
        };
        SDL_SetRenderDrawColor(k__window.renderer, 255, 0, 0, 255);
        SDL_RenderDrawRectF(k__window.renderer, &draw_rect);
    }

    else if (K_COLLISION_BOX_CIRCLE == box->box_type) {
        struct k_collision_box_circle *circle = &box->circle;

        float center_x = *circle->x + circle->offset_x;
        float center_y = *circle->y + circle->offset_y;

        SDL_SetRenderDrawColor(k__window.renderer, 255, 0, 0, 255);
        // SDL_RenderDrawCircleF(k__window.renderer, x, y, circle->r);
        int x = (int)circle->r;
        int y = 0;
        int err = 0;
        int centerX = (int)center_x;
        int centerY = (int)center_y;

        while (x >= y) {
            // 绘制圆的 8 个对称点
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
            }
            if (err > 0) {
                x -= 1;
                err -= 2 * x + 1;
            }
        }
    }
}

int box_init(struct k_component *component, void *params) {
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

    k_component_add_draw_callback(component, box_debug_draw, INT_MAX - 2);
    return 0;
}

int k__component_def_collision_box(void) {

    struct k_component_type_config config = K_COMPONENT_TYPE_CONFIG_INIT;
    config.type_name = "k/C-box";
    config.data_size = sizeof(union k_collision_box);
    config.fn_init   = box_init;

    return NULL != k_component_define(&config) ? 0 : -1;
}
