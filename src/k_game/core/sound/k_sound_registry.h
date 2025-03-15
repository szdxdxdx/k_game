#ifndef K_GAME__SOUND_REGISTRY_H
#define K_GAME__SOUND_REGISTRY_H

int k__sound_registry_init(void);

void k__sound_registry_cleanup(void);

void k__sound_registry_add_BGM(struct k_sound_BGM *sound);

void k__sound_registry_del_BGM(struct k_sound_BGM *sound);

void k__sound_registry_add_SFX(struct k_sound_SFX *sound);

void k__sound_registry_del_SFX(struct k_sound_SFX *sound);

#endif
