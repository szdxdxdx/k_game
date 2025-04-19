#ifndef K__VIEW_H
#define K__VIEW_H

struct k_view {

    /* 视野左上角在房间中的坐标 */
    float view_x;
    float view_y;

    /* 视野的宽高，最大不能超过画布的房间视口区域 */
    float view_h;
    float view_w;

    /* 记录视野与窗口的缩放比，改变视野宽高或窗口宽高时同步修改该值 */
    float view_window_ratio;
};

extern struct k_view k__view;

#endif
