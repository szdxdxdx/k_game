#ifndef K__SOUND_REGISTRY_H
#define K__SOUND_REGISTRY_H

struct k_sound_bgm;
struct k_sound_sfx;

int k__sound_bgm_registry_init(void);

void k__sound_bgm_registry_cleanup(void);

void k__sound_bgm_registry_add(struct k_sound_bgm *bgm);

void k__sound_bgm_registry_del(struct k_sound_bgm *bgm);

int k__sound_sfx_registry_init(void);

void k__sound_sfx_registry_cleanup(void);

void k__sound_sfx_registry_add(struct k_sound_sfx *sfx);

void k__sound_sfx_registry_del(struct k_sound_sfx *sfx);

#endif
