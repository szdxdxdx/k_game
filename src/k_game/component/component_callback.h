#ifndef K_GAME__COMPONENT_CALLBACK_H
#define K_GAME__COMPONENT_CALLBACK_H

struct k_component;
struct k_callback;

struct k_component_callback {

    struct k_list_node list_node;

    struct k_callback *base;
};

void k__component_init_callback_list(struct k_component *component);

void k__component_cleanup_callback_list(struct k_component *component);

#endif
