#ifndef K_SOUND_H
#define K_SOUND_H

struct k_sound_BGM;

struct k_sound_SFX;

/* region [BGM_load] */

struct k_sound_BGM *k_load_sound_BGM(const char *filepath);

void k_release_sound_BGM(struct k_sound_BGM *sound);

int k_sound_BGM_set_name(struct k_sound_BGM *sound, const char *BGM_name);

struct k_sound_BGM *k_find_sound_BGM(const char *BGM_name);

/* endregion */

/* region [BGM_play] */

int k_loop_sound_BGM(struct k_sound_BGM *sound, int loops);

// int k_sound_BGM_is_playing(void);

// TODO void k_sound_BGM_stop(void);

// TODO int k_sound_set_BGM_volume(float volume);

/* endregion */

/* region [SFX_load] */

struct k_sound_SFX *k_load_sound_SFX(const char *filepath);

void k_release_sound_SFX(struct k_sound_SFX *sound);

int k_sound_SFX_set_name(struct k_sound_SFX *sound, const char *SFX_name);

struct k_sound_SFX *k_find_sound_SFX(const char *SFX_name);

/* endregion */

/* region [SFX_play] */

void k_play_sound_SFX(struct k_sound_SFX *sound);

void k_loop_sound_SFX(struct k_sound_SFX *sound, int loops);

/* endregion */

#endif
