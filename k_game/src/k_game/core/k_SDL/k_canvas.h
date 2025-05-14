#ifndef K__CANVAS_H
#define K__CANVAS_H

#include "SDL_render.h"

/* 实际上，k_game 使用一张画布来绘制房间和 UI，而不是两张。
 * 这一张画布的【左边】区域用于【绘制房间】，【右边】区域用于【绘制 UI】，
 * 游戏每一帧结束时，把两个区域的内容叠加显示到游戏窗口中。
 *
 * 因为 k_game 的每一帧没有细分出【绘制房间】和【绘制 UI】等阶段，
 * 而仅有一个【绘制】阶段，所以房间和 UI 的绘制可能是混合的。
 * 这种情况下，若使用两张画布，可能会频繁切换渲染器绑定的纹理，这似乎很耗性能。
 * 所以 k_game 使用一张画布，根据绘制内容限定画布的视口矩形。
 */

enum k_canvas_viewport {

    /* 当前没有限定画布的视口，执行绘制操作时自动重新指定 */
    K__CANVAS_VIEWPORT_NONE,

    /* 将画布的视口限定为房间区域 */
    K__CANVAS_VIEWPORT_ROOM,

    /* 将画布的视口限定为 UI 区域 */
    K__CANVAS_VIEWPORT_UI,
};

struct k_canvas {

    /* 离屏画布
     *
     * 每一帧先把图像绘制到这张画布上，
     * 帧结束时再将它统一绘制到游戏窗口中。
     */
    SDL_Texture *canvas;

    /* 画布的房间视口
     *
     * 房间的绘制被限定在该矩形区域。
     * 实际使用的区域可能不会占满整个矩形，这取决于房间视野的范围。
     */
    int room_viewport_x; // = 0
    int room_viewport_y; // = 0
    int room_viewport_w; // = view_w_max
    int room_viewport_h; // = view_h_max

    /* 画布的 UI 视口
     *
     * UI 内容的绘制被限定在该矩形区域。
     * 目前，该矩形区域的宽高与游戏窗口宽高保持一致。
     */
    int ui_viewport_x; // = 0 + room_viewport_w
    int ui_viewport_y; // = 0
    int ui_viewport_w; // = window_w
    int ui_viewport_h; // = window_h

    /* 当前的视口区域 */
    enum k_canvas_viewport current_viewport;

    /* 记录当前视口的宽高
     *
     * k_game 会粗略过滤掉超出视口的对象，避免不必要的绘制操作。
     * SDL 会根据视口的位置调整坐标，因此无需记录视口的 x 和 y。
     */
    float current_viewport_w;
    float current_viewport_h;
};

extern struct k_canvas k__canvas;

void k__canvas_present(void);

#endif
