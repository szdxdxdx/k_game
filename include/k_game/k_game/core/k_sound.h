#ifndef K_SOUND_H
#define K_SOUND_H

struct k_sound_bgm;

struct k_sound_sfx;

/* region [bgm_load] */

struct k_sound_bgm *k_sound_bgm_load(const char *file_path);

void k_sound_bgm_release(struct k_sound_bgm *sound);

int k_sound_bgm_set_name(struct k_sound_bgm *sound, const char *bgm_name);

struct k_sound_bgm *k_sound_bgm_find(const char *bgm_name);

/* endregion */

/* region [bgm_play] */

int k_sound_bgm_loop(struct k_sound_bgm *sound, int loops);

// int k_sound_bgm_is_playing(void);

// TODO void k_sound_bgm_stop(void);

// TODO int k_sound_set_bgm_volume(float volume);

/* endregion */

/* region [sfx_load] */

struct k_sound_sfx *k_sound_sfx_load(const char *file_path);

void k_sound_sfx_release(struct k_sound_sfx *sound);

int k_sound_sfx_set_name(struct k_sound_sfx *sound, const char *sfx_name);

struct k_sound_sfx *k_sound_sfx_find(const char *sfx_name);

/* endregion */

/* region [sfx_play] */

void k_sound_sfx_play(struct k_sound_sfx *sound);

void k_sound_sfx_loop(struct k_sound_sfx *sound, int loops);

/* endregion */

#endif
