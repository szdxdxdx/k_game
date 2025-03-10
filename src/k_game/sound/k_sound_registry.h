#ifndef K_GAME__SOUND_REGISTRY_H
#define K_GAME__SOUND_REGISTRY_H

struct k_sound;

int k__sound_registry_init(void);

void k__sound_registry_cleanup(void);

void k__sound_registry_add(struct k_sound *sound);

void k__sound_registry_del(struct k_sound *sound);

#endif
