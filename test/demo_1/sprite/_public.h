#ifndef YX__SPRITE__PUBLIC_H
#define YX__SPRITE__PUBLIC_H

struct k_sprite;

/* region [ynx] */

int yx_load_spr_ynx(void);

extern struct k_sprite *yx_spr_ynx_run;
extern struct k_sprite *yx_spr_ynx_idle;

/* endregion */

/* region [liliko] */

int yx_load_spr_liliko(void);

extern struct k_sprite *yx_spr_liliko_run;
extern struct k_sprite *yx_spr_liliko_idle;

/* endregion */

/* region [bubble] */

int yx_load_spr_bubble(void);

extern struct k_sprite *yx_spr_bubble_appear;
extern struct k_sprite *yx_spr_bubble_idle;
extern struct k_sprite *yx_spr_bubble_pop;

/* endregion */

#endif
