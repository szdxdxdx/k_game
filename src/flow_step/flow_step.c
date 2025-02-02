#include "k/flow_step.h"

size_t k_execute_forward_flow(const struct k_flow_step *steps, size_t steps_num, void *global_data) {

    const struct k_flow_step *step = steps;
    for (; step < steps + steps_num; step++) {
        if (NULL != step->fn_forward && 0 != step->fn_forward(global_data, step->step_data))
            break;
    }

    return step - steps;
}

void k_execute_backward_flow(const struct k_flow_step *steps, size_t steps_num, void *global_data) {

    const struct k_flow_step *step = steps + steps_num - 1;
    for (; step >= steps; step--) {
        if (NULL != step->fn_backward)
            step->fn_backward(global_data, step->step_data);
    }
}

size_t k_execute_flow(const struct k_flow_step *steps, size_t steps_num, void *global_data) {

    size_t successful_steps_count = k_execute_forward_flow(steps, steps_num, global_data);

    k_execute_backward_flow(steps, successful_steps_count, global_data);

    return successful_steps_count;
}
