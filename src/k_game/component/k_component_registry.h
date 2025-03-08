#ifndef K_GAME__COMPONENT_REGISTRY_H
#define K_GAME__COMPONENT_REGISTRY_H

struct k_component_type;

int k__component_registry_init(void);

void k__component_registry_cleanup(void);

int k__component_registry_add(struct k_component_type *component_type, const char *type_name);

void k__component_registry_del(struct k_component_type *component_type);

const char *k__component_type_get_name(struct k_component_type *component_type);


#endif
