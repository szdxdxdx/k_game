#ifndef K_GAME__OBJECT_COMPONENT_H
#define K_GAME__OBJECT_COMPONENT_H

struct k_object;
struct k_component;

void k__object_init_component_list(struct k_object *object);

void k__object_cleanup_component_list(struct k_object *object);

void k__object_component_list_add(struct k_object *object, struct k_component *component);

void k__object_component_list_del(struct k_component *component);

#endif
