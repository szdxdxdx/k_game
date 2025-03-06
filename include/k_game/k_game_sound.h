#ifndef K_GAME_SOUND_H
#define K_GAME_SOUND_H

struct k_sound;

struct k_sound k_sound_load(const char *sound_name, const char *filepath);

void k_sound_play(struct k_sound *sound);

#endif
