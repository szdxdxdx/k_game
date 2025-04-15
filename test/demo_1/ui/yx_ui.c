#include "k_game/core/k_object.h"

struct yx_ui {
    struct k_object *object;
};

struct yx_ui *yx_ui_create_context(void) {

    struct k_object *object = k_object_create(sizeof(struct yx_ui));
    if (NULL == object)
        return NULL;

    struct yx_ui *ui = k_object_get_data(object);
    ui->object = object;

    return ui;
}
