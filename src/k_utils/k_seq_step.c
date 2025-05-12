#include "k_seq_step.h"

size_t k_seq_step_exec_forward(const struct k_seq_step *steps, size_t steps_num, void *context) {
    /* 遍历步骤数组，执行前向函数 fn_forward */
    const struct k_seq_step *step = steps;
    for (; step < steps + steps_num; step++) {
        if (NULL != step->fn_forward && 0 != step->fn_forward(context))
            break;
    }
    return step - steps; /* 返回执行成功的步骤的个数 */
}

void k_seq_step_exec_backward(const struct k_seq_step *steps, size_t steps_num, void *context) {
    /* 倒序遍历步骤数组，执行反向函数 fn_backward，用于回滚 */
    const struct k_seq_step *step = steps + steps_num - 1;
    for (; step >= steps; step--) {
        if (NULL != step->fn_backward)
            step->fn_backward(context);
    }
}

int k_seq_step_exec(const struct k_seq_step *steps, size_t steps_num, void *context) {
    /* 遍历步骤数组，执行正向逻辑。若中途有步骤执行失败，则回滚 */
    size_t completed_count = k_seq_step_exec_forward(steps, steps_num, context);
    if (completed_count != steps_num) {
        k_seq_step_exec_backward(steps, completed_count, context);
        return -1;
    } else {
        return 0;
    }
}
