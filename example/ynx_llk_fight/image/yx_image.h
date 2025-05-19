#ifndef YX_IMAGE_H
#define YX_IMAGE_H

extern struct k_image *img_bkgd;

extern struct k_image *img_ui_start_idle;
extern struct k_image *img_ui_start_hovered;
extern struct k_image *img_ui_start_pushed;

int yx_image_load_on_game_start(void);

#endif
