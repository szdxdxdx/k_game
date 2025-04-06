#ifndef K_COMPONENT_TYPE_REGISTRY_H
#define K_COMPONENT_TYPE_REGISTRY_H

extern struct k_asset_registry k__component_type_registry;

int k__component_type_registry_init(void);

void k__component_type_registry_cleanup(void);

#endif
