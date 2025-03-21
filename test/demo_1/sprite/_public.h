#ifndef MY_GAME__SPRITE__PUBLIC_H
#define MY_GAME__SPRITE__PUBLIC_H

struct k_sprite;

/* region [ynx] */

int my_spr_ynx_load(void);

extern struct k_sprite *my_spr_ynx_run;
extern struct k_sprite *my_spr_ynx_idle;

/* endregion */

/* region [liliko] */

int my_spr_liliko_load(void);

extern struct k_sprite *my_spr_liliko_run;
extern struct k_sprite *my_spr_liliko_idle;

/* endregion */

/* region [bubble] */

int my_spr_bubble_load(void);

extern struct k_sprite *my_spr_bubble_new;
extern struct k_sprite *my_spr_bubble_del;

/* endregion */

#endif
