#ifndef K_GAME__IMAGE_REGISTRY_H
#define K_GAME__IMAGE_REGISTRY_H

struct k_image;

int k__image_registry_init(void);

void k__image_registry_cleanup(void);

void k__image_registry_add(struct k_image *image);

void k__image_registry_del(struct k_image *image);

#endif
