#include "./_internal.h"

#include "../../core/k_SDL/_public.h"

/* region [debug_draw] */

static void draw_point(float x, float y) {
    float r = 3.0f;
    SDL_RenderDrawLineF(k__window.renderer, x - r, y, x + r, y);
    SDL_RenderDrawLineF(k__window.renderer, x, y - r, x, y + r);
}

static void draw_line(float x1, float y1, float x2, float y2) {
    SDL_RenderDrawLineF(k__window.renderer, x1, y1, x2, y2);
}

static void draw_rect(float x1, float y1, float x2, float y2) {
    SDL_FRect draw_rect = {
        .x = x1 < x2 ? x1 : x2,
        .y = y1 < y2 ? y1 : y2,
        .w = x1 < x2 ? x2 - x1 : x1 - x2,
        .h = y1 < y2 ? y2 - y1 : y1 - y2,
    };
    SDL_RenderDrawRectF(k__window.renderer, &draw_rect);
}

static void draw_circle(float cx, float cy, float r) {

    /* SDL 没有绘制圆的 API，下述代码由 AI 给出
     * 【中点圆算法】通过模拟圆的对称性，只计算 1/8 的圆弧，然后通过对称性绘制整个圆
     */
    int x = (int)r;
    int y = 0;
    int err = 0; /* <- 表示当前点到理想圆路径的误差 */
    int centerX = (int)cx;
    int centerY = (int)cy;
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

void k__collision_debug_draw(struct k_component *component) {

    SDL_SetRenderDrawColor(k__window.renderer, 255, 0, 0, 255);

    struct k_collision_box *box = k_component_get_data(component);
    switch (box->type) {
        case K_COLLISION_POINT: {
            float x = *(box->x) + box->point.offset_x;
            float y = *(box->y) + box->point.offset_y;
            draw_point(x, y);
            break;
        }
        case K_COLLISION_LINE: {
            float x1 = *(box->x) + box->line.offset_x1;
            float y1 = *(box->y) + box->line.offset_y1;
            float x2 = *(box->x) + box->line.offset_x2;
            float y2 = *(box->y) + box->line.offset_y2;
            draw_line(x1, y1, x2, y2);
            break;
        }
        case K_COLLISION_RECTANGLE: {
            float x1 = *(box->x) + box->rect.offset_x1;
            float y1 = *(box->y) + box->rect.offset_y1;
            float x2 = *(box->x) + box->rect.offset_x2;
            float y2 = *(box->y) + box->rect.offset_y2;
            draw_rect(x1, y1, x2, y2);
            break;
        }
        case K_COLLISION_CIRCLE: {
            float cx = *(box->x) + box->circle.offset_cx;
            float cy = *(box->y) + box->circle.offset_cy;
            float r  = box->circle.r;
            draw_circle(cx, cy, r);
            break;
        }
    }
}

/* endregion */

/* region [set_debug] */

int k_collision_set_box_debug(struct k_collision_box *box, int debug) {

    if (0 == debug) {
        if (NULL != box->cb_debug_draw) {
            k_component_del_callback(box->cb_debug_draw);
            box->cb_debug_draw = NULL;
        }
        return 0;
    }

    if (NULL == box->cb_debug_draw) {
        box->cb_debug_draw = k_component_add_draw_callback(box->component, k__collision_debug_draw, 10000, 10000);
        if (NULL == box->cb_debug_draw)
            return -1;
    }

    return 0;
}

static struct k_collision_group *k__collision_find_or_add_group(int group_id) {

    struct k_component_manager *component_manager = k_room_get_component_manager(k__component_type_collision);
    if (NULL == component_manager)
        return NULL;

    struct k_collision_manager *manager = k_component_manager_get_data(component_manager);
    struct k_collision_group *group = k__collision_manager_find_or_add_group(manager, group_id);
    return group;
}

int k_collision_set_debug(int group_id, int debug) {

    struct k_collision_group *group = k__collision_find_or_add_group(group_id);
    if (NULL == group)
        return -1;



    return 0;
}

/* endregion */
