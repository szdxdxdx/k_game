#ifndef K__FONT_REGISTRY_H
#define K__FONT_REGISTRY_H

struct k_font;

int k__font_registry_init(void);

void k__font_registry_cleanup(void);

void k__font_registry_add(struct k_font *font);

void k__font_registry_del(struct k_font *font);

#endif
