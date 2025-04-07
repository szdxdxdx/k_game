#ifndef K__COMPONENT_TYPE_REGISTRY_H
#define K__COMPONENT_TYPE_REGISTRY_H

int k__component_type_registry_init(void);

void k__component_type_registry_cleanup(void);

void k__component_type_registry_add(struct k_component_type *component_type);

void k__component_type_registry_del(struct k_component_type *component_type);

#endif
