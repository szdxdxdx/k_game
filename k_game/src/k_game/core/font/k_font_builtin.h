#ifndef K__FONT_BUILTIN_H
#define K__FONT_BUILTIN_H

struct k_font;

extern struct k_font *k__font_builtin;

int k__font_load_builtin(void);

#endif
