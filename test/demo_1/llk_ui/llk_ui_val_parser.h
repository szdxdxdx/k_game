#ifndef LLK_UI_VAL_PARSER_H
#define LLK_UI_VAL_PARSER_H

#include <stdint.h>

#include "./llk_ui_fwd.h"

int llk__ui_parse_length_val(const char *str, float *get_val, enum llk_ui_unit *get_unit);

int llk__ui_parse_color_val(const char *str, uint32_t *get_val);

#endif
