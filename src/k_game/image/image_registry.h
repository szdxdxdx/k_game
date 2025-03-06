#ifndef K_GAME__IMAGE_REGISTRY_H
#define K_GAME__IMAGE_REGISTRY_H

struct k_image;

int k__image_registry_init(void);

void k__image_registry_cleanup(void);

int k__image_registry_add(struct k_image *image, const char *image_name);

void k__image_registry_del(struct k_image *image);

const char *k__image_get_name(struct k_image *image);

#endif
