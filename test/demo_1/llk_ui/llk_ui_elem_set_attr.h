#ifndef LLK_UI_ELEM_SET_ATTR_H
#define LLK_UI_ELEM_SET_ATTR_H

#include <ctype.h>
#include <stdint.h>
#include <string.h>

#include "./llk_ui_fwd.h"

#include "./llk_ui_val_parser.h"

#define llk__ui_key_match(key, str) \
    (0 == strncmp((key), "" str "", sizeof(str) - 1))

int llk__ui_elem_set_attr_default(struct llk_ui_elem *elem, const char *key, const char *val);

#endif
