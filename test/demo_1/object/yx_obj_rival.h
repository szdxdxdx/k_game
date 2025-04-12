#ifndef YX_OBJ_RIVAL_H
#define YX_OBJ_RIVAL_H

struct yx_obj_rival_config {
    float x;
    float y;

    struct k_sprite *spr_idle;
    struct k_sprite *spr_run;
};

struct k_object *yx_create_rival(const struct yx_obj_rival_config *config);

struct yx_obj_rival {

    struct k_position *position;
    float x;
    float y;

    struct k_sprite_renderer *spr_rdr;
    struct k_sprite *spr_idle;
    struct k_sprite *spr_run;
};

#endif
