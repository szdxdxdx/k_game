#ifndef K_GAME_SOUND_H
#define K_GAME_SOUND_H

struct k_sound;

enum k_sound_type {
    K_SOUND_BGM,
    K_SOUND_SFX,
};

struct k_sound *k_sound_load(const char *filepath, enum k_sound_type sound_type);

int k_sound_set_name(struct k_sound *sound, const char *name);

struct k_sound *k_sound_find(const char *sound_name);

int k_sound_play(struct k_sound *sound);

int k_sound_loop(struct k_sound *sound, int loops);

int k_sound_stop(struct k_sound *sound);

int k_sound_set_volume(struct k_sound *sound, float volume);

#endif
