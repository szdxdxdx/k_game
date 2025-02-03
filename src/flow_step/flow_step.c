#include <stddef.h>

#include "k/flow_step.h"

size_t k_execute_flow_forward(const struct k_flow_step *steps, size_t steps_num, void *global_data) {

    const struct k_flow_step *step = steps;
    for (; step < steps + steps_num; step++) {
        if (NULL != step->fn_forward && 0 != step->fn_forward(global_data, step->step_data))
            break;
    }

    return step - steps;
}

void k_execute_flow_backward(const struct k_flow_step *steps, size_t steps_num, void *global_data) {

    const struct k_flow_step *step = steps + steps_num - 1;
    for (; step >= steps; step--) {
        if (NULL != step->fn_backward)
            step->fn_backward(global_data, step->step_data);
    }
}
