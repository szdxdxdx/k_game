#ifndef K_GAME__OBJECT_COMPONENT_H
#define K_GAME__OBJECT_COMPONENT_H

struct k_object;

void k__object_init_component_list(struct k_object *object);

void k__object_cleanup_component_list(struct k_object *object);

#endif
