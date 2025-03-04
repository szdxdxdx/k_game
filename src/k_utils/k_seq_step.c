#include <stddef.h>

#include "k_seq_step.h"

size_t k_seq_step_exec_forward(const struct k_seq_step *steps, size_t steps_num, void *data) {

    const struct k_seq_step *step = steps;
    for (; step < steps + steps_num; step++) {
        if (NULL != step->fn_forward && 0 != step->fn_forward(data))
            break;
    }

    return step - steps;
}

void k_seq_step_exec_backward(const struct k_seq_step *steps, size_t steps_num, void *data) {

    const struct k_seq_step *step = steps + steps_num - 1;
    for (; step >= steps; step--) {
        if (NULL != step->fn_backward)
            step->fn_backward(data);
    }
}

int k_seq_step_exec_with_rollback(const struct k_seq_step *steps, size_t steps_num, void *data) {

    size_t completed_count = k_seq_step_exec_forward(steps, steps_num, data);
    if (completed_count != steps_num) {
        k_seq_step_exec_backward(steps, completed_count, data);
        return -1;
    }

    return 0;
}
