#ifndef K_FLOW_STEP_H
#define K_FLOW_STEP_H

#include <stddef.h>

struct k_flow_step {

    void *step_data;

    int (*fn_forward)(void *global_data, void *step_data);

    void (*fn_backward)(void *global_data, void *step_data);
};

size_t k_execute_flow_forward(const struct k_flow_step *steps, size_t steps_num, void *global_data);

void k_execute_flow_backward(const struct k_flow_step *steps, size_t steps_num, void *global_data);

#endif
