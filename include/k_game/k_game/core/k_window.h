#ifndef K_WINDOW_H
#define K_WINDOW_H

/**
 * \brief 设置窗口保持置顶
 *
 * 若 `enable` 为非 0 则设置窗口保持置顶，否则取消置顶。
 * 若成功，函数返回 0，否则返回非 0。
 */
int k_window_set_always_on_top(int enable);

#endif
