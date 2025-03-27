#include "./_internal.h"

#include "../../core/k_SDL/_public.h"

/* region [debug_draw] */

static void draw_point(float x, float y) {
    float r = 1.0f;
    SDL_RenderDrawLineF(k__window.renderer, x - r, y, x + r, y);
    SDL_RenderDrawLineF(k__window.renderer, x, y - r, x, y + r);
}

static void draw_line(float x1, float y1, float x2, float y2) {
    SDL_RenderDrawLineF(k__window.renderer, x1, y1, x2, y2);
}

static void draw_rect(float x1, float y1, float x2, float y2) {
    SDL_FPoint points[] = {
        { x1, y1 },
        { x1, y2 },
        { x2, y2 },
        { x2, y1 },
        { x1, y1 },
    };
    SDL_RenderDrawLinesF(k__window.renderer, points, 5);
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

static void draw_box(struct k_collision_box *box) {

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

static void draw_group(void *data) {
    struct k_collision_group *group = data;

    SDL_SetRenderDrawColor(k__window.renderer, 255, 0, 102, 255);

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        draw_box(box);
    }
}

/* endregion */

/* region [set_debug] */

int k_collision_set_debug(int group_id, int debug) {

    if (0 == debug) {
        struct k_collision_group *group = k__collision_find_group(group_id);
        if (NULL == group)
            return 0;

        if (NULL == group->cb_debug_draw)
            return 0;

        k_room_del_callback(group->cb_debug_draw);
        group->cb_debug_draw = NULL;
        return 0;
    }
    else {
        struct k_collision_group *group = k__collision_find_or_add_group(group_id);
        if (NULL == group)
            return -1;

        if (NULL != group->cb_debug_draw)
            return 0;

        group->cb_debug_draw = k_room_add_draw_callback(
        group, draw_group,
        K_COLLISION_DEBUG_Z_GROUP, K_COLLISION_DEBUG_Z_LAYER
        );

        if (NULL == group->cb_debug_draw)
            return -1;

        return 0;
    }
}

/* endregion */
