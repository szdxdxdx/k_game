#ifndef K_GAME__SPRITE_REGISTRY_H
#define K_GAME__SPRITE_REGISTRY_H

struct k_sprite;

int k__sprite_registry_init(void);

void k__sprite_registry_cleanup(void);

void k__sprite_registry_add(struct k_sprite *sprite);

void k__sprite_registry_del(struct k_sprite *sprite);

const char *k__sprite_get_name(struct k_sprite *sprite);

#endif
