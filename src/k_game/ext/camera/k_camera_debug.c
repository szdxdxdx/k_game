#include <assert.h>

#include "k_game/core/k_canvas.h"
#include "k_game/core/k_view.h"

#include "./k_camera_internal.h"

static void k__camera_auto_follow_debug_draw(struct k_camera *camera) {

    float dst_x = camera->dst_x;
    float dst_y = camera->dst_y;
    float r = 3.0f;

    if (0 != camera->targets_num) {
        goto draw_targets_line;
    } else {
        goto draw_view_center;
    }

draw_targets_line:
    {
        k_canvas_set_hex_color(0x55dd66ff);

        k_canvas_draw_circle(dst_x, dst_y, r);

        if (NULL == camera->primary_target) {

            size_t i = 0;
            for (; i < camera->targets_num; i++) {
                struct k_camera_target *target = camera->targets[i];

                float target_x = *target->x;
                float target_y = *target->y;
                k_canvas_draw_line(target_x, target_y, dst_x, dst_y);
            }
        }
        else {
            float view_left;
            float view_top;
            float view_w;
            float view_h;
            k_view_get_rect(&view_left, &view_top, &view_w, &view_h);
            float view_right  = view_left + view_w;
            float view_bottom = view_top + view_h;

            size_t i = 0;
            for (; i < camera->targets_num; i++) {
                struct k_camera_target *target = camera->targets[i];

                float target_x = *target->x;
                float target_y = *target->y;

                if (target != camera->primary_target) {
                    if (target_x < view_left)   continue;
                    if (target_y < view_top)    continue;
                    if (target_x > view_right)  continue;
                    if (target_y > view_bottom) continue;
                }

                k_canvas_draw_line(target_x, target_y, dst_x, dst_y);
            }
        }
    }

draw_view_center:
    {
        float view_cx;
        float view_cy;
        k_view_get_position(&view_cx, &view_cy);

        k_canvas_set_hex_color(0xffdd66ff);
        k_canvas_draw_circle(view_cx, view_cy, r);

        if (dst_x != view_cx || dst_y != view_cy) {
            k_canvas_draw_line(view_cx, view_cy, dst_x, dst_y);
        }
    }
}

static void k__camera_debug_draw(void *camera_) {
    struct k_camera *camera = camera_;

    switch (camera->state) {
        case K__CAMERA_DISABLE:
            assert(0);
            break;
        case K__CAMERA_AUTO_FOLLOW:
            k__camera_auto_follow_debug_draw(camera);
            break;
        case K__CAMERA_CINEMATIC:
            assert(0);
            break;
    }
}

int k_camera_set_debug(int debug) {

    struct k_component_manager *manager = k_room_get_component_manager(k__camera_component_type);
    if (NULL == manager)
        return -1;

    struct k_camera *camera = k_component_manager_get_data(manager);

    if (0 == debug) {
        if (NULL == camera->cb_camera_debug)
            return 0;

        k_callback_del(camera->cb_camera_debug);
        camera->cb_camera_debug = NULL;
        return 0;
    }
    else {
        if (NULL != camera->cb_camera_debug)
            return 0;

        camera->cb_camera_debug = k_component_manager_add_draw_callback(
            manager,
            camera,
            k__camera_debug_draw,
            K_DEBUG_Z_GROUP,
            K_DEBUG_Z_LAYER
        );
        if (NULL == camera->cb_camera_debug)
            return -1;

        return 0;
    }
}
