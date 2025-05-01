#include <stdlib.h>
#include <math.h>

#include "k_webui.h"

#include "k_game/core/k_view.h"

#include "./k_camera_internal.h"

struct k_camera_webui {
    size_t text;
    size_t checkbox;
};

static int k__camera_webui_print(void *camera_, struct k_str_buf *buf) {
    struct k_camera *camera = camera_;
    k_str_buf_clear(buf);

    k_str_buf_printf(buf, "摄像机的加速度：%.2f\n", camera->acceleration);
    k_str_buf_printf(buf, "摄像机的当前速度：%.2f（最大速度：%.2f）\n", sqrtf(camera->vx * camera->vx + camera->vy * camera->vy), camera->max_speed);

    float cx;
    float cy;
    k_view_get_position(&cx, &cy);
    k_str_buf_printf(buf, "摄像机的当前坐标：(%.2f, %.2f)\n", cx, cy);
    k_str_buf_printf(buf, "摄像机的目标位置：(%.2f, %.2f)\n", camera->dst_x, camera->dst_y);

    switch (camera->state) {
        case K__CAMERA_AUTO_FOLLOW:
            k_str_buf_puts(buf, "摄像机当前状态：自动跟随\n");
            k_str_buf_printf(buf, "摄像机跟随目标的个数：%d\n",  camera->targets_num);
            if (NULL == camera->primary_target) {
                k_str_buf_puts(buf, "摄像机的主跟随目标：无\n");
            } else {
                k_str_buf_printf(buf, "主跟随目标的位置：(%.2f, %.2f)\n", *(camera->primary_target->x), *(camera->primary_target->y));
            }
            break;
        default:
            break;
    }

    return 0;
}

static int k__camera_webui_checkbox_debug_draw_on_read(void *camera_, int *val) {
    struct k_camera *camera = camera_;
    *val = (NULL != camera->cb_camera_debug);
    return 0;
}

static int k__camera_webui_checkbox_debug_draw_on_change(void *camera_, int val) {
    struct k_camera *camera = camera_;
    return k__camera_set_debug_draw_enabled(camera, val);
}

void k__camera_webui(struct k_camera *camera, int enabled) {

    if ( ! enabled) {
        if (NULL == camera->webui)
            return;

        struct k_camera_webui *webui = camera->webui;
        camera->webui = NULL;

        k_webui_unbind(webui->text);
        k_webui_unbind(webui->checkbox);
        free(webui);
    }
    else {
        if (NULL != camera->webui)
            return;

        struct k_camera_webui *webui = malloc(sizeof(struct k_camera_webui));
        if (NULL == webui)
            return;

        camera->webui = webui;

        {
            struct k_webui_text_config text = K_WEBUI_TEXT_CONFIG_INIT;
            text.on_read = k__camera_webui_print;
            webui->text = k_webui_bind_text("", camera, &text);
        }

        {
            struct k_webui_checkbox_config checkbox =  K_WEBUI_CHECKBOX_CONFIG_INIT;
            checkbox.on_read   = k__camera_webui_checkbox_debug_draw_on_read;
            checkbox.on_change = k__camera_webui_checkbox_debug_draw_on_change;
            webui->checkbox = k_webui_bind_checkbox("绘制摄像机的跟随目标线：", camera, &checkbox);
        }
    }
}

void k_camera_webui(int enabled) {

    struct k_camera *camera = k_room_get_component_manager_data(k__camera_component_type);
    if (NULL == camera)
        return;

    k__camera_webui(camera, enabled);
}
