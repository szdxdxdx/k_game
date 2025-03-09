#ifndef K_SEQ_STEP_H
#define K_SEQ_STEP_H

#include <stddef.h>

struct k_seq_step {

    int (*fn_forward)(void *data);

    void (*fn_backward)(void *data);
};

size_t k_seq_step_exec_forward(const struct k_seq_step *steps, size_t steps_num, void *data);

void k_seq_step_exec_backward(const struct k_seq_step *steps, size_t steps_num, void *data);

int k_seq_step_exec(const struct k_seq_step *steps, size_t steps_num, void *data);

#define k_seq_step_array_len(arr) (sizeof(arr) / sizeof(arr[0]))

#endif
