#ifndef K_GAME_ERROR_H
#define K_GAME_ERROR_H

void k_error_set(const char *fmt, ...);

const char *k_error_get(void);

#endif
