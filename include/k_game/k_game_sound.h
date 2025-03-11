#ifndef K_GAME_SOUND_H
#define K_GAME_SOUND_H

struct k_sound_BGM;

struct k_sound_BGM *k_sound_BGM_load(const char *filepath);

void k_sound_BGM_free(struct k_sound_BGM *sound);

int k_sound_BGM_set_name(struct k_sound_BGM *sound, const char *name);

struct k_sound_BGM *k_sound_BGM_find(const char *BGM_name);

int k_sound_BGM_loop(struct k_sound_BGM *sound, int loops);

int k_sound_BGM_is_playing(void);

void k_sound_BGM_stop(void);

int k_sound_set_BGM_volume(float volume);

struct k_sound_SFX;

struct k_sound_SFX *k_sound_load_SFX(const char *filepath);

void k_sound_SFX_free(struct k_sound_SFX *sound);

int k_sound_SFX_set_name(struct k_sound_SFX *sound, const char *name);

struct k_sound_SFX *k_sound_SFX_find(const char *SFX_name);

int k_sound_SFX_play(struct k_sound_SFX *sound);

#endif
