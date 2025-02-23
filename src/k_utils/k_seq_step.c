#include <stddef.h>

#include "k_seq_step.h"

size_t k_execute_steps_forward(const struct k_seq_step *steps, size_t steps_num, void *data) {

    const struct k_seq_step *step = steps;
    for (; step < steps + steps_num; step++) {
        if (NULL != step->fn_forward && 0 != step->fn_forward(data))
            break;
    }

    return step - steps;
}

void k_execute_steps_backward(const struct k_seq_step *steps, size_t steps_num, void *data) {

    const struct k_seq_step *step = steps + steps_num - 1;
    for (; step >= steps; step--) {
        if (NULL != step->fn_backward)
            step->fn_backward(data);
    }
}
