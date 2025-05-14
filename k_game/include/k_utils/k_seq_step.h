#ifndef K_SEQ_STEP_H
#define K_SEQ_STEP_H

#include <stddef.h>

/** \brief 表示一个执行步骤 */
struct k_seq_step {

    /** \brief 步骤的前向函数。若步骤执行成功，函数应返回 0，否则应返回非 0 */
    int (*fn_forward)(void *context);

    /** \brief 步骤的回滚函数 */
    void (*fn_backward)(void *context);
};

/**
 * \brief 按顺序执行每个步骤的前向函数
 *
 * `steps` 为要执行的步骤的数组，`steps_num` 指定数组长度，
 * `context` 是传递给各步骤中的函数的上下文信息。
 *
 * 函数将按顺序调用每个步骤的前向函数 `fn_forward()`，
 * 若 `fn_forward` 为 `NULL` 则跳过。
 *
 * 一旦有步骤失败，则立即停止后续步骤的执行。
 * 函数返回成功完成的步骤数量，若为 `steps_num` 则表示所有步骤均执行成功。
 */
size_t k_seq_step_exec_forward(const struct k_seq_step *steps, size_t steps_num, void *context);

/**
 * \brief 按逆序执行每个步骤的回滚函数
 *
 * `steps` 为要执行的步骤的数组，`steps_num` 指定数组长度，
 * `context` 是传递给各步骤中的函数的上下文信息。
 *
 * 函数将按逆序调用每个步骤的回滚函数 `fn_backward()`，
 * 若 `fn_backward` 为 `NULL` 则跳过。
 */
void k_seq_step_exec_backward(const struct k_seq_step *steps, size_t steps_num, void *context);

/**
 * \brief 完整执行整个步骤链
 *
 * `steps` 为要执行的步骤的数组，`steps_num` 指定数组长度，
 * `context` 是传递给各步骤中的函数的上下文信息。
 *
 * 函数将按顺序调用每个步骤的前向函数 `fn_forward()`，
 * 若 `fn_forward` 为 `NULL` 则跳过。
 * 一旦有步骤失败，则立即停止后续步骤的执行。从失败的步骤开始回滚，
 * 函数将逆序调用之前每个步骤的回滚函数 `fn_backward()`，
 * 若 `fn_backward` 为 `NULL` 则跳过。
 *
 * 注意：若当前步骤 `fn_forward()` 返回非 0，不会执行对应的 `fn_backward()`，
 * 也就是说，若当前步骤执行失败，要回滚的是之前已成功的步骤，而不包括当前步骤。
 * 请确保 `fn_forward()` 能在失败时自行回滚。若不能，则需要该步骤拆开成两个：
 * 前一个步骤的 `fn_forward` 为 `NULL`，后一个的 `fn_backward` 为 `NULL`。
 *
 * 若所有步骤都执行成功，则函数返回 0，
 * 若中途有步骤失败，则函数回滚已成功的步骤，并最终返回非 0。
 */
int k_seq_step_exec(const struct k_seq_step *steps, size_t steps_num, void *context);

/** \brief 获取静态数组中元素的个数 */
#define k_seq_step_array_len(arr) (sizeof(arr) / sizeof(arr[0]))

#endif
