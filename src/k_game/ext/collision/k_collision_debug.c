#include <assert.h>

#include "./k_collision_internal.h"

#include "k_game/core/k_canvas.h"

/* region [debug_draw] */

static void k__collision_draw_box(struct k_collision_box *box) {

    switch (box->type) {
        case K__COLLISION_POINT: {
            float x = *(box->x) + box->point.offset_x;
            float y = *(box->y) + box->point.offset_y;
            float r = 1.0f;
            k_canvas_room_draw_line(x - r, y, x + r, y);
            k_canvas_room_draw_line(x, y - r, x, y + r);
            break;
        }
        case K__COLLISION_LINE: {
            float x1 = *(box->x) + box->line.offset_x1;
            float y1 = *(box->y) + box->line.offset_y1;
            float x2 = *(box->x) + box->line.offset_x2;
            float y2 = *(box->y) + box->line.offset_y2;
            k_canvas_room_draw_line(x1, y1, x2, y2);
            break;
        }
        case K__COLLISION_RECTANGLE: {
            float x = *(box->x) + box->rect.offset_x1;
            float y = *(box->y) + box->rect.offset_y1;
            float w = box->rect.offset_x2 - box->rect.offset_x1;
            float h = box->rect.offset_y2 - box->rect.offset_y1;
            if (w < 0) { x += w; w = -w; }
            if (h < 0) { y += h; h = -h; }
            k_canvas_room_draw_rect(x, y, w, h);
            break;
        }
        case K__COLLISION_CIRCLE: {
            float cx = *(box->x) + box->circle.offset_cx;
            float cy = *(box->y) + box->circle.offset_cy;
            float r  = box->circle.r;
            k_canvas_room_draw_circle(cx, cy, r);
            break;
        }
        default:
            assert(0);
    }
}

static void k__collision_group_on_debug_draw(void *group_) {
    struct k_collision_group *group = group_;

    k_canvas_set_draw_color_rgba(0xee0000ff);

    struct k_collision_box *box;
    struct k_list *list = &group->box_list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        box = container_of(iter, struct k_collision_box, box_list_node);

        k__collision_draw_box(box);
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

        k_callback_del(group->cb_debug_draw);
        group->cb_debug_draw = NULL;
        return 0;
    }
    else {
        struct k_collision_group *group = k__collision_find_or_add_group(group_id);
        if (NULL == group)
            return -1;

        if (NULL != group->cb_debug_draw)
            return 0;

        group->cb_debug_draw = k_component_manager_add_draw_callback(
            group->manager->component_manager,
            group,
            k__collision_group_on_debug_draw,
            K_DEBUG_Z_GROUP,
            K_DEBUG_Z_LAYER
        );
        if (NULL == group->cb_debug_draw)
            return -1;

        return 0;
    }
}

/* endregion */
