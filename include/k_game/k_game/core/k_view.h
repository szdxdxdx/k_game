#ifndef K_VIEW_H
#define K_VIEW_H

/**
 * \brief 调整视野矩形的大小
 *
 * 调整视野的尺寸，使其尽可能完整地包含指定宽高的矩形。
 *
 * 视野矩形的大小受限于当前房间的尺寸和游戏窗口的画布尺寸，
 * 且视野矩形始终保持与游戏窗口一致的宽高比例。
 * 所以函数不会简单地将视野矩形的尺寸设置为指定值，
 * 而是在不超出房间边界与画布边界，且保证宽高比例的前提下，
 * 调整视野的矩形大小，使其尽可能包含目标矩形。
 *
 * 目标尺寸的宽或高必须为正值。
 * 若调整成功，函数返回 0，否则返回非 0。
 */
int k_view_fit_rect(float w, float h);

/**
 * \brief 设置视野矩形的中心点坐标
 *
 * 将视野矩形的中心点移至房间的 `(cx, cy)` 处。
 * 若指定的坐标位置会导致视野矩形超出房间的边界，
 * 则函数会修正坐标，使视野矩形仍被完全包含在房间内。
 *
 * 若设置成功，函数返回 0，否则返回非 0。
 */
int k_view_set_position(float cx, float cy);

/**
 * \brief 获取视野矩形的位置和宽高
 *
 * 出参 `get_x` 和 `get_y` 返回视野矩形左上角在房间中的坐标，
 * 出参 `get_w` 和 `get_h` 返回视野矩形的宽高，
 * 若传 `NULL` 则不接受该出参。
 */
void k_view_get_rect(float *get_x, float *get_y, float *get_w, float *get_h);

/**
 * \brief 获取视野矩形中心的位置
 *
 * 出参 `get_cx` 和 `get_cy` 返回视野矩形中心点的在房间中的坐标。
 * 若传 `NULL` 则不接受该出参。
 */
void k_view_get_position(float *get_cx, float *get_cy);

#endif
