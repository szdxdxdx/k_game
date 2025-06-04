#ifndef K_SOUND_H
#define K_SOUND_H

#include "./k_game_fwd.h"

/* region [bgm_load] */

/**
 * \brief 加载背景音乐资源
 *
 * 目前支持加载 .wav 和 .mp3 格式的音频。
 *
 * 若加载成功，函数返回音乐的指针，否则返回 `NULL`。
 */
struct k_sound_bgm *k_sound_bgm_load(const char *file_path);

/**
 * \brief 释放音乐资源
 *
 * 若 `sound` 为 `NULL`，则函数立即返回。
 *
 * 游戏结束时 k_game 会释放音乐资源，无需手动释放。
 */
void k_sound_bgm_release(struct k_sound_bgm *sound);

/* endregion */

/* region [bgm_play] */

/**
 * \brief 循环播放背景音乐
 *
 * 循环播放背景音乐 `sound`，总共播放 `loops` 次。
 * 若当前已有音乐正在播放，则立即停止并替换为新的音乐。
 *
 * 若希望音乐无限循环播放，则可将 `loops` 的值设为 `INT_MAX`。
 */
void k_sound_bgm_loop(struct k_sound_bgm *sound, int loops);

// int k_sound_bgm_is_playing(void);

// TODO void k_sound_bgm_stop(void);

// TODO int k_sound_set_bgm_volume(float volume);

/* endregion */

/* region [sfx_load] */

/**
 * \brief 加载背景音效资源
 *
 * 目前支持加载 .wav 和 .mp3 格式的音频。
 *
 * 若加载成功，函数返回音效的指针，否则返回 `NULL`。
 */
struct k_sound_sfx *k_sound_sfx_load(const char *file_path);

/**
 * \brief 释放音效资源
 *
 * 若 `sound` 为 `NULL`，则函数立即返回。
 *
 * 游戏结束时 k_game 会释放音效资源，无需手动释放。
 */
void k_sound_sfx_release(struct k_sound_sfx *sound);

/* endregion */

/* region [sfx_play] */

/** \brief 播放一次音效 */
void k_sound_sfx_play(struct k_sound_sfx *sound);

/** \brief 循环播放音效 */
void k_sound_sfx_loop(struct k_sound_sfx *sound, int loops);

/* endregion */

#endif
