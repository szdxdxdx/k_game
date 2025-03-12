#ifndef K_GAME__COMPONENT_TYPE_REGISTRY_H
#define K_GAME__COMPONENT_TYPE_REGISTRY_H

struct k_component_type;

int k__component_type_registry_init(void);

void k__component_type_registry_cleanup(void);

void k__component_type_registry_add(struct k_component_type *component_type);

void k__component_type_registry_del(struct k_component_type *component_type);

#endif
