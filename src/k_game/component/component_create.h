#ifndef K_GAME__COMPONENT_CREATE_H
#define K_GAME__COMPONENT_CREATE_H

struct k_component_type;
struct k_object;

struct k_component *k__component_create(const struct k_component_type *component_type, struct k_object *object, void *params);

void k__component_destroy(struct k_component *component);

#endif
