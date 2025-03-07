#ifndef K_GAME__COMPONENT_CALLBACK_H
#define K_GAME__COMPONENT_CALLBACK_H

struct k_component;

void k__component_init_callback_list(struct k_component *component);

void k__component_cleanup_callback_list(struct k_component *component);

#endif
