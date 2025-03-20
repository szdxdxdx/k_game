#include "./k_collision.h"

#include "../../core/k_SDL/k_SDL.h"

void k__collision_debug_draw(struct k_component *component) {

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
